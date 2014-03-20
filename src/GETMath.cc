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

#include "GETDecoder.hh"
#include "GETFrame.hh"

ClassImp(GETMath);

GETMath::GETMath()
{
  Reset();
}

GETMath::GETMath(GETDecoder *decoder)
{
  Reset();
  fDecoder = decoder;
}

GETMath::~GETMath()
{
}

void GETMath::Add(Double_t value)
{
  /**
    * This method uses the formulae below to calculate mean and RMS. <br>
    * \f$\text{mean}\Rightarrow \mu_{n+1} = \mu_n + \displaystyle\frac{x_{n+1} - \mu_n}{n + 1},\quad(n\geq0)\f$ <br>
    * \f$\text{RMS}\Rightarrow\sigma^2_{n+1} = \displaystyle\frac{n}{n + 1}\sigma^2_n + \displaystyle\frac{(x_{n+1} - \mu_{n+1})^2}{n},\quad(n>0)\f$ 
   **/
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

Double_t **GETMath::GetAverage(Int_t numChannels, Int_t *chList, Int_t frameNo)
{
  /**
    * Explanation will be here.
   **/

  for (Int_t iAget = 0; iAget < 4; iAget++)
    for (Int_t iTb = 0; iTb < GETNumTbs; iTb++)
      fAdc[iAget][iTb] = 0;

  if (fDecoder == NULL) {
    std::cout << "== GETDecoder is not set!" << std::endl;

    return 0;
  }

  if (frameNo == -1)
    fFrame = fDecoder -> GetFrame();
  else
    fFrame = fDecoder -> GetFrame(frameNo);

  if (!fFrame)
    return 0;

  std::cout << "== Calculating average of frame " << fDecoder -> GetCurrentFrameID();
  if (numChannels == 0)
    std::cout << " of all channels" << std::endl;
  else {
    if (numChannels > 0)
      std::cout << " of channels" << std::endl;
    else
      std::cout << " of all channels except" << std::endl;
    std::cout << " > ";

    for (Int_t iCh = 0; iCh < abs(numChannels); iCh++)
      std::cout << chList[iCh] << " ";
    std::cout << std::endl;
  }

  for (Int_t iAget = 0; iAget < 4; iAget++) {
    Int_t divider = 0;
    Double_t tb[GETNumTbs] = {0};

    for (Int_t iCh = 0; iCh < 68; iCh++) {
      Bool_t isSkip = 0;
      if (numChannels < 0) {
        for (Int_t iMask = 0; iMask < -numChannels; iMask++)
          if (iCh == chList[iMask]) {
            isSkip = 1;
            break;
          }
      } else if (numChannels > 0) {
        isSkip = 1;

        for (Int_t iDraw = 0; iDraw < numChannels; iDraw++)
          if (iCh == chList[iDraw]) {
            isSkip = 0;
            break;
          }
      }

      if (isSkip)
        continue;

      Int_t *rawadc = fFrame -> GetRawADC(iAget, iCh);

      for (Int_t iTb = 0; iTb < GETNumTbs; iTb++) {
        tb[iTb] = iTb;
        fAdc[iAget][iTb] += rawadc[iTb];
      }

      divider++;
    }

    for (Int_t iTb = 0; iTb < GETNumTbs; iTb++) 
      fAdc[iAget][iTb] /= (Double_t) divider;
  }

  return fAdc;
}

void GETMath::Reset()
{
  fDecoder = 0;
  fFrame = 0;

  fNumValues = 0;
  fMean = 0;
  fRms = 0;

  for (Int_t iAget = 0; iAget < 4; iAget++)
    fAdc[iAget] = new Double_t[GETNumTbs];
}
