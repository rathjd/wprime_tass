#define CombineHistogramDumpster_cxx
#include "CombineHistogramDumpster.h"
#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFitResult.h>
#include <TMatrixD.h>
#include <TMath.h>
#include <TF1.h>
#include <cmath>

//small function to calculate covariance matrix envelope of fit function
float CalculateCovError(float STval, TMatrixD covM, int jetNumber){
  vector<double> Derivatives;
  if(jetNumber==5) Derivatives = {1./STval/STval/STval, 1./STval/STval, 1./STval, 1., STval, STval*STval};
  else		   Derivatives = {1./STval, 1., STval, STval*STval};
  float FinalEnvelope = 0.;
  for(unsigned x = 0; x < Derivatives.size(); ++x){
    for(unsigned y = 0; y < Derivatives.size(); ++y){
      FinalEnvelope += Derivatives[x] * Derivatives[y] * covM(x,y);
    }
  }
  return FinalEnvelope;
}

void CombineHistogramDumpster::Loop()
{
//   In a ROOT session, you can do:
//      root> .L CombineHistogramDumpster.cc.C
//      root> CombineHistogramDumpster.cc t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch

  //define bin for analysis
  TString gn = dset.GroupName;

  //determine the SF functions for ST
  TFile *SFfile;
  vector<TF1> SFs;
  vector<TMatrixD> SFcovs;

  //define luminosity uncertainties depending on the year
  float LumiCorrVal = 0.0;
  float LumiStatVal = 0.0;

  //set sample weight
  int Year = 0;
  int year = 0;
  float Lumi = 0.;
  if(YearType == "2016apv")  {Year = 0; year = 2016; Lumi = 19.52; LumiCorrVal = 0.006; LumiStatVal = 0.01;}
  else if(YearType == "2016") {Year = 1; year = 2016; Lumi = 16.81; LumiCorrVal = 0.006; LumiStatVal = 0.01;}
  else if(YearType == "2017") {Year = 2; year = 2017; Lumi = 41.48; LumiCorrVal = 0.009; LumiStatVal = 0.02;}
  else if(YearType == "2018") {Year = 3; year = 2018; Lumi = 59.83; LumiCorrVal = 0.02; LumiStatVal = 0.015;}
  float SampleWeight = 1.;
  if(dset.Type != 0) SampleWeight = Lumi * dset.CrossSection / dset.Size[Year];

  TString binS = TString::Format("Wprime%d_%d", bin, year);

  //define variations
  vector<vector<TH1F*> > FitMass;
  vector<vector<TH1F*> > HT;
  vector<vector<TH2F*> > FitMass_2D, HT_2D;
  vector<TH2F*> FitMass_2D_STstatUp;
  vector<TH2F*> FitMass_2D_STstatDown;
  vector<TH2F*> HT_2D_STstatUp;
  vector<TH2F*> HT_2D_STstatDown;
  vector<TH1F*> FitMass_STstatUp;
  vector<TH1F*> FitMass_STstatDown;
  vector<TH1F*> HT_STstatUp;
  vector<TH1F*> HT_STstatDown;
  TH1F* STrew_STstatUp;
  TH1F* STrew_STstatDown;

  vector<TH1F*> ST;
  vector<TH1F*> STrew;

  vector<TH1F*> NegLogLnoB;
  vector<TH2F*> NegLogLnoBvsNegLogL;

  TString YearS = TString::Format("%d",year);

  //first variations are all weight variations and map 1:1, region variations start at index 21
  vector<TString> variations = {"" // 0
  , "electronScale"+YearS+"Up", "electronScale"+YearS+"Down", "electronRes"+YearS+"Up", "electronRes"+YearS+"Down", "JES"+YearS+"Up", "JES"+YearS+"Down", "JER"+YearS+"Up", "JER"+YearS+"Down" // 1 - 8
  , "electron"+YearS+"Up", "electron"+YearS+"Down", "muonTrigger"+YearS+"Up", "muonTrigger"+YearS+"Down", "muonId"+YearS+"Up", "muonId"+YearS+"Down", "muonIso"+YearS+"Up", "muonIso"+YearS+"Down" // 9 - 16
  , "BjetTagCorrUp", "BjetTagCorrDown", "BjetTagUncorr"+YearS+"Up", "BjetTagUncorr"+YearS+"Down", "PUID"+YearS+"Up", "PUID"+YearS+"Down", "L1PreFiring"+YearS+"Up", "L1PreFiring"+YearS+"Down" // 17 - 24
  , "PUreweight"+YearS+"Up", "PUreweight"+YearS+"Down", "PDFUp", "PDFDown", "LHEScaleUp", "LHEScaleDown", // 25 - 30
  "LumiCorrUp", "LumiCorrDown", "LumiStat"+YearS+"Up", "LumiStat"+YearS+"Down" //31-34
  };

  vector<vector<TString> > variationsName, HTvariationsName, FitMass2Dnames, HT2Dnames;
  for (unsigned m = 3; m < 12; m++){
    vector<TString> dummy;
    variationsName.push_back(dummy);
    HTvariationsName.push_back(dummy);
    FitMass2Dnames.push_back(dummy);
    HT2Dnames.push_back(dummy);
    for (unsigned i = 0; i < variations.size(); ++i) {
      variationsName[m-3].push_back(gn + "_" + binS + TString::Format("_M%d_",m*100) + variations[i]);
      HTvariationsName[m-3].push_back("HT_" + variationsName[m-3][i]);
      FitMass2Dnames[m-3].push_back("FitMass2D_" + variationsName[m-3][i]);
      HT2Dnames[m-3].push_back("HT2D_" + variationsName[m-3][i]);
    }
  }

  vector<TString> variationsNamePlain;
  for(unsigned i = 0; i < variations.size(); ++i) variationsNamePlain.push_back(gn + "_" + binS + "_" + variations[i]);

  for(unsigned m = 3; m < 12; ++m){
    vector<TH1F*> dummy1D;
    FitMass.push_back(dummy1D);
    HT.push_back(dummy1D);
    vector<TH2F*> dummy2D;
    FitMass_2D.push_back(dummy2D);
    HT_2D.push_back(dummy2D);

    for(unsigned i = 0; i < variations.size(); ++i){

      //Extraction variable block
      if(bin % 100 < 60){
        if(bin % 10 == 3){ //5 jets 3 b-tags
	  double FitLimits[36] = {85., 220., 230., 239., 247., 255., 263., 270., 276., 282., 288., 294., 301., 308., 315., 322., 329., 336., 343., 350., 360., 372., 384., 396., 410., 425., 445., 465., 490., 515., 550., 590., 650., 730., 880., 2000.};
          FitMass[m-3].push_back(new TH1F(variationsName[m-3][i],"fitted W' mass; m_{W'} [GeV/c^{2}]; Events", 35, FitLimits));
	  double HTlimits[47] = {125., 230., 245., 258., 265., 273., 281., 288., 294., 300., 306., 312., 318., 324., 330., 336., 342., 348., 354., 360., 367., 374., 381., 388., 395., 402., 409., 417., 426., 435., 445., 455., 465., 475., 487., 501., 515., 531., 550., 570., 595., 625., 660., 705., 780., 910.,  2000.};
          HT[m-3].push_back(new TH1F(HTvariationsName[m-3][i],"H_{T}; H_{T} [GeV/c^{2}]; Events", 46, HTlimits));
	  double NLLlimits[53] = {0., 0.80, 1.05, 1.25, 1.45, 1.60, 1.75, 1.90, 2.00, 2.15, 2.30, 2.40, 2.65, 2.80, 2.92, 3.05, 3.20, 3.30, 3.45, 3.60, 3.75, 3.90, 4.00, 4.20, 4.30, 4.45, 4.60, 4.75, 4.90, 5.10, 5.25, 5.40, 5.60, 5.80, 6.00, 6.25, 6.50, 6.75, 7.00, 7.35, 7.70, 8.05, 8.40, 8.80, 9.20, 9.70, 10.10, 10.65, 11.25, 12.00, 12.95, 14.40, 30.};
          FitMass_2D[m-3].push_back(new TH2F(FitMass2Dnames[m-3][i],"fitted W' mass vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", 35, FitLimits, 52, NLLlimits));
          HT_2D[m-3].push_back(new TH2F(HT2Dnames[m-3][i],"HT vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", 46, HTlimits, 52, NLLlimits));
        } else if(bin % 10 == 4){//5 jets 4 b-tags
	    double FitLimits[2] = {120., 2000.};
	    FitMass[m-3].push_back(new TH1F(variationsName[m-3][i],"fitted W' mass; m_{W'} [GeV/c^{2}]; Events", 1, FitLimits));
	    double HTlimits[15] = {155., 270., 300., 325., 345., 365., 385., 410., 435., 465., 495., 545., 610., 740., 2000.};
	    HT[m-3].push_back(new TH1F(HTvariationsName[m-3][i],"H_{T}; H_{T} [GeV/c^{2}]; Events", 14, HTlimits));
	    double NLLlimits[53] = {0., 0.80, 1.05, 1.25, 1.45, 1.60, 1.75, 1.90, 2.00, 2.15, 2.30, 2.40, 2.65, 2.80, 2.92, 3.05, 3.20, 3.30, 3.45, 3.60, 3.75, 3.90, 4.00, 4.20, 4.30, 4.45, 4.60, 4.75, 4.90, 5.10, 5.25, 5.40, 5.60, 5.80, 6.00, 6.25, 6.50, 6.75, 7.00, 7.35, 7.70, 8.05, 8.40, 8.80, 9.20, 9.70, 10.10, 10.65, 11.25, 12.00, 12.95, 14.40, 30.};
          FitMass_2D[m-3].push_back(new TH2F(FitMass2Dnames[m-3][i],"fitted W' mass vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", 1, FitLimits, 52, NLLlimits));
          HT_2D[m-3].push_back(new TH2F(HT2Dnames[m-3][i],"HT vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", 14, HTlimits, 52, NLLlimits));
        } else {
          FitMass[m-3].push_back(new TH1F(variationsName[m-3][i],"fitted W' mass; m_{W'} [GeV/c^{2}]; Events", 400, 0., 2000.));
          HT[m-3].push_back(new TH1F(HTvariationsName[m-3][i],"H_{T}; H_{T} [GeV/c^{2}]; Events", 400, 0., 2000.));
          FitMass_2D[m-3].push_back(new TH2F(FitMass2Dnames[m-3][i],"fitted W' mass vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", 400, 0., 2000., 60, 0., 30.));
          HT_2D[m-3].push_back(new TH2F(HT2Dnames[m-3][i],"HT vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", 400, 0., 2000., 60, 0., 30.));
        }
      }
      else{
        if(bin % 10 == 3){//6 jets 3 b-tags
	  double FitLimits[28] = {55., 234., 246., 257., 268., 279., 290., 299., 308., 317., 326., 335., 345., 360., 375., 390., 405., 425., 450., 475., 505., 540., 580., 635., 715., 820., 1020., 2000.};
          FitMass[m-3].push_back(new TH1F(variationsName[m-3][i],"fitted W' mass; m_{W'} [GeV/c^{2}]; Events", 27, FitLimits));
	  double HTlimits[31] = {155., 195., 320., 335., 351., 363., 375., 387., 399., 411., 422., 433., 445., 457., 469., 483., 497., 511., 527., 544., 562., 581., 605., 630., 660., 690., 735., 795., 875., 1000.,  2000.};
          HT[m-3].push_back(new TH1F(HTvariationsName[m-3][i],"H_{T}; H_{T} [GeV/c^{2}]; Events", 30, HTlimits));
          double NLLlimits[24] = {0., 0.80, 1.10, 1.35, 1.60, 1.80, 2.05, 2.30, 2.50, 2.75, 3.00, 3.30, 3.65, 4.00, 4.35, 4.70, 5.15, 5.70, 6.45, 7.75, 9.20, 11.00, 24.55, 30.};
          FitMass_2D[m-3].push_back(new TH2F(FitMass2Dnames[m-3][i],"fitted W' mass vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", 27, FitLimits, 23, NLLlimits));
          HT_2D[m-3].push_back(new TH2F(HT2Dnames[m-3][i],"HT vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", 30, HTlimits, 23, NLLlimits));	      
        } else if(bin % 10 == 4){//6 jets 4 b-tags
	  double FitLimits[19] = {185., 325., 355., 380., 400., 420., 440., 460., 480., 500., 525., 550., 575., 615., 655., 710., 785., 915., 2000.};
	  FitMass[m-3].push_back(new TH1F(variationsName[m-3][i],"fitted W' mass; m_{W'} [GeV/c^{2}]; Events", 18, FitLimits));
	  double HTlimits[37] {185., 290., 315., 340., 355., 365., 375., 385., 395., 405., 415., 425., 435., 445., 455., 465., 475., 485., 495., 505., 515., 525., 535., 545., 555., 570., 590., 610., 635., 660., 690., 720., 760., 820., 890., 1010., 2000.};
	  HT[m-3].push_back(new TH1F(HTvariationsName[m-3][i],"H_{T}; H_{T} [GeV/c^{2}]; Events", 36, HTlimits));
	  double NLLlimits[24] = {0., 0.80, 1.10, 1.35, 1.60, 1.80, 2.05, 2.30, 2.50, 2.75, 3.00, 3.30, 3.65, 4.00, 4.35, 4.70, 5.15, 5.70, 6.45, 7.75, 9.20, 11.00, 24.55, 30.};
          FitMass_2D[m-3].push_back(new TH2F(FitMass2Dnames[m-3][i],"fitted W' mass vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", 18, FitLimits, 23, NLLlimits));
          HT_2D[m-3].push_back(new TH2F(HT2Dnames[m-3][i],"HT vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", 36, HTlimits, 23, NLLlimits));
        } else {
          FitMass[m-3].push_back(new TH1F(variationsName[m-3][i],"fitted W' mass; m_{W'} [GeV/c^{2}]; Events", 400, 0., 2000.));
          HT[m-3].push_back(new TH1F(HTvariationsName[m-3][i],"H_{T}; H_{T} [GeV/c^{2}]; Events", 400, 0., 2000.));
          FitMass_2D[m-3].push_back(new TH2F(FitMass2Dnames[m-3][i],"fitted W' mass vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", 400, 0., 2000., 60, 0., 30.));
          HT_2D[m-3].push_back(new TH2F(HT2Dnames[m-3][i],"HT vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", 400, 0., 2000., 60, 0., 30.));
        }
      }
    

      //need to fill ST collection, but only once
      if(m > 3) continue;

      TString STname = "ST_";
      STname.Append(variationsNamePlain[i]);

      TString STrewName = "STrew_";
      STrewName.Append(variationsNamePlain[i]);
      if(bin % 100 < 60){//5j version optimized to have about 1k stats in 2017 electron data per bin
        double STlimits[58] = {180., 320., 337., 348., 358., 367., 374., 381., 388., 394., 400., 406., 412., 418., 424., 429., 435., 440., 446., 451., 457., 463., 468., 474., 480., 486., 492., 498., 504., 511., 517.,  524., 531., 538., 545., 553., 561., 570., 578., 588., 597., 607., 618., 630., 643., 656., 671., 688., 705., 724., 747., 773., 805., 843., 896., 972., 1104., 2000.};
        ST.push_back(new TH1F(STname,"ST; ST [GeV/c]; Events", 57, STlimits));
        STrew.push_back(new TH1F(STrewName,"ST reweighted; ST [GeV/c]; Events", 57, STlimits));
      } else {//6j version optimized to have about 1k stats in 2017 electron data per bin
        double STlimits[24] = {210., 406., 435., 457., 476., 494., 509., 526., 543., 559., 577., 596., 615., 636., 658., 682., 711., 742., 779., 823., 880., 960., 1101., 2000.};
        ST.push_back(new TH1F(STname,"ST; ST [GeV/c]; Events", 23, STlimits));
        STrew.push_back(new TH1F(STrewName,"ST reweighted; ST [GeV/c]; Events", 23, STlimits));
      }


      //only activate for SR runs with ttbar sample
      if(SFreg != 0 && Iterator == 2){
        SFfile = new TFile(TString::Format("TestHistograms/SF_Bin%d_%d.root",SFreg,year));
        TH1F *SF = (TH1F*)SFfile->Get("SF_"+variations[i]);
        TF1 *SFfit;
        if(bin % 100 < 60) SFfit = new TF1(TString::Format("fitFunction%d",i),"[0]/x/x/x+[1]/x/x+[2]/x+[3]+[4]*x+[5]*x*x", 180., 2000.);
        else 		 SFfit = new TF1(TString::Format("fitFunction%d",i),"[0]/x+[1]+[2]*x+[3]*x*x", 210., 2000.);
        TFitResultPtr fr = SF->Fit(SFfit,"SRF");
        TMatrixD cov = fr->GetCovarianceMatrix();
        SFs.push_back(*SFfit);
        SFcovs.push_back(cov);
      }
    }//end variations loop

    //ST stat variations block
    FitMass_STstatUp.push_back( (TH1F*) FitMass[m-3][0]->Clone(variationsName[m-3][0]+"STfit_"+YearS+TString::Format("_%d_STfitUp",SFreg)));
    FitMass_STstatDown.push_back(  (TH1F*) FitMass[m-3][0]->Clone(variationsName[m-3][0]+"STfit_"+YearS+TString::Format("_%d_STfitDown",SFreg)));
    HT_STstatUp.push_back(  (TH1F*) HT[m-3][0]->Clone(HTvariationsName[m-3][0]+"STfit_"+YearS+TString::Format("_%d_STfitUp",SFreg)));
    HT_STstatDown.push_back(  (TH1F*) HT[m-3][0]->Clone(HTvariationsName[m-3][0]+"STfit_"+YearS+TString::Format("_%d_STfitDown",SFreg)));
    FitMass_2D_STstatUp.push_back(  (TH2F*) FitMass_2D[m-3][0]->Clone(FitMass2Dnames[m-3][0]+"STfit_"+YearS+TString::Format("_%d_STfitUp",SFreg)));
    FitMass_2D_STstatDown.push_back(  (TH2F*) FitMass_2D[m-3][0]->Clone(FitMass2Dnames[m-3][0]+"STfit_"+YearS+TString::Format("_%d_STfitDown",SFreg)));
    HT_2D_STstatUp.push_back(  (TH2F*) HT_2D[m-3][0]->Clone(HT2Dnames[m-3][0]+"STfit_"+YearS+TString::Format("_%d_STfitUp",SFreg)));
    HT_2D_STstatDown.push_back(  (TH2F*) HT_2D[m-3][0]->Clone(HT2Dnames[m-3][0]+"STfit_"+YearS+TString::Format("_%d_STfitDown",SFreg)));
  
    //negative log likelihood block
    TString NLLname = "NegLogLnoB_";
    NLLname.Append(variationsName[m-3][0]);
    TString NLL2Dname = "NegLogLnoBvsNegLogL_";
    NLL2Dname.Append(variationsName[m-3][0]);
    if(bin % 100 < 60){
      double NLLlimits[53] = {0., 0.80, 1.05, 1.25, 1.45, 1.60, 1.75, 1.90, 2.00, 2.15, 2.30, 2.40, 2.65, 2.80, 2.92, 3.05, 3.20, 3.30, 3.45, 3.60, 3.75, 3.90, 4.00, 4.20, 4.30, 4.45, 4.60, 4.75, 4.90, 5.10, 5.25, 5.40, 5.60, 5.80, 6.00, 6.25, 6.50, 6.75, 7.00, 7.35, 7.70, 8.05, 8.40, 8.80, 9.20, 9.70, 10.10, 10.65, 11.25, 12.00, 12.95, 14.40, 30.};
      NegLogLnoB.push_back( new TH1F(NLLname, "-log(L) without b-tagging; -log(L)_{!b}; Events", 52, NLLlimits));
    
      NegLogLnoBvsNegLogL.push_back( new TH2F(NLL2Dname, "-log(L) without b-tagging vs -log(L); -log(L)_{!b}; -log(L); Events", 52, NLLlimits, 52, NLLlimits));
    }
    else{
      double NLLlimits[24] = {0., 0.80, 1.10, 1.35, 1.60, 1.80, 2.05, 2.30, 2.50, 2.75, 3.00, 3.30, 3.65, 4.00, 4.35, 4.70, 5.15, 5.70, 6.45, 7.75, 9.20, 11.00, 24.55, 30.};
      NegLogLnoB.push_back( new TH1F(NLLname, "-log(L) without b-tagging; -log(L)_{!b}; Events", 23, NLLlimits));
      NegLogLnoBvsNegLogL.push_back( new TH2F(NLL2Dname, "-log(L) without b-tagging vs -log(L); -log(L)_{!b}; -log(L); Events", 23, NLLlimits, 23, NLLlimits));
    }

  }//end mass variant loop

  //STrew only needs to be done once
  STrew_STstatUp = (TH1F*) STrew[0]->Clone("STrew_" + gn + "_STfit_" + YearS + "_" + binS + "_" + "STfitUp");
  STrew_STstatDown = (TH1F*) STrew[0]->Clone("STrew_" + gn + "_STfit_" + YearS + "_" + binS + "_" + "STfitDown");

  //calculate jet multiplicity
  int jetMult = 0;
  if(bin % 100 < 60) jetMult = 5;
  else		     jetMult = 6;

  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();

  //loop over events
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    //blind data in SRs
    if(Iterator < 2 && bin % 10 >= 3) continue;

    //float Vals[9] = {LeptonPt, LeptonPt_SU, LeptonPt_SD, LeptonPt_RU, LeptonPt_RD, LeptonPt, LeptonPt, LeptonPt, LeptonPt}; //Lepton pT
    //int n = 4;
    //float Vals[9] = {JetPt->at(n), JetPt->at(n), JetPt->at(n), JetPt->at(n), JetPt->at(n), JetPt_SU->at(n), JetPt_SD->at(n), JetPt_RU->at(n), JetPt_RD->at(n)}; //Jet pT
    //float Vals[9] = {METPt, METPt, METPt, METPt, METPt, METPt_SU, METPt_SD, METPt_RU, METPt_RD};
    float Vals[9] = {0., 0., 0., 0., 0., 0., 0., 0., 0.};//HT version

    //calculate ST
    float STvals[9] = {0., 0., 0., 0., 0., 0., 0., 0., 0.};

    //find the actual jet count per variation
    int JetCounts[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    int bJetMainCount = 0;

    //0: default
    float defHT = 0.;
    for(unsigned i = 0; i < JetPt->size(); ++i) if(JetPt->at(i) > 30.){
      defHT += JetPt->at(i);
      /*JetCounts[0]++;
      JetCounts[1]++;
      JetCounts[2]++;
      JetCounts[3]++;
      JetCounts[4]++;
      if(JetbTag->at(i)) bJetMainCount++;*/
    }
    STvals[0] = LeptonPt + METPt + defHT; Vals[0] = defHT;
    //1: eScaleUp
    STvals[1] = LeptonPt_SU + METPt + defHT; Vals[1] = defHT;
    //2: eScaleDown
    STvals[2] = LeptonPt_SD + METPt + defHT; Vals[2] = defHT;
    //3: eResUp
    STvals[3] = LeptonPt_RU + METPt + defHT; Vals[3] = defHT;
    //4: eResDown
    STvals[4] = LeptonPt_RD + METPt + defHT; Vals[4] = defHT;
    //5: JESup
    STvals[5] = LeptonPt + METPt_SU;
    for(unsigned i = 0; i < JetPt_SU->size(); ++i){
      if(JetPt_SU->at(i) > 30.){
	STvals[5] += JetPt_SU->at(i);
       	Vals[5] += JetPt_SU->at(i);
	JetCounts[5]++;
      }
    }
    //6: JESdown
    STvals[6] = LeptonPt + METPt_SD;
    for(unsigned i = 0; i < JetPt_SD->size(); ++i){
      if(JetPt_SD->at(i) > 30.){
	STvals[6] += JetPt_SD->at(i);
	Vals[6] += JetPt_SD->at(i);
	JetCounts[6]++;
      }
    }
    //7: JERup
    STvals[7] = LeptonPt + METPt_RU;
    for(unsigned i = 0; i < JetPt_RU->size(); ++i){
      if(JetPt_RU->at(i) > 30.){
        STvals[7] += JetPt_RU->at(i);
	Vals[7] += JetPt_RU->at(i);
	JetCounts[7]++;
      }
    }
    //7: JERdown
    STvals[8] = LeptonPt + METPt_RD;
    for(unsigned i = 0; i < JetPt_RD->size(); ++i){
      if(JetPt_RD->at(i) > 30.){
	STvals[8] += JetPt_RD->at(i);
	Vals[8] += JetPt_RD->at(i);
	JetCounts[8]++;
      }
    }

    //validate actual region
    
    /*int RegionIdents[9];
    RegionIdents[0] = (RegionIdentifier[0]/1000)*1000 + 100 + JetCounts[0]*10 + bJetMainCount;
    RegionIdents[1] = (RegionIdentifier[1]/1000)*1000 + 100 + JetCounts[1]*10 + RegionIdentifier[1] % 10;
    RegionIdents[2] = (RegionIdentifier[2]/1000)*1000 + 100 + JetCounts[2]*10 + RegionIdentifier[2] % 10;
    RegionIdents[3] = (RegionIdentifier[3]/1000)*1000 + 100 + JetCounts[3]*10 + RegionIdentifier[3] % 10;
    RegionIdents[4] = (RegionIdentifier[4]/1000)*1000 + 100 + JetCounts[4]*10 + RegionIdentifier[4] % 10;    
    RegionIdents[5] = (RegionIdentifier[5]/1000)*1000 + 100 + JetCounts[5]*10 + RegionIdentifier[5] % 10;
    RegionIdents[6] = (RegionIdentifier[6]/1000)*1000 + 100 + JetCounts[6]*10 + RegionIdentifier[6] % 10;    
    RegionIdents[7] = (RegionIdentifier[7]/1000)*1000 + 100 + JetCounts[7]*10 + RegionIdentifier[7] % 10;
    RegionIdents[8] = (RegionIdentifier[8]/1000)*1000 + 100 + JetCounts[8]*10 + RegionIdentifier[8] % 10;*/

    //variations of mass interpreation
    for(unsigned m = 3; m < 12; ++m){

      float fillBranchZero = 0.;
      if(m==3) fillBranchZero = Best_WPrimeMass_300->at(0);
      else if(m==4) fillBranchZero  = Best_WPrimeMass_400->at(0);
      else if(m==5) fillBranchZero  = Best_WPrimeMass_500->at(0);
      else if(m==6) fillBranchZero  = Best_WPrimeMass_600->at(0);
      else if(m==7) fillBranchZero  = Best_WPrimeMass_700->at(0);
      else if(m==8) fillBranchZero  = Best_WPrimeMass_800->at(0);
      else if(m==9) fillBranchZero  = Best_WPrimeMass_900->at(0);
      else if(m==10) fillBranchZero  = Best_WPrimeMass_1000->at(0);
      else if(m==11) fillBranchZero  = Best_WPrimeMass_1100->at(0);

      float NLLfillZero = 0.;
      if(m==3) NLLfillZero = Best_Likelihood_300->at(0) >= 0 ? -log(Best_Likelihood_300->at(0)) : -1.;
      else if(m==4) NLLfillZero = Best_Likelihood_400->at(0) >= 0 ? -log(Best_Likelihood_400->at(0)) : -1.;
      else if(m==5) NLLfillZero = Best_Likelihood_500->at(0) >= 0 ? -log(Best_Likelihood_500->at(0)) : -1.;
      else if(m==6) NLLfillZero = Best_Likelihood_600->at(0) >= 0 ? -log(Best_Likelihood_600->at(0)) : -1.;
      else if(m==7) NLLfillZero = Best_Likelihood_700->at(0) >= 0 ? -log(Best_Likelihood_700->at(0)) : -1.;
      else if(m==8) NLLfillZero = Best_Likelihood_800->at(0) >= 0 ? -log(Best_Likelihood_800->at(0)) : -1.;
      else if(m==9) NLLfillZero = Best_Likelihood_900->at(0) >= 0 ? -log(Best_Likelihood_900->at(0)) : -1.;
      else if(m==10) NLLfillZero = Best_Likelihood_1000->at(0) >= 0 ? -log(Best_Likelihood_1000->at(0)) : -1.;
      else if(m==11) NLLfillZero = Best_Likelihood_1100->at(0) >= 0 ? -log(Best_Likelihood_1100->at(0)) : -1.;

      float NLLnoBfillZero = 0.;
      if(m==3) NLLnoBfillZero = NLLfillZero >= 0 ? -log(Best_Likelihood_300->at(0)/Best_PbTag_300->at(0)) : -1.;
      else if(m==4) NLLnoBfillZero = NLLfillZero >= 0 ? -log(Best_Likelihood_400->at(0)/Best_PbTag_400->at(0)) : -1.;
      else if(m==5) NLLnoBfillZero = NLLfillZero >= 0 ? -log(Best_Likelihood_500->at(0)/Best_PbTag_500->at(0)) : -1.;
      else if(m==6) NLLnoBfillZero = NLLfillZero >= 0 ? -log(Best_Likelihood_600->at(0)/Best_PbTag_600->at(0)) : -1.;
      else if(m==7) NLLnoBfillZero = NLLfillZero >= 0 ? -log(Best_Likelihood_700->at(0)/Best_PbTag_700->at(0)) : -1.;
      else if(m==8) NLLnoBfillZero = NLLfillZero >= 0 ? -log(Best_Likelihood_800->at(0)/Best_PbTag_800->at(0)) : -1.;
      else if(m==9) NLLnoBfillZero = NLLfillZero >= 0 ? -log(Best_Likelihood_900->at(0)/Best_PbTag_900->at(0)) : -1.;
      else if(m==10) NLLnoBfillZero = NLLfillZero >= 0 ? -log(Best_Likelihood_1000->at(0)/Best_PbTag_1000->at(0)) : -1.;
      else if(m==11) NLLnoBfillZero = NLLfillZero >= 0 ? -log(Best_Likelihood_1100->at(0)/Best_PbTag_1100->at(0)) : -1.;

      //variations of selections
      for(unsigned i = 0; i < 9; ++ i){
        if(RegionIdentifier[i] != bin) continue;
        //FIXME: Additional Lepton cuts
        /*if(i != 1 && i != 2 && i != 3 && i != 4 && LeptonPt < 40.) continue;
        else if(i == 1 && LeptonPt_SU < 40.) continue;
        else if(i == 2 && LeptonPt_SD < 40.) continue;
        else if(i == 3 && LeptonPt_RU < 40.) continue;
        else if(i == 4 && LeptonPt_RD < 40.) continue;*/

        //determine fill variable
        float fillVar = Vals[i];
        float fillBranch = 0.;
	if(m==3) fillBranch = Best_WPrimeMass_300->at(i); 
	else if(m==4) fillBranch = Best_WPrimeMass_400->at(i);
	else if(m==5) fillBranch = Best_WPrimeMass_500->at(i);
	else if(m==6) fillBranch = Best_WPrimeMass_600->at(i);
	else if(m==7) fillBranch = Best_WPrimeMass_700->at(i);
	else if(m==8) fillBranch = Best_WPrimeMass_800->at(i);
	else if(m==9) fillBranch = Best_WPrimeMass_900->at(i);
	else if(m==10) fillBranch = Best_WPrimeMass_1000->at(i);
	else if(m==11) fillBranch = Best_WPrimeMass_1100->at(i);

	float NLLfill = 0.;
	if(m==3) NLLfill = Best_Likelihood_300->at(i) >= 0 ? -log(Best_Likelihood_300->at(i)) : -1.;
	else if(m==4) NLLfill = Best_Likelihood_400->at(i) >= 0 ? -log(Best_Likelihood_400->at(i)) : -1.;
	else if(m==5) NLLfill = Best_Likelihood_500->at(i) >= 0 ? -log(Best_Likelihood_500->at(i)) : -1.;
	else if(m==6) NLLfill = Best_Likelihood_600->at(i) >= 0 ? -log(Best_Likelihood_600->at(i)) : -1.;
	else if(m==7) NLLfill = Best_Likelihood_700->at(i) >= 0 ? -log(Best_Likelihood_700->at(i)) : -1.;
	else if(m==8) NLLfill = Best_Likelihood_800->at(i) >= 0 ? -log(Best_Likelihood_800->at(i)) : -1.;
	else if(m==9) NLLfill = Best_Likelihood_900->at(i) >= 0 ? -log(Best_Likelihood_900->at(i)) : -1.;
	else if(m==10) NLLfill = Best_Likelihood_1000->at(i) >= 0 ? -log(Best_Likelihood_1000->at(i)) : -1.;
	else if(m==11) NLLfill = Best_Likelihood_1100->at(i) >= 0 ? -log(Best_Likelihood_1100->at(i)) : -1.;

	float NLLnoBfill = 0.;
	if(m==3) NLLnoBfill = NLLfill >= 0 ? -log(Best_Likelihood_300->at(i)/Best_PbTag_300->at(i)) : -1.;
	else if(m==4) NLLnoBfill = NLLfill >= 0 ? -log(Best_Likelihood_400->at(i)/Best_PbTag_400->at(i)) : -1.;
	else if(m==5) NLLnoBfill = NLLfill >= 0 ? -log(Best_Likelihood_500->at(i)/Best_PbTag_500->at(i)) : -1.;
	else if(m==6) NLLnoBfill = NLLfill >= 0 ? -log(Best_Likelihood_600->at(i)/Best_PbTag_600->at(i)) : -1.;
	else if(m==7) NLLnoBfill = NLLfill >= 0 ? -log(Best_Likelihood_700->at(i)/Best_PbTag_700->at(i)) : -1.;
	else if(m==8) NLLnoBfill = NLLfill >= 0 ? -log(Best_Likelihood_800->at(i)/Best_PbTag_800->at(i)) : -1.;
	else if(m==9) NLLnoBfill = NLLfill >= 0 ? -log(Best_Likelihood_900->at(i)/Best_PbTag_900->at(i)) : -1.;
	else if(m==10) NLLnoBfill = NLLfill >= 0 ? -log(Best_Likelihood_1000->at(i)/Best_PbTag_1000->at(i)) : -1.;
	else if(m==11) NLLnoBfill = NLLfill >= 0 ? -log(Best_Likelihood_1100->at(i)/Best_PbTag_1100->at(i)) : -1.;

        string HistName;
        if(Iterator == 2 && SFreg != 0){ //take care of all pT variations and their impact also on the ST values
	  const float STcorrCentralWeight = EventWeight[0]*SampleWeight*SFs[i].Eval(STvals[i]);
	  FitMass[m-3][i]->Fill(fillBranch, STcorrCentralWeight);
	  HT[m-3][i]->Fill(fillVar, STcorrCentralWeight);

	  FitMass_2D[m-3][i]->Fill(fillBranch, NLLfill, STcorrCentralWeight);
	  HT_2D[m-3][i]->Fill(fillVar, NLLfill, STcorrCentralWeight);

	  if(m==3) STrew[i]->Fill(STvals[i], STcorrCentralWeight);//only do this once for all masses

	  if(i == 0){//make sure to scale ttbar and get stat. unc. of fit propagated
	    const float statSFunc = CalculateCovError(STvals[0], SFcovs[0], jetMult);
	    const float STcorrStatUp = STcorrCentralWeight + statSFunc;
	    const float STcorrStatDown = STcorrCentralWeight - statSFunc;
	    FitMass_STstatUp[m-3]->Fill(fillBranch, STcorrStatUp);
            FitMass_STstatDown[m-3]->Fill(fillBranch, STcorrStatDown);
	    HT_STstatUp[m-3]->Fill(fillVar, STcorrStatUp);
	    HT_STstatDown[m-3]->Fill(fillVar, STcorrStatDown);

	    FitMass_2D_STstatUp[m-3]->Fill(fillBranch, NLLfillZero, STcorrStatUp);
	    FitMass_2D_STstatDown[m-3]->Fill(fillBranch, NLLfillZero, STcorrStatDown);
	    HT_2D_STstatUp[m-3]->Fill(fillVar, NLLfillZero, STcorrStatUp);
	    HT_2D_STstatDown[m-3]->Fill(fillVar, NLLfillZero, STcorrStatDown);

	    if(m==3){//only do this once for all masses
	      STrew_STstatUp->Fill(STvals[0], STcorrStatUp);
              STrew_STstatDown->Fill(STvals[0], STcorrStatDown);
	    }

	    NegLogLnoB[m-3]->Fill(NLLnoBfill, STcorrCentralWeight);
	    NegLogLnoBvsNegLogL[m-3]->Fill(NLLnoBfillZero, NLLfillZero, STcorrCentralWeight);
          }
        }
        else{
	  const float CentralWeight = EventWeight[0]*SampleWeight;
	  FitMass[m-3][i]->Fill(fillBranch, CentralWeight);
	  HT[m-3][i]->Fill(fillVar, CentralWeight);

	  FitMass_2D[m-3][i]->Fill(fillBranch, NLLfill, CentralWeight);
	  HT_2D[m-3][i]->Fill(fillVar, NLLfill, CentralWeight);

	  if(i==0){
	    NegLogLnoB[m-3]->Fill(NLLnoBfill, CentralWeight);
            NegLogLnoBvsNegLogL[m-3]->Fill(NLLnoBfillZero, NLLfillZero, CentralWeight);
	  }
        }
        if(m==3) ST[i]->Fill(STvals[i], EventWeight[0]*SampleWeight); //only do this once for all masses
      }

      //Make sure to match default region for default objects with event weight variations
      if(RegionIdentifier[0] == bin){
        //EventWeight variations
        for(unsigned i = 9; i < variations.size(); ++i){//last four variations are luminosity
	  //FIXME: Additional lepton cut
	  //if(LeptonPt < 40.) break;
	
          string HistName;

	  //determine fill variable
	  float fillVar = Vals[0];

	  float EvWeight = 1.;
	  if(i < variations.size()-4) EvWeight = EventWeight[i-8];
	  else{
	    if(i == variations.size()-4) EvWeight += LumiCorrVal;
	    else if(i == variations.size()-3) EvWeight -= LumiCorrVal;
	    else if(i == variations.size()-2) EvWeight += LumiStatVal;
	    else if(i == variations.size()-1) EvWeight -= LumiStatVal;
	    EvWeight *= EventWeight[0];
	  }

	  const float CentralWeight = EvWeight*SampleWeight;
          if(Iterator == 2 && SFreg != 0){
	    const float CentralWeightSTcorr = EvWeight*SampleWeight*SFs[i].Eval(STvals[0]);
	    FitMass[m-3][i]->Fill(fillBranchZero, CentralWeightSTcorr);
	    HT[m-3][i]->Fill(fillVar, CentralWeightSTcorr);

	    FitMass_2D[m-3][i]->Fill(fillBranchZero, NLLfillZero, CentralWeightSTcorr);
	    HT_2D[m-3][i]->Fill(fillVar, NLLfillZero, CentralWeightSTcorr);
	    if(m==3) STrew[i]->Fill(STvals[0], CentralWeightSTcorr); //do this only once for all masses
	  }
          else{
	    FitMass[m-3][i]->Fill(fillBranchZero, CentralWeight);
	    HT[m-3][i]->Fill(fillVar, CentralWeight);

	    FitMass_2D[m-3][i]->Fill(fillBranchZero, NLLfillZero, CentralWeight);
            HT_2D[m-3][i]->Fill(fillVar, NLLfillZero, CentralWeight);
	  }
	  if(m==3) ST[i]->Fill(STvals[0], CentralWeight); //only do this once for all masses
        }
      }
    }
  }
  //save all the W' variation histograms into files
  //fit mass file
  TFile *savefile;
  savefile = new TFile(TString::Format("TestHistograms/SimpleShapes_Bin%d_",bin)+YearType+TString::Format("_%d.root",Iterator),"RECREATE");
  TString STvarName = gn + "_" + binS + "_" + "STfit_" + YearS + "_";
  STvarName.Append(TString::Format("%d",SFreg));
  savefile->cd();
  for(unsigned i = 0; i < FitMass.size(); ++i){
    for(unsigned j = 0; j < FitMass[i].size(); ++j){
      if(dset.Type == 0){
        if(j>2) continue;
        if(Iterator <= 1){
	  FitMass[i][j]->Write("data_obs_" + binS + TString::Format("_M%d_",(i+3)*100)  + variations[j]);
	  if(i==0) ST[j]->Write("ST_data_obs_" + binS + "_" + variations[j]);
        } 
        else continue;
      }
      else if(Iterator == 2 && SFreg != 0){ //case of applying SF to ttbar
        if(j == 0) { //function to propagate ST-fit SF uncertainty to bin error
	  FitMass_STstatUp[i]->Write();
	  FitMass_STstatDown[i]->Write();
        }	
        FitMass[i][j]->Write();
        if(i==0){
          STrew[j]->Write();
          ST[j]->Write();
	  if(j==0){
            STrew_STstatUp->Write();
            STrew_STstatDown->Write();
	  }
        }
      }
      else{
        FitMass[i][j]->Write();
        if(i==0) ST[j]->Write();
      }
    }
    NegLogLnoB[i]->Write();
    NegLogLnoBvsNegLogL[i]->Write();
  }
  savefile->Close();

  //HT file
  TFile *savefileHT;
  savefileHT = new TFile(TString::Format("TestHistograms/HT_SimpleShapes_Bin%d_",bin)+YearType+TString::Format("_%d.root",Iterator),"RECREATE");
  savefileHT->cd();
  for(unsigned i = 0; i < HT.size(); ++i){
    for(unsigned j = 0; j < HT[i].size(); ++j){
      if(dset.Type == 0){
        if(j>2) continue;
        if(Iterator <= 1){
          HT[i][j]->Write("HT_data_obs_" + binS + TString::Format("_M%d_",(i+3)*100) + variations[j]);
          if(i==0) ST[i]->Write("ST_data_obs_" + binS + "_" + variations[j]);
        }
        else continue;
      }
      else if(Iterator == 2 && SFreg != 0){ //case of applying SF to ttbar
        if(j == 0) { //function to propagate ST-fit SF uncertainty to bin error
	  HT_STstatUp[i]->Write();
	  HT_STstatDown[i]->Write();
        }
        HT[i][j]->Write();
	if(i==0) ST[j]->Write();
      }
      else{
        HT[i][j]->Write();
        if(i==0) ST[j]->Write();
      }
    }
    NegLogLnoB[i]->Write();
    NegLogLnoBvsNegLogL[i]->Write();
  }
  savefileHT->Close();

  //2D histograms file for cutting on NLL and splitting between fit mass and HT
  TFile* savefile2D;
  savefile2D = new TFile(TString::Format("TestHistograms/TwoD_SimpleShapes_Bin%d_",bin)+YearType+TString::Format("_%d.root",Iterator),"RECREATE");
  savefile2D->cd();
  for(unsigned i = 0; i < HT.size(); ++i){
    for(unsigned j = 0; j < HT[i].size(); ++j){
      if(dset.Type == 0){
        if(j>2) continue;
        if(Iterator <= 1){
          HT_2D[i][j]->Write("HT_data_obs_" + binS + TString::Format("_M%d_",(i+3)*100) + variations[j]);
	  FitMass_2D[i][j]->Write("FitMass_data_obs_" + binS + TString::Format("_M%d_",(i+3)*100) + variations[j]);
        }
        else continue;
      }
      else if(Iterator == 2 && SFreg != 0){ //case of applying SF to ttbar
        if(j == 0){
	  FitMass_2D_STstatUp[i]->Write();
	  FitMass_2D_STstatDown[i]->Write();
	  HT_2D_STstatUp[i]->Write();
	  HT_2D_STstatDown[i]->Write();
        }
        HT_2D[i][j]->Write();
        FitMass_2D[i][j]->Write();
      }
      else{
        HT_2D[i][j]->Write();
        FitMass_2D[i][j]->Write();
      }
    }
    NegLogLnoB[i]->Write();
    NegLogLnoBvsNegLogL[i]->Write();
  }
  savefile2D->Close();

}
