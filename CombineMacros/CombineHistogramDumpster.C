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
float CalculateCovError(float STval, TMatrixD covM){
  float Deriv0 = 1./STval/STval;
  float Deriv1 = 1./STval;
  float Deriv2 = 1.;
  float Deriv3 = STval;
  float FinalEnvelope = 0.;
  for(unsigned x = 0; x < 4; ++x){
    for(unsigned y = 0; y < 4; ++y){
      float Component = 1.;
      if(x == 0) Component *= Deriv0;
      else if (x == 1) Component *= Deriv1;
      else if (x == 2) Component *= Deriv2;
      else if (x == 3) Component *= Deriv3;
      if(y == 0) Component *= Deriv0;
      else if (y == 1) Component *= Deriv1;
      else if (y == 2) Component *= Deriv2;
      else if (y == 3) Component *= Deriv3;
      Component *= covM(x,y);
      FinalEnvelope += Component;
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
  TString binS = TString::Format("Wprime%d", bin);
  TString gn = dset.GroupName;

  //determine the SF functions for ST
  TFile *SFfile;
  vector<TF1> SFs;
  vector<TMatrixD> SFcovs;

  //set sample weight
  int Year = 0;
  int year = 0;
  float Lumi = 0.;
  if(YearType == "2016_APV")  {Year = 0; year = 2016; Lumi = 0.;}
  else if(YearType == "2016") {Year = 1; year = 2016; Lumi = 41.58;}
  else if(YearType == "2017") {Year = 2; year = 2017; Lumi = 49.81;}
  else if(YearType == "2018") {Year = 3; year = 2018; Lumi = 67.86;}
  float SampleWeight = 1.;
  if(dset.Type != 0) SampleWeight = Lumi * dset.CrossSection / dset.Size[Year];

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

  vector<TH1F*> ST;

  //first variations are all weight variations and map 1:1, region variations start at index 21
  vector<TString> variations = {"" // 0
  , "electronScaleUp", "electronScaleDown", "electronResUp", "electronResDown", "JESUp", "JESDown", "JERUp", "JERDown" // 1 - 8
  , "electronUp", "electronDown", "muonTriggerUp", "muonTriggerDown", "muonIdUp", "muonIdDown", "muonIsoUp", "muonIsoDown" // 9 - 16
  , "BjetTagCorrUp", "BjetTagCorrDown", "BjetTagUncorr"+YearType+"Up", "BjetTagUncorr"+YearType+"Down", "PUIDUp", "PUIDDown", "L1PreFiringUp", "L1PreFiringDown" // 17 - 24
  , "PUreweightUp", "PUreweightDown", "PDFUp", "PDFDown", "LHEScaleUp", "LHEScaleDown", // 25 - 30
  };

  vector<TString> variationsName, HTvariationsName, FitMass2Dnames, HT2Dnames;
  for (unsigned i = 0; i < variations.size(); ++i) {
    variationsName.push_back(gn + "_" + binS + "_" + variations[i]);
    HTvariationsName.push_back("HT_"+variationsName[i]);
    FitMass2Dnames.push_back("FitMass2D_"+variationsName[i]);
    HT2Dnames.push_back("HT2D_"+variationsName[i]);
  }

  for(unsigned i = 0; i < variations.size(); ++i){
    FitMass.push_back(new TH1F(variationsName[i],"fitted W' mass; m_{W'} [GeV/c^{2}]; Events", 400, 0., 2000.));
    HT.push_back(new TH1F(HTvariationsName[i],"H_{T}; H_{T} [GeV/c^{2}]; Events", 400, 0., 2000.));
    FitMass_2D.push_back(new TH2F(FitMass2Dnames[i],"fitted W' mass vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", 400, 0., 2000., 60, 0., 30.));
    HT_2D.push_back(new TH2F(HT2Dnames[i],"HT vs best -log(likelihood); m_{W'} [GeV/c^{2}]; -log(likelihood); Events", 400, 0., 2000., 60, 0., 30.));

    //need to fill ST collection
    TString STname = "ST_";
    STname.Append(variationsName[i]);
    ST.push_back(new TH1F(STname,"ST; ST [GeV/c]; Events", 400, 0., 2000.));

    //only activate for SR runs with ttbar sample
    if(SFreg != 0 && Iterator == 2){
      SFfile = new TFile(TString::Format("TestHistograms/SF_Bin%d_%d.root",SFreg,year));
      TH1F *SF = (TH1F*)SFfile->Get("SF_"+variations[i]);
      TF1 *SFfit = new TF1(TString::Format("fitFunction%d",i),"[0]/x/x+[1]/x+[2]+[3]*x", 150., 2000.);
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
    //0: default
    float defHT = 0.;
    for(unsigned i = 0; i < JetPt->size(); ++i) defHT += JetPt->at(i);
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
    for(unsigned i = 0; i < JetPt_SU->size(); ++i) {STvals[5] += JetPt_SU->at(i); Vals[5] += JetPt_SU->at(i);}
    //6: JESdown
    STvals[6] = LeptonPt + METPt_SD;
    for(unsigned i = 0; i < JetPt_SD->size(); ++i) {STvals[6] += JetPt_SD->at(i); Vals[6] += JetPt_SD->at(i);}
    //7: JERup
    STvals[7] = LeptonPt + METPt_RU;
    for(unsigned i = 0; i < JetPt_RU->size(); ++i) {STvals[7] += JetPt_RU->at(i); Vals[7] += JetPt_RU->at(i);}
    //7: JERdown
    STvals[8] = LeptonPt + METPt_RD;
    for(unsigned i = 0; i < JetPt_RD->size(); ++i) {STvals[8] += JetPt_RD->at(i); Vals[8] += JetPt_RD->at(i);}

    //variations of selections
    for(unsigned i = 0; i < 9; ++ i){
      if(RegionIdentifier[i] != bin) continue;

      //determine fill variable
      float fillVar = Vals[i];

      string HistName;
      if(Iterator == 2 && SFreg != 0){ //take care of all pT variations and their impact also on the ST values
	FitMass[i]->Fill(Best_WPrimeMass->at(i),EventWeight[0]*SampleWeight*SFs[i].Eval(STvals[i]));
	HT[i]->Fill(fillVar,EventWeight[0]*SampleWeight*SFs[i].Eval(STvals[i]));

	FitMass_2D[i]->Fill(Best_WPrimeMass->at(i),-log(Best_Likelihood->at(i)),EventWeight[0]*SampleWeight*SFs[i].Eval(STvals[i]));
	HT_2D[i]->Fill(fillVar,-log(Best_Likelihood->at(i)),EventWeight[0]*SampleWeight*SFs[i].Eval(STvals[i]));

	if(i == 0){//make sure to scale ttbar and get stat. unc. of fit propagated
	  float statSFunc = CalculateCovError(STvals[0], SFcovs[0]);
	  FitMass_STstatUp->Fill(Best_WPrimeMass->at(0),EventWeight[0]*SampleWeight*(SFs[0].Eval(STvals[0])+statSFunc));
          FitMass_STstatDown->Fill(Best_WPrimeMass->at(0),EventWeight[0]*SampleWeight*(SFs[0].Eval(STvals[0])-statSFunc));
	  HT_STstatUp->Fill(fillVar,EventWeight[0]*SampleWeight*(SFs[0].Eval(STvals[0])+statSFunc));
	  HT_STstatDown->Fill(fillVar,EventWeight[0]*SampleWeight*(SFs[0].Eval(STvals[0])-statSFunc));

	  FitMass_2D_STstatUp->Fill(Best_WPrimeMass->at(0),-log(Best_Likelihood->at(0)),EventWeight[0]*SampleWeight*(SFs[0].Eval(STvals[0])+statSFunc));
	  FitMass_2D_STstatDown->Fill(Best_WPrimeMass->at(0),-log(Best_Likelihood->at(0)),EventWeight[0]*SampleWeight*(SFs[0].Eval(STvals[0])-statSFunc));
	  HT_2D_STstatUp->Fill(fillVar,-log(Best_Likelihood->at(0)),EventWeight[0]*SampleWeight*(SFs[0].Eval(STvals[0])+statSFunc));
	  HT_2D_STstatDown->Fill(fillVar,-log(Best_Likelihood->at(0)),EventWeight[0]*SampleWeight*(SFs[0].Eval(STvals[0])-statSFunc));
        }
      }
      else{
	FitMass[i]->Fill(Best_WPrimeMass->at(i),EventWeight[0]*SampleWeight);
	HT[i]->Fill(fillVar,EventWeight[0]*SampleWeight);

	FitMass_2D[i]->Fill(Best_WPrimeMass->at(i),-log(Best_Likelihood->at(i)),EventWeight[0]*SampleWeight);
	HT_2D[i]->Fill(fillVar,-log(Best_Likelihood->at(i)),EventWeight[0]*SampleWeight);
      }
      ST[i]->Fill(STvals[i],EventWeight[0]*SampleWeight);
    }
    //Muon type variations, for a test
    if(RegionIdentifier[0] == bin){
      //EventWeight variations
      for(unsigned i = 9; i < variations.size(); ++i){
        string HistName;

	//determine fill variable
	float fillVar = Vals[0];

        if(Iterator == 2 && SFreg != 0){
	  FitMass[i]->Fill(Best_WPrimeMass->at(0),EventWeight[i-8]*SampleWeight*SFs[0].Eval(STvals[0]));
	  HT[i]->Fill(fillVar,EventWeight[i-8]*SampleWeight*SFs[0].Eval(STvals[0]));

	  FitMass_2D[i]->Fill(Best_WPrimeMass->at(0),-log(Best_Likelihood->at(0)),EventWeight[i-8]*SampleWeight*SFs[0].Eval(STvals[0]));
	  HT_2D[i]->Fill(fillVar,-log(Best_Likelihood->at(0)),EventWeight[i-8]*SampleWeight*SFs[0].Eval(STvals[0]));
	}
        else{
	  FitMass[i]->Fill(Best_WPrimeMass->at(0),EventWeight[i-8]*SampleWeight);
	  HT[i]->Fill(fillVar,EventWeight[i-8]*SampleWeight);

	  FitMass_2D[i]->Fill(Best_WPrimeMass->at(0),-log(Best_Likelihood->at(0)),EventWeight[i-8]*SampleWeight);
          HT_2D[i]->Fill(fillVar,-log(Best_Likelihood->at(0)),EventWeight[i-8]*SampleWeight);
	}
	ST[i]->Fill(STvals[0],EventWeight[i-8]*SampleWeight);
      }
    }
    
  }
  //save all the W' variation histograms into a file
  TFile *savefile;
  savefile = new TFile(TString::Format("TestHistograms/SimpleShapes_Bin%d_%d.root",bin,Iterator),"RECREATE");
  //only activate for SR runs with ttbar sample
  //if(SFreg != 0 && Iterator == 2) SFfile = new TFile(TString::Format("TestHistograms/SF_Bin%d_%d.root",SFreg,year));
  savefile->cd();
  for(unsigned i = 0; i < FitMass.size(); ++i){
    if(dset.Type == 0){
      if(i>0) continue;
      if(Iterator <= 1){
	FitMass[i]->Write("data_obs_" + binS + "_");
	ST[i]->Write("ST_data_obs_" + binS + "_");
      } 
      else continue;
    }
    else if(Iterator == 2 && SFreg != 0){ //case of applying SF to ttbar
      if(i == 0) for(unsigned x = 0; x < FitMass[i]->GetNbinsX(); ++x){ //function to propagate ST-fit SF uncertainty to bin error
	float statBase = FitMass[i]->GetBinError(x+1);
	float SFstat = fabs(FitMass_STstatUp->GetBinContent(x+1) - FitMass[i]->GetBinContent(x+1));
        FitMass[i]->SetBinError(x+1,sqrt(pow(statBase,2)+pow(SFstat,2)));
      }	
      FitMass[i]->Write(variationsName[i]);
    }
    else{
      FitMass[i]->Write(variationsName[i]);
      ST[i]->Write(ST[i]->GetName());
    }
  }
  savefile->Close();

  TFile *savefileHT;
  savefileHT = new TFile(TString::Format("TestHistograms/HT_SimpleShapes_Bin%d_%d.root",bin,Iterator),"RECREATE");
  savefileHT->cd();
  for(unsigned i = 0; i < HT.size(); ++i){
    if(dset.Type == 0){
      if(i>0) continue;
      if(Iterator <= 1){
        HT[i]->Write("data_obs_" + binS + "_");
        ST[i]->Write("ST_data_obs_" + binS + "_");
      }
      else continue;
    }
    else if(Iterator == 2 && SFreg != 0){ //case of applying SF to ttbar
      if(i == 0) for(unsigned x = 0; x < HT[i]->GetNbinsX(); ++x){ //function to propagate ST-fit SF uncertainty to bin error
        float statBase = HT[i]->GetBinError(x+1);
        float SFstat = fabs(HT_STstatUp->GetBinContent(x+1) - HT[i]->GetBinContent(x+1));
        HT[i]->SetBinError(x+1,sqrt(pow(statBase,2)+pow(SFstat,2)));
      }
      HT[i]->Write(variationsName[i]);
    }
    else{
      HT[i]->Write(variationsName[i]);
      ST[i]->Write(ST[i]->GetName());
    }
  }
  savefileHT->Close();

  TFile* savefile2D;
  savefile2D = new TFile(TString::Format("TestHistograms/TwoD_SimpleShapes_Bin%d_%d.root",bin,Iterator),"RECREATE");
  savefile2D->cd();
  for(unsigned i = 0; i < HT.size(); ++i){
    if(dset.Type == 0){
      if(i>0) continue;
      if(Iterator <= 1){
        HT_2D[i]->Write("HT_data_obs_" + binS + "_");
	FitMass_2D[i]->Write("FitMass_data_obs_" + binS + "_");
      }
      else continue;
    }
    else if(Iterator == 2 && SFreg != 0){ //case of applying SF to ttbar
      if(i == 0){
	for(unsigned x = 0; x < FitMass_2D[i]->GetNbinsX(); ++x) for(unsigned y = 0; y < FitMass_2D[i]->GetNbinsY(); ++y){ //function to propagate ST-fit SF uncertainty to bin error
          float statBase = FitMass_2D[i]->GetBinError(x+1, y+1);
          float SFstat = fabs(FitMass_STstatUp->GetBinContent(x+1, y+1) - FitMass_2D[i]->GetBinContent(x+1, y+1));
          FitMass_2D[i]->SetBinError(x+1,y+1,sqrt(pow(statBase,2)+pow(SFstat,2)));
	}
	for(unsigned x = 0; x < HT_2D[i]->GetNbinsX(); ++x) for(unsigned y = 0; y < HT_2D[i]->GetNbinsY(); ++y){ //function to propagate ST-fit SF uncertainty to bin error
          float statBase = HT_2D[i]->GetBinError(x+1, y+1);
          float SFstat = fabs(HT_STstatUp->GetBinContent(x+1, y+1) - HT_2D[i]->GetBinContent(x+1, y+1));
          HT_2D[i]->SetBinError(x+1,y+1,sqrt(pow(statBase,2)+pow(SFstat,2)));
        }
      }
      HT_2D[i]->Write(HT_2D[i]->GetName());
      FitMass_2D[i]->Write(FitMass_2D[i]->GetName());
    }
    else{
      HT_2D[i]->Write(HT_2D[i]->GetName());
      FitMass_2D[i]->Write(FitMass_2D[i]->GetName());
    }
  }
  savefile2D->Close();

}
