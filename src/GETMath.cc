// =================================================
//  GETMath Class
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

/*!
This method uses the formulae below to calculate mean and RMS. <br>
\f$\text{mean}\Rightarrow \mu_{n+1} = \mu_n + \displaystyle\frac{x_n - \mu_n}{n + 1},\quad(n\leq0)\f$ <br>
\f$\text{RMS}\Rightarrow\sigma^2_{n+1} = \displaystyle\frac{n}{n + 1}\sigma^2_n + \displaystyle\frac{(x_n - \mu_{n+1})^2}{n},\quad(n>0)\f$ 
*/
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
