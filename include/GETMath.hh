// =================================================
//  GETMath Class
// 
//  Description:
//    Thinking what to write
// 
//  Author:
//    Genie Jhang ( geniejhang@majimak.com )
//  
//  Log:
//    - 2013. 10. 24
//      Start writing class
// =================================================

#ifndef _GETMATH_H_
#define _GETMATH_H_

#include "TObject.h"
#include "TROOT.h"

class GETMath : public TObject
{
  public:
    GETMath();
    virtual ~GETMath();

    void Add(Double_t value);
    Double_t GetMean();
    Double_t GetRMS();

    void Reset();

  private:
    Int_t fNumValues;
    Double_t fMean;
    Double_t fRms;

  ClassDef(GETMath, 1);
};

#endif
