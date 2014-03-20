/** 
  * \example plot04.cc
  *  
  * This example draws an averaged data of specified channels in the specified rawdata file. 
  * Make sure that the shared library file is in the same folder in this example macro file.
  *  
  * \par Usage
  *      root 'plot04.cc("/path/to/GRAWFILE.graw")'
  *
  * \par Output
  *      CoBo#1-AsAd#2-AveragedRawFrame#3.png <br>
  *      where #1 is CoBo ID, #2 is AsAd ID, and #3 is frame number.
  *
  * \note
  *   Key method in this example is **ShowAverage()**. <br><br>
  *   The first argument is number of channels to draw (+) or to exclude (-)
  *   The second argument is a channel list. <br>
  *   This rule is also used in ShowRawFrame() and ShowFrame() methods. <br><br>
  *   If the third argument is -1, the method automatically read the next frame and draws plot. <br>
  *   The third argument can be ignored.
  *   \code{.cc}
  *   Int_t list[5] = {11, 22, 45, 56, 67};
  *   TCanvas *cvs = plot -> ShowAverage(-5, list);
  *   \endcode
  *
  * \par Source Code
 **/
void plot04(Char_t *filename) {
  if (filename == "") {
    cout << "Run 'root \'plot04.cc(\"/path/to/GRAWFILE.graw\")\'" << endl;
    return;
  }

  gSystem -> Load("libGETDecoder");

  GETDecoder *decoder = new GETDecoder(filename);
  GETPlot *plot = decoder -> GetGETPlot();

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
  TCanvas *cvs = plot -> ShowAverage(-5, list);

  cvs -> SaveAs(Form("CoBo%d-AsAd%d-AveragedRawFrame%d.png", coboID, asadID, frameID));
}
