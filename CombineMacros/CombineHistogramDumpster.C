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
#include "Systematics.C"
#include "BinTables.C"

//small function to calculate covariance matrix envelope of fit function
float CalculateCovError(float STval, TMatrixD covM, int jetNumber){
  vector<double> Derivatives;
  if(jetNumber==5) Derivatives = {1./STval/STval, 1./STval, 1., STval, STval*STval};
  else		   Derivatives = {1./STval/STval, 1./STval, 1., STval};
  float FinalEnvelope = 0.;
  for(unsigned x = 0; x < Derivatives.size(); ++x){
    for(unsigned y = 0; y < Derivatives.size(); ++y){
      FinalEnvelope += Derivatives[x] * Derivatives[y] * covM(x,y);
    }
  }
  return sqrt(FinalEnvelope);
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
  TF1 SFparDown, SFparUp, SFQCDDown, SFQCDUp;
  vector<TMatrixD> SFcovs;

  //define electron HLT inefficiency Zvtx uncertainty
  float EleHLTzvtx = 1.;

  //define size of pT systematics block
  unsigned sizePtSysts = 8;

  //set sample weight
  int Year = 0;
  int year = 0;
  float Lumi = 0.;
  if(YearType == "2016_APV")  {Year = 0; year = 2016; Lumi = 19.52;}
  else if(YearType == "2016") {Year = 1; year = 2016; Lumi = 16.81;}
  else if(YearType == "2017") {Year = 2; year = 2017; Lumi = 41.48; EleHLTzvtx = 0.991;}
  else if(YearType == "2018") {Year = 3; year = 2018; Lumi = 59.83;}
  float SampleWeight = 1.;
  if(dset.Type != 0){
    SampleWeight = Lumi * dset.CrossSection / dset.Size[Year];
  }

  TString YearS = YearType;
  if(YearS=="2016_APV") YearS="2016apv";  //for compatibility reasons, interface Patrick's APV naming with Sifu's APV naming scheme

  TString binS = TString::Format("Wprime%d_", bin) + YearS;

  //define variations
  vector<vector<TH1F*> > FitMass;
  vector<vector<TH1F*> > HT;
  vector<vector<TH2F*> > FitMass_2D, HT_2D;
  vector<TH2F*> FitMass_2D_STstatUp, FitMass_2D_STstatDown;
  vector<TH2F*> HT_2D_STstatUp, HT_2D_STstatDown;
  vector<TH1F*> FitMass_STstatUp, FitMass_STstatDown;
  vector<TH1F*> HT_STstatUp, HT_STstatDown;
  TH1F* STrew_STstatUp, *STrew_STstatDown;
  vector<TH2F*> FitMass_2D_STparUp, FitMass_2D_STparDown;
  vector<TH2F*> HT_2D_STparUp, HT_2D_STparDown;
  vector<TH1F*> FitMass_STparUp, FitMass_STparDown;
  vector<TH1F*> HT_STparUp, HT_STparDown;
  TH1F* STrew_STparUp, *STrew_STparDown;
  TH1F* STrew_STQCDUp, *STrew_STQCDDown;

  vector<TH1F*> ST;
  vector<TH1F*> STrew;

  vector<TH1F*> NegLogLnoB;
  vector<TH2F*> NegLogLnoBvsNegLogL;

  TString B2Gn = "25008"; //placeholder, until we get a cadi line number
  TString sampleType = gn;
  if(dset.Type == 2) sampleType = "signal";

  TH1::AddDirectory(false);//FIXME:Suppress warnings for histogram declarations

  //Decide if this is ttbar with SF from ST correction
  bool IsSF_ttbar = Iterator >= 2 && Iterator <= 7 && SFreg != 0;

  //Declare hardcoded what the size of the systematics variations is:
  unsigned varSize = 45;

  //assemble histograms with variations for Fit mass, HT, 2D Fit mass vs NLL, 2D HT vs NLL, looping over the mass interpretations from 300 GeV to 1.1 TeV
  vector<vector<TString> > variationsName, HTvariationsName, FitMass2Dnames, HT2Dnames;
  for(unsigned m = 3; m < 12; m++){
    vector<TString> dummy;
    variationsName.push_back(dummy);
    HTvariationsName.push_back(dummy);
    FitMass2Dnames.push_back(dummy);
    HT2Dnames.push_back(dummy);
    for(unsigned i = 0; i < varSize; ++i) { //standard systematic names list
      TString variation = Systematics(i, YearS, sampleType, B2Gn);
      variationsName[m-3].push_back(gn + "_" + binS + TString::Format("_M%d_",m*100) + variation);
      HTvariationsName[m-3].push_back("HT_" + variationsName[m-3][i]);
      FitMass2Dnames[m-3].push_back("FitMass2D_" + variationsName[m-3][i]);
      HT2Dnames[m-3].push_back("HT2D_" + variationsName[m-3][i]);
    }
  }

  vector<TString> variationsNamePlain;
  for(unsigned i = 0; i < varSize; ++i){
    TString variation = Systematics(i, YearS, sampleType, B2Gn);
    variationsNamePlain.push_back(gn + "_" + binS + "_" + variation);
  }

  for(unsigned m = 3; m < 12; ++m){
    vector<TH1F*> dummy1D;
    FitMass.push_back(dummy1D);
    HT.push_back(dummy1D);
    vector<TH2F*> dummy2D;
    FitMass_2D.push_back(dummy2D);
    HT_2D.push_back(dummy2D);

    for(unsigned i = 0; i < varSize; ++i){
      TString variation = "";
      if(i < varSize) variation = Systematics(i, YearS, sampleType, B2Gn);
      else            variation = Systematics(i-varSize, YearS, sampleType, B2Gn, true);

      //Extraction variable block
      if(bin % 100 < 60){
        FitMass[m-3].push_back(new TH1F(variationsName[m-3][i],"fitted W' mass; m_{W'} [GeV/c^{2}]; Events", nFitLimits53_500, FitLimits53_500));
        HT[m-3].push_back(new TH1F(HTvariationsName[m-3][i],"H_{T}; H_{T} [GeV/c^{2}]; Events", nHTlimits53, HTlimits53));
        FitMass_2D[m-3].push_back(new TH2F(FitMass2Dnames[m-3][i],"fitted W' mass vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", nFitLimits53_500, FitLimits53_500, nNLLlimits53_500, NLLlimits53_500));
        HT_2D[m-3].push_back(new TH2F(HT2Dnames[m-3][i],"HT vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", nHTlimits53, HTlimits53, nNLLlimits53_500, NLLlimits53_500));
      }
      else{
        if(bin % 10 <= 3){//6 jets 3 b-tags
          FitMass[m-3].push_back(new TH1F(variationsName[m-3][i],"fitted W' mass; m_{W'} [GeV/c^{2}]; Events", nFitLimits63_500, FitLimits63_500));
          HT[m-3].push_back(new TH1F(HTvariationsName[m-3][i],"H_{T}; H_{T} [GeV/c^{2}]; Events", nHTlimits63, HTlimits63));
          FitMass_2D[m-3].push_back(new TH2F(FitMass2Dnames[m-3][i],"fitted W' mass vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", nFitLimits63_500, FitLimits63_500, nNLLlimits64_500, NLLlimits64_500));
          HT_2D[m-3].push_back(new TH2F(HT2Dnames[m-3][i],"HT vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", nHTlimits63, HTlimits63, nNLLlimits64_500, NLLlimits64_500));	      
        } else if(bin % 10 == 4){//6 jets 4 b-tags
	  FitMass[m-3].push_back(new TH1F(variationsName[m-3][i],"fitted W' mass; m_{W'} [GeV/c^{2}]; Events", nFitLimits64_500, FitLimits64_500));
	  HT[m-3].push_back(new TH1F(HTvariationsName[m-3][i],"H_{T}; H_{T} [GeV/c^{2}]; Events", nHTlimits64, HTlimits64));
          FitMass_2D[m-3].push_back(new TH2F(FitMass2Dnames[m-3][i],"fitted W' mass vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", nFitLimits64_500, FitLimits64_500, nNLLlimits64_500, NLLlimits64_500));
          HT_2D[m-3].push_back(new TH2F(HT2Dnames[m-3][i],"HT vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", nHTlimits64, HTlimits64, nNLLlimits64_500, NLLlimits64_500));
	}
      }
    

      //need to fill ST collection, but only once
      if(m > 3) continue;

      TString STname = "ST_";
      STname.Append(variationsNamePlain[i]);

      TString STrewName = "STrew_";
      STrewName.Append(variationsNamePlain[i]);
      if(bin % 100 < 60){//5j version optimized to have about 1k stats in 2017 electron data per bin
        ST.push_back(new TH1F(STname,"ST; ST [GeV/c]; Events", nSTlimits5, STlimits5));
        STrew.push_back(new TH1F(STrewName,"ST reweighted; ST [GeV/c]; Events", nSTlimits5, STlimits5));
      } else {//6j version optimized to have about 1k stats in 2017 electron data per bin
        ST.push_back(new TH1F(STname,"ST; ST [GeV/c]; Events", nSTlimits6, STlimits6));
        STrew.push_back(new TH1F(STrewName,"ST reweighted; ST [GeV/c]; Events", nSTlimits6, STlimits6));
      }


      //only activate for SR runs with ttbar sample
      if(IsSF_ttbar){
	TString SFloc = TString::Format("/eos/cms/store/group/phys_b2g/wprime/temp/SF_Bin%d_",SFreg)+YearS+".root";
        SFfile = new TFile(SFloc);
        TH1F *SF = (TH1F*)SFfile->Get("SF_"+variation);
        TF1 *SFfit, *SFfitParUp, *SFfitParDown, *SFfitQCDUp, *SFfitQCDDown;
        if(bin % 100 < 60){
	  SFfit = new TF1(TString::Format("fitFunction%d",i),"[0]/x/x+[1]/x+[2]+[3]*x+[4]*x*x", 180., 2000.);
	  if(i == 0){
	    SFfitParUp   = new TF1(TString::Format("fitFunctionParUp%d",i),"[0]/x+[1]+[2]*x+[3]*x*x", 180., 2000.);
	    SFfitParDown = new TF1(TString::Format("fitFunctionParDown%d",i),"[0]/x/x+[1]/x+[2]+[3]*x", 180., 2000.);
	    SFfitQCDUp   = new TF1(TString::Format("fitFunctionQCDUp%d",i),"[0]/x+[1]+[2]*x+[3]*x*x", 180., 2000.);
	    SFfitQCDDown = new TF1(TString::Format("fitFunctionQCDDown%d",i),"[0]/x+[1]+[2]*x+[3]*x*x", 180., 2000.);
	  }
	}
        else{
	  SFfit = new TF1(TString::Format("fitFunction%d",i),"[0]/x/x+[1]/x+[2]+[3]*x"        , 210., 2000.);
	  if(i == 0){
	    SFfitParUp   = new TF1(TString::Format("fitFunctionParUp%d",i),"[0]/x+[1]+[2]*x"        , 210., 2000.);
	    SFfitParDown = new TF1(TString::Format("fitFunctionParDown%d",i),"[0]/x/x+[1]/x+[2]"        , 210., 2000.);
	    SFfitQCDUp   = new TF1(TString::Format("fitFunctionQCDUp%d",i),"[0]/x+[1]+[2]*x"        , 210., 2000.);
            SFfitQCDDown = new TF1(TString::Format("fitFunctionQCDDown%d",i),"[0]/x/x+[1]/x+[2]"        , 210., 2000.);
	  }
	}
	
	//variations of fit functions for a systematic
	if(i == 0){
	  SF->Fit(SFfitParUp,"RF");
	  SF->Fit(SFfitParDown,"RF");
	  TH1F *SF_QCDUp = (TH1F*)SFfile->Get("SF_STfitQCDUp");
	  SF_QCDUp->Fit(SFfitQCDUp,"RF");
	  TH1F *SF_QCDDown = (TH1F*)SFfile->Get("SF_STfitQCDDown");
	  SF_QCDDown->Fit(SFfitQCDDown,"RF");
	  SFparUp = *SFfitParUp;
	  SFparDown = *SFfitParDown;
	  SFQCDUp = *SFfitQCDUp;
	  SFQCDDown = *SFfitQCDDown;
	}

	//central ST fit and systematic variations (latter are not necessary anymore)
        TFitResultPtr fr = SF->Fit(SFfit,"SRF");
        TMatrixD cov = fr->GetCovarianceMatrix();
        SFs.push_back(*SFfit);
        SFcovs.push_back(cov);
      }
    }//end variations loop



    //translate SF region identifier to region names for Combine cards
    TString region = "";
    if      (SFreg == 1151) region = "mu5j1b";
    else if (SFreg == 1152) region = "mu5j2b";
    else if (SFreg == 1161) region = "mu6j1b";
    else if (SFreg == 1162) region = "mu6j2b";
    else if (SFreg == 2151) region = "e5j1b";
    else if (SFreg == 2152) region = "e5j2b";
    else if (SFreg == 2161) region = "e6j1b";
    else if (SFreg == 2162) region = "e6j2b";

    //ST stat variations block
    FitMass_STstatUp.push_back(     (TH1F*) FitMass[m-3][0]->Clone(   variationsName[m-3][0]  +"CMS_B2G"+B2Gn+"_STfit_"+YearS+"_"+region+"Up"  ));
    FitMass_STstatDown.push_back(   (TH1F*) FitMass[m-3][0]->Clone(   variationsName[m-3][0]  +"CMS_B2G"+B2Gn+"_STfit_"+YearS+"_"+region+"Down"));
    HT_STstatUp.push_back(          (TH1F*) HT[m-3][0]->Clone(        HTvariationsName[m-3][0]+"CMS_B2G"+B2Gn+"_STfit_"+YearS+"_"+region+"Up"  ));
    HT_STstatDown.push_back(        (TH1F*) HT[m-3][0]->Clone(        HTvariationsName[m-3][0]+"CMS_B2G"+B2Gn+"_STfit_"+YearS+"_"+region+"Down"));
    FitMass_2D_STstatUp.push_back(  (TH2F*) FitMass_2D[m-3][0]->Clone(FitMass2Dnames[m-3][0]  +"CMS_B2G"+B2Gn+"_STfit_"+YearS+"_"+region+"Up"  ));
    FitMass_2D_STstatDown.push_back((TH2F*) FitMass_2D[m-3][0]->Clone(FitMass2Dnames[m-3][0]  +"CMS_B2G"+B2Gn+"_STfit_"+YearS+"_"+region+"Down"));
    HT_2D_STstatUp.push_back(       (TH2F*) HT_2D[m-3][0]->Clone(     HT2Dnames[m-3][0]       +"CMS_B2G"+B2Gn+"_STfit_"+YearS+"_"+region+"Up"  ));
    HT_2D_STstatDown.push_back(     (TH2F*) HT_2D[m-3][0]->Clone(     HT2Dnames[m-3][0]       +"CMS_B2G"+B2Gn+"_STfit_"+YearS+"_"+region+"Down"));

    //ST fit parameter variations block
    FitMass_STparUp.push_back(     (TH1F*) FitMass[m-3][0]->Clone(   variationsName[m-3][0]  +"CMS_B2G"+B2Gn+"_STfitFunc_"+YearS+"_"+region+"Up"  ));
    FitMass_STparDown.push_back(   (TH1F*) FitMass[m-3][0]->Clone(   variationsName[m-3][0]  +"CMS_B2G"+B2Gn+"_STfitFunc_"+YearS+"_"+region+"Down"));
    HT_STparUp.push_back(          (TH1F*) HT[m-3][0]->Clone(        HTvariationsName[m-3][0]+"CMS_B2G"+B2Gn+"_STfitFunc_"+YearS+"_"+region+"Up"  ));
    HT_STparDown.push_back(        (TH1F*) HT[m-3][0]->Clone(        HTvariationsName[m-3][0]+"CMS_B2G"+B2Gn+"_STfitFunc_"+YearS+"_"+region+"Down"));
    FitMass_2D_STparUp.push_back(  (TH2F*) FitMass_2D[m-3][0]->Clone(FitMass2Dnames[m-3][0]  +"CMS_B2G"+B2Gn+"_STfitFunc_"+YearS+"_"+region+"Up"  ));
    FitMass_2D_STparDown.push_back((TH2F*) FitMass_2D[m-3][0]->Clone(FitMass2Dnames[m-3][0]  +"CMS_B2G"+B2Gn+"_STfitFunc_"+YearS+"_"+region+"Down"));
    HT_2D_STparUp.push_back(       (TH2F*) HT_2D[m-3][0]->Clone(     HT2Dnames[m-3][0]       +"CMS_B2G"+B2Gn+"_STfitFunc_"+YearS+"_"+region+"Up"  ));
    HT_2D_STparDown.push_back(     (TH2F*) HT_2D[m-3][0]->Clone(     HT2Dnames[m-3][0]       +"CMS_B2G"+B2Gn+"_STfitFunc_"+YearS+"_"+region+"Down"));

    //ST QCD variations block
    FitMass_STstatUp.push_back(     (TH1F*) FitMass[m-3][0]->Clone(   variationsName[m-3][0]  +"CMS_B2G"+B2Gn+"_STfitQCDUp"  ));
    FitMass_STstatDown.push_back(   (TH1F*) FitMass[m-3][0]->Clone(   variationsName[m-3][0]  +"CMS_B2G"+B2Gn+"_STfitQCDDown"));
    HT_STstatUp.push_back(          (TH1F*) HT[m-3][0]->Clone(        HTvariationsName[m-3][0]+"CMS_B2G"+B2Gn+"_STfitQCDUp"  ));
    HT_STstatDown.push_back(        (TH1F*) HT[m-3][0]->Clone(        HTvariationsName[m-3][0]+"CMS_B2G"+B2Gn+"_STfitQCDDown"));
    FitMass_2D_STstatUp.push_back(  (TH2F*) FitMass_2D[m-3][0]->Clone(FitMass2Dnames[m-3][0]  +"CMS_B2G"+B2Gn+"_STfitQCDUp"  ));
    FitMass_2D_STstatDown.push_back((TH2F*) FitMass_2D[m-3][0]->Clone(FitMass2Dnames[m-3][0]  +"CMS_B2G"+B2Gn+"_STfitQCDDown"));
    HT_2D_STstatUp.push_back(       (TH2F*) HT_2D[m-3][0]->Clone(     HT2Dnames[m-3][0]       +"CMS_B2G"+B2Gn+"_STfitQCDUp"  ));
    HT_2D_STstatDown.push_back(     (TH2F*) HT_2D[m-3][0]->Clone(     HT2Dnames[m-3][0]       +"CMS_B2G"+B2Gn+"_STfitQCDDown"));

  
    //negative log likelihood block
    TString NLLname = "NegLogLnoB_";
    NLLname.Append(variationsName[m-3][0]);
    TString NLL2Dname = "NegLogLnoBvsNegLogL_";
    NLL2Dname.Append(variationsName[m-3][0]);
    if(bin % 100 < 60){
      NegLogLnoB.push_back( new TH1F(NLLname, "-log(L) without b-tagging; -log(L)_{!b}; Events", nNLLlimits53_500, NLLlimits53_500));
    
      NegLogLnoBvsNegLogL.push_back( new TH2F(NLL2Dname, "-log(L) without b-tagging vs -log(L); -log(L)_{!b}; -log(L); Events", nNLLlimits53_500, NLLlimits53_500, nNLLlimits53_500, NLLlimits53_500));
    }
    else{
      NegLogLnoB.push_back( new TH1F(NLLname, "-log(L) without b-tagging; -log(L)_{!b}; Events", nNLLlimits64_500, NLLlimits64_500));
      NegLogLnoBvsNegLogL.push_back( new TH2F(NLL2Dname, "-log(L) without b-tagging vs -log(L); -log(L)_{!b}; -log(L); Events", nNLLlimits64_500, NLLlimits64_500, nNLLlimits64_500, NLLlimits64_500));
    }

  }//end mass variant loop

  //STrew only needs to be done once
  STrew_STstatUp   = (TH1F*) STrew[0]->Clone("STrew_" + gn + "_STfit_" + YearS + "_" + binS + "_" + "STfitUp");
  STrew_STstatDown = (TH1F*) STrew[0]->Clone("STrew_" + gn + "_STfit_" + YearS + "_" + binS + "_" + "STfitDown");
  STrew_STparUp   = (TH1F*) STrew[0]->Clone("STrew_" + gn + "_STfit_" + YearS + "_" + binS + "_" + "STfitFuncUp");
  STrew_STparDown = (TH1F*) STrew[0]->Clone("STrew_" + gn + "_STfit_" + YearS + "_" + binS + "_" + "STfitFuncDown");
  STrew_STQCDUp   = (TH1F*) STrew[0]->Clone("STrew_" + gn + "_STfit_" + YearS + "_" + binS + "_" + "STfitQCDUp");
  STrew_STQCDDown = (TH1F*) STrew[0]->Clone("STrew_" + gn + "_STfit_" + YearS + "_" + binS + "_" + "STfitQCDDown");

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

    float LeptonPtVars[9] = {LeptonPt, LeptonPt_SU, LeptonPt_SD, LeptonPt_RU, LeptonPt_RD, LeptonPt, LeptonPt, LeptonPt, LeptonPt}; //Lepton pT

    //calculate HT variations
    float Vals[9] = {0., 0., 0., 0., 0., 0., 0., 0., 0.};//HT version

    //calculate ST variations
    float STvals[9] = {0., 0., 0., 0., 0., 0., 0., 0., 0.};

    //find the actual jet count per variation
    int JetCounts[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    int bJetMainCount = 0;

    //0: default
    float defHT = 0.;

    //calculate central HT
    for(unsigned i = 0; i < JetPt->size(); ++i) if(JetPt->at(i) > 30.){
      defHT += JetPt->at(i);
    }

    //calculate all ibject pT variation STs and HTs
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
       	Vals[5]   += JetPt_SU->at(i);
	JetCounts[5]++;
      }
    }
    //6: JESdown
    STvals[6] = LeptonPt + METPt_SD;
    for(unsigned i = 0; i < JetPt_SD->size(); ++i){
      if(JetPt_SD->at(i) > 30.){
	STvals[6] += JetPt_SD->at(i);
	Vals[6]   += JetPt_SD->at(i);
	JetCounts[6]++;
      }
    }
    //7: JERup
    STvals[7] = LeptonPt + METPt_RU;
    for(unsigned i = 0; i < JetPt_RU->size(); ++i){
      if(JetPt_RU->at(i) > 30.){
        STvals[7] += JetPt_RU->at(i);
	Vals[7]   += JetPt_RU->at(i);
	JetCounts[7]++;
      }
    }
    //7: JERdown
    STvals[8] = LeptonPt + METPt_RD;
    for(unsigned i = 0; i < JetPt_RD->size(); ++i){
      if(JetPt_RD->at(i) > 30.){
	STvals[8] += JetPt_RD->at(i);
	Vals[8]   += JetPt_RD->at(i);
	JetCounts[8]++;
      }
    }

    //mass-interpretation-independent variables
    //variation of selections
    for(unsigned i = 0; i < sizePtSysts + 1; ++ i){
      if(RegionIdentifier[i] != bin) continue;
      //additional 2017 lepton pT cut
      if(year == 2017){
        if(RegionIdentifier[i]/1000 == 1 && LeptonPtVars[i] < 30.) continue;
	if(RegionIdentifier[i]/1000 == 2 && LeptonPtVars[i] < 40.) continue;
      }
      else if (year == 2018){
        if(RegionIdentifier[i]/1000 == 2 && LeptonPtVars[i] < 32.) continue;
      }
      //additional ST cut test
      if(STvals[i] < 400.) continue;

      float EvWeight = EventWeight[0];
      if(YearS == "2017" && bin/1000 == 2 && dset.Type != 0) EvWeight *= EleHLTzvtx;
      
      const float CentralWeight = EvWeight*SampleWeight*EventWeightObjectVariations[i];
      if(IsSF_ttbar){ //take care of all pT variations and their impact also on the ST values
	const float STcorr = SFs[0].Eval(STvals[i]);
	const float STcorrCentralWeight = CentralWeight * STcorr;
	STrew[i]->Fill(STvals[i], STcorrCentralWeight);
	if(i == 0){      
	  const float statSFunc = CalculateCovError(STvals[0], SFcovs[0], jetMult);
          const float STcorrStatUp = CentralWeight * (STcorr + statSFunc);
          const float STcorrStatDown = CentralWeight * (STcorr - statSFunc);
          STrew_STstatUp->Fill(STvals[0], STcorrStatUp);
          STrew_STstatDown->Fill(STvals[0], STcorrStatDown);

	  //vary ST function parameters and evaluate impact on ST
	  const float SFcorrParUp = SFparUp.Eval(STvals[i]);
	  const float STcorrParUp = CentralWeight * SFcorrParUp;
	  const float SFcorrParDown = SFparDown.Eval(STvals[i]);
          const float STcorrParDown = CentralWeight * SFcorrParDown;
	  STrew_STparUp->Fill(STvals[0], STcorrParUp);
	  STrew_STparDown->Fill(STvals[0], STcorrParDown);

	  //vary QCD contamination in 1b regions and evaluate impact on ST
	  const float SFcorrQCDUp = SFQCDUp.Eval(STvals[i]);
          const float STcorrQCDUp = CentralWeight * SFcorrQCDUp;
          const float SFcorrQCDDown = SFQCDDown.Eval(STvals[i]);
          const float STcorrQCDDown = CentralWeight * SFcorrQCDDown;
          STrew_STQCDUp->Fill(STvals[0], STcorrQCDUp);
          STrew_STQCDDown->Fill(STvals[0], STcorrQCDDown);
	}
      }
      ST[i]->Fill(STvals[i], CentralWeight);
    }

    //std::cout<<"stage 2"<<std::endl;

    //variation of systematic events weights
    if(RegionIdentifier[0] == bin) for(unsigned i = sizePtSysts+1; i < varSize; ++i){
      //additional 2017 lepton pT cut
      if(year == 2017){
        if(RegionIdentifier[0]/1000 == 1 && LeptonPtVars[0] < 30.) continue;
        if(RegionIdentifier[0]/1000 == 2 && LeptonPtVars[0] < 40.) continue;
      }
      else if (year == 2018){
        if(RegionIdentifier[0]/1000 == 2 && LeptonPtVars[0] < 32.) continue;
      }
      //additional ST cut test
      if(STvals[0] < 400.) continue;

      float EvWeight = 1.;

      //block for HLT z vtx inefficiency correction in 2017 only for electron channel only
      if(YearS == "2017" && bin/1000 == 2 && dset.Type != 0){
	EvWeight *= EleHLTzvtx;
      }

      //make sure to get the correct event weight, either a variation or the central
      /*if(i < varSize-sizePtSysts)	EvWeight *= EventWeight[i-sizePtSysts];// >= 0. ? EventWeight[i-8] : EventWeight[0]; //catch negative weights
      else				EvWeight *= EventWeight[0]; //normalization variation normal*/
      EvWeight *= EventWeight[i-sizePtSysts];
      
      const float CentralWeight = EvWeight * SampleWeight * EventWeightObjectVariations[0];

      if(IsSF_ttbar){
	const float CentralWeightSTcorr = CentralWeight * SFs[0].Eval(STvals[0]);
	STrew[i]->Fill(STvals[0], CentralWeightSTcorr);	
      }
      ST[i]->Fill(STvals[0], CentralWeight);
    }

    //std::cout<<"stage 3"<<std::endl;

    //mass-interpretation-dependent variables
    //variations of mass interpretation
    for(unsigned m = 3; m < 12; ++m){

      float fillBranchZero = 0.;
      if(m==3)       fillBranchZero  = Best_WPrimeMass_300->at(0);
      else if(m==4)  fillBranchZero  = Best_WPrimeMass_400->at(0);
      else if(m==5)  fillBranchZero  = Best_WPrimeMass_500->at(0);
      else if(m==6)  fillBranchZero  = Best_WPrimeMass_600->at(0);
      else if(m==7)  fillBranchZero  = Best_WPrimeMass_700->at(0);
      else if(m==8)  fillBranchZero  = Best_WPrimeMass_800->at(0);
      else if(m==9)  fillBranchZero  = Best_WPrimeMass_900->at(0);
      else if(m==10) fillBranchZero  = Best_WPrimeMass_1000->at(0);
      else if(m==11) fillBranchZero  = Best_WPrimeMass_1100->at(0);

      float NLLfillZero = 0.;
      if(m==3)       NLLfillZero = Best_Likelihood_300->at(0)  >= 0 ? -log(Best_Likelihood_300->at(0))  : -1.;
      else if(m==4)  NLLfillZero = Best_Likelihood_400->at(0)  >= 0 ? -log(Best_Likelihood_400->at(0))  : -1.;
      else if(m==5)  NLLfillZero = Best_Likelihood_500->at(0)  >= 0 ? -log(Best_Likelihood_500->at(0))  : -1.;
      else if(m==6)  NLLfillZero = Best_Likelihood_600->at(0)  >= 0 ? -log(Best_Likelihood_600->at(0))  : -1.;
      else if(m==7)  NLLfillZero = Best_Likelihood_700->at(0)  >= 0 ? -log(Best_Likelihood_700->at(0))  : -1.;
      else if(m==8)  NLLfillZero = Best_Likelihood_800->at(0)  >= 0 ? -log(Best_Likelihood_800->at(0))  : -1.;
      else if(m==9)  NLLfillZero = Best_Likelihood_900->at(0)  >= 0 ? -log(Best_Likelihood_900->at(0))  : -1.;
      else if(m==10) NLLfillZero = Best_Likelihood_1000->at(0) >= 0 ? -log(Best_Likelihood_1000->at(0)) : -1.;
      else if(m==11) NLLfillZero = Best_Likelihood_1100->at(0) >= 0 ? -log(Best_Likelihood_1100->at(0)) : -1.;

      float NLLnoBfillZero = 0.;
      if(m==3)       NLLnoBfillZero = NLLfillZero >= 0 ? -log(Best_Likelihood_300->at(0) /Best_PbTag_300->at(0))  : -1.;
      else if(m==4)  NLLnoBfillZero = NLLfillZero >= 0 ? -log(Best_Likelihood_400->at(0) /Best_PbTag_400->at(0))  : -1.;
      else if(m==5)  NLLnoBfillZero = NLLfillZero >= 0 ? -log(Best_Likelihood_500->at(0) /Best_PbTag_500->at(0))  : -1.;
      else if(m==6)  NLLnoBfillZero = NLLfillZero >= 0 ? -log(Best_Likelihood_600->at(0) /Best_PbTag_600->at(0))  : -1.;
      else if(m==7)  NLLnoBfillZero = NLLfillZero >= 0 ? -log(Best_Likelihood_700->at(0) /Best_PbTag_700->at(0))  : -1.;
      else if(m==8)  NLLnoBfillZero = NLLfillZero >= 0 ? -log(Best_Likelihood_800->at(0) /Best_PbTag_800->at(0))  : -1.;
      else if(m==9)  NLLnoBfillZero = NLLfillZero >= 0 ? -log(Best_Likelihood_900->at(0) /Best_PbTag_900->at(0))  : -1.;
      else if(m==10) NLLnoBfillZero = NLLfillZero >= 0 ? -log(Best_Likelihood_1000->at(0)/Best_PbTag_1000->at(0)) : -1.;
      else if(m==11) NLLnoBfillZero = NLLfillZero >= 0 ? -log(Best_Likelihood_1100->at(0)/Best_PbTag_1100->at(0)) : -1.;

      //variations of selections
      for(unsigned i = 0; i < sizePtSysts+1; ++ i){
        if(RegionIdentifier[i] != bin) continue;
	//additional 2017 lepton pT cut
        if(year == 2017){
          if(RegionIdentifier[i]/1000 == 1 && LeptonPtVars[i] < 30.) continue;
          if(RegionIdentifier[i]/1000 == 2 && LeptonPtVars[i] < 40.) continue;
        }
	else if (year == 2018){
          if(RegionIdentifier[i]/1000 == 2 && LeptonPtVars[i] < 32.) continue;
        }
	//additional ST cut test
        if(STvals[i] < 400.) continue;

        //determine fill variable
        float fillVar = Vals[i];
        float fillBranch = 0.;
	if(m==3)       fillBranch = Best_WPrimeMass_300->at(i); 
	else if(m==4)  fillBranch = Best_WPrimeMass_400->at(i);
	else if(m==5)  fillBranch = Best_WPrimeMass_500->at(i);
	else if(m==6)  fillBranch = Best_WPrimeMass_600->at(i);
	else if(m==7)  fillBranch = Best_WPrimeMass_700->at(i);
	else if(m==8)  fillBranch = Best_WPrimeMass_800->at(i);
	else if(m==9)  fillBranch = Best_WPrimeMass_900->at(i);
	else if(m==10) fillBranch = Best_WPrimeMass_1000->at(i);
	else if(m==11) fillBranch = Best_WPrimeMass_1100->at(i);

	float NLLfill = 0.;
	if(m==3)       NLLfill = Best_Likelihood_300->at(i)  >= 0 ? -log(Best_Likelihood_300->at(i))  : -1.;
	else if(m==4)  NLLfill = Best_Likelihood_400->at(i)  >= 0 ? -log(Best_Likelihood_400->at(i))  : -1.;
	else if(m==5)  NLLfill = Best_Likelihood_500->at(i)  >= 0 ? -log(Best_Likelihood_500->at(i))  : -1.;
	else if(m==6)  NLLfill = Best_Likelihood_600->at(i)  >= 0 ? -log(Best_Likelihood_600->at(i))  : -1.;
	else if(m==7)  NLLfill = Best_Likelihood_700->at(i)  >= 0 ? -log(Best_Likelihood_700->at(i))  : -1.;
	else if(m==8)  NLLfill = Best_Likelihood_800->at(i)  >= 0 ? -log(Best_Likelihood_800->at(i))  : -1.;
	else if(m==9)  NLLfill = Best_Likelihood_900->at(i)  >= 0 ? -log(Best_Likelihood_900->at(i))  : -1.;
	else if(m==10) NLLfill = Best_Likelihood_1000->at(i) >= 0 ? -log(Best_Likelihood_1000->at(i)) : -1.;
	else if(m==11) NLLfill = Best_Likelihood_1100->at(i) >= 0 ? -log(Best_Likelihood_1100->at(i)) : -1.;

	float NLLnoBfill = 0.;
	if(m==3)       NLLnoBfill = NLLfill >= 0 ? -log(Best_Likelihood_300->at(i)/Best_PbTag_300->at(i))   : -1.;
	else if(m==4)  NLLnoBfill = NLLfill >= 0 ? -log(Best_Likelihood_400->at(i)/Best_PbTag_400->at(i))   : -1.;
	else if(m==5)  NLLnoBfill = NLLfill >= 0 ? -log(Best_Likelihood_500->at(i)/Best_PbTag_500->at(i))   : -1.;
	else if(m==6)  NLLnoBfill = NLLfill >= 0 ? -log(Best_Likelihood_600->at(i)/Best_PbTag_600->at(i))   : -1.;
	else if(m==7)  NLLnoBfill = NLLfill >= 0 ? -log(Best_Likelihood_700->at(i)/Best_PbTag_700->at(i))   : -1.;
	else if(m==8)  NLLnoBfill = NLLfill >= 0 ? -log(Best_Likelihood_800->at(i)/Best_PbTag_800->at(i))   : -1.;
	else if(m==9)  NLLnoBfill = NLLfill >= 0 ? -log(Best_Likelihood_900->at(i)/Best_PbTag_900->at(i))   : -1.;
	else if(m==10) NLLnoBfill = NLLfill >= 0 ? -log(Best_Likelihood_1000->at(i)/Best_PbTag_1000->at(i)) : -1.;
	else if(m==11) NLLnoBfill = NLLfill >= 0 ? -log(Best_Likelihood_1100->at(i)/Best_PbTag_1100->at(i)) : -1.;

	float EvWeight = EventWeight[0];
        if(YearS == "2017" && bin/1000 == 2 && dset.Type != 0) EvWeight *= EleHLTzvtx;

        string HistName;
        if(IsSF_ttbar){ //take care of all pT variations and their impact also on the ST values
          const float CentralWeight = EvWeight * SampleWeight * EventWeightObjectVariations[i];
	  const float STcorrCentralWeight = CentralWeight * SFs[0].Eval(STvals[i]);
	  FitMass[m-3][i]->Fill(fillBranch, STcorrCentralWeight);
	  HT[m-3][i]->Fill(fillVar, STcorrCentralWeight);

	  FitMass_2D[m-3][i]->Fill(fillBranch, NLLfill, STcorrCentralWeight);
	  HT_2D[m-3][i]->Fill(fillVar, NLLfill, STcorrCentralWeight);

	  if(i == 0){//make sure to scale ttbar and get stat. and parameter uncs. of fit propagated
            const float STcorr = SFs[0].Eval(STvals[i]);
	    const float statSFunc = CalculateCovError(STvals[0], SFcovs[0], jetMult);
	    const float STcorrStatUp = CentralWeight * (STcorr + statSFunc);
	    const float STcorrStatDown = CentralWeight * (STcorr - statSFunc);
	    FitMass_STstatUp[m-3]->Fill(fillBranch, STcorrStatUp);
            FitMass_STstatDown[m-3]->Fill(fillBranch, STcorrStatDown);
	    HT_STstatUp[m-3]->Fill(fillVar, STcorrStatUp);
	    HT_STstatDown[m-3]->Fill(fillVar, STcorrStatDown);

	    FitMass_2D_STstatUp[m-3]->Fill(fillBranch, NLLfillZero, STcorrStatUp);
	    FitMass_2D_STstatDown[m-3]->Fill(fillBranch, NLLfillZero, STcorrStatDown);
	    HT_2D_STstatUp[m-3]->Fill(fillVar, NLLfillZero, STcorrStatUp);
	    HT_2D_STstatDown[m-3]->Fill(fillVar, NLLfillZero, STcorrStatDown);

	    const float SFcorrParUp = SFparUp.Eval(STvals[i]);
            const float STcorrParUp = CentralWeight * SFcorrParUp;
            const float SFcorrParDown = SFparDown.Eval(STvals[i]);
            const float STcorrParDown = CentralWeight * SFcorrParDown;

	    FitMass_STparUp[m-3]->Fill(fillBranch, STcorrParUp);
            FitMass_STparDown[m-3]->Fill(fillBranch, STcorrParDown);
            HT_STparUp[m-3]->Fill(fillVar, STcorrParUp);
            HT_STparDown[m-3]->Fill(fillVar, STcorrParDown);

            FitMass_2D_STparUp[m-3]->Fill(fillBranch, NLLfillZero, STcorrParUp);
            FitMass_2D_STparDown[m-3]->Fill(fillBranch, NLLfillZero, STcorrParDown);
            HT_2D_STparUp[m-3]->Fill(fillVar, NLLfillZero, STcorrParUp);
            HT_2D_STparDown[m-3]->Fill(fillVar, NLLfillZero, STcorrParDown);

	    NegLogLnoB[m-3]->Fill(NLLnoBfill, STcorrCentralWeight);
	    NegLogLnoBvsNegLogL[m-3]->Fill(NLLnoBfillZero, NLLfillZero, STcorrCentralWeight);
          }
        }
        else{
	  const float CentralWeight = EvWeight * SampleWeight * EventWeightObjectVariations[i];
	  FitMass[m-3][i]->Fill(fillBranch, CentralWeight);
	  HT[m-3][i]->Fill(fillVar, CentralWeight);

	  FitMass_2D[m-3][i]->Fill(fillBranch, NLLfill, CentralWeight);
	  HT_2D[m-3][i]->Fill(fillVar, NLLfill, CentralWeight);

	  if(i==0){
	    NegLogLnoB[m-3]->Fill(NLLnoBfill, CentralWeight);
            NegLogLnoBvsNegLogL[m-3]->Fill(NLLnoBfillZero, NLLfillZero, CentralWeight);
	  }
        }
      }

      //Make sure to match default region for default objects with event weight variations
      if(RegionIdentifier[0] == bin){
	//additional 2017 lepton pT cut
        if(year == 2017){
          if(RegionIdentifier[0]/1000 == 1 && LeptonPtVars[0] < 30.) continue;
          if(RegionIdentifier[0]/1000 == 2 && LeptonPtVars[0] < 40.) continue;
        }
	else if (year == 2018){
          if(RegionIdentifier[0]/1000 == 2 && LeptonPtVars[0] < 32. ) continue;
        }
	//additional ST cut test
        if(STvals[0] < 400.) continue;

        //EventWeight variations
        for(unsigned i = sizePtSysts+1; i < varSize; ++i){//after pT variation block for systematics, divided into systWeights block, normalization block, and SFttbar blocks (if applicable for the last)
	
          string HistName;

	  //determine fill variable
	  float fillVar = Vals[0];

	  float EvWeight = 1.;

      	  //block for HLT z vtx inefficiency correction in 2017 only for electron channel only
	  if(YearS == "2017" && bin/1000 == 2 && dset.Type != 0){
	    EvWeight *= EleHLTzvtx;
          }

          //make sure to get the correct event weight, either a variation or the central
	  /*if(i < varSize-sizePtSysts)   EvWeight *= EventWeight[i-sizePtSysts];// >= 0. ? EventWeight[i-8] : EventWeight[0]; //catch negative weights
          else               		EvWeight *= EventWeight[0]; //normalization variation normal*/
	  EvWeight *= EventWeight[i-sizePtSysts];

	  const float CentralWeight = EvWeight * SampleWeight * EventWeightObjectVariations[0];
          if(IsSF_ttbar){
	    const float CentralWeightSTcorr = CentralWeight * SFs[0].Eval(STvals[0]); //note that SFs[i] runs the full length of variations = varSize+varOff
	    FitMass[m-3][i]->Fill(fillBranchZero, CentralWeightSTcorr);
	    HT[m-3][i]->Fill(fillVar, CentralWeightSTcorr);

	    FitMass_2D[m-3][i]->Fill(fillBranchZero, NLLfillZero, CentralWeightSTcorr);
	    HT_2D[m-3][i]->Fill(fillVar, NLLfillZero, CentralWeightSTcorr);
	  }
          else{
	    FitMass[m-3][i]->Fill(fillBranchZero, CentralWeight);
	    HT[m-3][i]->Fill(fillVar, CentralWeight);

	    FitMass_2D[m-3][i]->Fill(fillBranchZero, NLLfillZero, CentralWeight);
            HT_2D[m-3][i]->Fill(fillVar, NLLfillZero, CentralWeight);
	  }
        }
      }
    }
  }

  std::cout<<"saving stage"<<std::endl;

  //save all the W' variation histograms into files
  //fit mass file
  TFile *savefile;
  savefile = new TFile(TString::Format("/eos/cms/store/group/phys_b2g/wprime/temp/SimpleShapes_Bin%d_",bin)+YearS+TString::Format("_%d.root",Iterator),"RECREATE");
  TString STvarName = gn + "_" + binS + "_" + "STfit_" + YearS + "_";
  STvarName.Append(TString::Format("%d",SFreg));
  savefile->cd();
  for(unsigned i = 0; i < FitMass.size(); ++i){
    for(unsigned j = 0; j < FitMass[i].size(); ++j){
      if(dset.Type == 0){
        if(j>2) continue;
        if(Iterator <= 1){
	  TString variation = Systematics(j, YearS, sampleType, B2Gn);
	  FitMass[i][j]->Write("data_obs_" + binS + TString::Format("_M%d_",(i+3)*100)  + variation);
	  if(i==0) ST[j]->Write("ST_data_obs_" + binS + "_" + variation);
        } 
        else continue;
      }
      else if(IsSF_ttbar){ //case of applying SF to ttbar
        if(j == 0) { //function to propagate ST-fit SF uncertainty to bin error
	  FitMass_STstatUp[i]->Write();
	  FitMass_STstatDown[i]->Write();
	  FitMass_STparUp[i]->Write();
	  FitMass_STparDown[i]->Write();
        }	
        FitMass[i][j]->Write();
        if(i==0){
	  std::cout<<"writing ST reweight histogram"<<std::endl;
          STrew[j]->Write();
          ST[j]->Write();
	  if(j==0){
            STrew_STstatUp->Write();
            STrew_STstatDown->Write();
	    STrew_STparUp->Write();
	    STrew_STparDown->Write();
	    STrew_STQCDUp->Write();
            STrew_STQCDDown->Write();
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
  savefileHT = new TFile(TString::Format("/eos/cms/store/group/phys_b2g/wprime/temp/HT_SimpleShapes_Bin%d_",bin)+YearS+TString::Format("_%d.root",Iterator),"RECREATE");
  savefileHT->cd();
  for(unsigned i = 0; i < HT.size(); ++i){
    for(unsigned j = 0; j < HT[i].size(); ++j){
      if(dset.Type == 0){
        if(j>2) continue;
        if(Iterator <= 1){
	  TString variation = Systematics(j, YearS, sampleType, B2Gn);
          HT[i][j]->Write("HT_data_obs_" + binS + TString::Format("_M%d_",(i+3)*100) + variation);
          if(i==0) ST[i]->Write("ST_data_obs_" + binS + "_" + variation);
        }
        else continue;
      }
      else if(IsSF_ttbar){ //case of applying SF to ttbar
        if(j == 0) { //function to propagate ST-fit SF uncertainty to bin error
	  HT_STstatUp[i]->Write();
	  HT_STstatDown[i]->Write();
	  HT_STparUp[i]->Write();
          HT_STparDown[i]->Write();
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
  savefile2D = new TFile(TString::Format("/eos/cms/store/group/phys_b2g/wprime/temp/TwoD_SimpleShapes_Bin%d_",bin)+YearS+TString::Format("_%d.root",Iterator),"RECREATE");
  savefile2D->cd();
  for(unsigned i = 0; i < FitMass_2D.size(); ++i){
    for(unsigned j = 0; j < FitMass_2D[i].size(); ++j){
      if(dset.Type == 0){
        if(j>2) continue;
        if(Iterator <= 1){
	  TString variation = Systematics(j, YearS, sampleType, B2Gn);
          HT_2D[i][j]->Write("HT_data_obs_" + binS + TString::Format("_M%d_",(i+3)*100) + variation);
	  FitMass_2D[i][j]->Write("FitMass_data_obs_" + binS + TString::Format("_M%d_",(i+3)*100) + variation);
        }
        else continue;
      }
      else if(IsSF_ttbar){ //case of applying SF to ttbar
        if(j == 0){
	  FitMass_2D_STstatUp[i]->Write();
	  FitMass_2D_STstatDown[i]->Write();
	  HT_2D_STstatUp[i]->Write();
	  HT_2D_STstatDown[i]->Write();
	  FitMass_2D_STparUp[i]->Write();
          FitMass_2D_STparDown[i]->Write();
          HT_2D_STparUp[i]->Write();
          HT_2D_STparDown[i]->Write();
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
