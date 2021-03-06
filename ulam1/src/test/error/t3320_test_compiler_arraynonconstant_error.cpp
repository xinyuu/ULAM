#include "TestCase_EndToEndCompiler.h"

namespace MFM {

  BEGINTESTCASECOMPILER(t3320_test_compiler_arraynonconstant_error)
  {
    std::string GetAnswerKey()
    {
      /*
	A.ulam:1:58: ERROR: Array size specifier in [] is not a constant integer.
      */

      return std::string("Ue_A { Int(32) a(7);  Int(32) test() {  a 7 = } }\n");
    }

    std::string PresetTest(FileManagerString * fms)
    {
      bool rtn1 = fms->add("A.ulam","element A { Int a; Int test() {a = 7; Int c; c = 5; Int b[a]; b[4] = 2; return b[4]; } }");

      if(rtn1)
	return std::string("A.ulam");

      return std::string("");
    }
  }

  ENDTESTCASECOMPILER(t3320_test_compiler_arraynonconstant_error)

} //end MFM
