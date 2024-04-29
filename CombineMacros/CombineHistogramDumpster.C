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
  vector<TH1F*> FitMass;
  vector<TH1F*> HT;
  vector<TH2F*> FitMass_2D, HT_2D;
  TH2F* FitMass_2D_STstatUp;
  TH2F* FitMass_2D_STstatDown;
  TH2F* HT_2D_STstatUp;
  TH2F* HT_2D_STstatDown;
  TH1F* FitMass_STstatUp;
  TH1F* FitMass_STstatDown;
  TH1F* HT_STstatUp;
  TH1F* HT_STstatDown;
  TH1F* STrew_STstatUp;
  TH1F* STrew_STstatDown;

  vector<TH1F*> ST;
  vector<TH1F*> STrew;

  TH1F* NegLogLnoB;
  TH2F* NegLogLnoBvsNegLogL;

  TString YearS = TString::Format("%d",year);

  //first variations are all weight variations and map 1:1, region variations start at index 21
  vector<TString> variations = {"" // 0
  , "electronScale"+YearS+"Up", "electronScale"+YearS+"Down", "electronRes"+YearS+"Up", "electronRes"+YearS+"Down", "JES"+YearS+"Up", "JES"+YearS+"Down", "JER"+YearS+"Up", "JER"+YearS+"Down" // 1 - 8
  , "electron"+YearS+"Up", "electron"+YearS+"Down", "muonTrigger"+YearS+"Up", "muonTrigger"+YearS+"Down", "muonId"+YearS+"Up", "muonId"+YearS+"Down", "muonIso"+YearS+"Up", "muonIso"+YearS+"Down" // 9 - 16
  , "BjetTagCorrUp", "BjetTagCorrDown", "BjetTagUncorr"+YearS+"Up", "BjetTagUncorr"+YearS+"Down", "PUID"+YearS+"Up", "PUID"+YearS+"Down", "L1PreFiring"+YearS+"Up", "L1PreFiring"+YearS+"Down" // 17 - 24
  , "PUreweight"+YearS+"Up", "PUreweight"+YearS+"Down", "PDFUp", "PDFDown", "LHEScaleUp", "LHEScaleDown", // 25 - 30
  "LumiCorrUp", "LumiCorrDown", "LumiStat"+YearS+"Up", "LumiStat"+YearS+"Down" //31-34
  };

  vector<TString> variationsName, HTvariationsName, FitMass2Dnames, HT2Dnames;
  for (unsigned i = 0; i < variations.size(); ++i) {
    variationsName.push_back(gn + "_" + binS + "_" + variations[i]);
    HTvariationsName.push_back("HT_"+variationsName[i]);
    FitMass2Dnames.push_back("FitMass2D_"+variationsName[i]);
    HT2Dnames.push_back("HT2D_"+variationsName[i]);
  }

  for(unsigned i = 0; i < variations.size(); ++i){

    //Extraction variable block
    if(bin % 100 < 60){
      if(bin % 10 == 3){ //5 jets 3 b-tags
	double FitLimits[36] = {85., 220., 230., 239., 247., 255., 263., 270., 276., 282., 288., 294., 301., 308., 315., 322., 329., 336., 343., 350., 360., 372., 384., 396., 410., 425., 445., 465., 490., 515., 550., 590., 650., 730., 880., 2000.};
        FitMass.push_back(new TH1F(variationsName[i],"fitted W' mass; m_{W'} [GeV/c^{2}]; Events", 35, FitLimits));
	double HTlimits[47] = {125., 230., 245., 258., 265., 273., 281., 288., 294., 300., 306., 312., 318., 324., 330., 336., 342., 348., 354., 360., 367., 374., 381., 388., 395., 402., 409., 417., 426., 435., 445., 455., 465., 475., 487., 501., 515., 531., 550., 570., 595., 625., 660., 705., 780., 910.,  2000.};
        HT.push_back(new TH1F(HTvariationsName[i],"H_{T}; H_{T} [GeV/c^{2}]; Events", 46, HTlimits));
	double NLLlimits[53] = {0., 0.80, 1.05, 1.25, 1.45, 1.60, 1.75, 1.90, 2.00, 2.15, 2.30, 2.40, 2.65, 2.80, 2.92, 3.05, 3.20, 3.30, 3.45, 3.60, 3.75, 3.90, 4.00, 4.20, 4.30, 4.45, 4.60, 4.75, 4.90, 5.10, 5.25, 5.40, 5.60, 5.80, 6.00, 6.25, 6.50, 6.75, 7.00, 7.35, 7.70, 8.05, 8.40, 8.80, 9.20, 9.70, 10.10, 10.65, 11.25, 12.00, 12.95, 14.40, 30.};
        FitMass_2D.push_back(new TH2F(FitMass2Dnames[i],"fitted W' mass vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", 35, FitLimits, 52, NLLlimits));
        HT_2D.push_back(new TH2F(HT2Dnames[i],"HT vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", 46, HTlimits, 52, NLLlimits));
      } else if(bin % 10 == 4){//5 jets 4 b-tags
	  double FitLimits[2] = {120., 2000.};
	  FitMass.push_back(new TH1F(variationsName[i],"fitted W' mass; m_{W'} [GeV/c^{2}]; Events", 1, FitLimits));
	  double HTlimits[15] = {155., 270., 300., 325., 345., 365., 385., 410., 435., 465., 495., 545., 610., 740., 2000.};
	  HT.push_back(new TH1F(HTvariationsName[i],"H_{T}; H_{T} [GeV/c^{2}]; Events", 14, HTlimits));
	  double NLLlimits[53] = {0., 0.80, 1.05, 1.25, 1.45, 1.60, 1.75, 1.90, 2.00, 2.15, 2.30, 2.40, 2.65, 2.80, 2.92, 3.05, 3.20, 3.30, 3.45, 3.60, 3.75, 3.90, 4.00, 4.20, 4.30, 4.45, 4.60, 4.75, 4.90, 5.10, 5.25, 5.40, 5.60, 5.80, 6.00, 6.25, 6.50, 6.75, 7.00, 7.35, 7.70, 8.05, 8.40, 8.80, 9.20, 9.70, 10.10, 10.65, 11.25, 12.00, 12.95, 14.40, 30.};
        FitMass_2D.push_back(new TH2F(FitMass2Dnames[i],"fitted W' mass vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", 1, FitLimits, 52, NLLlimits));
        HT_2D.push_back(new TH2F(HT2Dnames[i],"HT vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", 14, HTlimits, 52, NLLlimits));
      } else {
        FitMass.push_back(new TH1F(variationsName[i],"fitted W' mass; m_{W'} [GeV/c^{2}]; Events", 400, 0., 2000.));
        HT.push_back(new TH1F(HTvariationsName[i],"H_{T}; H_{T} [GeV/c^{2}]; Events", 400, 0., 2000.));
        FitMass_2D.push_back(new TH2F(FitMass2Dnames[i],"fitted W' mass vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", 400, 0., 2000., 60, 0., 30.));
        HT_2D.push_back(new TH2F(HT2Dnames[i],"HT vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", 400, 0., 2000., 60, 0., 30.));
      }
    }
    else{
      if(bin % 10 == 3){//6 jets 3 b-tags
	double FitLimits[28] = {55., 234., 246., 257., 268., 279., 290., 299., 308., 317., 326., 335., 345., 360., 375., 390., 405., 425., 450., 475., 505., 540., 580., 635., 715., 820., 1020., 2000.};
        FitMass.push_back(new TH1F(variationsName[i],"fitted W' mass; m_{W'} [GeV/c^{2}]; Events", 27, FitLimits));
	double HTlimits[31] = {155., 195., 320., 335., 351., 363., 375., 387., 399., 411., 422., 433., 445., 457., 469., 483., 497., 511., 527., 544., 562., 581., 605., 630., 660., 690., 735., 795., 875., 1000.,  2000.};
        HT.push_back(new TH1F(HTvariationsName[i],"H_{T}; H_{T} [GeV/c^{2}]; Events", 30, HTlimits));
        double NLLlimits[24] = {0., 0.80, 1.10, 1.35, 1.60, 1.80, 2.05, 2.30, 2.50, 2.75, 3.00, 3.30, 3.65, 4.00, 4.35, 4.70, 5.15, 5.70, 6.45, 7.75, 9.20, 11.00, 24.55, 30.};
        FitMass_2D.push_back(new TH2F(FitMass2Dnames[i],"fitted W' mass vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", 27, FitLimits, 23, NLLlimits));
        HT_2D.push_back(new TH2F(HT2Dnames[i],"HT vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", 30, HTlimits, 23, NLLlimits));	      
      } else if(bin % 10 == 4){//6 jets 4 b-tags
	double FitLimits[19] = {185., 325., 355., 380., 400., 420., 440., 460., 480., 500., 525., 550., 575., 615., 655., 710., 785., 915., 2000.};
	FitMass.push_back(new TH1F(variationsName[i],"fitted W' mass; m_{W'} [GeV/c^{2}]; Events", 18, FitLimits));
	double HTlimits[37] {185., 290., 315., 340., 355., 365., 375., 385., 395., 405., 415., 425., 435., 445., 455., 465., 475., 485., 495., 505., 515., 525., 535., 545., 555., 570., 590., 610., 635., 660., 690., 720., 760., 820., 890., 1010., 2000.};
	HT.push_back(new TH1F(HTvariationsName[i],"H_{T}; H_{T} [GeV/c^{2}]; Events", 36, HTlimits));
	double NLLlimits[24] = {0., 0.80, 1.10, 1.35, 1.60, 1.80, 2.05, 2.30, 2.50, 2.75, 3.00, 3.30, 3.65, 4.00, 4.35, 4.70, 5.15, 5.70, 6.45, 7.75, 9.20, 11.00, 24.55, 30.};
        FitMass_2D.push_back(new TH2F(FitMass2Dnames[i],"fitted W' mass vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", 18, FitLimits, 23, NLLlimits));
        HT_2D.push_back(new TH2F(HT2Dnames[i],"HT vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", 36, HTlimits, 23, NLLlimits));
      } else {
        FitMass.push_back(new TH1F(variationsName[i],"fitted W' mass; m_{W'} [GeV/c^{2}]; Events", 400, 0., 2000.));
        HT.push_back(new TH1F(HTvariationsName[i],"H_{T}; H_{T} [GeV/c^{2}]; Events", 400, 0., 2000.));
        FitMass_2D.push_back(new TH2F(FitMass2Dnames[i],"fitted W' mass vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", 400, 0., 2000., 60, 0., 30.));
        HT_2D.push_back(new TH2F(HT2Dnames[i],"HT vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", 400, 0., 2000., 60, 0., 30.));
      }
    }

    

    //need to fill ST collection
    TString STname = "ST_";
    STname.Append(variationsName[i]);

    TString STrewName = "STrew_";
    STrewName.Append(variationsName[i]);
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
      if(bin % 100 < 60) SFfit = new TF1(TString::Format("fitFunction%d",i),"[0]/x/x/x+[1]/x/x+[2]/x+[3]+[4]*x+[5]*x*x", 150., 2000.);
      else 		 SFfit = new TF1(TString::Format("fitFunction%d",i),"[0]/x+[1]+[2]*x+[3]*x*x", 150., 2000.);
      TFitResultPtr fr = SF->Fit(SFfit,"SRF");
      TMatrixD cov = fr->GetCovarianceMatrix();
      SFs.push_back(*SFfit);
      SFcovs.push_back(cov);
    }
  }
  //ST stat variations block
  FitMass_STstatUp = (TH1F*) FitMass[0]->Clone("STstatUp");
  FitMass_STstatDown = (TH1F*) FitMass[0]->Clone("STstatDown");
  HT_STstatUp = (TH1F*) HT[0]->Clone("STstatUp");
  HT_STstatDown = (TH1F*) HT[0]->Clone("STstatDown");
  FitMass_2D_STstatUp = (TH2F*) FitMass_2D[0]->Clone("FitMass2D_STstatUp");
  FitMass_2D_STstatDown = (TH2F*) FitMass_2D[0]->Clone("FitMass2D_STstatDown");
  HT_2D_STstatUp = (TH2F*) HT_2D[0]->Clone("HT2D_STstatUp");
  HT_2D_STstatDown = (TH2F*) HT_2D[0]->Clone("HT2D_STstatDown");
  STrew_STstatUp = (TH1F*) STrew[0]->Clone("STrew_" + gn + "_" + binS + "_" + "STstatUp");
  STrew_STstatDown = (TH1F*) STrew[0]->Clone("STrew_" + gn + "_" + binS + "_" + "STstatDown");
  

  //negative log likelihood block
  TString NLLname = "NegLogLnoB_";
  NLLname.Append(variationsName[0]);
  TString NLL2Dname = "NegLogLnoBvsNegLogL_";
  NLL2Dname.Append(variationsName[0]);
  if(bin % 100 < 60){
    double NLLlimits[53] = {0., 0.80, 1.05, 1.25, 1.45, 1.60, 1.75, 1.90, 2.00, 2.15, 2.30, 2.40, 2.65, 2.80, 2.92, 3.05, 3.20, 3.30, 3.45, 3.60, 3.75, 3.90, 4.00, 4.20, 4.30, 4.45, 4.60, 4.75, 4.90, 5.10, 5.25, 5.40, 5.60, 5.80, 6.00, 6.25, 6.50, 6.75, 7.00, 7.35, 7.70, 8.05, 8.40, 8.80, 9.20, 9.70, 10.10, 10.65, 11.25, 12.00, 12.95, 14.40, 30.};
    NegLogLnoB = new TH1F(NLLname, "-log(L) without b-tagging; -log(L)_{!b}; Events", 52, NLLlimits);
    
    NegLogLnoBvsNegLogL = new TH2F(NLL2Dname, "-log(L) without b-tagging vs -log(L); -log(L)_{!b}; -log(L); Events", 52, NLLlimits, 52, NLLlimits);
  }
  else{
    double NLLlimits[24] = {0., 0.80, 1.10, 1.35, 1.60, 1.80, 2.05, 2.30, 2.50, 2.75, 3.00, 3.30, 3.65, 4.00, 4.35, 4.70, 5.15, 5.70, 6.45, 7.75, 9.20, 11.00, 24.55, 30.};
    NegLogLnoB = new TH1F(NLLname, "-log(L) without b-tagging; -log(L)_{!b}; Events", 23, NLLlimits);
    NegLogLnoBvsNegLogL = new TH2F(NLL2Dname, "-log(L) without b-tagging vs -log(L); -log(L)_{!b}; -log(L); Events", 23, NLLlimits, 23, NLLlimits);
  }

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
      JetCounts[0]++;
      JetCounts[1]++;
      JetCounts[2]++;
      JetCounts[3]++;
      JetCounts[4]++;
      if(JetbTag->at(i)) bJetMainCount++;
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
    
    int RegionIdents[9];
    RegionIdents[0] = (RegionIdentifier[0]/1000)*1000 + 100 + JetCounts[0]*10 + bJetMainCount;
    RegionIdents[1] = (RegionIdentifier[1]/1000)*1000 + 100 + JetCounts[1]*10 + RegionIdentifier[1] % 10;
    RegionIdents[2] = (RegionIdentifier[2]/1000)*1000 + 100 + JetCounts[2]*10 + RegionIdentifier[2] % 10;
    RegionIdents[3] = (RegionIdentifier[3]/1000)*1000 + 100 + JetCounts[3]*10 + RegionIdentifier[3] % 10;
    RegionIdents[4] = (RegionIdentifier[4]/1000)*1000 + 100 + JetCounts[4]*10 + RegionIdentifier[4] % 10;    
    RegionIdents[5] = (RegionIdentifier[5]/1000)*1000 + 100 + JetCounts[5]*10 + RegionIdentifier[5] % 10;
    RegionIdents[6] = (RegionIdentifier[6]/1000)*1000 + 100 + JetCounts[6]*10 + RegionIdentifier[6] % 10;    
    RegionIdents[7] = (RegionIdentifier[7]/1000)*1000 + 100 + JetCounts[7]*10 + RegionIdentifier[7] % 10;
    RegionIdents[8] = (RegionIdentifier[8]/1000)*1000 + 100 + JetCounts[8]*10 + RegionIdentifier[8] % 10;

    //variations of selections
    for(unsigned i = 0; i < 9; ++ i){
      //if(RegionIdentifier[i] != bin) continue;
      if(RegionIdents[i] != bin) continue;

      //FIXME: Additional Lepton cuts
      /*if(i != 1 && i != 2 && i != 3 && i != 4 && LeptonPt < 40.) continue;
      else if(i == 1 && LeptonPt_SU < 40.) continue;
      else if(i == 2 && LeptonPt_SD < 40.) continue;
      else if(i == 3 && LeptonPt_RU < 40.) continue;
      else if(i == 4 && LeptonPt_RD < 40.) continue;*/

      //determine fill variable
      float fillVar = Vals[i];
      float fillBranch = Best_WPrimeMass->at(i); 
      //float fillBranch = WPrimeMassSimpleFL->at(i);


      string HistName;
      if(Iterator == 2 && SFreg != 0){ //take care of all pT variations and their impact also on the ST values
	FitMass[i]->Fill(fillBranch,EventWeight[0]*SampleWeight*SFs[i].Eval(STvals[i]));
	HT[i]->Fill(fillVar,EventWeight[0]*SampleWeight*SFs[i].Eval(STvals[i]));

	FitMass_2D[i]->Fill(fillBranch,-log(Best_Likelihood->at(i)),EventWeight[0]*SampleWeight*SFs[i].Eval(STvals[i]));
	HT_2D[i]->Fill(fillVar,-log(Best_Likelihood->at(i)),EventWeight[0]*SampleWeight*SFs[i].Eval(STvals[i]));

	STrew[i]->Fill(STvals[i],EventWeight[0]*SampleWeight*SFs[i].Eval(STvals[i]));

	if(i == 0){//make sure to scale ttbar and get stat. unc. of fit propagated
	  float statSFunc = CalculateCovError(STvals[0], SFcovs[0], jetMult);
	  FitMass_STstatUp->Fill(fillBranch,EventWeight[0]*SampleWeight*(SFs[0].Eval(STvals[0])+statSFunc));
          FitMass_STstatDown->Fill(fillBranch,EventWeight[0]*SampleWeight*(SFs[0].Eval(STvals[0])-statSFunc));
	  HT_STstatUp->Fill(fillVar,EventWeight[0]*SampleWeight*(SFs[0].Eval(STvals[0])+statSFunc));
	  HT_STstatDown->Fill(fillVar,EventWeight[0]*SampleWeight*(SFs[0].Eval(STvals[0])-statSFunc));

	  FitMass_2D_STstatUp->Fill(fillBranch,-log(Best_Likelihood->at(0)),EventWeight[0]*SampleWeight*(SFs[0].Eval(STvals[0])+statSFunc));
	  FitMass_2D_STstatDown->Fill(fillBranch,-log(Best_Likelihood->at(0)),EventWeight[0]*SampleWeight*(SFs[0].Eval(STvals[0])-statSFunc));
	  HT_2D_STstatUp->Fill(fillVar,-log(Best_Likelihood->at(0)),EventWeight[0]*SampleWeight*(SFs[0].Eval(STvals[0])+statSFunc));
	  HT_2D_STstatDown->Fill(fillVar,-log(Best_Likelihood->at(0)),EventWeight[0]*SampleWeight*(SFs[0].Eval(STvals[0])-statSFunc));

	  STrew_STstatUp->Fill(STvals[0],EventWeight[0]*SampleWeight*(SFs[0].Eval(STvals[0])+statSFunc));
          STrew_STstatDown->Fill(STvals[0],EventWeight[0]*SampleWeight*(SFs[0].Eval(STvals[0])-statSFunc));

	  NegLogLnoB->Fill(-log(Best_Likelihood->at(i)/Best_PbTag->at(i)),EventWeight[0]*SampleWeight*SFs[i].Eval(STvals[i]));
	  NegLogLnoBvsNegLogL->Fill(-log(Best_Likelihood->at(0)/Best_PbTag->at(0)),-log(Best_Likelihood->at(0)),EventWeight[0]*SampleWeight*SFs[i].Eval(STvals[i]));

        }
      }
      else{
	FitMass[i]->Fill(fillBranch,EventWeight[0]*SampleWeight);
	HT[i]->Fill(fillVar,EventWeight[0]*SampleWeight);

	FitMass_2D[i]->Fill(fillBranch,-log(Best_Likelihood->at(i)),EventWeight[0]*SampleWeight);
	HT_2D[i]->Fill(fillVar,-log(Best_Likelihood->at(i)),EventWeight[0]*SampleWeight);

	if(i==0){
	  NegLogLnoB->Fill(-log(Best_Likelihood->at(i)/Best_PbTag->at(i)),EventWeight[0]*SampleWeight);
          NegLogLnoBvsNegLogL->Fill(-log(Best_Likelihood->at(0)/Best_PbTag->at(0)),-log(Best_Likelihood->at(0)),EventWeight[0]*SampleWeight);
	}
      }
      ST[i]->Fill(STvals[i],EventWeight[0]*SampleWeight);
    }
    //Make sure to match default region for default objects with event weight variations
    //if(RegionIdentifier[0] == bin){
    if(RegionIdents[0] == bin){
      //EventWeight variations
      for(unsigned i = 9; i < variations.size(); ++i){//last four variations are luminosity

	//FIXME: Additional lepton cut
	//if(LeptonPt < 40.) break;
	
        string HistName;

	//determine fill variable
	float fillVar = Vals[0];
	float fillBranch = Best_WPrimeMass->at(0);
	//float fillBranch = WPrimeMassSimpleFL->at(0);
	float EvWeight = 1.;
	if(i < variations.size()-4) EvWeight = EventWeight[i-8];
	else{
	  if(i == variations.size()-4) EvWeight += LumiCorrVal;
	  else if(i == variations.size()-3) EvWeight -= LumiCorrVal;
	  else if(i == variations.size()-2) EvWeight += LumiStatVal;
	  else if(i == variations.size()-1) EvWeight -= LumiStatVal;
	  EvWeight *= EventWeight[0];
	}

        if(Iterator == 2 && SFreg != 0){
	  FitMass[i]->Fill(fillBranch,EvWeight*SampleWeight*SFs[i].Eval(STvals[0]));
	  HT[i]->Fill(fillVar,EvWeight*SampleWeight*SFs[i].Eval(STvals[0]));

	  FitMass_2D[i]->Fill(fillBranch,-log(Best_Likelihood->at(0)),EvWeight*SampleWeight*SFs[i].Eval(STvals[0]));
	  HT_2D[i]->Fill(fillVar,-log(Best_Likelihood->at(0)),EvWeight*SampleWeight*SFs[i].Eval(STvals[0]));
	  
	  STrew[i]->Fill(STvals[0],EvWeight*SampleWeight*SFs[i].Eval(STvals[0]));
	}
        else{
	  FitMass[i]->Fill(fillBranch,EvWeight*SampleWeight);
	  HT[i]->Fill(fillVar,EvWeight*SampleWeight);

	  FitMass_2D[i]->Fill(fillBranch,-log(Best_Likelihood->at(0)),EvWeight*SampleWeight);
          HT_2D[i]->Fill(fillVar,-log(Best_Likelihood->at(0)),EvWeight*SampleWeight);
	}
	ST[i]->Fill(STvals[0],EvWeight*SampleWeight);
      }

      //luminosity variations

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
    if(dset.Type == 0){
      if(i>2) continue;
      if(Iterator <= 1){
	FitMass[i]->Write("data_obs_" + binS + "_" + variations[i]);
	ST[i]->Write("ST_data_obs_" + binS + "_" + variations[i]);
      } 
      else continue;
    }
    else if(Iterator == 2 && SFreg != 0){ //case of applying SF to ttbar
      if(i == 0) /*for(unsigned x = 0; x < FitMass[i]->GetNbinsX(); ++x)*/{ //function to propagate ST-fit SF uncertainty to bin error
	FitMass_STstatUp->Write(STvarName + "_STfitUp");
	FitMass_STstatDown->Write(STvarName + "_STfitDown");
	//float statBase = FitMass[i]->GetBinError(x+1);
	//float SFstat = fabs(FitMass_STstatUp->GetBinContent(x+1) - FitMass[i]->GetBinContent(x+1));
        //FitMass[i]->SetBinError(x+1,sqrt(pow(statBase,2)+pow(SFstat,2)));
      }	
      FitMass[i]->Write(variationsName[i]);
      STrew[i]->Write(STrew[i]->GetName());
      ST[i]->Write(ST[i]->GetName());
      STrew_STstatUp->Write(STrew_STstatUp->GetName());
      STrew_STstatDown->Write(STrew_STstatDown->GetName());
    }
    else{
      FitMass[i]->Write(variationsName[i]);
      ST[i]->Write(ST[i]->GetName());
    }
  }
  NegLogLnoB->Write(NegLogLnoB->GetName());
  NegLogLnoBvsNegLogL->Write(NegLogLnoBvsNegLogL->GetName());
  savefile->Close();

  //HT file
  TFile *savefileHT;
  savefileHT = new TFile(TString::Format("TestHistograms/HT_SimpleShapes_Bin%d_",bin)+YearType+TString::Format("_%d.root",Iterator),"RECREATE");
  savefileHT->cd();
  for(unsigned i = 0; i < HT.size(); ++i){
    if(dset.Type == 0){
      if(i>2) continue;
      if(Iterator <= 1){
        HT[i]->Write("HT_data_obs_" + binS + "_" + variations[i]);
        ST[i]->Write("ST_data_obs_" + binS + "_" + variations[i]);
      }
      else continue;
    }
    else if(Iterator == 2 && SFreg != 0){ //case of applying SF to ttbar
      if(i == 0) /*for(unsigned x = 0; x < HT[i]->GetNbinsX(); ++x)*/{ //function to propagate ST-fit SF uncertainty to bin error
	HT_STstatUp->Write("HT_" + STvarName + "_STfitUp");
	HT_STstatDown->Write("HT_" + STvarName + "_STfitDown");
        //float statBase = HT[i]->GetBinError(x+1);
        //float SFstat = fabs(HT_STstatUp->GetBinContent(x+1) - HT[i]->GetBinContent(x+1));
        //HT[i]->SetBinError(x+1,sqrt(pow(statBase,2)+pow(SFstat,2)));
      }
      HT[i]->Write(HTvariationsName[i]);
      ST[i]->Write(ST[i]->GetName());
    }
    else{
      HT[i]->Write(HTvariationsName[i]);
      ST[i]->Write(ST[i]->GetName());
    }
  }
  NegLogLnoB->Write(NegLogLnoB->GetName());
  NegLogLnoBvsNegLogL->Write(NegLogLnoBvsNegLogL->GetName());
  savefileHT->Close();

  //2D histograms file for cutting on NLL and splitting between fit mass and HT
  TFile* savefile2D;
  savefile2D = new TFile(TString::Format("TestHistograms/TwoD_SimpleShapes_Bin%d_",bin)+YearType+TString::Format("_%d.root",Iterator),"RECREATE");
  savefile2D->cd();
  for(unsigned i = 0; i < HT.size(); ++i){
    if(dset.Type == 0){
      if(i>2) continue;
      if(Iterator <= 1){
        HT_2D[i]->Write("HT_data_obs_" + binS + "_" + variations[i]);
	FitMass_2D[i]->Write("FitMass_data_obs_" + binS + "_" + variations[i]);
      }
      else continue;
    }
    else if(Iterator == 2 && SFreg != 0){ //case of applying SF to ttbar
      if(i == 0){
	/*for(unsigned x = 0; x < FitMass_2D[i]->GetNbinsX(); ++x) for(unsigned y = 0; y < FitMass_2D[i]->GetNbinsY(); ++y){ //function to propagate ST-fit SF uncertainty to bin error
          float statBase = FitMass_2D[i]->GetBinError(x+1, y+1);
          float SFstat = fabs(FitMass_STstatUp->GetBinContent(x+1, y+1) - FitMass_2D[i]->GetBinContent(x+1, y+1));
          FitMass_2D[i]->SetBinError(x+1,y+1,sqrt(pow(statBase,2)+pow(SFstat,2)));
	}
	for(unsigned x = 0; x < HT_2D[i]->GetNbinsX(); ++x) for(unsigned y = 0; y < HT_2D[i]->GetNbinsY(); ++y){ //function to propagate ST-fit SF uncertainty to bin error
          float statBase = HT_2D[i]->GetBinError(x+1, y+1);
          float SFstat = fabs(HT_STstatUp->GetBinContent(x+1, y+1) - HT_2D[i]->GetBinContent(x+1, y+1));
          HT_2D[i]->SetBinError(x+1,y+1,sqrt(pow(statBase,2)+pow(SFstat,2)));
        }*/
	FitMass_2D_STstatUp->Write("FitMass2D_" + STvarName + "_STfitUp");
	FitMass_2D_STstatDown->Write("FitMass2D_" + STvarName + "_STfitDown");
	HT_2D_STstatUp->Write("HT2D_" + STvarName + "_STfitUp");
	HT_2D_STstatDown->Write("HT2D_" + STvarName + "_STfitDown");
      }
      HT_2D[i]->Write(HT_2D[i]->GetName());
      FitMass_2D[i]->Write(FitMass_2D[i]->GetName());
    }
    else{
      HT_2D[i]->Write(HT_2D[i]->GetName());
      FitMass_2D[i]->Write(FitMass_2D[i]->GetName());
    }
  }
  NegLogLnoB->Write(NegLogLnoB->GetName());
  NegLogLnoBvsNegLogL->Write(NegLogLnoBvsNegLogL->GetName());
  savefile2D->Close();

}
