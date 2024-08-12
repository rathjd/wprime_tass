//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed May 31 13:29:44 2023 by ROOT version 6.22/09
// from TTree t/EventTree
// found on file: /eos/user/s/siluo/WPrimeAnalysis/Validation/2018_FL500.root
//////////////////////////////////////////////////////////

#ifndef CombineHistogramDumpster_h
#define CombineHistogramDumpster_h

#include "../Utilities/Dataset.cc"
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>

class CombineHistogramDumpster {
public :
  TChain          *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain

  // Fixed size dimensions of array or collections stored in the TTree if any.

  // Declaration of leaf types
  Int_t           RegionIdentifier[9];
  Float_t         EventWeight[23];
  Float_t         LeptonPt;
  Float_t         LeptonPt_SU;
  Float_t         LeptonPt_SD;
  Float_t         LeptonPt_RU;
  Float_t         LeptonPt_RD;
  /*Float_t         LeptonEta;
  Float_t	  LeptonPhi;*/
  vector<float>   *JetPt;
  vector<float>   *JetPt_SU;
  vector<float>   *JetPt_SD;
  vector<float>   *JetPt_RU;
  vector<float>   *JetPt_RD;
  /*vector<float>   *JetEta;
  vector<float>   *JetPhi;*/
  vector<bool>	  *JetbTag;
  Float_t         METPt;
  Float_t         METPt_SU;
  Float_t         METPt_SD;
  Float_t         METPt_RU;
  Float_t         METPt_RD;
  /*Float_t         METPhi;
  Float_t	  dPhiMetLep;
  vector<float>   *mT;
  vector<float>   *WPrimeMassSimpleFL;
  vector<float>   *WPrimeMassSimpleLL;*/
  //vector<float>   *WPrimeMass;
  //vector<float>   *Likelihood;
  //vector<int>     *WPType;
  /*Int_t           nPU;
  Float_t         nTrueInt;
  Int_t           nPV;
  Int_t           nPVGood;*/
  vector<double>   *Best_Likelihood_300;
  vector<double>   *Best_Likelihood_400;
  vector<double>   *Best_Likelihood_500;
  vector<double>   *Best_Likelihood_600;
  vector<double>   *Best_Likelihood_700;
  vector<double>   *Best_Likelihood_800;
  vector<double>   *Best_Likelihood_900;
  vector<double>   *Best_Likelihood_1000;
  vector<double>   *Best_Likelihood_1100;
  vector<double>   *Best_WPrimeMass_300;
  vector<double>   *Best_WPrimeMass_400;
  vector<double>   *Best_WPrimeMass_500;
  vector<double>   *Best_WPrimeMass_600;
  vector<double>   *Best_WPrimeMass_700;
  vector<double>   *Best_WPrimeMass_800;
  vector<double>   *Best_WPrimeMass_900;
  vector<double>   *Best_WPrimeMass_1000;
  vector<double>   *Best_WPrimeMass_1100;
  vector<double>   *Best_PbTag_300;
  vector<double>   *Best_PbTag_400;
  vector<double>   *Best_PbTag_500;
  vector<double>   *Best_PbTag_600;
  vector<double>   *Best_PbTag_700;
  vector<double>   *Best_PbTag_800;
  vector<double>   *Best_PbTag_900;
  vector<double>   *Best_PbTag_1000;
  vector<double>   *Best_PbTag_1100;
  Float_t	   EventWeightObjectVariations[9];

  // List of branches
  TBranch        *b_RegionIdentifier;   //!
  TBranch        *b_EventWeight;   //!
  TBranch        *b_LeptonPt;   //!
  TBranch        *b_LeptonPt_SU;   //!
  TBranch        *b_LeptonPt_SD;   //!
  TBranch        *b_LeptonPt_RU;   //!
  TBranch        *b_LeptonPt_RD;   //!
  /*TBranch        *b_LeptonEta;   //!
  TBranch        *b_LeptonPhi;   //!*/
  TBranch        *b_JetPt;   //!
  TBranch        *b_JetPt_SU;   //!
  TBranch        *b_JetPt_SD;   //!
  TBranch        *b_JetPt_RU;   //!
  TBranch        *b_JetPt_RD;   //!
  /*TBranch        *b_JetEta;   //!
  TBranch	 *b_JetPhi;  //!*/
  TBranch	 *b_JetbTag; //!
  TBranch        *b_METPt;   //!
  TBranch        *b_METPt_SU;   //!
  TBranch        *b_METPt_SD;   //!
  TBranch        *b_METPt_RU;   //!
  TBranch        *b_METPt_RD;   //!
  /*TBranch        *b_METPhi;   //!
  TBranch	 *b_dPhiMetLep; //!
  TBranch        *b_mT;   //!
  TBranch        *b_WPrimeMassSimpleFL;   //!
  TBranch        *b_WPrimeMassSimpleLL;   //!*/
  //TBranch	 *b_WPrimeMass; //!
  //TBranch	 *b_Likelihood; //!
  //TBranch	 *b_WPType; //!
  /*TBranch        *b_nPU;   //!
  TBranch        *b_nTrueInt;   //!
  TBranch        *b_nPV;   //!
  TBranch        *b_nPVGood;   //!*/
  TBranch	 *b_Best_Likelihood_300; //!
  TBranch        *b_Best_Likelihood_400; //!
  TBranch        *b_Best_Likelihood_500; //!
  TBranch        *b_Best_Likelihood_600; //!
  TBranch        *b_Best_Likelihood_700; //!
  TBranch        *b_Best_Likelihood_800; //!
  TBranch        *b_Best_Likelihood_900; //!
  TBranch        *b_Best_Likelihood_1000; //!
  TBranch        *b_Best_Likelihood_1100; //!
  TBranch	 *b_Best_WPrimeMass_300; //!
  TBranch        *b_Best_WPrimeMass_400; //!
  TBranch        *b_Best_WPrimeMass_500; //!
  TBranch        *b_Best_WPrimeMass_600; //!
  TBranch        *b_Best_WPrimeMass_700; //!
  TBranch        *b_Best_WPrimeMass_800; //!
  TBranch        *b_Best_WPrimeMass_900; //!
  TBranch        *b_Best_WPrimeMass_1000; //!
  TBranch        *b_Best_WPrimeMass_1100; //!
  TBranch	 *b_Best_PbTag_300; //!
  TBranch        *b_Best_PbTag_400; //!
  TBranch        *b_Best_PbTag_500; //!
  TBranch        *b_Best_PbTag_600; //!
  TBranch        *b_Best_PbTag_700; //!
  TBranch        *b_Best_PbTag_800; //!
  TBranch        *b_Best_PbTag_900; //!
  TBranch        *b_Best_PbTag_1000; //!
  TBranch        *b_Best_PbTag_1100; //!
  TBranch	 *b_EventWeightObjectVariations; //!

  CombineHistogramDumpster(TChain *tree = 0, unsigned it_ = 99, int bin_ = 1152, TString year_ = "2018", int SFreg_ = 0);
  virtual ~CombineHistogramDumpster();
  virtual Int_t    Cut(Long64_t entry);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void     Init(TChain *tree);
  virtual void     Loop();
  virtual Bool_t   Notify();
  virtual void     Show(Long64_t entry = -1);

  unsigned Iterator;
  string YearType;
  Dataset dset;
  int bin;
  int SFreg;
};

#endif

#ifdef CombineHistogramDumpster_cxx
CombineHistogramDumpster::CombineHistogramDumpster(TChain *tree, unsigned it_, int bin_, TString year_, int SFreg_) : fChain(0) 
{
  if(it_>39) {
    std::cout<<"iterator out of range"<<std::endl;
    return;
  }
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
  if (tree == 0) {
    dset = dlib.GetDataset(it_);
    TString FilePath;
    FilePath = "/eos/cms/store/group/phys_b2g/wprime/2017analyzetestmass/"+ year_ + "_" + dset.Name + "/*.root";
    tree = new TChain("t");
    tree->Add(FilePath);
    Iterator = it_;
    YearType = year_;
    bin = bin_;
    SFreg = SFreg_;
  }
  Init(tree);
}

CombineHistogramDumpster::~CombineHistogramDumpster()
{
  if (!fChain) return;
  delete fChain->GetCurrentFile();
}

Int_t CombineHistogramDumpster::GetEntry(Long64_t entry)
{
// Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}
Long64_t CombineHistogramDumpster::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (fChain->GetTreeNumber() != fCurrent) {
    fCurrent = fChain->GetTreeNumber();
    Notify();
  }
  return centry;
}

void CombineHistogramDumpster::Init(TChain *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).

  // Set object pointer
  JetPt = 0;
  JetPt_SU = 0;
  JetPt_SD = 0;
  JetPt_RU = 0;
  JetPt_RD = 0;
  /*JetEta = 0;
  JetPhi = 0;*/
  JetbTag = 0;
  /*mT = 0;
  WPrimeMassSimpleFL = 0;
  WPrimeMassSimpleLL = 0;*/
  //WPrimeMass = 0;
  //Likelihood = 0;
  //WPType = 0;
  Best_WPrimeMass_300 = 0;
  Best_WPrimeMass_400 = 0;
  Best_WPrimeMass_500 = 0;
  Best_WPrimeMass_600 = 0;
  Best_WPrimeMass_700 = 0;
  Best_WPrimeMass_800 = 0;
  Best_WPrimeMass_900 = 0;
  Best_WPrimeMass_1000 = 0;
  Best_WPrimeMass_1100 = 0;
  Best_Likelihood_300 = 0;
  Best_Likelihood_400 = 0;
  Best_Likelihood_500 = 0;
  Best_Likelihood_600 = 0;
  Best_Likelihood_700 = 0;
  Best_Likelihood_800 = 0;
  Best_Likelihood_900 = 0;
  Best_Likelihood_1000 = 0;
  Best_Likelihood_1100 = 0;
  Best_PbTag_300 = 0;
  Best_PbTag_400 = 0;
  Best_PbTag_500 = 0;
  Best_PbTag_600 = 0;
  Best_PbTag_700 = 0;
  Best_PbTag_800 = 0;
  Best_PbTag_900 = 0;
  Best_PbTag_1000 = 0;
  Best_PbTag_1100 = 0;
  for(unsigned i = 0; i < 9; ++i) EventWeightObjectVariations[i] = 1.; //be sure to set this to a default value of 1, if empty anywhere but in 2018

  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);

  fChain->SetBranchAddress("RegionIdentifier", RegionIdentifier, &b_RegionIdentifier);
  fChain->SetBranchAddress("EventWeight", EventWeight, &b_EventWeight);
  fChain->SetBranchAddress("LeptonPt", &LeptonPt, &b_LeptonPt);
  fChain->SetBranchAddress("LeptonPt_SU", &LeptonPt_SU, &b_LeptonPt_SU);
  fChain->SetBranchAddress("LeptonPt_SD", &LeptonPt_SD, &b_LeptonPt_SD);
  fChain->SetBranchAddress("LeptonPt_RU", &LeptonPt_RU, &b_LeptonPt_RU);
  fChain->SetBranchAddress("LeptonPt_RD", &LeptonPt_RD, &b_LeptonPt_RD);
  /*fChain->SetBranchAddress("LeptonEta", &LeptonEta, &b_LeptonEta);
  fChain->SetBranchAddress("LeptonPhi", &LeptonPhi, &b_LeptonPhi);*/
  fChain->SetBranchAddress("JetPt", &JetPt, &b_JetPt);
  fChain->SetBranchAddress("JetPt_SU", &JetPt_SU, &b_JetPt_SU);
  fChain->SetBranchAddress("JetPt_SD", &JetPt_SD, &b_JetPt_SD);
  fChain->SetBranchAddress("JetPt_RU", &JetPt_RU, &b_JetPt_RU);
  fChain->SetBranchAddress("JetPt_RD", &JetPt_RD, &b_JetPt_RD);
  /*fChain->SetBranchAddress("JetEta", &JetEta, &b_JetEta);
  fChain->SetBranchAddress("JetPhi", &JetPhi, &b_JetPhi);*/
  fChain->SetBranchAddress("JetbTag", &JetbTag, &b_JetbTag);
  fChain->SetBranchAddress("METPt", &METPt, &b_METPt);
  fChain->SetBranchAddress("METPt_SU", &METPt_SU, &b_METPt_SU);
  fChain->SetBranchAddress("METPt_SD", &METPt_SD, &b_METPt_SD);
  fChain->SetBranchAddress("METPt_RU", &METPt_RU, &b_METPt_RU);
  fChain->SetBranchAddress("METPt_RD", &METPt_RD, &b_METPt_RD);
  /*fChain->SetBranchAddress("METPhi", &METPhi, &b_METPhi);
  fChain->SetBranchAddress("dPhiMetLep", &dPhiMetLep, &b_dPhiMetLep);
  fChain->SetBranchAddress("mT", &mT, &b_mT);
  fChain->SetBranchAddress("WPrimeMassSimpleFL", &WPrimeMassSimpleFL, &b_WPrimeMassSimpleFL);
  fChain->SetBranchAddress("WPrimeMassSimpleLL", &WPrimeMassSimpleLL, &b_WPrimeMassSimpleLL);*/
  //fChain->SetBranchAddress("WPrimeMass", &WPrimeMass, &b_WPrimeMass);
  //fChain->SetBranchAddress("Likelihood", &Likelihood, &b_Likelihood);
  //fChain->SetBranchAddress("WPType", &WPType, &b_WPType);
  /*fChain->SetBranchAddress("nPU", &nPU, &b_nPU);
  fChain->SetBranchAddress("nTrueInt", &nTrueInt, &b_nTrueInt);
  fChain->SetBranchAddress("nPV", &nPV, &b_nPV);
  fChain->SetBranchAddress("nPVGood", &nPVGood, &b_nPVGood);*/
  fChain->SetBranchAddress("Best_Likelihood_300", &Best_Likelihood_300, &b_Best_Likelihood_300);
  fChain->SetBranchAddress("Best_Likelihood_400", &Best_Likelihood_400, &b_Best_Likelihood_400);
  fChain->SetBranchAddress("Best_Likelihood_500", &Best_Likelihood_500, &b_Best_Likelihood_500);
  fChain->SetBranchAddress("Best_Likelihood_600", &Best_Likelihood_600, &b_Best_Likelihood_600);
  fChain->SetBranchAddress("Best_Likelihood_700", &Best_Likelihood_700, &b_Best_Likelihood_700);
  fChain->SetBranchAddress("Best_Likelihood_800", &Best_Likelihood_800, &b_Best_Likelihood_800);
  fChain->SetBranchAddress("Best_Likelihood_900", &Best_Likelihood_900, &b_Best_Likelihood_900);
  fChain->SetBranchAddress("Best_Likelihood_1000", &Best_Likelihood_1000, &b_Best_Likelihood_1000);
  fChain->SetBranchAddress("Best_Likelihood_1100", &Best_Likelihood_1100, &b_Best_Likelihood_1100);
  fChain->SetBranchAddress("Best_WPrimeMass_300", &Best_WPrimeMass_300, &b_Best_WPrimeMass_300);
  fChain->SetBranchAddress("Best_WPrimeMass_400", &Best_WPrimeMass_400, &b_Best_WPrimeMass_400);
  fChain->SetBranchAddress("Best_WPrimeMass_500", &Best_WPrimeMass_500, &b_Best_WPrimeMass_500);
  fChain->SetBranchAddress("Best_WPrimeMass_600", &Best_WPrimeMass_600, &b_Best_WPrimeMass_600);
  fChain->SetBranchAddress("Best_WPrimeMass_700", &Best_WPrimeMass_700, &b_Best_WPrimeMass_700);
  fChain->SetBranchAddress("Best_WPrimeMass_800", &Best_WPrimeMass_800, &b_Best_WPrimeMass_800);
  fChain->SetBranchAddress("Best_WPrimeMass_900", &Best_WPrimeMass_900, &b_Best_WPrimeMass_900);
  fChain->SetBranchAddress("Best_WPrimeMass_1000", &Best_WPrimeMass_1000, &b_Best_WPrimeMass_1000);
  fChain->SetBranchAddress("Best_WPrimeMass_1100", &Best_WPrimeMass_1100, &b_Best_WPrimeMass_1100);
  fChain->SetBranchAddress("Best_PbTag_300", &Best_PbTag_300, &b_Best_PbTag_300);
  fChain->SetBranchAddress("Best_PbTag_400", &Best_PbTag_400, &b_Best_PbTag_400);
  fChain->SetBranchAddress("Best_PbTag_500", &Best_PbTag_500, &b_Best_PbTag_500);
  fChain->SetBranchAddress("Best_PbTag_600", &Best_PbTag_600, &b_Best_PbTag_600);
  fChain->SetBranchAddress("Best_PbTag_700", &Best_PbTag_700, &b_Best_PbTag_700);
  fChain->SetBranchAddress("Best_PbTag_800", &Best_PbTag_800, &b_Best_PbTag_800);
  fChain->SetBranchAddress("Best_PbTag_900", &Best_PbTag_900, &b_Best_PbTag_900);
  fChain->SetBranchAddress("Best_PbTag_1000", &Best_PbTag_1000, &b_Best_PbTag_1000);
  fChain->SetBranchAddress("Best_PbTag_1100", &Best_PbTag_1100, &b_Best_PbTag_1100);
  if(YearType=="2018") fChain->SetBranchAddress("EventWeightObjectVariations", &EventWeightObjectVariations, &b_EventWeightObjectVariations);
  Notify();
}

Bool_t CombineHistogramDumpster::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}

void CombineHistogramDumpster::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
  if (!fChain) return;
  fChain->Show(entry);
}
Int_t CombineHistogramDumpster::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
  return 1;
}
#endif // #ifdef CombineHistogramDumpster_cxx
