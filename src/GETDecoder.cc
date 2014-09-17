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
#include "GETMath.hh"

ClassImp(GETDecoder);

GETDecoder::GETDecoder()
{
  /**
    * If you use this constructor, you have to add the rawdata using
    * AddData() method and set the file with SetData() method, manually.
   **/
   
  Initialize();
}

GETDecoder::GETDecoder(TString filename)
{
  /**
    * Automatically add the rawdata file to the list
    * and set the file to read.
   **/

  Initialize();
  AddData(filename);
  SetData(0);
}

GETDecoder::~GETDecoder()
{
  /*
  if (fFrame != 0)
    delete fFrame;

  if (fGETPlot != 0)
    delete fGETPlot;

  if (fGETMath != 0)
    delete fGETMath;
    */
}

void GETDecoder::Initialize()
{
  fNumTbs = 512;

  fFrameType = -1;
  fMergedHeaderSize = 0;
  fNumMergedFrames = 0;
  fCurrentMergedFrameSize = 0;

  fDebugMode = kFALSE;

  fCurrentDataID = -1;
  fIsAutoReload = kTRUE;
  fIsPositivePolarity = kFALSE;

  fFrame = 0;
  fCurrentFrameID = -1;
  fCurrentInnerFrameID = -1;

  fGETMath = 0;

  fGETPlot = 0;
  fEOF = kFALSE;
}

void GETDecoder::SetNumTbs(Int_t value)
{
  fNumTbs = value;
}

void GETDecoder::SetDebugMode(Bool_t value)
{
  fDebugMode = value;
}

Bool_t GETDecoder::AddData(TString filename)
{
  /**
    * Check if there is a file named `filename`. If exists, add it to the list.
   **/

  TString nextData = filename;

  TObjArray *pathElements = 0;
  pathElements = nextData.Tokenize("/");

  Int_t numElements = pathElements -> GetLast();

  TString path = "";
  if (numElements == 0)
    path = gSystem -> pwd();
  else {
    for (Int_t i = 0; i < numElements; i++) {
      path.Append(((TObjString *) pathElements -> At(i)) -> GetString());
      path.Append("/");
    }
  }

  TString tempDataFile = ((TObjString *) pathElements -> Last()) -> GetString();

  nextData = gSystem -> Which(path, tempDataFile);
  if (!nextData.EqualTo("")) {
    std::cout << "== [GETDecoder] Data file found: " << filename << std::endl;

    Bool_t isExist = 0;
    for (Int_t iIdx = 0; iIdx < fDataList.size(); iIdx++) {
      if (fDataList.at(0) == nextData) {
        std::cout << "== [GETDecoder] The file already exists in the list!" << std::endl;
        isExist = 1;
      }
    }

    if (!isExist)
      fDataList.push_back(nextData);
  } else {
    std::cout << "== [GETDecoder] Data file not found: " << filename << std::endl;

    return kFALSE;
  }

  delete pathElements;

  return kTRUE;
}

Bool_t GETDecoder::SetData(Int_t index)
{
  if (index >= fDataList.size()) {
    std::cout << "== [GETDecoder] End of list!" << std::endl;

    return kFALSE;
  }

  if (fData.is_open())
    fData.close();

  fEOF = kFALSE;

  TString filename = fDataList.at(index);

  fData.open(filename.Data(), std::ios::in|std::ios::ate|std::ios::binary);

  if (!(fData.is_open())) {
    std::cout << "== [GETDecoder] GRAW file open error! Check it exists!" << std::endl;

    return kFALSE;
  } else {
    fFileSize = fData.tellg();

    std::cout << "== [GETDecoder] " << filename << " is opened!" << std::endl;
    fData.seekg(0);

    UShort_t metaType = 0;
    UShort_t headerSize = 0;
    UInt_t numMergedFrames = 0;
    fData.read(reinterpret_cast<Char_t *>(&metaType), 1);
    fData.seekg(8);
    fData.read(reinterpret_cast<Char_t *>(&headerSize), 2);
    fData.seekg(0);

    /*
      Note:
        In the merged frame data file, merged frame header appears only once
        in front of merged frames.
    */

    fEndianness = ((metaType&0x80) >> 7);
    if (fEndianness == kBig) // First bit of the first byte determines endianness. 0:Big, 1:Little
      headerSize = htons(headerSize);

    if (headerSize == 20) { // Merged frame by event number
      fFrameType = kMergedID;
      fMergedHeaderSize = headerSize;
    } else if (headerSize == 24) { // Merged frame by event time
      fFrameType = kMergedTime;
      fMergedHeaderSize = headerSize;
    } else { // Normal frame by CoBo
      fFrameType = kNormal;
      fMergedHeaderSize = 0;
      fNumMergedFrames = 0;
    }

    std::cout << "== [GETDecoder] Frame Type: ";
    if (fFrameType == kNormal) std::cout << "Normal CoBo frame";
    else if (fFrameType == kMergedID) std::cout << "Event number merged frame";
    else if (fFrameType == kMergedTime) std::cout << "Event time merged frame";
    std::cout << std::endl;

    fCurrentDataID = index;

    return kTRUE;
  }
}

Bool_t GETDecoder::SetNextFile()
{
  return SetData(fCurrentDataID + 1);
}

void GETDecoder::SetNoAutoReload(Bool_t value)
{
  fIsAutoReload = value;
}

void GETDecoder::SetPositivePolarity(Bool_t value)
{
  fIsPositivePolarity = value;
}

void GETDecoder::ShowList()
{
  std::cout << "== [GETDecoder] Index GRAW file" << std::endl;
  for (Int_t iItem = 0; iItem < fDataList.size(); iItem++) {
    if (iItem == fCurrentDataID)
      std::cout << " *" << std::setw(6);
    else
      std::cout << std::setw(8);

    std::cout << iItem << "  " << fDataList.at(iItem) << std::endl;
  }
}

Int_t GETDecoder::GetNumData()
{
  return fDataList.size();
}

TString GETDecoder::GetDataName(Int_t index)
{
  if (index >= fDataList.size()) {
    std::cout << "== [GETDecoder] Size of the list is " << fDataList.size() << "!" << std::endl;

    return TString("No data!");
  }

  return fDataList.at(index);
}

Int_t GETDecoder::GetNumTbs()
{
  return fNumTbs;
}

GETPlot *GETDecoder::GetGETPlot()
{
  if (!fGETPlot)
    fGETPlot = new GETPlot(this);

  return fGETPlot;
}

GETMath *GETDecoder::GetGETMath()
{
  if (!fGETMath)
    fGETMath = new GETMath(this);

  return fGETMath;
}

Int_t GETDecoder::GetFrameType()
{
  return fFrameType;
}

Int_t GETDecoder::GetCurrentFrameID()
{
  return fCurrentFrameID;
}

Int_t GETDecoder::GetCurrentInnerFrameID()
{
  return fCurrentInnerFrameID;
}

GETFrame *GETDecoder::GetFrame(Int_t frameNo)
{
  if (fFrameType != kNormal)
    return GetFrame(frameNo, -1);

  if (frameNo == -1)
    frameNo = fCurrentFrameID + 1;

  if (fCurrentFrameID == frameNo) {
    if (fDebugMode)
      PrintFrameInfo(frameNo, fFrame -> GetEventID(), fFrame -> GetCoboID(), fFrame -> GetAsadID());

    return fFrame;
  } else if (frameNo < -1) {
    std::cout << "== [GETDecoder] Frame number should be a positive integer!" << std::endl;

    return 0;
  }

  while (1) {
    UInt_t frameSize;
    UShort_t headerSize;
    UInt_t nItems;
    UInt_t eventIdx;
    UShort_t coboIdx;
    UShort_t asadIdx;

    // Skip the frames until it reaches the given frame number, frameNo.
    while (frameNo > fCurrentFrameID + 1) {
      if (fDebugMode)
        std::cout << "== [GETDecoder] Skipping Frame No. " << fCurrentFrameID + 1 << std::endl;

      fData.ignore(1);

      fData.read(reinterpret_cast<Char_t *>(&frameSize), 3);

      if (fData.eof()) {
        std::cout << "== [GETDecoder] End of the file! (last frame: " << fCurrentFrameID << ")" << std::endl;

        if (SetNextFile() && fIsAutoReload)
          return GetFrame(0);

        return 0;
      }

      frameSize = (htonl(frameSize) >> 8)*64;

      fData.seekg((Int_t)fData.tellg() - 4 + frameSize);

      fCurrentFrameID++;
    }

    if (frameNo < fCurrentFrameID) {
      fCurrentFrameID = -1;
      fData.clear();
      fData.seekg(0);

      return GetFrame(frameNo);
    }

    fData.ignore(8);
    fData.read(reinterpret_cast<Char_t *>(&headerSize), 2);
    fData.ignore(2);
    fData.read(reinterpret_cast<Char_t *>(&nItems), 4);
    fData.ignore(6);
    fData.read(reinterpret_cast<Char_t *>(&eventIdx), 4);
    fData.read(reinterpret_cast<Char_t *>(&coboIdx), 1);
    fData.read(reinterpret_cast<Char_t *>(&asadIdx), 1);

    if (fData.eof()) {
      std::cout << "== [GETDecoder] End of the file! (last frame: " << fCurrentFrameID << ")" << std::endl;

      if (SetNextFile() && fIsAutoReload)
        return GetFrame(0);

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
    fFrame -> SetPolarity((fIsPositivePolarity ? +1 : -1));

    fData.seekg((Int_t) fData.tellg() - 28 + headerSize);

    UInt_t data;
    for (Int_t iItem = 0; iItem < nItems; iItem++) {
      fData.read(reinterpret_cast<Char_t *>(&data), 4);
      data = htonl(data);

      UShort_t agetIdx = ((data & 0xc0000000) >> 30);
      UShort_t chanIdx = ((data & 0x3f800000) >> 23);
      UShort_t buckIdx = ((data & 0x007fc000) >> 14);
      UShort_t sample = (data & 0x00000fff);         

      if (chanIdx >= 68 || agetIdx >= 4 || buckIdx >= 512)
        continue; 
                                                                     
      fFrame -> SetRawADC(agetIdx, chanIdx, buckIdx, sample); 
    }

    fCurrentFrameID = frameNo;

    return fFrame;
  }
}

GETFrame *GETDecoder::GetFrame(Int_t frameNo, Int_t innerFrameNo)
{
  if (fFrameType == kNormal)
    return GetFrame(frameNo);

  ReadMergedFrameInfo();

  if (frameNo == -1 && innerFrameNo == -1) {
    if (fEOF) {
      std::cout << "== [GETDecoder] End of the file! (last frame: " << fCurrentFrameID << ")" << std::endl;

      if (SetNextFile() && fIsAutoReload)
        return GetFrame(0, 0);

      return 0;
    } else if (fCurrentFrameID == -1 && fCurrentInnerFrameID == -1) {
      frameNo = 0;
      innerFrameNo = 0;
    } else if (fCurrentInnerFrameID + 1 == fNumMergedFrames) {
      frameNo = fCurrentFrameID + 1;
      innerFrameNo = 0;
      fCurrentInnerFrameID = -1;
    } else {
      frameNo = fCurrentFrameID;
      innerFrameNo = fCurrentInnerFrameID + 1;
    }
  } else if (frameNo == -1 && innerFrameNo != -1) {
    frameNo = fCurrentFrameID + 1;
  } else if (frameNo != -1 && innerFrameNo == -1) {
    innerFrameNo = fCurrentInnerFrameID + 1;

    if (frameNo == fCurrentFrameID && fCurrentInnerFrameID + 1 == fNumMergedFrames) {
      std::cout << "== [GETDecoder] Reached the end of the merged frame!" << std::endl;
      
      return 0;
    } else if (frameNo != fCurrentFrameID) {
      fCurrentInnerFrameID = -1;
      innerFrameNo = 0;
    }
  }

  if (fCurrentFrameID == frameNo && fCurrentInnerFrameID == innerFrameNo) {
    if (fDebugMode)
      PrintFrameInfo(frameNo, fFrame -> GetEventID(), fFrame -> GetCoboID(), fFrame -> GetAsadID());

    return fFrame;
  } else if (frameNo < -1 || innerFrameNo < -1) {
    std::cout << "== [GETDecoder] Frame number or inner frame number should be a positive integer!" << std::endl;

    return 0;
  } else if (innerFrameNo >= fNumMergedFrames) {
    std::cout << "== [GETDecoder] Inner frame number should be smaller than " << fNumMergedFrames << std::endl;

    return 0;
  }

  if (frameNo < fCurrentFrameID || innerFrameNo < fCurrentInnerFrameID) {
    fCurrentFrameID = -1;
    fCurrentInnerFrameID = -1;
    fData.clear();
    fData.seekg(0);
    ReadMergedFrameInfo();
    CheckEOF();

    return GetFrame(frameNo, innerFrameNo);
  }

  // Skip the frames until it reaches the given frame number, frameNo.
  while ((frameNo > fCurrentFrameID) && (frameNo > 0)) {
    if (fCurrentFrameID == -1) {
      fCurrentFrameID++;
      continue;
    }

    fData.clear();

    if (fDebugMode)
      std::cout << "== [GETDecoder] Skipping Frame No. " << fCurrentFrameID << std::endl;

    SkipMergedFrame();
    CheckEOF();
    ReadMergedFrameInfo();

    if (fEOF) {
      std::cout << "== [GETDecoder] End of the file! (last frame: " << fCurrentFrameID << ")" << std::endl;
      fCurrentInnerFrameID = fNumMergedFrames - 1;

      if (SetNextFile() && fIsAutoReload)
        return GetFrame(0, 0);

      return 0;
    }

    fCurrentFrameID++;
  }

  if (fEOF) {
    std::cout << "== [GETDecoder] End of the file! (last frame: " << fCurrentFrameID << ")" << std::endl;

    if (SetNextFile() && fIsAutoReload)
      return GetFrame(0, 0);

    return 0;
  }

  UInt_t frameSize;
  UShort_t headerSize;
  UInt_t nItems;
  UInt_t eventIdx;
  UShort_t coboIdx;
  UShort_t asadIdx;

  fData.ignore(fMergedHeaderSize);

  for (Int_t iSkip = 0; iSkip < innerFrameNo; iSkip++) {
    ReadInnerFrameInfo();
    SkipInnerFrame();
  }

  fData.ignore(8);
  fData.read(reinterpret_cast<Char_t *>(&headerSize), 2);
  fData.ignore(2);
  fData.read(reinterpret_cast<Char_t *>(&nItems), 4);
  fData.ignore(6);
  fData.read(reinterpret_cast<Char_t *>(&eventIdx), 4);
  fData.read(reinterpret_cast<Char_t *>(&coboIdx), 1);
  fData.read(reinterpret_cast<Char_t *>(&asadIdx), 1);

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
  fFrame -> SetPolarity((fIsPositivePolarity ? +1 : -1));

  fData.seekg((Int_t) fData.tellg() - 28 + headerSize);

  UInt_t data;
  for (Int_t iItem = 0; iItem < nItems; iItem++) {
    fData.read(reinterpret_cast<Char_t *>(&data), 4);
    data = htonl(data);

    UShort_t agetIdx = ((data & 0xc0000000) >> 30);
    UShort_t chanIdx = ((data & 0x3f800000) >> 23);
    UShort_t buckIdx = ((data & 0x007fc000) >> 14);
    UShort_t sample = (data & 0x00000fff);         

    if (chanIdx >= 68 || agetIdx >= 4 || buckIdx >= 512)
      continue; 
                                                                   
    fFrame -> SetRawADC(agetIdx, chanIdx, buckIdx, sample); 
  }

  CheckEOF();

  // Return to merged frame head
  fData.seekg(fMergedFrameStartPoint);

  fCurrentFrameID = frameNo;
  fCurrentInnerFrameID = innerFrameNo;

  if (fDebugMode) {
    std::cout << "frameNo: " << frameNo << std::endl;
    std::cout << "innerFrameNo: " << innerFrameNo << std::endl;
    std::cout << "fCurrentFrameID: " << fCurrentFrameID << std::endl;
    std::cout << "fCurrentInnerFrameID: " << fCurrentInnerFrameID << std::endl;
    std::cout << "fMergedHeaderSize: " << fMergedHeaderSize << std::endl;
    std::cout << "fCurrentMergedFrameSize: " << fCurrentMergedFrameSize << std::endl;
    std::cout << "fNumMergedFrames: " << fNumMergedFrames << std::endl;
    std::cout << "fMergedFrameStartPoint: " << fMergedFrameStartPoint << std::endl;
  }

  return fFrame;
}

Int_t GETDecoder::GetNumMergedFrames()
{
  return fNumMergedFrames;
}

void GETDecoder::PrintFrameInfo(Int_t frameNo, Int_t eventID, Int_t coboID, Int_t asadID)
{
  std::cout << "== Frame Info -";
  std::cout << " Frame:" << frameNo;
  std::cout << " Event:" << eventID;
  std::cout << " CoBo:" << coboID;
  std::cout << " AsAd:" << asadID << std::endl;
}

void GETDecoder::SkipInnerFrame()
{
  fData.ignore(fCurrentInnerFrameSize);
}

void GETDecoder::SkipMergedFrame()
{
  fData.ignore(fCurrentMergedFrameSize);
}

void GETDecoder::ReadMergedFrameInfo()
{
  fMergedFrameStartPoint = fData.tellg();
  fData.ignore(1);
  fData.read(reinterpret_cast<Char_t *>(&fCurrentMergedFrameSize), 3);
  fData.ignore(8);
  fData.read(reinterpret_cast<Char_t *>(&fNumMergedFrames), 4);
  fData.seekg((Int_t)fData.tellg() - 16);

  if (fEndianness == kBig) {
    fCurrentMergedFrameSize = htonl(fCurrentMergedFrameSize) >> 8;
    fNumMergedFrames = htonl(fNumMergedFrames);
  }
}

void GETDecoder::ReadInnerFrameInfo()
{
  fData.ignore(1);
  fData.read(reinterpret_cast<Char_t *>(&fCurrentInnerFrameSize), 3);
  fData.seekg((Int_t)fData.tellg() - 4);

  fCurrentInnerFrameSize = (htonl(fCurrentInnerFrameSize) >> 8)*64;
}

void GETDecoder::CheckEOF() {
  if (fData.tellg() >= fFileSize || fData.fail())
    fEOF = 1;
  else
    fEOF = 0;
}
