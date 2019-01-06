//
// makespectrum.C - make template spectra for rate fitting in the analyzer.C routine
//
//
// A.P. Colijn

#define SIGMA0 1.66
#define SIGMA1 1.36e-2

/*--------------------------------------------------------------------------------------------------*/
const int    nbin      = 600;
const int    iwindow   = 60;
const double emin      = 0;
const double emax      = 3000;
const double E_cut_off = 200; //keV
const double delta_cut = 3.5;

/*--------------------------------------------------------------------------------------------------*/
double resolution(double E){
    //
    // Retrieve the energy resolution - FWHM/E
    //
    // parameters SIGMA0 and SIGMA1 obtained from modexp/analysis/monitor/resolution.C function
    //
    double c0  = SIGMA0/TMath::Sqrt(E);
    double c1  = SIGMA1;
    double res = TMath::Sqrt(c0*c0+c1*c1);
    return res;
}
/*--------------------------------------------------------------------------------------------------*/
double rate_smear(double R, double E, double Es){
  // get the energy resolution at E=E
  double sigma = resolution(E)*E/2.35;
  // calculate the smeared rate at E=Es
  double Rs = R*TMath::Exp(-0.5*pow((E-Es)/sigma,2))/sigma;
  
  return Rs;
}
/*--------------------------------------------------------------------------------------------------*/
void makespectrum(string basename, string source_name){
    
    //
    // Open the Geant4 root output file
    //
    char cmd[128];
    sprintf(cmd,"%s/%s*.root",basename.c_str(),source_name.c_str());
    
    //
    // construct the TChain with simulated data
    //
    TChain *tt = new TChain("events/evt");
    tt->Add(cmd);

    //
    // get the unsmeared energy spectrum
    //
    cout << "fill histo"<<endl;
    TH1F *_spectrum_no_smear = new TH1F("spectrum_no_smear","spectrum_no_smear",nbin,emin,emax);
    tt->Draw("ed>>spectrum_no_smear");
    cout << "fill histo done"<<endl;

    TH1F *_spectrum_bg   = new TH1F("h2","h2",nbin,emin,emax);
    TH1F *_spectrum_full = new TH1F("spectrum_full","spectrum_full",nbin,emin,emax);
    
    
    double val,val_old,Rbg,Rall;
    int nbin = _spectrum_no_smear->GetNbinsX();
    //
    // Loop over all the bins of the full spectrum
    //
    for(int ibin = 0; ibin<nbin; ibin++){
        //
        // get the energy in bin - ibin -
        //
        double E = _spectrum_no_smear->GetBinCenter(ibin);
        if(E > E_cut_off){
            // get the y-value of teh histogram
            val = _spectrum_no_smear->GetBinContent(ibin);
            //
            // This is a key line of code: if the new value is N-times higher than the
            // previous one, we have found a photopeak. In that case we will keep the current estimate
            // for the BG also for this bin.
            //
            double delta=0;
            if(val_old>1) delta = (val - val_old) / val_old;
         
            //
            // if the spectrum does not change too much we are seeing background
            //
            if(delta < delta_cut) Rbg = val;
            //
            // we also make a graph of the full spectrum
            //
            Rall = val;
            
            //
            // we smear each energy bin with the energy resolution
            //
            
            // define the window for smearing
            int jmin = ibin - iwindow;
            if(jmin<0) jmin = 0;
            
            int jmax = ibin + iwindow;
            if(jmax>nbin) jmax = nbin;
            
            cout << ibin << " "<<E<<" R = "<<Rbg<<" "<<jmin<<" "<<jmax<<" DELTA ="<<delta<<endl;
            //
            // do the smearing....
            //
            for(int jbin = jmin; jbin<jmax; jbin++){
                double Es = _spectrum_full->GetBinCenter(jbin);
                // background rate
                double Rs = rate_smear(Rbg, E, Es);
                _spectrum_bg->Fill(Es,Rs);
                // ful rate
                Rs =rate_smear(Rall, E, Es);
                _spectrum_full->Fill(Es,Rs);
            }
            val_old = val;
        }
    }
    
    //
    // plot the spectra 
    //
    _spectrum_full->SetLineColor(1);
    _spectrum_full->Draw("hist");
    _spectrum_bg->SetLineColor(2);
    _spectrum_bg->Draw("histsame");
    
    TH1F *_spectrum_peak = (TH1F*)_spectrum_full->Clone("spectrum_full");
    _spectrum_peak->SetTitle("spectrum_peak");
    _spectrum_peak->SetName("spectrum_peak");
    
    _spectrum_peak->Add(_spectrum_bg,-1);
    _spectrum_peak->SetLineColor(4);
    _spectrum_peak->Draw("histsame");
    
    //
    // write the histograms to file
    //
    char root_out[128];
    sprintf(root_out,"MC_%s_modulation.root",source_name.c_str());
    TFile *_f_out = new TFile(root_out,"RECREATE");
    _spectrum_no_smear->Write();
    _spectrum_bg->Write();
    _spectrum_full->Write();
    _spectrum_peak->Write();
    _f_out->Write();
}
/*--------------------------------------------------------------------------------------------------*/

