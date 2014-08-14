#include <stdio.h>
#include "NodeBlockClass.h"
#include "NodeBlockFunction.h"
#include "CompilerState.h"


namespace MFM {

  NodeBlockClass::NodeBlockClass(NodeBlock * prevBlockNode, CompilerState & state, NodeStatements * s) : NodeBlock(prevBlockNode, state, s)
  {}

  NodeBlockClass::~NodeBlockClass()
  {}


  void NodeBlockClass::print(File * fp)
  {
    printNodeLocation(fp);
    UlamType * myut = getNodeType();
    char id[255];
    if(!myut)    
      sprintf(id,"%s<NOTYPE>\n", prettyNodeName().c_str());
    else
      sprintf(id,"%s<%s>\n", prettyNodeName().c_str(), myut->getUlamTypeName().c_str());
    fp->write(id);

    if(m_nextNode)
      m_nextNode->print(fp);  //datamember var decls

    NodeBlockFunction * func = findMainFunctionNode();
    if(func)
      func->print(fp);

    sprintf(id,"-----------------%s\n", prettyNodeName().c_str());
    fp->write(id);
  }


  void NodeBlockClass::printPostfix(File * fp)
  {
    fp->write(" {");
    // has no m_node! 
    if(m_nextNode)
      m_nextNode->printPostfix(fp);  //datamember vardecls


    NodeBlockFunction * func = findMainFunctionNode();
    if(func)
      func->printPostfix(fp);
    else
      fp->write(" <NOMAIN>");  //not an error

    fp->write(" }");
  }


  const char * NodeBlockClass::getName()
  {
    return "{}";
  }


  const std::string NodeBlockClass::prettyNodeName()
  {
    return nodeName(__PRETTY_FUNCTION__);
  }


  UlamType * NodeBlockClass::checkAndLabelType()
  { 
    // label all the function definition bodies first.
    m_functionST.labelTableOfFunctions();

    if(m_nextNode)
      m_nextNode->checkAndLabelType();  //side-effect for datamember vardelcs

    return Node::checkAndLabelType();
  }


  EvalStatus NodeBlockClass::eval()
  {
#if 0
    //determine size of stackframe (enable for test t3116
    u32 stackframetotal = getSizeOfFuncSymbolsInTable();
    u32 numberoffuncs = getNumberOfFuncSymbolsInTable();
    {
      std::ostringstream msg;
      msg << stackframetotal << " is the total stackframe size required for " << numberoffuncs << " functions";
      MSG(getNodeLocationAsString().c_str(), msg.str().c_str(), DEBUG);
    }
#endif

    evalNodeProlog(0); //new current frame pointer for nodeeval stack

    EvalStatus evs;

    if(m_nextNode)
      {
	u32 slot = makeRoomForNodeType(m_nextNode->getNodeType());
	evs = m_nextNode->eval();  //side-effect for datamember vardecls
	if(evs != NORMAL)
	  {
	    evalNodeEpilog();
	    return evs;
	  }
	m_state.m_nodeEvalStack.popArgs(slot);
      }

    NodeBlockFunction * funcNode = findMainFunctionNode();
    if(funcNode)
      {
	UlamType * funcType = funcNode->getNodeType();
	makeRoomForNodeType(funcType);  //Int return
	
	//makeRoomForNodeType(funcType, STACK); no args to main
	evs = funcNode->eval();
	if(evs != NORMAL)
	  {
	    evalNodeEpilog();
	    return evs;
	  }

	UlamValue mainUV = m_state.m_nodeEvalStack.popArg();

	// now what to do with the answer???
	//assignReturnValueToStack(mainUV);
      }

    evalNodeEpilog();
    return NORMAL;
  }


  //override to check both variables and function names.
  bool NodeBlockClass::isIdInScope(u32 id, Symbol * & symptrref)
  {
    return (m_ST.isInTable(id, symptrref) || isFuncIdInScope(id, symptrref));
  }


  bool NodeBlockClass::isFuncIdInScope(u32 id, Symbol * & symptrref)
  {
    return m_functionST.isInTable(id, symptrref);
  }


  void NodeBlockClass::addFuncIdToScope(u32 id, Symbol * symptr)
  {
    m_functionST.addToTable(id, symptr);
  }


  u32 NodeBlockClass::getNumberOfFuncSymbolsInTable()
  {
    return m_functionST.getTableSize();
  }


  u32 NodeBlockClass::getSizeOfFuncSymbolsInTable()
  {
    return m_functionST.getTotalSymbolSize();
  }


  //don't set nextNode since it'll get deleted with program.
  NodeBlockFunction * NodeBlockClass::findMainFunctionNode()
  {
    Symbol * fsym;
    NodeBlockFunction * func = NULL;
    u32 mainid = m_state.m_pool.getIndexForDataString("main");
    if(isFuncIdInScope(mainid, fsym))
      {
	func = ((SymbolFunction * ) fsym)->getFunctionNode();
      }
    return func;
  }

} //end MFM