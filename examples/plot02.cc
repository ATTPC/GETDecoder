/** 
  * \example plot02.cc
  *  
  * This example draws a frame data in the specified rawdata file. 
  * Make sure that the shared library file is in the same folder in this example macro file.
  *  
  * \par Usage
  *      root 'plot02.cc("/path/to/GRAWFILE.graw")'
  *
  * \par Output
  *      CoBo#1-AsAd#2-RawFrame#3.png <br>
  *      where #1 is CoBo ID, #2 is AsAd ID, and #3 is frame number.
  *
  * \note
  *   Key method in this example is **ShowRawFrame()**. <br><br>
  *   If the first argument is -1 or all the arguments are ignored 
  *   the method automatically read the next frame and draws plot. <br><br>
  *   If the second and third arguments are ignored, the method plots all the channels. <br>
  *   The second argument is number of channels to draw (+) or to exclude (-) 
  *   The third argument is a channel list. <br>
  *   This rule is also used in ShowFrame() and ShowAverage() methods. <br><br>
  *   **example** <br>
  *   Plot all the signals of frame ID 2
  *   \code{.cc}
  *   TCanvas *cvs = plot -> ShowRawFrame(2);
  *   \endcode <br>
  *   Plot the signals of channels in the list in frame ID 2
  *   \code{.cc}
  *   Int_t list[5] = {11, 22, 45, 56, 67};
  *   TCanvas *cvs = plot -> ShowRawFrame(2, 5, list);
  *   \endcode <br>
  *   Plot the signals of channels except those in the list in frame ID 2
  *   \code{.cc}
  *   Int_t list[5] = {11, 22, 45, 56, 67};
  *   TCanvas *cvs = plot -> ShowRawFrame(2, -5, list);
  *   \endcode
  *
  * \par Source Code
 **/
void plot02(Char_t *filename) {
  if (filename == "") {
    cout << "Run 'root \'plot02.cc(\"/path/to/GRAWFILE.graw\")\'" << endl;
    return;
  }
  gSystem -> Load("libGETDecoder");

  GETDecoder *decoder = new GETDecoder(filename);
  // The number of time buckets in a frame should be set unless it's not 512.
  decoder -> SetNumTbs(512);

  GETPlot *plot = decoder -> GetGETPlot();

  GETFrame *frame = decoder -> GetFrame(0);

  Int_t coboID = frame -> GetCoboID();
  Int_t asadID = frame -> GetAsadID();
  Int_t frameID = frame -> GetFrameID();

  // If the first argument is -1 or all the arguments are ignored
  // the method automatically read the next frame and draws plot.
  //
  // If the second and third arguments are ignored, the method plots all the channels.
  // The second argument is number of channels to draw (+) or to exclude (-)
  // The third argument is a channel list.
  // This rule is also used in ShowFrame() and ShowAverage() methods.
  //
  // example)
  // TCanvas *cvs = plot -> ShowRawFrame(2); // Plot all the signals of frame ID 2
  //
  // Plot the signals of channels in the list in frame ID 2
  // Int_t list[5] = {11, 22, 45, 56, 67};
  // TCanvas *cvs = plot -> ShowRawFrame(2, 5, list);
  //
  // Plot the signals of channels except those in the list in frame ID 2
  // Int_t list[5] = {11, 22, 45, 56, 67};
  // TCanvas *cvs = plot -> ShowRawFrame(2, -5, list);
  TCanvas *cvs = plot -> ShowRawFrame();

  cvs -> SaveAs(Form("CoBo%d-AsAd%d-RawFrame%d.png", coboID, asadID, frameID));
}
