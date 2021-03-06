#include "TestCase_EndToEndCompiler.h"

namespace MFM {

  BEGINTESTCASECOMPILER(t3396_test_compiler_unseenfunccall_error)
  {
    std::string GetAnswerKey()
    {
      //./R.ulam:2:9: ERROR: Incomplete Class: Colony(UNKNOWN)<12> was never defined, fails sizing.
      //./R.ulam:2:9: ERROR: Incomplete Class: Colony(UNKNOWN)<12> was never defined, fails labeling.

      return std::string("Exit status: 0\nUe_A { /* empty class block */ }Ue_R { Int(32) test() {  typedef Colony(3u) Ish;  Colony(3u) ish;  ish ( 7u )setTailAge . 0 return } }\nUq_Colony { typedef Unsigned(UNKNOWN) Tail;  constant Unsigned(32) widthc = NONREADYCONST;  typedef Telomeree(width) Telo;  Telomeree(width) t( constant Unsigned(32) width = NONREADYCONST; );  <NOMAIN> }\nUq_Telomeree { constant Unsigned(32) width = NONREADYCONST;  typedef Unsigned(UNKNOWN) Tail;  Unsigned(UNKNOWN) age(0);  <NOMAIN> }\n");
    }

    std::string PresetTest(FileManagerString * fms)
    {
      //tripped upon by 3394, causes assert in Compiler after resolving loop, after counting Nav's before all iterations used up
      // because we start with R.ulam, and never see Colony.ulam.
      bool rtn2 = fms->add("A.ulam", "use R;\nuse Colony;\n element A{\n}\n");
      bool rtn3 = fms->add("R.ulam", "element R{\ntypedef Colony Ish;\n Int test() { Ish ish;\n ish.setTailAge(7u);\n return 0;\n}\n}");
      bool rtn1 = fms->add("Colony.ulam","use Telomeree;\n quark Colony{\nconstant Unsigned widthc = 3u;\n typedef Telomeree(widthc) Telo;\n typedef Telo.Tail Tail;\n Telo t;\n Void setTailAge(Unsigned newage) {\n t.setAge((Tail) newage);\n }\n }");
      bool rtn4 = fms->add("Telomeree.ulam","quark Telomeree(Unsigned width){\n typedef Unsigned(width) Tail;\n Tail age;\nTail getAge(){\n return age;\n}\nVoid setAge(Tail newAge){\nage = newAge;\n}\n }");

      if(rtn1 && rtn2 && rtn3 && rtn4)
	return std::string("R.ulam");

      return std::string("");
    }
  }

  ENDTESTCASECOMPILER(t3396_test_compiler_unseenfunccall_error)

} //end MFM
