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
class GETMath;

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
    void SetDebugMode(Bool_t value = 1);
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
    //! Return GETMath object pointer if there exists. If not, create a new one and return it.
    GETMath *GetGETMath();
    //! Return the frame type. This is used when drawing merged frame.
    Int_t GetFrameType();

    //! Return the number of frames counted by CountFrames() method.
    Int_t GetCurrentFrameID();
    //! Return the number of inner frame in the current frame.
    Int_t GetCurrentInnerFrameID();
    //! Return specific frame of the given frame number. If **frameIdx** is -1, this method returns next frame.
    GETFrame *GetFrame(Int_t frameIdx = -1);
    GETFrame *GetFrame(Int_t frameIdx, Int_t innerFrameIdx);

  private:
    //! Initialize variables used in the class.
    void Initialize();

    //! Print the information of the returned frame.
    void PrintFrameInfo(Int_t frameID, Int_t eventID, Int_t coboID, Int_t asadID);

    //! Skip a frame for accessing frames behind. This method also used in merged frame to skip an inner frame.
    void SkipInnerFrame();

    //! Skip a merged frame for accessing frames behind.
    void SkipMergedFrame();

    //! Get needed information of given frame to the internal variables
    void ReadMergedFrameInfo();
    //! Get needed information of given inner frame to the internal variables
    void ReadInnerFrameInfo();

    //! Check if the file is end
    void CheckEOF();

    Int_t fFrameType;  //!< frame type. 0: normal frame, 1: event number merged, 2: event time merged
    Int_t fMergedHeaderSize; //!< header size of merged frame. For additional skip bytes when finding frame by frame number.
    Int_t fNumMergedFrames; //!< the number of merged frames. For additional skip bytes when finding frame by frame number.
    UInt_t fMergedFrameStartPoint; //!< byte number of the merged frame start point. For navigational feature in a merged frame.
    UInt_t fCurrentMergedFrameSize; //!< size of a merged frame of the frame ID **fCurrentFrameID**. For additional skip bytes when finding frame by frame number.
    UInt_t fCurrentInnerFrameSize; //!< size of an inner frame.

    Bool_t fDebugMode; //!< flag for debug mode

    std::ifstream fGraw;            //!< rawdata filestream
    UInt_t fFileSize; //!< file size
    std::vector<TString> fGrawList; //!< rawdata file list
    Int_t fCurrentGrawID;        //!< current file index in list

    GETFrame *fFrame;      //!< frame container
    Int_t fCurrentFrameID; //!< current frame index
    Int_t fCurrentInnerFrameID; //!< current inner frame index
    Bool_t fEOF; //!< check if end of file

    GETPlot *fGETPlot;     //!< GETPlot pointer
    GETMath *fGETMath;     //!< GETMath pointer

  ClassDef(GETDecoder, 1); //!< added for making dictionary by ROOT
};

#endif
