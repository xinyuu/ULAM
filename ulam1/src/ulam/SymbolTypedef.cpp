#include "SymbolTypedef.h"

namespace MFM {

  SymbolTypedef::SymbolTypedef(u32 id, UlamType * utype) : Symbol(id, utype){}

  SymbolTypedef::~SymbolTypedef()
  {}

  bool SymbolTypedef::isTypedef()
  {
    return true;
  }

} //end MFM