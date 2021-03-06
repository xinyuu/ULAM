#include "TestCase_EndToEndCompiler.h"

namespace MFM {

  BEGINTESTCASECOMPILER(t3112_test_compiler_funcdecl_incomplete_error)
  {
    std::string GetAnswerKey()
    {
      //./D.ulam:1:36: ERROR: Unexpected input!! Token: <;> after function declaration..
      return std::string("Uq_D { Void(0) m(Int(32) m, Bool(1) b[3]); }\n");
    }

    std::string PresetTest(FileManagerString * fms)
    {
      bool rtn1 = fms->add("D.ulam","quark D { Void m(Int m, Bool b[3]) ; }");

      if(rtn1)
	return std::string("D.ulam");

      return std::string("");
    }
  }

  ENDTESTCASECOMPILER(t3112_test_compiler_funcdecl_incomplete_error)

} //end MFM
