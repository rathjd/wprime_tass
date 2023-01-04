#ifndef BRANCHREADER_CC
#define BRANCHREADER_CC

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class Events {
public:
  TTree          *chain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain
  UInt_t          run;
  UInt_t          luminosityBlock;
  ULong64_t       event;

  string          SampleYear;
  bool            IsMC;

  // GenPart
  UInt_t          nGenPart;
  Float_t         GenPart_pt[149];   //[nGenPart]
  Float_t         GenPart_eta[149];   //[nGenPart]
  Float_t         GenPart_phi[149];   //[nGenPart]
  Float_t         GenPart_mass[149];   //[nGenPart]
  Int_t           GenPart_pdgId[149];   //[nGenPart]
  Int_t           GenPart_genPartIdxMother[149];   //[nGenPart]
  Int_t           GenPart_status[149];   //[nGenPart]
  // Int_t           GenPart_statusFlags[149];   //[nGenPart]

  // GenJet
  UInt_t          nGenJet;
  Float_t         GenJet_eta[24];   //[nGenJet]
  Float_t         GenJet_mass[24];   //[nGenJet]
  Float_t         GenJet_phi[24];   //[nGenJet]
  Float_t         GenJet_pt[24];   //[nGenJet]
  Int_t           GenJet_partonFlavour[24];   //[nGenJet]
  UChar_t         GenJet_hadronFlavour[24];   //[nGenJet]

  // Jet
  UInt_t          nJet;
  Float_t         Jet_pt[25];   //[nJet]
  Float_t         Jet_eta[25];   //[nJet]
  Float_t         Jet_phi[25];   //[nJet]
  Float_t         Jet_mass[25];   //[nJet]
  Int_t           Jet_jetId[25];   //[nJet]
  Int_t           Jet_puId[25];   //[nJet]
  Int_t           Jet_genJetIdx[25];   //[nJet]
  // Float_t         Jet_btagDeepB[25];   //[nJet]
  Float_t         Jet_btagDeepFlavB[25];   //[nJet]
  Int_t           Jet_hadronFlavour[25];   //[nJet]
  Int_t           Jet_partonFlavour[25];   //[nJet]
  Float_t	  Jet_pt_nom[25];   //[nJet]
  Float_t         Jet_pt_jesTotalUp[25];   //[nJet]
  Float_t         Jet_pt_jesTotalDown[25];   //[nJet]
  Float_t         Jet_pt_jerUp[25];   //[nJet]
  Float_t         Jet_pt_jerDown[25];   //[nJet]
  Float_t         Jet_mass_nom[25];   //[nJet]
  Float_t         Jet_mass_jesTotalUp[25];   //[nJet]
  Float_t         Jet_mass_jesTotalDown[25];   //[nJet]
  Float_t         Jet_mass_jerUp[25];   //[nJet]
  Float_t         Jet_mass_jerDown[25];   //[nJet]
  Float_t	  Jet_bRegCorr[25];   //[nJet]
  Float_t	  Jet_bRegRes[25];  //[nJet]

  // Electron
  UInt_t          nElectron;
  Float_t         Electron_pt[9];   //[nElectron]
  Float_t         Electron_eta[9];   //[nElectron]
  Float_t         Electron_phi[9];   //[nElectron]
  Float_t         Electron_mass[9];   //[nElectron]
  Int_t           Electron_charge[9];   //[nElectron]
  Int_t           Electron_cutBased[9];   //[nElectron]
  Bool_t          Electron_cutBased_HEEP[9];   //[nElectron]
  Float_t	  Electron_dEsigmaDown[9];    //[nElectron]
  Float_t	  Electron_dEsigmaUp[9];    //[nElectron]
  // Int_t           Electron_genPartIdx[9];   //[nElectron]

  //Muon
  UInt_t          nMuon;
  Float_t         Muon_pt[18];   //[nMuon]
  Float_t         Muon_eta[18];   //[nMuon]
  Float_t         Muon_phi[18];   //[nMuon]
  Float_t         Muon_mass[18];   //[nMuon]
  Int_t           Muon_charge[18];   //[nMuon]
  // Int_t           Muon_genPartIdx[18];   //[nMuon]
  Int_t           Muon_tightId[18];   //[nMuon]
  Bool_t          Muon_looseId[18];   //[nMuon]
  UChar_t	  Muon_highPtId[18];  //[nMuon]
  Float_t         Muon_pfRelIso04_all[18];   //[nMuon]

  //MET
  Float_t         MET_phi;
  Float_t         MET_pt;
  Float_t	  MET_T1_pt;
  Float_t	  MET_T1_phi;
  Float_t	  MET_T1smear_pt;
  Float_t	  MET_T1smear_phi;
  Float_t	  MET_T1smear_pt_jesTotalUp;
  Float_t         MET_T1smear_phi_jesTotalUp;
  Float_t         MET_T1smear_pt_jesTotalDown;
  Float_t         MET_T1smear_phi_jesTotalDown;
  Float_t	  MET_T1smear_pt_jerUp;
  Float_t         MET_T1smear_phi_jerUp;
  Float_t         MET_T1smear_pt_jerDown;
  Float_t         MET_T1smear_phi_jerDown;

  //GenMET
  Float_t         GenMET_phi;
  Float_t         GenMET_pt;

  //TrigObj
  UInt_t          nTrigObj;
  Float_t         TrigObj_pt[49];   //[nTrigObj]
  Float_t         TrigObj_eta[49];   //[nTrigObj]
  Float_t         TrigObj_phi[49];   //[nTrigObj]
  Int_t           TrigObj_id[49];   //[nTrigObj]

  //SFs
  Float_t	  Electron_scaleFactor[9]; //nElectron
  Float_t	  Electron_scaleFactorUp[9]; //[nElectron]
  Float_t	  Electron_scaleFactorDown[9]; //[nElectron]
  Float_t	  Muon_scaleFactor[18]; //[nMuon]
  Float_t	  Muon_scaleFactorSyst[18]; //[nMuon]
  Float_t	  Muon_scaleFactorStat[18]; //[nMuon]
  Float_t	  Jet_bTagScaleFactorTight[25]; //[nJet]
  Float_t	  Jet_bTagScaleFactorTightUp[25]; //[nJet]
  Float_t	  Jet_bTagScaleFactorTightDown[25]; //[nJet]
  Float_t         Jet_bTagScaleFactorMedium[25]; //[nJet]
  Float_t         Jet_bTagScaleFactorMediumUp[25]; //[nJet]
  Float_t         Jet_bTagScaleFactorMediumDown[25]; //[nJet]
  Float_t         Jet_bTagScaleFactorLoose[25]; //[nJet]
  Float_t         Jet_bTagScaleFactorLooseUp[25]; //[nJet]
  Float_t         Jet_bTagScaleFactorLooseDown[25]; //[nJet]
  Float_t	  Jet_puIdScaleFactorTight[25]; //[nJet]
  Float_t	  Jet_puIdScaleFactorTightUp[25]; //nJet]
  Float_t	  Jet_puIdScaleFactorTightDown[25]; //[nJet]
  Float_t         Jet_puIdScaleFactorMedium[25]; //[nJet]
  Float_t         Jet_puIdScaleFactorMediumUp[25]; //nJet]
  Float_t         Jet_puIdScaleFactorMediumDown[25]; //[nJet]
  Float_t         Jet_puIdScaleFactorLoose[25]; //[nJet]
  Float_t         Jet_puIdScaleFactorLooseUp[25]; //nJet]
  Float_t         Jet_puIdScaleFactorLooseDown[25]; //[nJet]

  //Trigger
  bool isolated_electron_trigger;
  bool isolated_muon_trigger;
  bool isolated_muon_track_trigger;

  //Pileup
  Float_t         Pileup_nTrueInt;
  Float_t         Pileup_pudensity;
  Float_t         Pileup_gpudensity;
  Int_t           Pileup_nPU;
  Int_t           Pileup_sumEOOT;
  Int_t           Pileup_sumLOOT;

  // Primary Vertices
  Int_t PV_npvs;
  Int_t PV_npvsGood;

  // Flags
  bool Flag_goodVertices;
  bool Flag_globalSuperTightHalo2016Filter;
  bool Flag_HBHENoiseFilter;
  bool Flag_HBHENoiseIsoFilter;
  bool Flag_EcalDeadCellTriggerPrimitiveFilter;
  bool Flag_BadPFMuonFilter;
  bool Flag_BadPFMuonDzFilter;
  bool Flag_eeBadScFilter;
  bool Flag_ecalBadCalibFilter;

  // L1PreFiring weights for 2016 and 2017
  Float_t L1PreFiringWeight_Nom;
  Float_t L1PreFiringWeight_Up;
  Float_t L1PreFiringWeight_Down;


  Events(TChain *chain_, string sy_, bool mc_){
     chain = chain_;
     SampleYear = sy_;
     IsMC = mc_;
     Init();
  };

  ~Events() {
     if (!chain) return;
     delete chain->GetCurrentFile();
  };

  Int_t GetEntry(Long64_t entry) {
  // Read contents of entry.
     if (!chain) return 0;
     return chain->GetEntry(entry);
  }

  void Init() {
    chain->SetBranchAddress("nJet", &nJet);
    chain->SetBranchAddress("Jet_pt", &Jet_pt);
    chain->SetBranchAddress("Jet_eta", &Jet_eta);
    chain->SetBranchAddress("Jet_phi", &Jet_phi);
    chain->SetBranchAddress("Jet_mass", &Jet_mass);
    chain->SetBranchAddress("Jet_jetId", &Jet_jetId);
    chain->SetBranchAddress("Jet_puId", &Jet_puId);
    chain->SetBranchAddress("Jet_btagDeepFlavB", &Jet_btagDeepFlavB);
    chain->SetBranchAddress("Jet_bRegCorr", &Jet_bRegCorr);
    chain->SetBranchAddress("Jet_bRegRes", &Jet_bRegRes);
    chain->SetBranchAddress("Jet_pt_nom", &Jet_pt_nom);
    chain->SetBranchAddress("Jet_mass_nom", &Jet_mass_nom);

    chain->SetBranchAddress("nElectron", &nElectron);
    chain->SetBranchAddress("Electron_pt", &Electron_pt);
    chain->SetBranchAddress("Electron_eta", &Electron_eta);
    chain->SetBranchAddress("Electron_phi", &Electron_phi);
    chain->SetBranchAddress("Electron_mass", &Electron_mass);
    chain->SetBranchAddress("Electron_charge", &Electron_charge);
    chain->SetBranchAddress("Electron_cutBased", &Electron_cutBased);
    chain->SetBranchAddress("Electron_cutBased_HEEP", &Electron_cutBased_HEEP);

    chain->SetBranchAddress("nMuon", &nMuon);
    chain->SetBranchAddress("Muon_pt", &Muon_pt);
    chain->SetBranchAddress("Muon_eta", &Muon_eta);
    chain->SetBranchAddress("Muon_phi", &Muon_phi);
    chain->SetBranchAddress("Muon_mass", &Muon_mass);
    chain->SetBranchAddress("Muon_charge", &Muon_charge);
    chain->SetBranchAddress("Muon_looseId", &Muon_looseId);
    chain->SetBranchAddress("Muon_tightId", &Muon_tightId);
    chain->SetBranchAddress("Muon_highPtId", &Muon_highPtId);
    chain->SetBranchAddress("Muon_pfRelIso04_all", &Muon_pfRelIso04_all);

    chain->SetBranchAddress("MET_phi", &MET_phi);
    chain->SetBranchAddress("MET_pt", &MET_pt);
    chain->SetBranchAddress("MET_T1_pt", &MET_T1_pt);
    chain->SetBranchAddress("MET_T1_phi", &MET_T1_phi);

    chain->SetBranchAddress("nTrigObj", &nTrigObj);
    chain->SetBranchAddress("TrigObj_pt", &TrigObj_pt);
    chain->SetBranchAddress("TrigObj_eta", &TrigObj_eta);
    chain->SetBranchAddress("TrigObj_phi", &TrigObj_phi);
    chain->SetBranchAddress("TrigObj_id", &TrigObj_id);

    chain->SetBranchAddress("run", &run);
    chain->SetBranchAddress("luminosityBlock", &luminosityBlock);

    if (IsMC){
      chain->SetBranchAddress("Jet_pt_jesTotalUp", &Jet_pt_jesTotalUp);
      chain->SetBranchAddress("Jet_pt_jesTotalDown", &Jet_pt_jesTotalDown);
      chain->SetBranchAddress("Jet_pt_jerUp", &Jet_pt_jerUp);
      chain->SetBranchAddress("Jet_pt_jerDown", &Jet_pt_jerDown);
      chain->SetBranchAddress("Jet_mass_jesTotalUp", &Jet_mass_jesTotalUp);
      chain->SetBranchAddress("Jet_mass_jesTotalDown", &Jet_mass_jesTotalDown);
      chain->SetBranchAddress("Jet_mass_jerUp", &Jet_mass_jerUp);
      chain->SetBranchAddress("Jet_mass_jerDown", &Jet_mass_jerDown);

      chain->SetBranchAddress("Jet_hadronFlavour", &Jet_hadronFlavour);
      chain->SetBranchAddress("Jet_partonFlavour", &Jet_partonFlavour);
      chain->SetBranchAddress("Jet_genJetIdx", &Jet_genJetIdx);

      chain->SetBranchAddress("nGenJet", &nGenJet);
      chain->SetBranchAddress("GenJet_pt", &GenJet_pt);
      chain->SetBranchAddress("GenJet_eta", &GenJet_eta);
      chain->SetBranchAddress("GenJet_phi", &GenJet_phi);
      chain->SetBranchAddress("GenJet_mass", &GenJet_mass);
      chain->SetBranchAddress("GenJet_hadronFlavour", &GenJet_hadronFlavour);
      chain->SetBranchAddress("GenJet_partonFlavour", &GenJet_partonFlavour);

      chain->SetBranchAddress("MET_T1smear_pt", &MET_T1smear_pt);
      chain->SetBranchAddress("MET_T1smear_phi", &MET_T1smear_phi);
      chain->SetBranchAddress("MET_T1smear_pt_jesTotalUp", &MET_T1smear_pt_jesTotalUp);
      chain->SetBranchAddress("MET_T1smear_phi_jesTotalUp", &MET_T1smear_phi_jesTotalUp);
      chain->SetBranchAddress("MET_T1smear_pt_jesTotalDown", &MET_T1smear_pt_jesTotalDown);
      chain->SetBranchAddress("MET_T1smear_phi_jesTotalDown", &MET_T1smear_phi_jesTotalDown);
      chain->SetBranchAddress("MET_T1smear_pt_jerUp", &MET_T1smear_pt_jerUp);
      chain->SetBranchAddress("MET_T1smear_phi_jerUp", &MET_T1smear_phi_jerUp);
      chain->SetBranchAddress("MET_T1smear_pt_jerDown", &MET_T1smear_pt_jerDown);
      chain->SetBranchAddress("MET_T1smear_phi_jerDown", &MET_T1smear_phi_jerDown);

      chain->SetBranchAddress("GenMET_phi", &GenMET_phi);
      chain->SetBranchAddress("GenMET_pt", &GenMET_pt);

      chain->SetBranchAddress("nGenPart", &nGenPart);
      chain->SetBranchAddress("GenPart_pt", &GenPart_pt);
      chain->SetBranchAddress("GenPart_eta", &GenPart_eta);
      chain->SetBranchAddress("GenPart_phi", &GenPart_phi);
      chain->SetBranchAddress("GenPart_mass", &GenPart_mass);
      chain->SetBranchAddress("GenPart_pdgId", &GenPart_pdgId);
      chain->SetBranchAddress("GenPart_genPartIdxMother", &GenPart_genPartIdxMother);
      chain->SetBranchAddress("GenPart_status", &GenPart_status);

      chain->SetBranchAddress("Pileup_gpudensity", &Pileup_gpudensity);
      chain->SetBranchAddress("Pileup_nPU", &Pileup_nPU);
      chain->SetBranchAddress("Pileup_nTrueInt", &Pileup_nTrueInt);
      chain->SetBranchAddress("Pileup_pudensity", &Pileup_pudensity);
      chain->SetBranchAddress("Pileup_sumEOOT", &Pileup_sumEOOT);
      chain->SetBranchAddress("Pileup_sumLOOT", &Pileup_sumLOOT);

      chain->SetBranchAddress("Electron_scaleFactor", &Electron_scaleFactor);
      chain->SetBranchAddress("Electron_scaleFactorUp", &Electron_scaleFactorUp);
      chain->SetBranchAddress("Electron_scaleFactorDown", &Electron_scaleFactorDown);
      chain->SetBranchAddress("Muon_scaleFactor", &Muon_scaleFactor);
      chain->SetBranchAddress("Muon_scaleFactorSyst", &Muon_scaleFactorSyst);
      chain->SetBranchAddress("Muon_scaleFactorStat", &Muon_scaleFactorStat);
      chain->SetBranchAddress("Jet_bTagScaleFactorTight", &Jet_bTagScaleFactorTight);
      chain->SetBranchAddress("Jet_bTagScaleFactorTightUp", &Jet_bTagScaleFactorTightUp);
      chain->SetBranchAddress("Jet_bTagScaleFactorTightDown", &Jet_bTagScaleFactorTightDown);
      chain->SetBranchAddress("Jet_bTagScaleFactorMedium", &Jet_bTagScaleFactorMedium);
      chain->SetBranchAddress("Jet_bTagScaleFactorMediumUp", &Jet_bTagScaleFactorMediumUp);
      chain->SetBranchAddress("Jet_bTagScaleFactorMediumDown", &Jet_bTagScaleFactorMediumDown);
      chain->SetBranchAddress("Jet_bTagScaleFactorLoose", &Jet_bTagScaleFactorLoose);
      chain->SetBranchAddress("Jet_bTagScaleFactorLooseUp", &Jet_bTagScaleFactorLooseUp);
      chain->SetBranchAddress("Jet_bTagScaleFactorLooseDown", &Jet_bTagScaleFactorLooseDown);
      chain->SetBranchAddress("Jet_puIdScaleFactorTight", &Jet_puIdScaleFactorTight);
      chain->SetBranchAddress("Jet_puIdScaleFactorTightUp", &Jet_puIdScaleFactorTightUp);
      chain->SetBranchAddress("Jet_puIdScaleFactorTightDown", &Jet_puIdScaleFactorTightDown);
      chain->SetBranchAddress("Jet_puIdScaleFactorMedium", &Jet_puIdScaleFactorMedium);
      chain->SetBranchAddress("Jet_puIdScaleFactorMediumUp", &Jet_puIdScaleFactorMediumUp);
      chain->SetBranchAddress("Jet_puIdScaleFactorMediumDown", &Jet_puIdScaleFactorMediumDown);
      chain->SetBranchAddress("Jet_puIdScaleFactorLoose", &Jet_puIdScaleFactorLoose);
      chain->SetBranchAddress("Jet_puIdScaleFactorLooseUp", &Jet_puIdScaleFactorLooseUp);
      chain->SetBranchAddress("Jet_puIdScaleFactorLooseDown", &Jet_puIdScaleFactorLooseDown);
    }

    if (SampleYear == "2016" || SampleYear == "2016apv"){
      chain->SetBranchAddress("HLT_Ele27_WPTight_Gsf", &isolated_electron_trigger);
      chain->SetBranchAddress("HLT_IsoMu24",&isolated_muon_trigger);
      chain->SetBranchAddress("HLT_IsoTkMu24",&isolated_muon_track_trigger);
      Flag_eeBadScFilter = true;
      Flag_ecalBadCalibFilter = true;
    }
    else if (SampleYear == "2017"){
      chain->SetBranchAddress("HLT_Ele35_WPTight_Gsf", &isolated_electron_trigger);
      chain->SetBranchAddress("HLT_IsoMu27",&isolated_muon_trigger);
      isolated_muon_track_trigger = false;
      chain->SetBranchAddress("Flag_eeBadScFilter", &Flag_eeBadScFilter);
      chain->SetBranchAddress("Flag_ecalBadCalibFilter", &Flag_ecalBadCalibFilter);
    }
    else if (SampleYear == "2018"){
      chain->SetBranchAddress("HLT_Ele32_WPTight_Gsf", &isolated_electron_trigger);
      chain->SetBranchAddress("HLT_IsoMu24",&isolated_muon_trigger);
      isolated_muon_track_trigger = false;
      chain->SetBranchAddress("Flag_eeBadScFilter", &Flag_eeBadScFilter);
      chain->SetBranchAddress("Flag_ecalBadCalibFilter", &Flag_ecalBadCalibFilter);
    }

    if(SampleYear == "2016" || SampleYear == "2016apv" || SampleYear == "2017"){
      chain->SetBranchAddress("L1PreFiringWeight_Nom", &L1PreFiringWeight_Nom);
      chain->SetBranchAddress("L1PreFiringWeight_Up", &L1PreFiringWeight_Up);
      chain->SetBranchAddress("L1PreFiringWeight_Down", &L1PreFiringWeight_Down);
    }

    chain->SetBranchAddress("PV_npvs", &PV_npvs);
    chain->SetBranchAddress("PV_npvsGood", &PV_npvsGood);

    chain->SetBranchAddress("Flag_goodVertices", &Flag_goodVertices);
    chain->SetBranchAddress("Flag_globalSuperTightHalo2016Filter", &Flag_globalSuperTightHalo2016Filter);
    chain->SetBranchAddress("Flag_HBHENoiseFilter", &Flag_HBHENoiseFilter);
    chain->SetBranchAddress("Flag_HBHENoiseIsoFilter", &Flag_HBHENoiseIsoFilter);
    chain->SetBranchAddress("Flag_EcalDeadCellTriggerPrimitiveFilter", &Flag_EcalDeadCellTriggerPrimitiveFilter);
    chain->SetBranchAddress("Flag_BadPFMuonFilter", &Flag_BadPFMuonFilter);
    chain->SetBranchAddress("Flag_BadPFMuonDzFilter", &Flag_BadPFMuonDzFilter);
  }

};

#endif
