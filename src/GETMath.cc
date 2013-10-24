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

#include <cmath>

#include "GETMath.hh"
#include "Riostream.h"

ClassImp(GETMath);

GETMath::GETMath()
{
  Reset();
}

GETMath::~GETMath()
{
}

void GETMath::Add(Double_t value)
{
  fMean += (value - fMean)/(Double_t)(fNumValues + 1);
  
  if (fNumValues > 0)
    fRms = fNumValues*fRms/(Double_t)(fNumValues + 1) + pow(value - fMean, 2)/(Double_t)(fNumValues);

  fNumValues++;
}

Double_t GETMath::GetMean()
{
  return fMean;
}

Double_t GETMath::GetRMS()
{
  return sqrt(fRms);
}

void GETMath::Reset()
{
  fNumValues = 0;
  fMean = 0;
  fRms = 0;
}
