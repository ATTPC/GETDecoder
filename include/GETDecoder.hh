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
#include <vector>

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
    //! Add the data file to the list of rawdata.
    void AddGraw(const Char_t *filename);
    //! Set the data file to the class.
    Bool_t SetData(Int_t index);
    //! Search the next file and set it if exists. Returns 1 if successful.
    Bool_t SetNextFile();
    //! Print rawdata file list on the screen.
    void ShowList();

    //! Return GETPlot object pointer if there exists. If not, create a new one and return it.
    GETPlot *GetGETPlot();
    //! Return the frame type. This is used when drawing merged frame.
    Int_t GetFrameType();

    //! Return the number of frames counted by CountFrames() method.
    Int_t GetCurrentFrameID();
    //! Return specific frame of given frame number. If frameIdx is -1, thie method returns next frame.
    GETFrame *GetFrame(Int_t frameIdx = -1);

  private:
    //! Initialize variables used in the class.
    void Initialize();

    //! Print the information of the returned frame.
    void PrintFrameInfo(Int_t frameID, Int_t eventID, Int_t coboID, Int_t asadID);

    Int_t fFrameType;  //!< frame type. 0: normal frame, 1: event number merged, 2: event time merged
    Int_t fMergedHeaderSize; //!< header size of merged frame. For additional skip bytes when finding frame by frame number.

    Bool_t fDebugMode; //!< flag for debug mode

    std::ifstream fGraw;            //!< rawdata filestream
    std::vector<TString> fGrawList; //!< rawdata file list
    Int_t fCurrentGrawID;        //!< current file index in list

    GETFrame *fFrame;      //!< frame container
    Int_t fCurrentFrameID; //!< current frame index

    GETPlot *fGETPlot;     //!< GETPlot pointer

  ClassDef(GETDecoder, 1); //!< added for making dictionary by ROOT
};

#endif
