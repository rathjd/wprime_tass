#include "Utilities/TreeClass/ValidationTree.C"
#include "Utilities/DrawDataFormat.cc"
#include "Utilities/MCReweight.cc"
#include "../Utilities/ProgressBar.cc"
// #include "../Utilities/Configs.cc" // For checkpoints

#include <math.h>

#include "TChain.h"
#include "TFile.h"

using namespace std;

class InterTree : public ValidationTree {
public:
  InterTree(TTree* t) : ValidationTree(t) {

  };

  void FillHistograms() {}
};

void MakeHistValidation(int isampleyear = 3, int isampletype = 0, int ifile = -1, bool DoMCReweight = false, bool DrawMCReweight = false) {
  if (isampletype != 2 && DoMCReweight) return;
  rm.TightOnlyInit();
  string basepath = "/eos/user/s/siluo/WPrimeAnalysis/Validation/";
  string itpath = "";
  string SampleYear = dlib.SampleYears[isampleyear];
  // string HistFilePath = "outputs/";
  string HistFilePath = basepath;
  string HistFilePrefix = SampleYear + "_Validation";
  Histograms HistCol;
  vector<string> SampleTypes = dlib.DatasetNames;
  string SampleType = SampleTypes[isampletype];
  HistFilePath = HistFilePath + "Hists/";
  if (ifile > -1) HistFilePath = HistFilePath + "batch/";
  if (isampletype != -1) {
    
  }
  if (DoMCReweight) HistFilePrefix += "_RW";
  else if (SampleType == "ttbar") HistFilePrefix += "_NRW";
  if (SampleType == "ZZ") return;

  MCReweightManager *mcrm = new MCReweightManager();
  if ((DoMCReweight && (SampleType == "ttbar" || SampleType == "")) || DrawMCReweight) {
    mcrm->Init();
    string idr1161 = rm.StringRanges[rm.GetRangeIndex(1161)];
    string idr1151 = rm.StringRanges[rm.GetRangeIndex(1151)];
    string SourceObs = "WPrimeMassSimpleFL";
    string SourcePath = HistFilePath;
    string SourcePrefix = SampleYear + "_Validation";
    mcrm->ReadFromFile(SourcePath, SourcePrefix, SourceObs);
    if (DrawMCReweight) {
      TString fweightname = StandardNames::HistFileName(HistFilePath, HistFilePrefix, "ReweightSF");
      mcrm->SaveToFile(fweightname);
    }
  }

  if (DrawMCReweight) return;

  HistCol.SetSampleTypes(SampleTypes);
  HistCol.AddObservable("LeptonPt",50,0,500);
  HistCol.AddObservable("LeptonEta",90,-4.5,4.5);
  HistCol.AddObservable("LeptonPhi",90,-4.5,4.5);
  HistCol.AddObservable("Jet0Pt",100,0,1000);
  HistCol.AddObservable("Jet0Eta",90,-4.5,4.5);
  HistCol.AddObservable("Jet0Phi",90,-4.5,4.5);
  HistCol.AddObservable("Jet1Pt",100,0,1000);
  HistCol.AddObservable("Jet1Eta",90,-4.5,4.5);
  HistCol.AddObservable("Jet1Phi",90,-4.5,4.5);
  HistCol.AddObservable("Jet2Pt",100,0,1000);
  HistCol.AddObservable("Jet2Eta",90,-4.5,4.5);
  HistCol.AddObservable("Jet2Phi",90,-4.5,4.5);
  HistCol.AddObservable("Jet3Pt",100,0,1000);
  HistCol.AddObservable("Jet3Eta",90,-4.5,4.5);
  HistCol.AddObservable("Jet3Phi",90,-4.5,4.5);
  HistCol.AddObservable("Jet4Pt",100,0,1000);
  HistCol.AddObservable("Jet4Eta",90,-4.5,4.5);
  HistCol.AddObservable("Jet4Phi",90,-4.5,4.5);
  HistCol.AddObservable("METPt",100,0,2000);
  HistCol.AddObservable("METPhi",64,-3.2,3.2);
  HistCol.AddObservable("dPhiMetLep",90,-4.5,4.5);
  HistCol.AddObservable("mT",100,0,2000);
  HistCol.AddObservable("HT",200,0,2000);
  HistCol.AddObservable("WPrimeMassSimpleFL",100,0,2000);
  HistCol.AddObservable("WPrimeMassSimpleLL",100,0,2000);
  // HistCol.AddObservable("WPrimeMass",100,0,2000);
  // HistCol.AddObservable("WPrimeMassFL",100,0,2000);
  // HistCol.AddObservable("WPrimeMassLL",100,0,2000);
  // HistCol.AddObservable("Likelihood",100,-10,0);
  HistCol.CreateHistograms(HistFilePath, HistFilePrefix, SampleType, ifile);
  Progress* progress = new Progress(0,10000);


  TChain* t = new TChain("t");
  float NormFactor = dlib.GetNormFactor(SampleType, isampleyear);
  cout << endl << "Start processing " << SampleType << endl;
  string SamplePath = SampleYear + "_" + SampleType + "/" + SampleYear + "_" + SampleType;
  if (ifile != -1) SamplePath += Form("_%i.root",ifile);
  else SamplePath += "*.root";
  TString InFilePath = basepath + itpath + SamplePath;
  cout << "The InputFile path is " << InFilePath << endl;
  t->Add(InFilePath);
  InterTree *r = new InterTree(t);
  Long64_t EntryMax = t->GetEntries();
  // Long64_t EntryMax = t->GetEntriesFast();
  // Long64_t EntryMax = 2000000;
  progress->SetEntryMax(EntryMax);
  int n_nan_weight = 0;
  for (Long64_t ievt = 0; ievt < EntryMax; ++ievt) {
    r->GetEntry(ievt);
    progress->Print(ievt);
    // checkpoint(0);

    bool hasnan = false;
    for (unsigned iv = 9; iv < HistCol.Variations.size(); ++iv) {
      if (r->EventWeight[iv-8] != r->EventWeight[iv-8]) hasnan = true;
    }
    if (hasnan) {
      n_nan_weight++;
      continue;
    }
    // checkpoint(1);

    for (unsigned iv = 0; iv < HistCol.Variations.size(); ++iv) {
      float EventWeight = r->EventWeight[0];
      if (iv > 8) EventWeight = r->EventWeight[iv-8];
      EventWeight *= NormFactor;
      int RegionIdentifier = r->RegionIdentifier[0];
      if (RegionIdentifier / 10 % 10 == 0) continue;
      if (iv < 9) RegionIdentifier = r->RegionIdentifier[iv];
  
      //Start of customize part
      float LeptonPt = r->LeptonPt;
      float LeptonEta = r->LeptonEta;
      float LeptonPhi = r->LeptonPhi;
      float Jet0Pt = r->JetPt->at(0);
      float Jet0Eta = r->JetEta->at(0);
      float Jet0Phi = r->JetPhi->at(0);
      float Jet1Pt = r->JetPt->at(1);
      float Jet1Eta = r->JetEta->at(1);
      float Jet1Phi = r->JetPhi->at(1);
      float Jet2Pt = r->JetPt->at(2);
      float Jet2Eta = r->JetEta->at(2);
      float Jet2Phi = r->JetPhi->at(2);
      float Jet3Pt = r->JetPt->at(3);
      float Jet3Eta = r->JetEta->at(3);
      float Jet3Phi = r->JetPhi->at(3);
      float Jet4Pt = r->JetPt->at(4);
      float Jet4Eta = r->JetEta->at(4);
      float Jet4Phi = r->JetPhi->at(4);
      float METPt = r->METPt;
      float METPhi = r->METPhi;
      float dPhiMetLep = r->dPhiMetLep;
      float mT = r->mT->at(0);
      float HT = 0;
      for (unsigned ij = 0; ij < r->JetPt->size(); ++ij) {
        HT += r->JetPt->at(ij);
      }
      float WPrimeMassSimpleFL = r->WPrimeMassSimpleFL->at(0);
      float WPrimeMassSimpleLL = r->WPrimeMassSimpleLL->at(0);
      // float WPrimeMass = r->WPrimeMass->at(0);
      // float Likelihood = r->Likelihood->at(0);
      // int   WPType = r->WPType->at(0);
      if (iv > 0 && iv < 9) { // Variations on Physical quantities
        // "EleScaleUp", "EleScaleDown", "EleResUp", "EleResDown", "JESup", "JESdown", "JERup", "JERdown"
        if (iv == 1) LeptonPt = r->LeptonPt_SU;
        if (iv == 2) LeptonPt = r->LeptonPt_SD;
        if (iv == 3) LeptonPt = r->LeptonPt_RU;
        if (iv == 4) LeptonPt = r->LeptonPt_RD;
        if (iv == 5) {
          Jet0Pt = r->JetPt_SU->at(0);
          Jet1Pt = r->JetPt_SU->at(1);
          Jet2Pt = r->JetPt_SU->at(2);
          Jet3Pt = r->JetPt_SU->at(3);
          Jet4Pt = r->JetPt_SU->at(4);
        }
        if (iv == 6) {
          Jet0Pt = r->JetPt_SD->at(0);
          Jet1Pt = r->JetPt_SD->at(1);
          Jet2Pt = r->JetPt_SD->at(2);
          Jet3Pt = r->JetPt_SD->at(3);
          Jet4Pt = r->JetPt_SD->at(4);
        }
        if (iv == 7) {
          Jet0Pt = r->JetPt_RU->at(0);
          Jet1Pt = r->JetPt_RU->at(1);
          Jet2Pt = r->JetPt_RU->at(2);
          Jet3Pt = r->JetPt_RU->at(3);
          Jet4Pt = r->JetPt_RU->at(4);
        }
        if (iv == 8) {
          Jet0Pt = r->JetPt_RD->at(0);
          Jet1Pt = r->JetPt_RD->at(1);
          Jet2Pt = r->JetPt_RD->at(2);
          Jet3Pt = r->JetPt_RD->at(3);
          Jet4Pt = r->JetPt_RD->at(4);
        }
        mT = r->mT->at(iv);
        WPrimeMassSimpleFL = r->WPrimeMassSimpleFL->at(iv);
        WPrimeMassSimpleLL = r->WPrimeMassSimpleLL->at(iv);
      }
      if (SampleType == "ttbar" && DoMCReweight) {
        float mcrweight = mcrm->GetSF1DF(WPrimeMassSimpleFL, RegionIdentifier);
        EventWeight *= mcrweight;
      }
      HistCol.SetCurrentFill(isampletype, iv, RegionIdentifier, EventWeight);
      HistCol.Fill("LeptonPt", LeptonPt);
      HistCol.Fill("LeptonEta",LeptonEta);
      HistCol.Fill("LeptonPhi",LeptonPhi);
      HistCol.Fill("Jet0Pt",Jet0Pt);
      HistCol.Fill("Jet0Eta",Jet0Eta);
      HistCol.Fill("Jet0Phi",Jet0Phi);
      HistCol.Fill("Jet1Pt",Jet1Pt);
      HistCol.Fill("Jet1Eta",Jet1Eta);
      HistCol.Fill("Jet1Phi",Jet1Phi);
      HistCol.Fill("Jet2Pt",Jet2Pt);
      HistCol.Fill("Jet2Eta",Jet2Eta);
      HistCol.Fill("Jet2Phi",Jet2Phi);
      HistCol.Fill("Jet3Pt",Jet3Pt);
      HistCol.Fill("Jet3Eta",Jet3Eta);
      HistCol.Fill("Jet3Phi",Jet3Phi);
      HistCol.Fill("Jet4Pt",Jet4Pt);
      HistCol.Fill("Jet4Eta",Jet4Eta);
      HistCol.Fill("Jet4Phi",Jet4Phi);
      HistCol.Fill("METPt",METPt);
      HistCol.Fill("METPhi",METPhi);
      HistCol.Fill("dPhiMetLep",dPhiMetLep);
      HistCol.Fill("mT",mT);
      HistCol.Fill("HT",HT);
      HistCol.Fill("WPrimeMassSimpleFL",WPrimeMassSimpleFL);
      HistCol.Fill("WPrimeMassSimpleLL",WPrimeMassSimpleLL);
      // if (Likelihood > 0) {
      //   HistCol.Fill("WPrimeMass",WPrimeMass);
      //   if (WPType == 0) HistCol.Fill("WPrimeMassFL", WPrimeMass);
      //   else if (WPType == 1) HistCol.Fill("WPrimeMassLL", WPrimeMass);
      //   else cout << "Wrong WPType read : " << WPType << endl;
      //   HistCol.Fill("Likelihood", log10(Likelihood));
      // }
    }
    // checkpoint(2);
  }
  cout << "In SampleType: " << SampleTypes[isampletype];
  cout << ", Number of events with nan weight = " << n_nan_weight << endl;

  HistCol.PostProcess();
  cout << "Auto exiting" << endl;
  gApplication->Terminate();
}