#include "TestCase_EndToEndCompiler.h"

namespace MFM {

  BEGINTESTCASECOMPILER(t3102_test_compiler_bitwiseand)
  {
    std::string GetAnswerKey()
    {
      return std::string(" { Int a(3);  Int b(2);  Int c(2);  Bool d(true);  Int main() {  a 3 = b 2 = d c a b & = cast = c } }\n");
    }
    
    std::string PresetTest(FileManagerString * fms)
    {
      bool rtn1 = fms->add("a.ulam","ulam { Int a, b, c; Bool d; use main; a = 3; b = 2; d = c = a & b; c;} }");
      bool rtn2 = fms->add("main.ulam", "Int main() {");

      if(rtn1 & rtn2)
	return std::string("a.ulam");
      
      return std::string("");
    }
  }
  
  ENDTESTCASECOMPILER(t3102_test_compiler_bitwiseand)
  
} //end MFM

