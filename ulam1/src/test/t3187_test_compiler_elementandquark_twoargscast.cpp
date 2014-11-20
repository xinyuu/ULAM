#include "TestCase_EndToEndCompiler.h"

namespace MFM {

  BEGINTESTCASECOMPILER(t3187_test_compiler_elementandquark_twoargscast)
  {
    std::string GetAnswerKey()
    {
      return std::string("Ue_Foo { Int(32) m_idx(40);  Int(4) m_x(9);  Int(4) m_y(4);  Int(32) test() {  m_x 9 cast = m_y 4 cast = m_idx ( m_x cast m_y cast )func = m_x cast m_y cast * return } }\nExit status: 36");
    }
    
    std::string PresetTest(FileManagerString * fms)
    {
      bool rtn1 = fms->add("Foo.ulam","ulam 1; use C2D; element Foo { Int m_idx; Int(4) m_x, m_y; Int func(Int i, Int j) { C2D c;  c.init(i,j); return c.getIndex(0,0); } Int test() { m_x = 9; m_y = 4;  m_idx = func((Int) m_x,(Int) m_y); return (m_x * m_y); } }\n"); //cast args; exit status (36) truncates to 4

      bool rtn2 = fms->add("C2D.ulam","quark C2D { Int(4) m_width; Int(4) m_height;  Void init(Int x, Int y) { m_width = x; m_height = y; return; } Void init() { m_width = 9; m_height = 4; return; /* event window overload */ } Int getIndex(Int a, Int b){return ((m_height-b) * m_width + (m_height-a)); } }\n");


      if(rtn1 & rtn2)
	return std::string("Foo.ulam");
      
      return std::string("");
    }
  }
  
  ENDTESTCASECOMPILER(t3187_test_compiler_elementandquark_twoargscast)
  
} //end MFM

