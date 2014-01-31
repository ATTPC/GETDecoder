// =================================================
//  GETDecoder Class
// 
//  Author:
//    Genie Jhang ( geniejhang@majimak.com )
//  
//  Log:
//    - 2013. 09. 23
//      Start writing class
// =================================================

#include <iostream>
#include <iomanip>
#include <fstream>
#include <arpa/inet.h>

#include "TSystem.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TString.h"

#include "GETDecoder.hh"
#include "GETFrame.hh"
#include "GETPlot.hh"

ClassImp(GETDecoder);

GETDecoder::GETDecoder()
{
  /**
    * If you use this constructor, you have to add the rawdata using
    * AddGraw() method and set the file with SetData() method, manually.
   **/
   
  Initialize();
}

GETDecoder::GETDecoder(Char_t *filename)
{
  /**
    * Automatically add the rawdata file to the list
    * and set the file to read.
   **/

  Initialize();
  AddGraw(filename);
  SetData(0);
}

GETDecoder::~GETDecoder()
{
  if(fFrame != 0)
    delete fFrame;
}

void GETDecoder::Initialize()
{
  fFrameType = -1;
  fMergedHeaderSize = 0;

  fDebugMode = 0;

  fCurrentGrawID = -1;

  fFrame = 0;
  fCurrentFrameID = -1;

  fGETPlot = 0;
}

void GETDecoder::SetDebugMode(Bool_t value)
{
  fDebugMode = value;
}

void GETDecoder::AddGraw(const Char_t *filename)
{
  /**
    * Check if there is a file named `filename`. If exists, add it to the list.
   **/

  TString nextGraw = filename;

  TObjArray *pathElements = 0;
  pathElements = nextGraw.Tokenize("/");

  Int_t numElements = pathElements -> GetLast();

  TString path = "";
  if (numElements == 0)
    path = ".";
  else {
    path = "/";
    for (Int_t i = 0; i < numElements; i++) {
      path.Append(((TObjString *) pathElements -> At(i)) -> GetString());
      path.Append("/");
    }
  }

  TString tempGrawFile = ((TObjString *) pathElements -> Last()) -> GetString();

  nextGraw = gSystem -> Which(path, tempGrawFile);
  if (!nextGraw.EqualTo("")) {
    std::cout << "== Data file found: " << filename << std::endl;

    Bool_t isExist = 0;
    for (Int_t iIdx = 0; iIdx < fGrawList.size(); iIdx++) {
      if (fGrawList.at(0) == nextGraw) {
        std::cout << "== The file already exists in the list!" << std::endl;
        isExist = 1;
      }
    }

    if (!isExist)
      fGrawList.push_back(nextGraw);
  } else
    std::cout << "== Data file not found: " << filename << std::endl;

  delete pathElements;

  return;
}

Bool_t GETDecoder::SetData(Int_t index)
{
  if (fGrawList.size() < index) {
    std::cout << "== End of list!" << std::endl;

    return 0;
  }

  if (fGraw.is_open())
    fGraw.close();

  TString filename = fGrawList.at(index);

  fGraw.open(filename.Data(), std::ios::in|std::ios::binary);

  if (!(fGraw.is_open())) {
    std::cout << "== GRAW file open error! Check it exists!" << std::endl;

    return 0;
  } else {
    std::cout << "== " << filename << " is opened!" << std::endl;
    fGraw.seekg(0);

    UShort_t headerSize = 0;
    fGraw.ignore(8);
    fGraw.read(reinterpret_cast<Char_t *>(&headerSize), 2);
    fGraw.seekg(0);

    if (headerSize == 20) { // Merged frame by event number
      fFrameType = 1;
      fMergedHeaderSize = headerSize;
    } else if (headerSize == 24) { // Merged frame by event time
      fFrameType = 2;
      fMergedHeaderSize = headerSize;
    } else { // Normal frame by CoBo
      fFrameType = 0;
      fMergedHeaderSize = 0;
    }


    std::cout << "== Frame Type: ";
    if (fFrameType == 0) std::cout << "Normal CoBo frame";
    else if (fFrameType == 1) std::cout << "Event number merged frame";
    else if (fFrameType == 2) std::cout << "Event time merged frame";
    std::cout << std::endl;

    fCurrentGrawID = index;

    return 1;
  }
}

Bool_t GETDecoder::SetNextFile()
{
  return SetData(fCurrentGrawID + 1);
}

void GETDecoder::ShowList()
{
  std::cout << "== Index GRAW file" << std::endl;
  for (Int_t iItem = 0; iItem < fGrawList.size(); iItem++) {
    if (iItem == fCurrentGrawID)
      std::cout << " *" << std::setw(6);
    else
      std::cout << std::setw(8);

    std::cout << iItem << "  " << fGrawList.at(iItem) << std::endl;
  }
}

GETPlot *GETDecoder::GetGETPlot()
{
  if (!fGETPlot)
    fGETPlot = new GETPlot(this);

  return fGETPlot;
}

Int_t GETDecoder::GetCurrentFrameID()
{
  return fCurrentFrameID;
}

GETFrame *GETDecoder::GetFrame(Int_t frameNo)
{
  if (frameNo == -1)
    frameNo = fCurrentFrameID + 1;

  if (fCurrentFrameID == frameNo) {
    if (fDebugMode)
      PrintFrameInfo(frameNo, fFrame -> GetEventID(), fFrame -> GetCoboID(), fFrame -> GetAsadID());

    return fFrame;
  } else if (frameNo < -1) {
    std::cout << "== Frame number should be a positive integer!" << std::endl;

    return 0;
  }

  while (1) {
    UInt_t frameSize;
    UShort_t headerSize;
    UInt_t nItems;
    UInt_t eventIdx;
    UShort_t coboIdx;
    UShort_t asadIdx;

    while (frameNo > fCurrentFrameID + 1) {
      if (fDebugMode)
        std::cout << "== Skipping Frame No. " << fCurrentFrameID + 1 << std::endl;

      fGraw.ignore(1);

      fGraw.read(reinterpret_cast<Char_t *>(&frameSize), 3);

      if (fGraw.eof()) {
        std::cout << "== End of the file! (last frame: " << fCurrentFrameID << ")" << std::endl;

        return 0;
      }

      if (fFrameType != 0)
        frameSize = (htonl(frameSize) >> 8);
      else
        frameSize = (htonl(frameSize) >> 8)*64;

      fGraw.seekg((Int_t)fGraw.tellg() - 4 + frameSize + fMergedHeaderSize);

      fCurrentFrameID++;
    }

    if (frameNo < fCurrentFrameID) {
      fCurrentFrameID = -1;
      fGraw.clear();
      fGraw.seekg(0);

      return GetFrame(frameNo);
    }

    fGraw.ignore(fMergedHeaderSize);

    fGraw.ignore(8);
    fGraw.read(reinterpret_cast<Char_t *>(&headerSize), 2);
    fGraw.ignore(2);
    fGraw.read(reinterpret_cast<Char_t *>(&nItems), 4);
    fGraw.ignore(6);
    fGraw.read(reinterpret_cast<Char_t *>(&eventIdx), 4);
    fGraw.read(reinterpret_cast<Char_t *>(&coboIdx), 1);
    fGraw.read(reinterpret_cast<Char_t *>(&asadIdx), 1);

    if (fGraw.eof()) {
      std::cout << "== End of the file! (last frame: " << fCurrentFrameID << ")" << std::endl;

      return 0;
    }

    headerSize = htons(headerSize)*64;
    nItems = htonl(nItems);
    eventIdx = htonl(eventIdx);
    coboIdx = (htons(coboIdx) >> 8);
    asadIdx = (htons(asadIdx) >> 8);

    if (fDebugMode)
      PrintFrameInfo(frameNo, eventIdx, coboIdx, asadIdx);

    if (fFrame != 0)
      delete fFrame;

    fFrame = new GETFrame();
    fFrame -> SetEventID(eventIdx);
    fFrame -> SetCoboID(coboIdx);
    fFrame -> SetAsadID(asadIdx);
    fFrame -> SetFrameID(frameNo);

    fGraw.seekg((Int_t) fGraw.tellg() - 28 + headerSize);

    UInt_t data;
    for (Int_t iItem = 0; iItem < nItems; iItem++) {
      fGraw.read(reinterpret_cast<Char_t *>(&data), 4);
      data = htonl(data);

      UShort_t agetIdx = ((data & 0xc0000000) >> 30);
      UShort_t chanIdx = ((data & 0x3f800000) >> 23);
      UShort_t buckIdx = ((data & 0x007fc000) >> 14);
      UShort_t sample = (data & 0x00000fff);         

      if (chanIdx >= 68 || agetIdx >= 4 || buckIdx >= GETNumTbs)
        continue; 
                                                                     
      fFrame -> SetRawADC(agetIdx, chanIdx, buckIdx, sample); 
    }

    fCurrentFrameID = frameNo;

    return fFrame;
  }
}

void GETDecoder::PrintFrameInfo(Int_t frameNo, Int_t eventID, Int_t coboID, Int_t asadID)
{
  std::cout << "== Frame Info -";
  std::cout << " Frame:" << frameNo;
  std::cout << " Event:" << eventID;
  std::cout << " CoBo:" << coboID;
  std::cout << " AsAd:" << asadID << std::endl;
}
