/** 
  * \example readFrame03.cc
  *  
  * \note 
  *   This example illustrates how to read the rawdata for post-processing. <br>
  *   Change the filename written as `GRAWFILE.graw` in the source code and implement the code under the commented line.
  *  
  * \par Source Code
 **/
void readFrame01() {
  gSystem -> Load("libGETDecoder");

  GETDecoder *decoder = new GETDecoder("GRAWFILE.graw");
  // The number of time buckets in a frame should be set unless it's not 512.
  decoder -> SetNumTbs(512);
  
  GETFrame *frame = NULL;
  while ((frame = decoder -> GetFrame())) { 
    UInt_t eventID = frame -> GetEventID();
    Int_t coboID = frame -> GetCoboID();
    Int_t asadID = frame -> GetAsadID();

    Int_t rawadc = 0;
    for (Int_t iAget = 0; iAget < 4; iAget++) {
      for (Int_t iCh = 0; iCh < 68; iCh++) {
        Int_t maxTb = decoder -> GetNumTbs();
        for (Int_t iTb = 0; iTb < maxTb; iTb++) {
          rawadc = frame -> GetRawADC(iAget, iCh, iTb);

          // Implement what you want to do with a frame here.
        }
      }
    }
  }
}
