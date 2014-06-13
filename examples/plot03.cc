/** 
  * \example plot03.cc
  *  
  * This example draws a pedestal-subtracted frame data in the specified rawdata file. 
  * Make sure that the shared library file is in the same folder in this example macro file.
  *  
  * \par Usage
  *      root 'plot03.cc("/path/to/GRAWFILE.graw")'
  *
  * \par Output
  *      CoBo#1-AsAd#2-Frame#3.png <br>
  *      where #1 is CoBo ID, #2 is AsAd ID, and #3 is frame number.
  *
  * \note
  *   Key method in this example is **ShowFrame()**. <br><br>
  *   If the first argument is -1 or all the argumets are ignored,
  *   the method automatically read the next frame and draws plot. <br>
  *   If the second to fifth arguments are ignored, the method plots all the channels. <br>
  *   Pedestal is calculated using 20 time buckets from 10th time bucket. <br>
  *   You can change the start position and the number of time buckets used in the calculation
  *   by changing the second (starting point) and the third (number of channels) arguments. <br><br>
  *   The fourth argument is number of channels to draw (+) or to exclude (-)
  *   The fifth argument is a channel list. <br>
  *   This rule is also used in ShowRawFrame() and ShowAverage() methods. <br><br>
  *   **example** <br>
  *   Plot all the signals of frame ID 2
  *   \code{.cc}
  *   TCanvas *cvs = plot -> ShowFrame(2);
  *   \endcode <br>
  *   Plot the signals of channels in the list in frame ID 2
  *   \code{.cc}
  *   Int_t list[5] = {11, 22, 45, 56, 67};
  *   TCanvas *cvs = plot -> ShowFrame(2, 10, 20, 5, list);
  *   \endcode <br>
  *   Plot the signals of channels except those in the list in frame ID 2
  *   \code{.cc}
  *   Int_t list[5] = {11, 22, 45, 56, 67};
  *   TCanvas *cvs = plot -> ShowFrame(2, 10, 20, -5, list);
  *   \endcode
  *
  * \par Source Code
 **/
void plot03(Char_t *filename) {
  if (filename == "") {
    cout << "Run 'root \'plot03.cc(\"/path/to/GRAWFILE.graw\")\'" << endl;
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

  // If the first argument is -1 or all the argumets are ignored,
  // the method automatically read the next frame and draws plot.
  // If the second to fifth arguments are ignored, the method plots all the channels.
  //
  // Pedestal is calculated using 20 time buckets from 10th time bucket.
  // You can change the start position and the number of time buckets used in the calculation
  // by changing the second (starting point) and the third (number of channels) arguments.
  //
  // The fourth argument is number of channels to draw (+) or to exclude (-)
  // The fifth argument is a channel list.
  // This rule is also used in ShowRawFrame() and ShowAverage() methods.
  //
  // example)
  // TCanvas *cvs = plot -> ShowFrame(2); // Plot all the signals of frame ID 2
  //
  // Plot the signals of channels in the list in frame ID 2
  // Int_t list[5] = {11, 22, 45, 56, 67};
  // TCanvas *cvs = plot -> ShowFrame(2, 10, 20, 5, list);
  //
  // Plot the signals of channels except those in the list in frame ID 2
  // Int_t list[5] = {11, 22, 45, 56, 67};
  // TCanvas *cvs = plot -> ShowFrame(2, 10, 20, -5, list);
  TCanvas *cvs = plot -> ShowFrame();

  cvs -> SaveAs(Form("CoBo%d-AsAd%d-Frame%d.png", coboID, asadID, frameID));
}
