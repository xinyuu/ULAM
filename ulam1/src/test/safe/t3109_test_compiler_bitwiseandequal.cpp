#include "TestCase_EndToEndCompiler.h"

namespace MFM {

  BEGINTESTCASECOMPILER(t3109_test_compiler_bitwiseandequal)
  {
    std::string GetAnswerKey()
    {
      return std::string(" { Int a(2);  Int b(2);  Int main() {  a 3 = b 2 = a b &= } }\n");
    }
    
    std::string PresetTest(FileManagerString * fms)
    {
      bool rtn1 = fms->add("a.ulam","ulam { Int a, b; Int main() {a = 3; b = 2; a&=b;} }");
      
      if(rtn1)
	return std::string("a.ulam");
      
      return std::string("");
    }
  }
  
  ENDTESTCASECOMPILER(t3109_test_compiler_bitwiseandequal)
  
} //end MFM

