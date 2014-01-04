/** 
  * \example plot01.cc
  *  
  * This example draws a summary spectra using all the frames in the specified rawdata file. 
  * Make sure that the shared library file is in the same folder in this example macro file.
  *  
  * \par Usage
  *      root 'plot01.cc("/path/to/GRAWFILE.graw")'
  * \par Output
  *      SummarySpectra-CoBo#1-AsAd#2.png <br>
  *      where #1 is CoBo ID and #2 is AsAd ID.
 **/
void plot01(Char_t *filename)
{
  if (filename == "") {
    cout << "Run 'root \'plot01.cc(\"/path/to/GRAWFILE.graw\")\'" << endl;
    return;
  }
  
  gSystem -> Load("libGETDecoder");

  GETDecoder *decoder = new GETDecoder(filename);
  GETPlot *plot = decoder -> GetGETPlot();

  GETFrame *frame = decoder -> GetFrame(0);

  Int_t coboID = frame -> GetCoboID();
  Int_t asadID = frame -> GetAsadID();

  // The line below draws the summary spectra of the set file.
  // In drawing, the pedestal is calculated with 20 time buckets
  // starting from 10th time bucket.
  TCanvas *cvs = plot -> ShowSummarySpectra(10, 20);

  cvs -> SaveAs(Form("SummarySpectra-CoBo%d-AsAd%d.png", coboID, asadID));
}
