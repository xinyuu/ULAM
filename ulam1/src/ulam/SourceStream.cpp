#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <string.h>
#include <errno.h>
#include "SourceStream.h"
#include "FileManager.h"

namespace MFM {

  SourceStream::SourceStream(FileManager * fm, CompilerState & state): m_fileManager(fm), m_state(state) 
  {
    filerec frec;
    m_fileRecords.push_back(frec);  // at position 0, null filerec id 0
    m_lastReadLoc = frec.m_loc;     // copy invalid loc 
  }


  SourceStream::~SourceStream() 
  {
    m_registeredFilenames.clear();

    // close any remaining open files
    while(discardTop());

    m_fileRecords.clear();
  }

  void SourceStream::unread()
  {
    if (m_openFilesStack.empty()) return;  //top behavior undefined when empty
    
    u16 id = m_openFilesStack.top();
    
    if(m_fileRecords[id].getUnreadFlag())
      {
	std::ostringstream msg;
	msg << "Unread called twice bogusly: <" << (char) m_fileRecords[id].getUnreadByte() << ">";
	Locator lastLoc = m_fileRecords[id].getUnreadLoc();
	MSG(m_state.getFullLocationAsString(lastLoc).c_str(), msg.str().c_str(),DEBUG);
      }

    m_fileRecords[id].setUnreadFlag(true);
  }

  s32 SourceStream::read()
  {
    // m_lastReadByte = -1; ???
    if (m_openFilesStack.empty()) return -1;  //top behavior undefined when empty

    u16 id = m_openFilesStack.top();

    if(m_fileRecords[id].getUnreadFlag())
      {
	m_fileRecords[id].setUnreadFlag(false);
	return m_fileRecords[id].getUnreadByte();
      }
    
    File * fp = m_fileRecords[id].m_fp;
    if( fp != NULL)
      {
	m_lastReadByte = fp->read();

	m_fileRecords[id].m_loc.updateLineByteNo(m_lastReadByte); // update access record

	//save this read byte, and  last read loc before updating, in case of future unread;
	m_fileRecords[id].updateUnread(false, m_lastReadByte, m_lastReadLoc);    

	m_lastReadLoc = m_fileRecords[id].m_loc;
	// automatically close and 'pop', and the next byte of the most 
	//recently suspended file is returned instead, if any remain. 
	//if(c < 0)
	// if error (not EOF) return error, do not pop the stack
	if(m_lastReadByte == EOF)
	  {
	    discardTop();
	    return read();
	  }
      }
    //else no further 'push'ed files remain, return -1 

    return m_lastReadByte;
  }


  bool SourceStream::push(std::string filename, bool onlyOnce)
  {
    //map filename to string pool index (u32)
    u32 findex = m_state.m_pool.getIndexForDataString(filename);

    if(onlyOnce)
      {
	// if the given filename has been push'ed before, return true
	std::map<u32,u16>::iterator it = m_registeredFilenames.find(findex);
	if(it != m_registeredFilenames.end())
	  {
	    return true;
	  }
      }
    
    if(!m_fileManager)
      {
	MSG("", "FileManager not found", ERR);
	return false;  //FM no good 
      }

    // attempt to open filename for reading; return false if failed.    
    File * fp = m_fileManager->open(filename, READ);
    if(fp == NULL)
      {
	std::ostringstream msg;
	msg << "Couldn't open file <" << filename << "> errno=" << errno << " " << strerror(errno);
	MSG("", msg.str().c_str(), ERR);
	return false;         //couldn't open the file
      }

    // open succeeds !!!
    // register new filename
    // push fp onto stack of open fp's
    // suspends reading whatever it is currently reading
    u16 newid = m_registeredFilenames.size() + 1;
    assert(newid == m_fileRecords.size());

    //    m_registeredFilenames.insert(std::pair<std::string,u16> (filename.c_str(),newid));
    m_registeredFilenames.insert(std::pair<u32,u16> (findex,newid));
    m_openFilesStack.push(newid);

    filerec frec;
    frec.init(newid, fp, findex);
    m_fileRecords.push_back(frec);  // at position id - 1; init to 0,0

    return true;
  }


  bool SourceStream::discardTop() 
  {
    if (m_openFilesStack.empty()) return false;
    
    u16 id = m_openFilesStack.top();
    m_openFilesStack.pop();

    m_fileRecords[id].closeFile();

    return true;
  }


  const std::string SourceStream::getPathFromLocator(Locator & loc) const
  {
    //u16 id = loc.getFileId();
    //return m_fileRecords[id].m_path;
    u32 idx = loc.getPathIndex();
    return m_state.m_pool.getDataAsString(idx);
  }

  
  Locator SourceStream::getLocator() const
  {
    // If the most recent call to read() returned EOF, return the first
    //	file that was push()ed into this SourceStream (i.e. everything popped).
    if (m_openFilesStack.empty())
      {
	if(m_fileRecords.size() > 1)
	  {
	    return m_fileRecords[1].m_loc; 
	  }
	else
	  {
	    return m_fileRecords[0].m_loc; //invalid locator w id 0
	  }
      }

    u16 id = m_openFilesStack.top();
    
    // if top has not yet been read, return the previous one
    if(m_fileRecords[id].m_fp != NULL && ((Locator) m_fileRecords[id].m_loc).hasNeverBeenRead())
      {
	return m_lastReadLoc;
      }

    // If the most recent call to read() returned an error, return the
    //	locator of the File that had the error. Still on stack.
    //  (same as normal case, except m_fp is NULL)
    if(m_fileRecords[id].getUnreadFlag())
      return m_fileRecords[id].getUnreadLoc();
    return m_fileRecords[id].m_loc;
  }


  u16 SourceStream::getLineNumber() const
  {
    Locator lloc = getLocator();
    return lloc.getLineNo();
}


  u16 SourceStream::getByteNumberInLine() const
  {
    Locator lloc = getLocator();
    return lloc.getByteNo();
  }

}  //end MFM