#include <iostream>
#include <stdio.h>
#include "UlamTypePtr.h"
#include "UlamValue.h"
#include "CompilerState.h"

namespace MFM {

  UlamTypePtr::UlamTypePtr(const UlamKeyTypeSignature key, CompilerState & state) : UlamType(key, state)
  {}


   ULAMTYPE UlamTypePtr::getUlamTypeEnum()
   {
     return Ptr;
   }

  const std::string UlamTypePtr::getUlamTypeVDAsStringForC()
    {
      return "VD::PTR";
    }


  bool UlamTypePtr::needsImmediateType()
  {
    return false;
  }


  bool UlamTypePtr::isMinMaxAllowed()
  {
    return false;
  }


  PACKFIT UlamTypePtr::getPackable()
  {
    return UNPACKED;
  }

} //end MFM
