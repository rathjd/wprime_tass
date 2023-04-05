#ifndef NANOAODREADER_CC
#define NANOAODREADER_CC

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <array>

#include "TChain.h"
#include "TFile.h"

#include "DataFormat.cc"
#include "BranchReader.cc"

#include "Configs.cc"
#include "bTag.cc"
#include "PUID.cc"

#include "XYMETCorrection_withUL17andUL18andUL16.h"

#include "ProgressBar.cc"

using namespace std;

class NanoAODReader {
public:
  NanoAODReader(Configs *conf_) {
    chain = new TChain("Events;2");

    conf = conf_;

    IsMC = conf->IsMC;

    R_BTSF = new bTagSFReader(conf);
    R_BTSF->ReadCSVFile();
    R_PUIDSF = new PUIDSFReader(conf);
    R_PUIDSF->ReadPUIDSF();

    if (conf->iFile >= 0 || conf->InputFile == "All") { // batch mode

      vector<string> rootfiles = GetFileNames();
      // rootfiles = {"/eos/user/p/pflanaga/andrewsdata/skimmed_samples/ttbar/2018/B047029C-C11B-A54B-A5F3-91981AB3D5DC.root"};
      for (string rf : rootfiles) {
        chain->Add(TString(rf));
        cout << "Successfully loaded root file: " << rf << endl;
      }
      cout << "In total, " << rootfiles.size() << " files are loaded" << endl;
    }
    else if (conf->InputFile == "") cout << "iFile set to negative but a valid test InputFile name is missing" << endl;
    else {
      chain->Add(conf->InputFile);
      cout << "Loaded test InputFile: " << conf->InputFile << endl;
    }
    cout << "Running with SampleYear = " << conf->SampleYear << ", SampleType = " << conf->SampleType <<  endl;
    evts = new Events(chain, conf->SampleYear, IsMC);
    if (conf->InputFile != "All" && conf->FilesPerJob == 1) cout << "This iteration contains " << GetEntries() << " events" <<endl;
  };

  ~NanoAODReader() {
    cout << "NanoAODReader Destructed" << endl;
  }

  vector<string> GetFileNames() {
    vector<string> out;
    string basepath = "filenames/";
    if (conf->DASInput) basepath = basepath + "DASFileNames/DAS_";
    string filename = basepath + conf->SampleType + "_" + conf->SampleYear + ".txt";
    ifstream infile(filename);
    if (!infile) {
      cout << "Cannot read from file " << filename << endl;
      throw runtime_error("Cannot Read from file");
      return out;
    }
    else cout << "Reading from file " << filename << endl;

    int startfile = conf->iFile * conf->FilesPerJob;
    int endfile = (conf->iFile + 1) * conf->FilesPerJob - 1;
    string rootf;
    int counter = -1;
    while (getline(infile, rootf)) {
      ++counter;
      if (counter < startfile) continue;
      if (counter > endfile && conf->InputFile != "All") break;
      if (rootf.find("/store/") == 0) rootf = "root://cms-xrd-global.cern.ch/" + rootf;
      cout << "Loading root file " << rootf << endl;
      out.push_back(rootf);
    }
    return out;
  }

  Long64_t GetEntries() {
    return chain->GetEntries();
  }

  Long64_t GetEntriesFast() {
    return chain->GetEntriesFast();
  }

  void SetbTag(bTagEff* bt) {
    bTE = bt;
  }

  //function to determine lepton-jet overlaps, gives answer depending on PUID passing or not
  vector<bool> OverlapCheck(Lepton ell_){
    vector<bool> out = {true, true, true};
    for(unsigned j = 0; j < Jets.size(); ++j) if(fabs(Jets[j].DeltaR(ell_)) < 0.4) out = Jets[j].PUIDpasses;//this solution presumes there is only 1 possible jet to match
    return out;
  }

  int ReadEvent(Long64_t i) {
    bTagWP = conf->bTagWP;
    PUIDWP = conf->PUIDWP;

    Long64_t evtcode = evts->LoadTree(i);
    if (evtcode < 0) return 0;
    evts->GetEntry(i);
    if(ReadMETFilterStatus() == false) return 1; //skip events not passing MET filter flags
    run = evts->run;
    luminosityBlock = evts->luminosityBlock;
    if (!IsMC && (run < 0 || luminosityBlock < 0)) cout << "Run/LuminosityBlock number is negative" <<endl;
    
    if (IsMC) {
      ReadGenParts();
      ReadGenJets();
      ReadGenMET();
      ReadPileup();
    }
    ReadJets();
    if (conf->bTagEffCreation || conf->JetScaleCreation) return 1;
    ReadLeptons();
    ReadMET();
    ReadTriggers();
    ReadVertices();
    RegionAssociations = RegionReader();
    KeepEvent = RegionAssociations.KeepEvent();
    EventWeights = CalcEventSFweights();
    return 1;
  }

  void ReadGenParts() {
    GenParts.clear();
    for (unsigned i = 0; i < evts->nGenPart; ++i) {
      GenPart tmp;
      tmp.SetPtEtaPhiM(evts->GenPart_pt[i],evts->GenPart_eta[i],evts->GenPart_phi[i],evts->GenPart_mass[i]);
      tmp.index = i;
      tmp.genPartIdxMother = evts->GenPart_genPartIdxMother[i];
      tmp.pdgId = evts->GenPart_pdgId[i];
      tmp.status = evts->GenPart_status[i];
      GenParts.push_back(tmp);
    }
  }

  void ReadGenJets() {
    GenJets.clear();
    for (unsigned i = 0; i < evts->nGenJet; ++i) {
      GenJet tmp;
      tmp.SetPtEtaPhiM(evts->GenJet_pt[i],evts->GenJet_eta[i],evts->GenJet_phi[i],evts->GenJet_mass[i]);
      tmp.index = i;
      tmp.partonFlavour = evts->GenJet_partonFlavour[i];
      tmp.hadronFlavour = evts->GenJet_hadronFlavour[i];
      GenJets.push_back(tmp);
    }
  }

  void ReadJets() {
    Jets.clear();
    // bool IsMC_back = IsMC; // force IsMC operations and get back after wards.
    // IsMC = true;
    for (unsigned i = 0; i < evts->nJet; ++i) {

      //determine maximum pT of all jet variations
      float maxPt = max(evts->Jet_pt_nom[i], evts->Jet_pt_jesTotalUp[i]);
      maxPt = max(maxPt, evts->Jet_pt_jesTotalDown[i]);
      maxPt = max(maxPt, evts->Jet_pt_jerUp[i]);
      maxPt = max(maxPt, evts->Jet_pt_jerDown[i]);

      //baseline jet selections
      if(maxPt < 30.) continue;
      if(evts->Jet_jetId[i] < 4) continue;
      if(fabs(evts->Jet_eta[i]) >= 5.0) continue;//added to accommodate PU ID limits

      //storing jet variations vectors and nominal -> all the same for data

      Jet tmp;
      tmp.SetPtEtaPhiM(evts->Jet_pt_nom[i],evts->Jet_eta[i],evts->Jet_phi[i],evts->Jet_mass_nom[i]); //the nominal here in MC contains JER while nanoAOD default does not
      tmp.index = i;

      TLorentzVector PtVars;
      PtVars.SetPtEtaPhiM(evts->Jet_pt_jesTotalUp[i], evts->Jet_eta[i], evts->Jet_phi[i], evts->Jet_mass_jesTotalUp[i]);
      tmp.JESup() = PtVars;

      PtVars.SetPtEtaPhiM(evts->Jet_pt_jesTotalDown[i], evts->Jet_eta[i], evts->Jet_phi[i], evts->Jet_mass_jesTotalDown[i]);
      tmp.JESdown() = PtVars;

      PtVars.SetPtEtaPhiM(evts->Jet_pt_jerUp[i], evts->Jet_eta[i], evts->Jet_phi[i], evts->Jet_mass_jerUp[i]);
      tmp.JERup() = PtVars;

      PtVars.SetPtEtaPhiM(evts->Jet_pt_jerDown[i], evts->Jet_eta[i], evts->Jet_phi[i], evts->Jet_mass_jerDown[i]);
      tmp.JERdown() = PtVars;

      //set PUID flags
      tmp.PUIDpasses = PUID(tmp.Pt(), fabs(tmp.Eta()), evts->Jet_puId[i], conf->SampleYear);

      //set PUID SFs
      if(IsMC && evts->Jet_pt_nom[i] < 50. && evts->Jet_genJetIdx[i] >= 0){ //unlike other SFs, PU Jets and jets failing ID are not supposed to contribute to event weights
        vector<vector<float> > PUIDSFs = {{1,1,1},{1,1,1},{1,1,1}};
        if (conf->UseSkims_PUIDSF || conf->Compare_PUIDSF) {
          if(tmp.PUIDpasses[0]){
            PUIDSFs[0][0] = evts->Jet_puIdScaleFactorLoose[i];
            PUIDSFs[1][0] = evts->Jet_puIdScaleFactorLooseUp[i];
            PUIDSFs[2][0] = evts->Jet_puIdScaleFactorLooseDown[i];
          }
          if(tmp.PUIDpasses[1]){
            PUIDSFs[0][1] = evts->Jet_puIdScaleFactorMedium[i];
            PUIDSFs[1][1] = evts->Jet_puIdScaleFactorMediumUp[i];
            PUIDSFs[2][1] = evts->Jet_puIdScaleFactorMediumDown[i];
          }
          if(tmp.PUIDpasses[2]){
            PUIDSFs[0][2] = evts->Jet_puIdScaleFactorTight[i];
            PUIDSFs[1][2] = evts->Jet_puIdScaleFactorTightUp[i];
            PUIDSFs[2][2] = evts->Jet_puIdScaleFactorTightDown[i];
          }
        }
        if (conf->Compare_PUIDSF) R_PUIDSF->CompareScaleFactors(tmp, PUIDSFs);
        if (!conf->UseSkims_PUIDSF) {
          PUIDSFs = R_PUIDSF->GetScaleFactors(tmp);
        }
        tmp.PUIDSFweights = PUIDSFs;
      }

      //set generator information
      if (IsMC) {
        tmp.genJetIdx = evts->Jet_genJetIdx[i];
        tmp.hadronFlavour = evts->Jet_hadronFlavour[i];
        tmp.partonFlavour = evts->Jet_partonFlavour[i];
      }

      //set btagging flags
      tmp.bTagPasses = bTag(evts->Jet_btagDeepFlavB[i], conf->SampleYear);
      vector<float> bTag_Eff = bTE->GetEff(tmp);
      // vector<float> bTag_Eff = {0.9,0.7,0.5};

      //set btagging SFs
      tmp.bJetSFweights = {{1.,1.,1.}, {1.,1.,1.}, {1.,1.,1.}};
      if (IsMC) {
        //FIXME: Need b-tagging efficiency per sample at some point, see https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagSFMethods#b_tagging_efficiency_in_MC_sampl
        vector<vector<float> > bTSFs = {{1,1,1},{1,1,1},{1,1,1}};
        if (conf->UseSkims_bTagSF || conf->Compare_bTagSF) {
          bTSFs[0] = {evts->Jet_bTagScaleFactorLoose[i], evts->Jet_bTagScaleFactorMedium[i], evts->Jet_bTagScaleFactorTight[i]};
          bTSFs[1] = {evts->Jet_bTagScaleFactorLooseUp[i], evts->Jet_bTagScaleFactorMediumUp[i], evts->Jet_bTagScaleFactorTightUp[i]};
          bTSFs[2] = {evts->Jet_bTagScaleFactorLooseDown[i], evts->Jet_bTagScaleFactorMediumDown[i], evts->Jet_bTagScaleFactorTightDown[i]};
        }
        if (conf->Compare_bTagSF) R_BTSF->CompareScaleFactors(tmp, bTSFs);
        if (!conf->UseSkims_bTagSF) {
          bTSFs = R_BTSF->GetScaleFactors(tmp);
        }
        
        for (unsigned iv = 0; iv < 3; ++iv) {
          for (unsigned iwp = 0; iwp < 3; ++iwp) {
            if (tmp.bTagPasses[iwp]) tmp.bJetSFweights[iv][iwp] = bTSFs[iv][iwp];
            else tmp.bJetSFweights[iv][iwp] = (1. - bTag_Eff[iwp] * bTSFs[iv][iwp]) / (1. - bTag_Eff[iwp]);
            if (conf->bTagEffCreation) continue;
            if (tmp.bJetSFweights[iv][iwp] <= 0 || tmp.bJetSFweights[iv][iwp] != tmp.bJetSFweights[iv][iwp]) {
              cout << "In bTag WP " << iwp << " , " << iv << " variation, ";
              if (tmp.bTagPasses[iwp]) cout << " bTagged ";
              else cout << " Non-bTagged ";
              cout << Form("Jet %i (pT = %f, eta = %f) has unexpected bJetSFweight. Eff = %f, SF = %f", i, tmp.Pt(), tmp.Eta(), bTag_Eff[iwp], bTSFs[iv][iwp]) << endl;
            }
          }
        }
      }

      Jets.push_back(tmp);
    }
    // IsMC = IsMC_back;
  }

  void ReadLeptons() {
    Leptons.clear();
    Electrons.clear();
    Muons.clear();
    for (unsigned i = 0; i < evts->nElectron; ++i) {
      Electron tmp;
      tmp.SetPtEtaPhiM(evts->Electron_pt[i],evts->Electron_eta[i],evts->Electron_phi[i],evts->Electron_mass[i]);
      //set resolution variations (only matter in MC, will be ineffective in data)
      tmp.ResUp().SetPtEtaPhiM(evts->Electron_pt[i],evts->Electron_eta[i],evts->Electron_phi[i],evts->Electron_mass[i]);
      tmp.ResUp().SetE(tmp.E()-evts->Electron_dEsigmaUp[i]);
      tmp.ResDown().SetPtEtaPhiM(evts->Electron_pt[i],evts->Electron_eta[i],evts->Electron_phi[i],evts->Electron_mass[i]);
      tmp.ResDown().SetE(tmp.E()-evts->Electron_dEsigmaDown[i]);
      //set scale variations (only filled in data, should be applied to MC, for now FIXME inactive)
      tmp.ScaleUp().SetPtEtaPhiM(evts->Electron_pt[i],evts->Electron_eta[i],evts->Electron_phi[i],evts->Electron_mass[i]);
      tmp.ScaleDown().SetPtEtaPhiM(evts->Electron_pt[i],evts->Electron_eta[i],evts->Electron_phi[i],evts->Electron_mass[i]);

      tmp.index = i;
      tmp.charge = evts->Electron_charge[i];

      //find maxmimum pT of any variation
      double maxPt = max(tmp.Pt(), tmp.ResUp().Pt());
      maxPt = max(maxPt, tmp.ResDown().Pt());
      maxPt = max(maxPt, tmp.ScaleUp().Pt());
      maxPt = max(maxPt, tmp.ScaleDown().Pt());

      //CommonSelectionBlock
      float absEta = fabs(tmp.Eta());
      bool passCommon = (absEta < 2.4);
      passCommon &= (absEta < 1.44 || absEta > 1.57);
      passCommon &= (maxPt >= 10.);

      //TripleSelectionsBlock
      bool passVeto = (evts->Electron_cutBased[i] >= 2) && passCommon;
      passCommon &= (maxPt >= 40.); //change pT cut for non-veto electrons to be on trigger plateau
      bool passLoose = (evts->Electron_cutBased[i] >= 3) && passCommon;
      bool passPrimary = evts->Electron_cutBased_HEEP[i] && passCommon;

      tmp.IsPrimary = passPrimary;
      tmp.IsLoose = passLoose;
      tmp.IsVeto = passVeto;

      if(!passVeto && !passLoose && !passPrimary) continue;

      //check for jet overlaps
      tmp.OverlapsJet = OverlapCheck(tmp);

      //set SF and variation for primary only, HEEP as in https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaRunIIRecommendations#HEEPV7_0
      tmp.SFs = {1., 1., 1.};
      if(passPrimary && IsMC){
        TString sampleyear;
        string sy = conf->SampleYear;
        if (sy == "2016" || sy == "2016apv") sampleyear = "2016";
        else sampleyear = sy;

        if(absEta < 1.4442){
          if(sampleyear == "2016"){
            tmp.SFs[0] = 0.983;
            float unc = tmp.Et() < 90 ? 0.01 : min(1 + (tmp.Et() - 90) * 0.0022, 0.03);
            tmp.SFs[1] = 0.983 + unc;
            tmp.SFs[2] = 0.983 - unc;
          }
          else if(sampleyear == "2017"){
            tmp.SFs[0] = 0.968;
            float unc = tmp.Et() < 90 ? 0.01 : min(1 + (tmp.Et() - 90) * 0.0022, 0.03);
            tmp.SFs[1] = 0.968 + unc;
            tmp.SFs[2] = 0.968 - unc;
          }
          else if(sampleyear == "2018"){
            tmp.SFs[0] = 0.969;
            float unc = tmp.Et() < 90 ? 0.01 : min(1 + (tmp.Et() - 90) * 0.0022, 0.03);
            tmp.SFs[1] = 0.969 + unc;
            tmp.SFs[2] = 0.969 - unc;
          }
        }
        else{
          if(sampleyear == "2016"){
            tmp.SFs[0] = 0.991;
            float unc = tmp.Et() < 90 ? 0.01 : min(1 + (tmp.Et() - 90) * 0.0143, 0.04);
            tmp.SFs[1] = 0.991 + unc;
            tmp.SFs[2] = 0.991 - unc;
          }
          else if(sampleyear == "2017"){
            tmp.SFs[0] = 0.973;
            float unc = tmp.Et() < 90 ? 0.02 : min(1 + (tmp.Et() - 90) * 0.0143, 0.05);
            tmp.SFs[1] = 0.973 + unc;
            tmp.SFs[2] = 0.973 - unc;
          }
          else if(sampleyear == "2018"){
            tmp.SFs[0] = 0.984;
            float unc = tmp.Et() < 90 ? 0.02 : min(1 + (tmp.Et() - 90) * 0.0143, 0.05);
            tmp.SFs[1] = 0.984 + unc;
            tmp.SFs[2] = 0.984 - unc;
          }
        }
      }

      Electrons.push_back(tmp);
      Leptons.push_back(tmp);
    }
    for (unsigned i = 0; i < evts->nMuon; ++i) {
      Muon tmp;
      tmp.SetPtEtaPhiM(evts->Muon_pt[i],evts->Muon_eta[i],evts->Muon_phi[i],evts->Muon_mass[i]);
      tmp.index = i;
      tmp.charge = evts->Muon_charge[i];

      //Dummy for scale variations, not to be used without Rochester corrections (not compulsory)
      TLorentzVector dummy;
      dummy.SetPtEtaPhiM(evts->Muon_pt[i],evts->Muon_eta[i],evts->Muon_phi[i],evts->Muon_mass[i]);
      tmp.ResUp() = dummy;
      tmp.ResDown() = dummy;
      tmp.ScaleUp() = dummy;
      tmp.ScaleDown() = dummy;

      //CommonSelectionBlock
      float absEta = fabs(tmp.Eta());
      bool passCommon = (fabs(tmp.Eta()) < 2.4);
      passCommon &= (tmp.Pt() > 10.);
      passCommon &= (evts->Muon_pfRelIso04_all[i] < 0.25);

      //TripleSelectionBlock
      bool passVeto = (evts->Muon_looseId[i]) && passCommon;
      passCommon &= (tmp.Pt() > 35.); //change pT cut for non-veto muons to be on trigger plateau
      bool passLoose = (evts->Muon_tightId[i]) && passCommon;
      passCommon &= (evts->Muon_pfRelIso04_all[i] < 0.15); //change isolation cut for primary muons
      // bool passPrimary = (evts->Muon_highPtId[i] == 2) && passCommon;
      bool passPrimary = (evts->Muon_tightId[i]) && passCommon;

      tmp.IsPrimary = passPrimary;
      tmp.IsLoose = passLoose;
      tmp.IsVeto = passVeto;

      if(!passVeto && !passLoose && !passPrimary) continue;

      //check for jet overlaps
      tmp.OverlapsJet = OverlapCheck(tmp);

      //set SF and variation for primary only
      if(passPrimary && IsMC){
        tmp.SFs[0] = evts->Muon_scaleFactor[i];
        tmp.SFs[1] = evts->Muon_scaleFactor[i] + sqrt( pow(evts->Muon_scaleFactorStat[i],2) + pow(evts->Muon_scaleFactorSyst[i],2) );
        tmp.SFs[2] = evts->Muon_scaleFactor[i] - sqrt( pow(evts->Muon_scaleFactorStat[i],2) + pow(evts->Muon_scaleFactorSyst[i],2) );
      }
      else tmp.SFs = {1., 1., 1.};

      Muons.push_back(tmp);
      Leptons.push_back(tmp);
    }
  }

  void ReadMET() {
    Met.v() = TLorentzVector();

    //conversion of the constant SampleYear into the correction's year string where needed
    string convertedYear = "";
    if(conf->SampleYear      == "2016apv") convertedYear = "2016APV";
    else if(conf->SampleYear == "2016")    convertedYear = "2016nonAPV";
    else                                   convertedYear = conf->SampleYear;


    if(IsMC){
      TLorentzVector JESup, JESdown, JERup, JERdown;

      std::pair<double,double> METXYCorr = METXYCorr_Met_MetPhi(evts->MET_T1Smear_pt, evts->MET_T1Smear_phi, evts->run, convertedYear, IsMC, evts->PV_npvs, true, false);
      Met.SetPtEtaPhiM(METXYCorr.first, 0, METXYCorr.second, 0);

      std::pair<double,double> METXYCorr_JESup = METXYCorr_Met_MetPhi(evts->MET_T1Smear_pt_jesTotalUp, evts->MET_T1Smear_phi_jesTotalUp, evts->run, convertedYear, IsMC, evts->PV_npvs, true, false);
      JESup.SetPtEtaPhiM(METXYCorr_JESup.first, 0, METXYCorr_JESup.second, 0);
      Met.JESup() = JESup;

      std::pair<double,double> METXYCorr_JESdown = METXYCorr_Met_MetPhi(evts->MET_T1Smear_pt_jesTotalDown, evts->MET_T1Smear_phi_jesTotalDown, evts->run, convertedYear, IsMC, evts->PV_npvs, true, false);
      JESdown.SetPtEtaPhiM(METXYCorr_JESdown.first, 0, METXYCorr_JESdown.second, 0);
      Met.JESdown() = JESdown;

      std::pair<double,double> METXYCorr_JERup = METXYCorr_Met_MetPhi(evts->MET_T1Smear_pt_jerUp, evts->MET_T1Smear_phi_jerUp, evts->run, convertedYear, IsMC, evts->PV_npvs, true, false);
      JERup.SetPtEtaPhiM(METXYCorr_JERup.first, 0, METXYCorr_JERup.second, 0);
      Met.JERup() = JERup;

      std::pair<double,double> METXYCorr_JERdown = METXYCorr_Met_MetPhi(evts->MET_T1Smear_pt_jerDown, evts->MET_T1Smear_phi_jerDown, evts->run, convertedYear, IsMC, evts->PV_npvs, true, false);
      JERdown.SetPtEtaPhiM(METXYCorr_JERdown.first, 0, METXYCorr_JERdown.second, 0);
      Met.JERdown() = JERdown;
    }
    else{
      TLorentzVector dummy;

      std::pair<double,double> METXYCorr = METXYCorr_Met_MetPhi(evts->MET_T1_pt, evts->MET_T1_phi, evts->run, convertedYear, IsMC, evts->PV_npvs, true, false);
      Met.SetPtEtaPhiM(METXYCorr.first, 0, METXYCorr.second, 0);

      dummy.SetPtEtaPhiM(METXYCorr.first, 0, METXYCorr.second, 0);
      Met.JESup() = dummy;
      Met.JESdown() = dummy;
      Met.JERup() = dummy;
      Met.JERdown() = dummy;
    }
  }

  void ReadGenMET() {
    GenMet = TLorentzVector();
    GenMet.SetPtEtaPhiM(evts->GenMET_pt, 0, evts->GenMET_phi, 0);
  }

  void ReadTriggers() {
    isolated_electron_trigger = evts->isolated_electron_trigger;
    isolated_muon_trigger = evts->isolated_muon_trigger;
    isolated_muon_track_trigger = evts->isolated_muon_track_trigger;
  }

  void ReadPileup() {
    Pileup_nPU = evts->Pileup_nPU;
    Pileup_nTrueInt = evts->Pileup_nTrueInt;
  }

  void ReadVertices() {
    PV_npvs = evts->PV_npvs;
    PV_npvsGood = evts->PV_npvsGood;
  }

  bool ReadMETFilterStatus() {
    bool status = true;
    status = status && evts->Flag_goodVertices;
    status = status && evts->Flag_globalSuperTightHalo2016Filter;
    status = status && evts->Flag_HBHENoiseFilter;
    status = status && evts->Flag_HBHENoiseIsoFilter;
    status = status && evts->Flag_EcalDeadCellTriggerPrimitiveFilter;
    status = status && evts->Flag_BadPFMuonFilter;
    status = status && evts->Flag_BadPFMuonDzFilter;
    status = status && evts->Flag_eeBadScFilter;
    status = status && evts->Flag_ecalBadCalibFilter;
    // cout << "1 " << evts->Flag_goodVertices << " 2 " << evts->Flag_globalSuperTightHalo2016Filter << " 3 " << evts->Flag_HBHENoiseFilter <<endl;
    return status;
  }

  //function to determine all regions an event belongs to as a function of all object pT variations
  RegionID RegionReader(){
    RegionID Regions;

    //loop over variations: nominal, e-scale up, e-scale down, e-res up, e-res down, JES up, JES down, JER up, JER down
    for(unsigned i = 0; i < Regions.RegionCount; ++i){
      int RegionNumber = -1; //-1 no region
      //check lepton multiplicity
      int nev = 0;
      int nel = 0;
      int nep = 0;
      for(unsigned j = 0; j<Electrons.size(); ++j){
        if (Electrons[j].v(i).Pt() >= 40.) {
          nev += Leptons[j].IsVeto;
          nel += Leptons[j].IsLoose;
          nep += Leptons[j].IsPrimary;
        }
        // if(i==1 && Electrons[j].ScaleUp.Pt() >= 40.){
        //   nev += Leptons[j].IsVeto;
        //   nel += Leptons[j].IsLoose;
        //   nep += Leptons[j].IsPrimary;
        // }
        // else if(i==2 && Electrons[j].ScaleDown.Pt() >= 40.){
        //   nev += Leptons[j].IsVeto;
        //   nel += Leptons[j].IsLoose;
        //   nep += Leptons[j].IsPrimary;
        // }
        // else if(i==3 && Electrons[j].ResUp.Pt() >= 40.){
        //   nev += Electrons[j].IsVeto;
        //   nel += Electrons[j].IsLoose;
        //   nep += Electrons[j].IsPrimary;
        // }
        // else if(i==4 && Electrons[j].ResDown.Pt() >= 40.){
        //   nev += Electrons[j].IsVeto;
        //   nel += Electrons[j].IsLoose;
        //   nep += Electrons[j].IsPrimary;
        // }
        // else if(Electrons[j].Pt() >= 40.){
        //   nev += Electrons[j].IsVeto;
        //   nel += Electrons[j].IsLoose;
        //   nep += Electrons[j].IsPrimary;
        // }
      }

      int nmuv = 0;
      int nmul = 0;
      int nmup = 0;
      for(unsigned j = 0; j<Muons.size(); ++j){//no scale variations foreseen as of yet, due to not applying Rochester corrections
        nmuv += Muons[j].IsVeto;
        nmul += Muons[j].IsLoose;
        nmup += Muons[j].IsPrimary;
      }

      //Region Formats key is in DataFormat.cc
      if(nmuv + nev != 1) {
        Regions.Regions[i] = -2; // Lepton failed
        continue;
      }
      if(nmup == 1) RegionNumber = 1100;
      else if(nep == 1) RegionNumber = 2100;
      else if(nmul == 1) RegionNumber = 1200;
      else if(nel ==1) RegionNumber = 2200;

      //check trigger matching lepton flavour
      if(RegionNumber/1000 == 2 && !isolated_electron_trigger ) {
        Regions.Regions[i] = -3;
        continue; //FIXME: Needs lepton trigger matching for veracity
      }
      else if(RegionNumber/1000 == 1 && !(isolated_muon_trigger || isolated_muon_track_trigger)) {
        Regions.Regions[i] = -3;
        continue; //FIXME: Needs lepton trigger matching for veracity
      }

      //check jet multiplicity
      int nj = 0;
      int nb = 0;
      for(unsigned j = 0; j<Jets.size(); ++j){
        float pT = Jets[j].v(i).Pt();
        // float pT = Jets[j].Pt();
        // if(i==5) pT = Jets[j].JESup.Pt();
        // else if(i==6) pT = Jets[j].JESdown.Pt();
        // else if(i==7) pT = Jets[j].JERup.Pt();
        // else if(i==8) pT = Jets[j].JERdown.Pt();
        if(pT < 30.) continue;
        if(!Jets[j].PUIDpasses[PUIDWP]) continue;//select working point for PUID to none by commenting this line out, loose by PUIDpasses 0, medium by 1, tight by 2
        nj++;
        if(Jets[j].bTagPasses[bTagWP]) nb++;//select working point for b-tagging by bTagPasses[0] = loose, 1 medium and 2 tight
      }
      if(nj<5 || nj>6) {
        Regions.Regions[i] = -4;
        continue; //in no region we're interested in
      }
      RegionNumber += nj*10;
      RegionNumber += nb;
      Regions.Regions[i]=RegionNumber;
    }

    return Regions;
  }

  //function to derive event weights from SFs on objects
  vector<pair<double, string> > CalcEventSFweights() {
    vector<EventWeight> SFweights;
    //set SF weights per object
    EventWeight electronW, muonW, BjetW, PUIDW, L1PreFiringW, PUreweight;
    electronW.source = "electron";
    muonW.source = "muon";
    BjetW.source = "BjetTag";
    PUIDW.source = "PUID";
    L1PreFiringW.source = "L1PreFiring";
    PUreweight.source = "PUreweight";

    if (IsMC) {
      int modes[3]={0, +1, -1};
      for(unsigned i=0; i<3; ++i){
        for(unsigned j = 0; j < Electrons.size(); ++j) electronW.variations[i] *= Electrons[j].SFs[i];
        for(unsigned j = 0; j < Muons.size(); ++j) muonW.variations[i] *= Muons[j].SFs[i];
        for(unsigned j = 0; j < Jets.size(); ++j){
          BjetW.variations[i] *= Jets[j].bJetSFweights[i][bTagWP];
          PUIDW.variations[i] *= Jets[j].PUIDSFweights[i][PUIDWP];
        }
      }
      if (BjetW.variations[0] != BjetW.variations[0]) cout << "Caught BjetW nan" <<endl;
      // return;
      string sampleyear;
      string sy = conf->SampleYear;
      if (sy == "2016apv") sampleyear = "2016preVFP";
      else if (sy == "2016") sampleyear = "2016postVFP";
      else if (sy == "2017") sampleyear = "2017";
      else if (sy == "2018") sampleyear = "2018";
      //L1PrefiringWeight
      if(sampleyear == "2016preVFP" || sampleyear == "2016postVFP" || sampleyear == "2017"){
        L1PreFiringW.variations[0] = evts->L1PreFiringWeight_Nom;
        L1PreFiringW.variations[1] = evts->L1PreFiringWeight_Up;
        L1PreFiringW.variations[2] = evts->L1PreFiringWeight_Down;
      }

      //PUreweight
      PUreweight.variations[0] = evts->Pileup_scaleFactor;
      PUreweight.variations[1] = evts->Pileup_scaleFactorUp;
      PUreweight.variations[2] = evts->Pileup_scaleFactorDown;
    }

    SFweights.push_back(electronW);
    SFweights.push_back(muonW);
    SFweights.push_back(BjetW);
    SFweights.push_back(PUIDW);
    SFweights.push_back(L1PreFiringW);
    SFweights.push_back(PUreweight);
    //FIXME: Needs PDF weight variations and ISR/FSR

    //determine nominal event weight
    vector<pair<double, string> > EventWeight_out;
    float CentralWeight = 1.;
    for(unsigned i = 0; i < SFweights.size(); ++i){
      CentralWeight *= SFweights[i].variations[0];
    }
    EventWeight_out.push_back(make_pair(CentralWeight, "Nominal"));

    //select source for up and down variations
    for(unsigned i = 0; i < SFweights.size(); ++i){

      //create variations with strings for later combine histograms
      EventWeight_out.push_back(make_pair(CentralWeight / SFweights[i].variations[0] * SFweights[i].variations[1], SFweights[i].source + "_up"));
      EventWeight_out.push_back(make_pair(CentralWeight / SFweights[i].variations[0] * SFweights[i].variations[2], SFweights[i].source + "_down"));
    }
    return EventWeight_out;
  }

  Configs *conf;

  bTagSFReader *R_BTSF;
  PUIDSFReader *R_PUIDSF;

  bool IsMC;
  int bTagWP, PUIDWP;
  TChain* chain;
  Events* evts;
  bTagEff* bTE;
  int run, luminosityBlock;
  vector<GenPart> GenParts;
  vector<GenJet> GenJets;
  vector<Jet> Jets;
  vector<Lepton> Leptons;
  vector<Electron> Electrons;
  vector<Muon> Muons;
  MET Met;
  GenMET GenMet;

  bool isolated_electron_trigger, isolated_muon_trigger, isolated_muon_track_trigger;
  int Pileup_nPU;
  float Pileup_nTrueInt;
  int PV_npvs, PV_npvsGood;
  // nlohmann::json GoodSections;
  // bool LumiStatus;

  RegionID RegionAssociations;
  bool KeepEvent;
  vector<pair<double, string> > EventWeights;
};


#endif
