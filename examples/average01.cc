/** 
  * \example average01.cc
  *  
  * This example explains how to get an averaged data of specified channels in the specified rawdata file. 
  * Make sure that the shared library file is in the same folder in this example macro file.
  *  
  * \par Usage
  *      root 'average01.cc("/path/to/GRAWFILE.graw")'
  *
  *
  * \note
  *   Key method in this example is **GetAverage()**. <br><br>
  *   The first argument is number of channels to draw (+) or to exclude (-)
  *   The second argument is a channel list. <br>
  *   This rule is also used in ShowRawFrame() and ShowFrame() methods. <br><br>
  *   If the third argument is -1, the method automatically read the next frame and calculate. <br>
  *   The third argument can be ignored.
  *   \code{.cc}
  *   Int_t list[5] = {11, 22, 45, 56, 67};
  *   Double_t **adc = math -> GetAverage(-5, list);
  *   \endcode
  *
  * \par Source Code
 **/
void average01(Char_t *filename) {
  if (filename == "") {
    cout << "Run 'root \'average01.cc(\"/path/to/GRAWFILE.graw\")\'" << endl;
    return;
  }

  gSystem -> Load("libGETDecoder");

  GETDecoder *decoder = new GETDecoder(filename);
  // The number of time buckets in a frame should be set unless it's not 512.
  decoder -> SetNumTbs(512);

  GETMath *math = decoder -> GetGETMath();

  GETFrame *frame = decoder -> GetFrame(0);

  Int_t coboID = frame -> GetCoboID();
  Int_t asadID = frame -> GetAsadID();
  Int_t frameID = frame -> GetFrameID();

  // The first argument is number of channels to draw (+) or to exclude (-)
  // The second argument is a channel list.
  // This rule is also used in ShowRawFrame() and ShowFrame() methods.
  //
  // If the third argument is -1, the method automatically read the next frame and draws plot.
  // The third argument can be ignored.
  Int_t list[5] = {11, 22, 45, 56, 67};
  Double_t **adc = math -> GetAverage(5, list);

  for (Int_t iAget = 0; iAget < 4; iAget++)
    for (Int_t iTb = 0; iTb < 512; iTb++)
      cout << iAget << " " << iTb << " " << adc[iAget][iTb] << endl;
}
