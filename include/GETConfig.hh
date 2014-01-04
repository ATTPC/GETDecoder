// =================================================
//  GETConfig Class
// 
//  Author:
//    Genie Jhang ( geniejhang@majimak.com )
// =================================================

#ifndef _GETCONFIG_H_
#define _GETCONFIG_H_

#include "TROOT.h"

static const Int_t GETNumTbs = 512;

/**
  * # This class is very important!!!!
  * ## Open `GETConfig.hh` file and change the value of `GETNumTbs` into the one used in taking data.
  * ## After that, don't forget to recompile the package.
  * ## Ignore the others. Those are just dummies to compiled automatically when the variable is changed.
 **/
class GETConfig : public TObject
{
  public:
    GETConfig() {};
    virtual ~GETConfig() {};
  
  ClassDef(GETConfig, 1);
};

#endif
