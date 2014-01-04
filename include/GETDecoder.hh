// =================================================
//  GETDecoder Class
// 
// 
//  Author:
//    Genie Jhang ( geniejhang@majimak.com )
//  
//  Log:
//    - 2013. 09. 23
//      Start writing class
// =================================================

#ifndef _GETDECODER_H_
#define _GETDECODER_H_

#include <fstream>

#include "GETConfig.hh"

#include "TObject.h"
#include "TROOT.h"
#include "TString.h"

class GETFrame;
class GETPlot;

/** Read the raw file from GET electronics and process it into GETFrame class **/
class GETDecoder : public TObject
{
  public:
    //! Constructor
    GETDecoder();
    //! Constructor
    GETDecoder(Char_t *filename /*!< GRAW filename including path */);
    //! Destructor
    ~GETDecoder();

    //! Setting debug mode. If set to 1, more information is printed out on the screen.
    void SetDebugMode(Bool_t value);
    //! Set the data file to the class.
    Bool_t SetGraw(const Char_t *filename);

    //! Return GETPlot object pointer if there exists. If not, create a new one and return it.
    GETPlot *GetGETPlot();
    //! Return the number of frames counted by CountFrames() method.
    Int_t GetNumFrames();
    //! Return the frame number currently read and returned frame.
    Int_t GetCurrentFrameID();
    //! Return next frame.
    GETFrame *GetFrame();
    //! Return specific frame of given frame number.
    GETFrame *GetFrame(Int_t frameIdx);

  private:
    //! Initialize variables used in the class.
    void Initialize();

    //! Print the information of the returned frame.
    void PrintFrameInfo(Int_t frameID, Int_t eventID, Int_t coboID, Int_t asadID);

    Bool_t fDebugMode; //!< flag for debug mode

    //! Set the data file to the class.
    Bool_t SetFile(const Char_t *filename);
    //! Count the number of frames in the set file and continuing files.
    void CountFrames();
    //! Searche the next file and set it if exists. Returns 1 if successful.
    Bool_t IsNextFile();

    Int_t fNumFrames; //!< the number of frames in data file
  
    std::ifstream fGraw;   //!< rawdata filestream
    TString fFirstGraw;    //!< first rawdata filename
    TString fNextGraw;     //!< next rawdata filename

    GETFrame *fFrame;      //!< frame container
    Int_t fCurrentFrameID; //!< current frame index

    GETPlot *fGETPlot;     //!< GETPlot pointer

  ClassDef(GETDecoder, 1); //!< added for making dictionary by ROOT
};

#endif
