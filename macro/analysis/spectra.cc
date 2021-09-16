/*
  Macro : Reading LBC G4APP output file
  -------------------------------------
*/

void spectra(std::string filename)
{

  TFile * infile = new TFile(filename.c_str());

  TTreeReader datareader("lbc_tree", infile);
  TTreeReaderValue<Double_t> data_energy(datareader,"edep");

  const int nBins = 5000;

  TH1F* h_edep = new TH1F("h_lbc", "LBC Spectra", nBins, 0, 1000);

  int counts    = 0;
  int counts_pp = 0;

  while( datareader.Next() )
  {
    h_edep->Fill( (*data_energy));
    if(*data_energy >= 511) counts_pp++;
    counts++;
  }

  std::cout << "Total Number of Events : " << counts << std::endl;
  std::cout << "Total Numer of PhotoElectric Events : " << counts_pp << std::endl;
  std::cout << "PhotoPeak to Total Ratio : " << (counts_pp*1.0)/counts << std::endl;

  TCanvas * c = new TCanvas("c","LBC Canvas");

  // h_edep->Scale(1.0/(counts));
  h_edep->Draw("HIST");
  h_edep->GetXaxis()->SetTitle("Energy [keV]");
  h_edep->GetYaxis()->SetTitle("Counts [renormalized]");


  TFile * outfile = new TFile("LBC_511keV_spectra.root","RECREATE");
  h_edep->Write();
  outfile->Close();
  return ;
}
