/** 
  * \example readFrame02.cc
  *  
  * \note 
  *   This example illustrates how to read the pedestal-subtracted data for post-processing. <br>
  *   Please read carefully the commented lines in source code. <br>
  *   Change the filename written as `GRAWFILE.graw` in the source code and implement the code under the second commented line.
  *  
  * \par Source Code
 **/
void readFrame02() {
  gSystem -> Load("libGETDecoder");

  GETDecoder *decoder = new GETDecoder("GRAWFILE.graw");
  // The number of time buckets in a frame should be set unless it's not 512.
  decoder -> SetNumTbs(512);
  
  GETFrame *frame = NULL;
  while ((frame = decoder -> GetFrame())) { 
    UInt_t eventID = frame -> GetEventID();
    Int_t coboID = frame -> GetCoboID();
    Int_t asadID = frame -> GetAsadID();

    // The line below calls pedestal subtracting method
    // It selects 20 time buckets from 10th time bucket
    // and calculates the average and RMS value.
    // Finally, it uses them as a pedestal for the channel.
    frame -> CalcPedestal(10, 20);

    Double_t *adc = NULL;
    for (Int_t iAget = 0; iAget < 4; iAget++) {
      for (Int_t iCh = 0; iCh < 68; iCh++) {
        adc = frame -> GetRawADC(iAget, iCh);

        // Implement what you want to do with a frame here.
      }
    }
  }
}
