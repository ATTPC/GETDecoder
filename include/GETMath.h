// =================================================
//  GETMath Class
// 
//  Author:
//    Genie Jhang ( geniejhang@majimak.com )
//  
//  Log:
//    - 2013. 10. 24
//      Start writing class
//
//  Note:
//    2014. 05. 16          Adapted for VIGRU
// =================================================

#ifndef _GETMATH_H_
#define _GETMATH_H_

#include "TObject.h"
#include "TROOT.h"


/*!
  * Basically, what this class does is the same as TH1D class in ROOT. 
  * The difference is that this class only provides the **mean** and **RMS** values.
  * Nothing else!
 **/

class GETMath : public TObject
{
  public:
    //! Constructor
    GETMath();
    //! Destructor
    virtual ~GETMath();

    //! With the mean and RMS, calculated before, use the **value** to calculate new **mean** and **RMS** recursively.
    void Add(Double_t value);
    //! Return the calculated **mean** value.
    Double_t GetMean();
    //! Return the calculated **RMS** value.
    Double_t GetRMS();
    
    //! Reset all the values.
    void Reset();

  private:
    Int_t fNumValues; //!< Number of values added
    Double_t fMean; //!< mean value
    Double_t fRms; //!< RMS value
    
  //! Added for dictionary making by ROOT
  ClassDef(GETMath, 1); 
};

#endif
