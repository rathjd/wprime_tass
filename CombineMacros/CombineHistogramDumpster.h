//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed May 31 13:29:44 2023 by ROOT version 6.22/09
// from TTree t/EventTree
// found on file: /eos/user/s/siluo/WPrimeAnalysis/Validation/2018_FL500.root
//////////////////////////////////////////////////////////

#ifndef CombineHistogramDumpster_h
#define CombineHistogramDumpster_h

#include "Dataset.cc"
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <ROOT/RVec.hxx>

// Header file for the classes stored in the TTree if any.
#include <vector>

class CombineHistogramDumpster {
public :
  TChain          *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain

  // Fixed size dimensions of array or collections stored in the TTree if any.

  // Declaration of leaf types
  ULong64_t       event;
  Int_t           RegionIdentifier[63];
  Float_t         EventWeight[37];
   Float_t         EventWeightObjectVariations[63];
   Float_t         LeptonPt;
   Float_t         LeptonPt_SU;
   Float_t         LeptonPt_SD;
   Float_t         LeptonPt_RU;
   Float_t         LeptonPt_RD;
   ROOT::VecOps::RVec<float> *JetPt;
   Float_t         METPt;
   vector<double>  *Best_Likelihood_300;
   vector<double>  *Best_Likelihood_400;
   vector<double>  *Best_Likelihood_500;
   vector<double>  *Best_Likelihood_600;
   vector<double>  *Best_Likelihood_700;
   vector<double>  *Best_Likelihood_800;
   vector<double>  *Best_Likelihood_900;
   vector<double>  *Best_Likelihood_1000;
   vector<double>  *Best_Likelihood_1100;
   vector<double>  *Best_WPrimeMass_300;
   vector<double>  *Best_WPrimeMass_400;
   vector<double>  *Best_WPrimeMass_500;
   vector<double>  *Best_WPrimeMass_600;
   vector<double>  *Best_WPrimeMass_700;
   vector<double>  *Best_WPrimeMass_800;
   vector<double>  *Best_WPrimeMass_900;
   vector<double>  *Best_WPrimeMass_1000;
   vector<double>  *Best_WPrimeMass_1100;
   vector<double>  *Best_PbTag_300;
   vector<double>  *Best_PbTag_400;
   vector<double>  *Best_PbTag_500;
   vector<double>  *Best_PbTag_600;
   vector<double>  *Best_PbTag_700;
   vector<double>  *Best_PbTag_800;
   vector<double>  *Best_PbTag_900;
   vector<double>  *Best_PbTag_1000;
   vector<double>  *Best_PbTag_1100;
   vector<vector<int> > *best_perm_300;
   vector<vector<int> > *best_perm_400;
   vector<vector<int> > *best_perm_500;
   vector<vector<int> > *best_perm_600;
   vector<vector<int> > *best_perm_700;
   vector<vector<int> > *best_perm_800;
   vector<vector<int> > *best_perm_900;
   vector<vector<int> > *best_perm_1000;
   vector<vector<int> > *best_perm_1100;
   vector<vector<double> > *best_scales_300;
   vector<vector<double> > *best_scales_400;
   vector<vector<double> > *best_scales_500;
   vector<vector<double> > *best_scales_600;
   vector<vector<double> > *best_scales_700;
   vector<vector<double> > *best_scales_800;
   vector<vector<double> > *best_scales_900;
   vector<vector<double> > *best_scales_1000;
   vector<vector<double> > *best_scales_1100;
   Float_t         leptonEta;
   Float_t         leptonEta_SU;
   Float_t         leptonEta_SD;
   Float_t         leptonEta_RU;
   Float_t         leptonEta_RD;
   Float_t         leptonPhi;
   Float_t         leptonPhi_SU;
   Float_t         leptonPhi_SD;
   Float_t         leptonPhi_RU;
   Float_t         leptonPhi_RD;
   Float_t         leptonMass;
   Float_t         leptonMass_SU;
   Float_t         leptonMass_SD;
   Float_t         leptonMass_RU;
   Float_t         leptonMass_RD;
   ROOT::VecOps::RVec<float> *jetEta;
   ROOT::VecOps::RVec<float> *jetPhi;
   ROOT::VecOps::RVec<float> *jetMass;
   Float_t         metPhi;
   Float_t         transverseMass;
   Float_t         transverseMassscaleup;
   Float_t         transverseMassscaledown;
   Int_t           numBtags;
   Float_t         metleptondelphi;
   Float_t         minLeptonJetDr;
   Int_t           PV_npvsGood;
   Float_t         transverseMassresup;
   Float_t         transverseMassresdown;
   Float_t         metleptondelphiscaleup;
   Float_t         metleptondelphiscaledown;
   Float_t         metleptondelphiresup;
   Float_t         metleptondelphiresdown;
   Float_t         minLeptonJetDrscaleup;
   Float_t         minLeptonJetDrscaledown;
   Float_t         minLeptonJetDrresup;
   Float_t         minLeptonJetDrresdown;
   ROOT::VecOps::RVec<float> *JetPtjerup;
   ROOT::VecOps::RVec<float> *jetEtajerup;
   ROOT::VecOps::RVec<float> *jetPhijerup;
   ROOT::VecOps::RVec<float> *jetMassjerup;
   Float_t         METPt_jerup;
   Float_t         metPhijerup;
   Int_t           numBtagsjerup;
   Float_t         metleptondelphijerup;
   Float_t         minLeptonJetDrjerup;
   ROOT::VecOps::RVec<float> *JetPtjerdown;
   ROOT::VecOps::RVec<float> *jetEtajerdown;
   ROOT::VecOps::RVec<float> *jetPhijerdown;
   ROOT::VecOps::RVec<float> *jetMassjerdown;
   Float_t         METPt_jerdown;
   Float_t         metPhijerdown;
   Int_t           numBtagsjerdown;
   Float_t         metleptondelphijerdown;
   Float_t         minLeptonJetDrjerdown;
   ROOT::VecOps::RVec<float> *JetPtjesAbsoluteStatup;
   ROOT::VecOps::RVec<float> *jetEtajesAbsoluteStatup;
   ROOT::VecOps::RVec<float> *jetPhijesAbsoluteStatup;
   ROOT::VecOps::RVec<float> *jetMassjesAbsoluteStatup;
   Float_t         METPt_jesAbsoluteStatup;
   Float_t         metPhijesAbsoluteStatup;
   Int_t           numBtagsjesAbsoluteStatup;
   Float_t         metleptondelphijesAbsoluteStatup;
   Float_t         minLeptonJetDrjesAbsoluteStatup;
   ROOT::VecOps::RVec<float> *JetPtjesAbsoluteStatdown;
   ROOT::VecOps::RVec<float> *jetEtajesAbsoluteStatdown;
   ROOT::VecOps::RVec<float> *jetPhijesAbsoluteStatdown;
   ROOT::VecOps::RVec<float> *jetMassjesAbsoluteStatdown;
   Float_t         METPt_jesAbsoluteStatdown;
   Float_t         metPhijesAbsoluteStatdown;
   Int_t           numBtagsjesAbsoluteStatdown;
   Float_t         metleptondelphijesAbsoluteStatdown;
   Float_t         minLeptonJetDrjesAbsoluteStatdown;
   ROOT::VecOps::RVec<float> *JetPtjesAbsoluteScaleup;
   ROOT::VecOps::RVec<float> *jetEtajesAbsoluteScaleup;
   ROOT::VecOps::RVec<float> *jetPhijesAbsoluteScaleup;
   ROOT::VecOps::RVec<float> *jetMassjesAbsoluteScaleup;
   Float_t         METPt_jesAbsoluteScaleup;
   Float_t         metPhijesAbsoluteScaleup;
   Int_t           numBtagsjesAbsoluteScaleup;
   Float_t         metleptondelphijesAbsoluteScaleup;
   Float_t         minLeptonJetDrjesAbsoluteScaleup;
   ROOT::VecOps::RVec<float> *JetPtjesAbsoluteScaledown;
   ROOT::VecOps::RVec<float> *jetEtajesAbsoluteScaledown;
   ROOT::VecOps::RVec<float> *jetPhijesAbsoluteScaledown;
   ROOT::VecOps::RVec<float> *jetMassjesAbsoluteScaledown;
   Float_t         METPt_jesAbsoluteScaledown;
   Float_t         metPhijesAbsoluteScaledown;
   Int_t           numBtagsjesAbsoluteScaledown;
   Float_t         metleptondelphijesAbsoluteScaledown;
   Float_t         minLeptonJetDrjesAbsoluteScaledown;
   ROOT::VecOps::RVec<float> *JetPtjesAbsoluteMPFBiasup;
   ROOT::VecOps::RVec<float> *jetEtajesAbsoluteMPFBiasup;
   ROOT::VecOps::RVec<float> *jetPhijesAbsoluteMPFBiasup;
   ROOT::VecOps::RVec<float> *jetMassjesAbsoluteMPFBiasup;
   Float_t         METPt_jesAbsoluteMPFBiasup;
   Float_t         metPhijesAbsoluteMPFBiasup;
   Int_t           numBtagsjesAbsoluteMPFBiasup;
   Float_t         metleptondelphijesAbsoluteMPFBiasup;
   Float_t         minLeptonJetDrjesAbsoluteMPFBiasup;
   ROOT::VecOps::RVec<float> *JetPtjesAbsoluteMPFBiasdown;
   ROOT::VecOps::RVec<float> *jetEtajesAbsoluteMPFBiasdown;
   ROOT::VecOps::RVec<float> *jetPhijesAbsoluteMPFBiasdown;
   ROOT::VecOps::RVec<float> *jetMassjesAbsoluteMPFBiasdown;
   Float_t         METPt_jesAbsoluteMPFBiasdown;
   Float_t         metPhijesAbsoluteMPFBiasdown;
   Int_t           numBtagsjesAbsoluteMPFBiasdown;
   Float_t         metleptondelphijesAbsoluteMPFBiasdown;
   Float_t         minLeptonJetDrjesAbsoluteMPFBiasdown;
   ROOT::VecOps::RVec<float> *JetPtjesFlavorQCDup;
   ROOT::VecOps::RVec<float> *jetEtajesFlavorQCDup;
   ROOT::VecOps::RVec<float> *jetPhijesFlavorQCDup;
   ROOT::VecOps::RVec<float> *jetMassjesFlavorQCDup;
   Float_t         METPt_jesFlavorQCDup;
   Float_t         metPhijesFlavorQCDup;
   Int_t           numBtagsjesFlavorQCDup;
   Float_t         metleptondelphijesFlavorQCDup;
   Float_t         minLeptonJetDrjesFlavorQCDup;
   ROOT::VecOps::RVec<float> *JetPtjesFlavorQCDdown;
   ROOT::VecOps::RVec<float> *jetEtajesFlavorQCDdown;
   ROOT::VecOps::RVec<float> *jetPhijesFlavorQCDdown;
   ROOT::VecOps::RVec<float> *jetMassjesFlavorQCDdown;
   Float_t         METPt_jesFlavorQCDdown;
   Float_t         metPhijesFlavorQCDdown;
   Int_t           numBtagsjesFlavorQCDdown;
   Float_t         metleptondelphijesFlavorQCDdown;
   Float_t         minLeptonJetDrjesFlavorQCDdown;
   ROOT::VecOps::RVec<float> *JetPtjesFragmentationup;
   ROOT::VecOps::RVec<float> *jetEtajesFragmentationup;
   ROOT::VecOps::RVec<float> *jetPhijesFragmentationup;
   ROOT::VecOps::RVec<float> *jetMassjesFragmentationup;
   Float_t         METPt_jesFragmentationup;
   Float_t         metPhijesFragmentationup;
   Int_t           numBtagsjesFragmentationup;
   Float_t         metleptondelphijesFragmentationup;
   Float_t         minLeptonJetDrjesFragmentationup;
   ROOT::VecOps::RVec<float> *JetPtjesFragmentationdown;
   ROOT::VecOps::RVec<float> *jetEtajesFragmentationdown;
   ROOT::VecOps::RVec<float> *jetPhijesFragmentationdown;
   ROOT::VecOps::RVec<float> *jetMassjesFragmentationdown;
   Float_t         METPt_jesFragmentationdown;
   Float_t         metPhijesFragmentationdown;
   Int_t           numBtagsjesFragmentationdown;
   Float_t         metleptondelphijesFragmentationdown;
   Float_t         minLeptonJetDrjesFragmentationdown;
   ROOT::VecOps::RVec<float> *JetPtjesPileUpDataMCup;
   ROOT::VecOps::RVec<float> *jetEtajesPileUpDataMCup;
   ROOT::VecOps::RVec<float> *jetPhijesPileUpDataMCup;
   ROOT::VecOps::RVec<float> *jetMassjesPileUpDataMCup;
   Float_t         METPt_jesPileUpDataMCup;
   Float_t         metPhijesPileUpDataMCup;
   Int_t           numBtagsjesPileUpDataMCup;
   Float_t         metleptondelphijesPileUpDataMCup;
   Float_t         minLeptonJetDrjesPileUpDataMCup;
   ROOT::VecOps::RVec<float> *JetPtjesPileUpDataMCdown;
   ROOT::VecOps::RVec<float> *jetEtajesPileUpDataMCdown;
   ROOT::VecOps::RVec<float> *jetPhijesPileUpDataMCdown;
   ROOT::VecOps::RVec<float> *jetMassjesPileUpDataMCdown;
   Float_t         METPt_jesPileUpDataMCdown;
   Float_t         metPhijesPileUpDataMCdown;
   Int_t           numBtagsjesPileUpDataMCdown;
   Float_t         metleptondelphijesPileUpDataMCdown;
   Float_t         minLeptonJetDrjesPileUpDataMCdown;
   ROOT::VecOps::RVec<float> *JetPtjesPileUpPtBBup;
   ROOT::VecOps::RVec<float> *jetEtajesPileUpPtBBup;
   ROOT::VecOps::RVec<float> *jetPhijesPileUpPtBBup;
   ROOT::VecOps::RVec<float> *jetMassjesPileUpPtBBup;
   Float_t         METPt_jesPileUpPtBBup;
   Float_t         metPhijesPileUpPtBBup;
   Int_t           numBtagsjesPileUpPtBBup;
   Float_t         metleptondelphijesPileUpPtBBup;
   Float_t         minLeptonJetDrjesPileUpPtBBup;
   ROOT::VecOps::RVec<float> *JetPtjesPileUpPtBBdown;
   ROOT::VecOps::RVec<float> *jetEtajesPileUpPtBBdown;
   ROOT::VecOps::RVec<float> *jetPhijesPileUpPtBBdown;
   ROOT::VecOps::RVec<float> *jetMassjesPileUpPtBBdown;
   Float_t         METPt_jesPileUpPtBBdown;
   Float_t         metPhijesPileUpPtBBdown;
   Int_t           numBtagsjesPileUpPtBBdown;
   Float_t         metleptondelphijesPileUpPtBBdown;
   Float_t         minLeptonJetDrjesPileUpPtBBdown;
   ROOT::VecOps::RVec<float> *JetPtjesPileUpPtEC1up;
   ROOT::VecOps::RVec<float> *jetEtajesPileUpPtEC1up;
   ROOT::VecOps::RVec<float> *jetPhijesPileUpPtEC1up;
   ROOT::VecOps::RVec<float> *jetMassjesPileUpPtEC1up;
   Float_t         METPt_jesPileUpPtEC1up;
   Float_t         metPhijesPileUpPtEC1up;
   Int_t           numBtagsjesPileUpPtEC1up;
   Float_t         metleptondelphijesPileUpPtEC1up;
   Float_t         minLeptonJetDrjesPileUpPtEC1up;
   ROOT::VecOps::RVec<float> *JetPtjesPileUpPtEC1down;
   ROOT::VecOps::RVec<float> *jetEtajesPileUpPtEC1down;
   ROOT::VecOps::RVec<float> *jetPhijesPileUpPtEC1down;
   ROOT::VecOps::RVec<float> *jetMassjesPileUpPtEC1down;
   Float_t         METPt_jesPileUpPtEC1down;
   Float_t         metPhijesPileUpPtEC1down;
   Int_t           numBtagsjesPileUpPtEC1down;
   Float_t         metleptondelphijesPileUpPtEC1down;
   Float_t         minLeptonJetDrjesPileUpPtEC1down;
   ROOT::VecOps::RVec<float> *JetPtjesPileUpPtEC2up;
   ROOT::VecOps::RVec<float> *jetEtajesPileUpPtEC2up;
   ROOT::VecOps::RVec<float> *jetPhijesPileUpPtEC2up;
   ROOT::VecOps::RVec<float> *jetMassjesPileUpPtEC2up;
   Float_t         METPt_jesPileUpPtEC2up;
   Float_t         metPhijesPileUpPtEC2up;
   Int_t           numBtagsjesPileUpPtEC2up;
   Float_t         metleptondelphijesPileUpPtEC2up;
   Float_t         minLeptonJetDrjesPileUpPtEC2up;
   ROOT::VecOps::RVec<float> *JetPtjesPileUpPtEC2down;
   ROOT::VecOps::RVec<float> *jetEtajesPileUpPtEC2down;
   ROOT::VecOps::RVec<float> *jetPhijesPileUpPtEC2down;
   ROOT::VecOps::RVec<float> *jetMassjesPileUpPtEC2down;
   Float_t         METPt_jesPileUpPtEC2down;
   Float_t         metPhijesPileUpPtEC2down;
   Int_t           numBtagsjesPileUpPtEC2down;
   Float_t         metleptondelphijesPileUpPtEC2down;
   Float_t         minLeptonJetDrjesPileUpPtEC2down;
   ROOT::VecOps::RVec<float> *JetPtjesPileUpPtHFup;
   ROOT::VecOps::RVec<float> *jetEtajesPileUpPtHFup;
   ROOT::VecOps::RVec<float> *jetPhijesPileUpPtHFup;
   ROOT::VecOps::RVec<float> *jetMassjesPileUpPtHFup;
   Float_t         METPt_jesPileUpPtHFup;
   Float_t         metPhijesPileUpPtHFup;
   Int_t           numBtagsjesPileUpPtHFup;
   Float_t         metleptondelphijesPileUpPtHFup;
   Float_t         minLeptonJetDrjesPileUpPtHFup;
   ROOT::VecOps::RVec<float> *JetPtjesPileUpPtHFdown;
   ROOT::VecOps::RVec<float> *jetEtajesPileUpPtHFdown;
   ROOT::VecOps::RVec<float> *jetPhijesPileUpPtHFdown;
   ROOT::VecOps::RVec<float> *jetMassjesPileUpPtHFdown;
   Float_t         METPt_jesPileUpPtHFdown;
   Float_t         metPhijesPileUpPtHFdown;
   Int_t           numBtagsjesPileUpPtHFdown;
   Float_t         metleptondelphijesPileUpPtHFdown;
   Float_t         minLeptonJetDrjesPileUpPtHFdown;
   ROOT::VecOps::RVec<float> *JetPtjesPileUpPtRefup;
   ROOT::VecOps::RVec<float> *jetEtajesPileUpPtRefup;
   ROOT::VecOps::RVec<float> *jetPhijesPileUpPtRefup;
   ROOT::VecOps::RVec<float> *jetMassjesPileUpPtRefup;
   Float_t         METPt_jesPileUpPtRefup;
   Float_t         metPhijesPileUpPtRefup;
   Int_t           numBtagsjesPileUpPtRefup;
   Float_t         metleptondelphijesPileUpPtRefup;
   Float_t         minLeptonJetDrjesPileUpPtRefup;
   ROOT::VecOps::RVec<float> *JetPtjesPileUpPtRefdown;
   ROOT::VecOps::RVec<float> *jetEtajesPileUpPtRefdown;
   ROOT::VecOps::RVec<float> *jetPhijesPileUpPtRefdown;
   ROOT::VecOps::RVec<float> *jetMassjesPileUpPtRefdown;
   Float_t         METPt_jesPileUpPtRefdown;
   Float_t         metPhijesPileUpPtRefdown;
   Int_t           numBtagsjesPileUpPtRefdown;
   Float_t         metleptondelphijesPileUpPtRefdown;
   Float_t         minLeptonJetDrjesPileUpPtRefdown;
   ROOT::VecOps::RVec<float> *JetPtjesRelativeFSRup;
   ROOT::VecOps::RVec<float> *jetEtajesRelativeFSRup;
   ROOT::VecOps::RVec<float> *jetPhijesRelativeFSRup;
   ROOT::VecOps::RVec<float> *jetMassjesRelativeFSRup;
   Float_t         METPt_jesRelativeFSRup;
   Float_t         metPhijesRelativeFSRup;
   Int_t           numBtagsjesRelativeFSRup;
   Float_t         metleptondelphijesRelativeFSRup;
   Float_t         minLeptonJetDrjesRelativeFSRup;
   ROOT::VecOps::RVec<float> *JetPtjesRelativeFSRdown;
   ROOT::VecOps::RVec<float> *jetEtajesRelativeFSRdown;
   ROOT::VecOps::RVec<float> *jetPhijesRelativeFSRdown;
   ROOT::VecOps::RVec<float> *jetMassjesRelativeFSRdown;
   Float_t         METPt_jesRelativeFSRdown;
   Float_t         metPhijesRelativeFSRdown;
   Int_t           numBtagsjesRelativeFSRdown;
   Float_t         metleptondelphijesRelativeFSRdown;
   Float_t         minLeptonJetDrjesRelativeFSRdown;
   ROOT::VecOps::RVec<float> *JetPtjesRelativeJEREC1up;
   ROOT::VecOps::RVec<float> *jetEtajesRelativeJEREC1up;
   ROOT::VecOps::RVec<float> *jetPhijesRelativeJEREC1up;
   ROOT::VecOps::RVec<float> *jetMassjesRelativeJEREC1up;
   Float_t         METPt_jesRelativeJEREC1up;
   Float_t         metPhijesRelativeJEREC1up;
   Int_t           numBtagsjesRelativeJEREC1up;
   Float_t         metleptondelphijesRelativeJEREC1up;
   Float_t         minLeptonJetDrjesRelativeJEREC1up;
   ROOT::VecOps::RVec<float> *JetPtjesRelativeJEREC1down;
   ROOT::VecOps::RVec<float> *jetEtajesRelativeJEREC1down;
   ROOT::VecOps::RVec<float> *jetPhijesRelativeJEREC1down;
   ROOT::VecOps::RVec<float> *jetMassjesRelativeJEREC1down;
   Float_t         METPt_jesRelativeJEREC1down;
   Float_t         metPhijesRelativeJEREC1down;
   Int_t           numBtagsjesRelativeJEREC1down;
   Float_t         metleptondelphijesRelativeJEREC1down;
   Float_t         minLeptonJetDrjesRelativeJEREC1down;
   ROOT::VecOps::RVec<float> *JetPtjesRelativeJEREC2up;
   ROOT::VecOps::RVec<float> *jetEtajesRelativeJEREC2up;
   ROOT::VecOps::RVec<float> *jetPhijesRelativeJEREC2up;
   ROOT::VecOps::RVec<float> *jetMassjesRelativeJEREC2up;
   Float_t         METPt_jesRelativeJEREC2up;
   Float_t         metPhijesRelativeJEREC2up;
   Int_t           numBtagsjesRelativeJEREC2up;
   Float_t         metleptondelphijesRelativeJEREC2up;
   Float_t         minLeptonJetDrjesRelativeJEREC2up;
   ROOT::VecOps::RVec<float> *JetPtjesRelativeJEREC2down;
   ROOT::VecOps::RVec<float> *jetEtajesRelativeJEREC2down;
   ROOT::VecOps::RVec<float> *jetPhijesRelativeJEREC2down;
   ROOT::VecOps::RVec<float> *jetMassjesRelativeJEREC2down;
   Float_t         METPt_jesRelativeJEREC2down;
   Float_t         metPhijesRelativeJEREC2down;
   Int_t           numBtagsjesRelativeJEREC2down;
   Float_t         metleptondelphijesRelativeJEREC2down;
   Float_t         minLeptonJetDrjesRelativeJEREC2down;
   ROOT::VecOps::RVec<float> *JetPtjesRelativeJERHFup;
   ROOT::VecOps::RVec<float> *jetEtajesRelativeJERHFup;
   ROOT::VecOps::RVec<float> *jetPhijesRelativeJERHFup;
   ROOT::VecOps::RVec<float> *jetMassjesRelativeJERHFup;
   Float_t         METPt_jesRelativeJERHFup;
   Float_t         metPhijesRelativeJERHFup;
   Int_t           numBtagsjesRelativeJERHFup;
   Float_t         metleptondelphijesRelativeJERHFup;
   Float_t         minLeptonJetDrjesRelativeJERHFup;
   ROOT::VecOps::RVec<float> *JetPtjesRelativeJERHFdown;
   ROOT::VecOps::RVec<float> *jetEtajesRelativeJERHFdown;
   ROOT::VecOps::RVec<float> *jetPhijesRelativeJERHFdown;
   ROOT::VecOps::RVec<float> *jetMassjesRelativeJERHFdown;
   Float_t         METPt_jesRelativeJERHFdown;
   Float_t         metPhijesRelativeJERHFdown;
   Int_t           numBtagsjesRelativeJERHFdown;
   Float_t         metleptondelphijesRelativeJERHFdown;
   Float_t         minLeptonJetDrjesRelativeJERHFdown;
   ROOT::VecOps::RVec<float> *JetPtjesRelativePtBBup;
   ROOT::VecOps::RVec<float> *jetEtajesRelativePtBBup;
   ROOT::VecOps::RVec<float> *jetPhijesRelativePtBBup;
   ROOT::VecOps::RVec<float> *jetMassjesRelativePtBBup;
   Float_t         METPt_jesRelativePtBBup;
   Float_t         metPhijesRelativePtBBup;
   Int_t           numBtagsjesRelativePtBBup;
   Float_t         metleptondelphijesRelativePtBBup;
   Float_t         minLeptonJetDrjesRelativePtBBup;
   ROOT::VecOps::RVec<float> *JetPtjesRelativePtBBdown;
   ROOT::VecOps::RVec<float> *jetEtajesRelativePtBBdown;
   ROOT::VecOps::RVec<float> *jetPhijesRelativePtBBdown;
   ROOT::VecOps::RVec<float> *jetMassjesRelativePtBBdown;
   Float_t         METPt_jesRelativePtBBdown;
   Float_t         metPhijesRelativePtBBdown;
   Int_t           numBtagsjesRelativePtBBdown;
   Float_t         metleptondelphijesRelativePtBBdown;
   Float_t         minLeptonJetDrjesRelativePtBBdown;
   ROOT::VecOps::RVec<float> *JetPtjesRelativePtEC1up;
   ROOT::VecOps::RVec<float> *jetEtajesRelativePtEC1up;
   ROOT::VecOps::RVec<float> *jetPhijesRelativePtEC1up;
   ROOT::VecOps::RVec<float> *jetMassjesRelativePtEC1up;
   Float_t         METPt_jesRelativePtEC1up;
   Float_t         metPhijesRelativePtEC1up;
   Int_t           numBtagsjesRelativePtEC1up;
   Float_t         metleptondelphijesRelativePtEC1up;
   Float_t         minLeptonJetDrjesRelativePtEC1up;
   ROOT::VecOps::RVec<float> *JetPtjesRelativePtEC1down;
   ROOT::VecOps::RVec<float> *jetEtajesRelativePtEC1down;
   ROOT::VecOps::RVec<float> *jetPhijesRelativePtEC1down;
   ROOT::VecOps::RVec<float> *jetMassjesRelativePtEC1down;
   Float_t         METPt_jesRelativePtEC1down;
   Float_t         metPhijesRelativePtEC1down;
   Int_t           numBtagsjesRelativePtEC1down;
   Float_t         metleptondelphijesRelativePtEC1down;
   Float_t         minLeptonJetDrjesRelativePtEC1down;
   ROOT::VecOps::RVec<float> *JetPtjesRelativePtEC2up;
   ROOT::VecOps::RVec<float> *jetEtajesRelativePtEC2up;
   ROOT::VecOps::RVec<float> *jetPhijesRelativePtEC2up;
   ROOT::VecOps::RVec<float> *jetMassjesRelativePtEC2up;
   Float_t         METPt_jesRelativePtEC2up;
   Float_t         metPhijesRelativePtEC2up;
   Int_t           numBtagsjesRelativePtEC2up;
   Float_t         metleptondelphijesRelativePtEC2up;
   Float_t         minLeptonJetDrjesRelativePtEC2up;
   ROOT::VecOps::RVec<float> *JetPtjesRelativePtEC2down;
   ROOT::VecOps::RVec<float> *jetEtajesRelativePtEC2down;
   ROOT::VecOps::RVec<float> *jetPhijesRelativePtEC2down;
   ROOT::VecOps::RVec<float> *jetMassjesRelativePtEC2down;
   Float_t         METPt_jesRelativePtEC2down;
   Float_t         metPhijesRelativePtEC2down;
   Int_t           numBtagsjesRelativePtEC2down;
   Float_t         metleptondelphijesRelativePtEC2down;
   Float_t         minLeptonJetDrjesRelativePtEC2down;
   ROOT::VecOps::RVec<float> *JetPtjesRelativePtHFup;
   ROOT::VecOps::RVec<float> *jetEtajesRelativePtHFup;
   ROOT::VecOps::RVec<float> *jetPhijesRelativePtHFup;
   ROOT::VecOps::RVec<float> *jetMassjesRelativePtHFup;
   Float_t         METPt_jesRelativePtHFup;
   Float_t         metPhijesRelativePtHFup;
   Int_t           numBtagsjesRelativePtHFup;
   Float_t         metleptondelphijesRelativePtHFup;
   Float_t         minLeptonJetDrjesRelativePtHFup;
   ROOT::VecOps::RVec<float> *JetPtjesRelativePtHFdown;
   ROOT::VecOps::RVec<float> *jetEtajesRelativePtHFdown;
   ROOT::VecOps::RVec<float> *jetPhijesRelativePtHFdown;
   ROOT::VecOps::RVec<float> *jetMassjesRelativePtHFdown;
   Float_t         METPt_jesRelativePtHFdown;
   Float_t         metPhijesRelativePtHFdown;
   Int_t           numBtagsjesRelativePtHFdown;
   Float_t         metleptondelphijesRelativePtHFdown;
   Float_t         minLeptonJetDrjesRelativePtHFdown;
   ROOT::VecOps::RVec<float> *JetPtjesRelativeBalup;
   ROOT::VecOps::RVec<float> *jetEtajesRelativeBalup;
   ROOT::VecOps::RVec<float> *jetPhijesRelativeBalup;
   ROOT::VecOps::RVec<float> *jetMassjesRelativeBalup;
   Float_t         METPt_jesRelativeBalup;
   Float_t         metPhijesRelativeBalup;
   Int_t           numBtagsjesRelativeBalup;
   Float_t         metleptondelphijesRelativeBalup;
   Float_t         minLeptonJetDrjesRelativeBalup;
   ROOT::VecOps::RVec<float> *JetPtjesRelativeBaldown;
   ROOT::VecOps::RVec<float> *jetEtajesRelativeBaldown;
   ROOT::VecOps::RVec<float> *jetPhijesRelativeBaldown;
   ROOT::VecOps::RVec<float> *jetMassjesRelativeBaldown;
   Float_t         METPt_jesRelativeBaldown;
   Float_t         metPhijesRelativeBaldown;
   Int_t           numBtagsjesRelativeBaldown;
   Float_t         metleptondelphijesRelativeBaldown;
   Float_t         minLeptonJetDrjesRelativeBaldown;
   ROOT::VecOps::RVec<float> *JetPtjesRelativeSampleup;
   ROOT::VecOps::RVec<float> *jetEtajesRelativeSampleup;
   ROOT::VecOps::RVec<float> *jetPhijesRelativeSampleup;
   ROOT::VecOps::RVec<float> *jetMassjesRelativeSampleup;
   Float_t         METPt_jesRelativeSampleup;
   Float_t         metPhijesRelativeSampleup;
   Int_t           numBtagsjesRelativeSampleup;
   Float_t         metleptondelphijesRelativeSampleup;
   Float_t         minLeptonJetDrjesRelativeSampleup;
   ROOT::VecOps::RVec<float> *JetPtjesRelativeSampledown;
   ROOT::VecOps::RVec<float> *jetEtajesRelativeSampledown;
   ROOT::VecOps::RVec<float> *jetPhijesRelativeSampledown;
   ROOT::VecOps::RVec<float> *jetMassjesRelativeSampledown;
   Float_t         METPt_jesRelativeSampledown;
   Float_t         metPhijesRelativeSampledown;
   Int_t           numBtagsjesRelativeSampledown;
   Float_t         metleptondelphijesRelativeSampledown;
   Float_t         minLeptonJetDrjesRelativeSampledown;
   ROOT::VecOps::RVec<float> *JetPtjesRelativeStatECup;
   ROOT::VecOps::RVec<float> *jetEtajesRelativeStatECup;
   ROOT::VecOps::RVec<float> *jetPhijesRelativeStatECup;
   ROOT::VecOps::RVec<float> *jetMassjesRelativeStatECup;
   Float_t         METPt_jesRelativeStatECup;
   Float_t         metPhijesRelativeStatECup;
   Int_t           numBtagsjesRelativeStatECup;
   Float_t         metleptondelphijesRelativeStatECup;
   Float_t         minLeptonJetDrjesRelativeStatECup;
   ROOT::VecOps::RVec<float> *JetPtjesRelativeStatECdown;
   ROOT::VecOps::RVec<float> *jetEtajesRelativeStatECdown;
   ROOT::VecOps::RVec<float> *jetPhijesRelativeStatECdown;
   ROOT::VecOps::RVec<float> *jetMassjesRelativeStatECdown;
   Float_t         METPt_jesRelativeStatECdown;
   Float_t         metPhijesRelativeStatECdown;
   Int_t           numBtagsjesRelativeStatECdown;
   Float_t         metleptondelphijesRelativeStatECdown;
   Float_t         minLeptonJetDrjesRelativeStatECdown;
   ROOT::VecOps::RVec<float> *JetPtjesRelativeStatFSRup;
   ROOT::VecOps::RVec<float> *jetEtajesRelativeStatFSRup;
   ROOT::VecOps::RVec<float> *jetPhijesRelativeStatFSRup;
   ROOT::VecOps::RVec<float> *jetMassjesRelativeStatFSRup;
   Float_t         METPt_jesRelativeStatFSRup;
   Float_t         metPhijesRelativeStatFSRup;
   Int_t           numBtagsjesRelativeStatFSRup;
   Float_t         metleptondelphijesRelativeStatFSRup;
   Float_t         minLeptonJetDrjesRelativeStatFSRup;
   ROOT::VecOps::RVec<float> *JetPtjesRelativeStatFSRdown;
   ROOT::VecOps::RVec<float> *jetEtajesRelativeStatFSRdown;
   ROOT::VecOps::RVec<float> *jetPhijesRelativeStatFSRdown;
   ROOT::VecOps::RVec<float> *jetMassjesRelativeStatFSRdown;
   Float_t         METPt_jesRelativeStatFSRdown;
   Float_t         metPhijesRelativeStatFSRdown;
   Int_t           numBtagsjesRelativeStatFSRdown;
   Float_t         metleptondelphijesRelativeStatFSRdown;
   Float_t         minLeptonJetDrjesRelativeStatFSRdown;
   ROOT::VecOps::RVec<float> *JetPtjesRelativeStatHFup;
   ROOT::VecOps::RVec<float> *jetEtajesRelativeStatHFup;
   ROOT::VecOps::RVec<float> *jetPhijesRelativeStatHFup;
   ROOT::VecOps::RVec<float> *jetMassjesRelativeStatHFup;
   Float_t         METPt_jesRelativeStatHFup;
   Float_t         metPhijesRelativeStatHFup;
   Int_t           numBtagsjesRelativeStatHFup;
   Float_t         metleptondelphijesRelativeStatHFup;
   Float_t         minLeptonJetDrjesRelativeStatHFup;
   ROOT::VecOps::RVec<float> *JetPtjesRelativeStatHFdown;
   ROOT::VecOps::RVec<float> *jetEtajesRelativeStatHFdown;
   ROOT::VecOps::RVec<float> *jetPhijesRelativeStatHFdown;
   ROOT::VecOps::RVec<float> *jetMassjesRelativeStatHFdown;
   Float_t         METPt_jesRelativeStatHFdown;
   Float_t         metPhijesRelativeStatHFdown;
   Int_t           numBtagsjesRelativeStatHFdown;
   Float_t         metleptondelphijesRelativeStatHFdown;
   Float_t         minLeptonJetDrjesRelativeStatHFdown;
   ROOT::VecOps::RVec<float> *JetPtjesSinglePionECALup;
   ROOT::VecOps::RVec<float> *jetEtajesSinglePionECALup;
   ROOT::VecOps::RVec<float> *jetPhijesSinglePionECALup;
   ROOT::VecOps::RVec<float> *jetMassjesSinglePionECALup;
   Float_t         METPt_jesSinglePionECALup;
   Float_t         metPhijesSinglePionECALup;
   Int_t           numBtagsjesSinglePionECALup;
   Float_t         metleptondelphijesSinglePionECALup;
   Float_t         minLeptonJetDrjesSinglePionECALup;
   ROOT::VecOps::RVec<float> *JetPtjesSinglePionECALdown;
   ROOT::VecOps::RVec<float> *jetEtajesSinglePionECALdown;
   ROOT::VecOps::RVec<float> *jetPhijesSinglePionECALdown;
   ROOT::VecOps::RVec<float> *jetMassjesSinglePionECALdown;
   Float_t         METPt_jesSinglePionECALdown;
   Float_t         metPhijesSinglePionECALdown;
   Int_t           numBtagsjesSinglePionECALdown;
   Float_t         metleptondelphijesSinglePionECALdown;
   Float_t         minLeptonJetDrjesSinglePionECALdown;
   ROOT::VecOps::RVec<float> *JetPtjesSinglePionHCALup;
   ROOT::VecOps::RVec<float> *jetEtajesSinglePionHCALup;
   ROOT::VecOps::RVec<float> *jetPhijesSinglePionHCALup;
   ROOT::VecOps::RVec<float> *jetMassjesSinglePionHCALup;
   Float_t         METPt_jesSinglePionHCALup;
   Float_t         metPhijesSinglePionHCALup;
   Int_t           numBtagsjesSinglePionHCALup;
   Float_t         metleptondelphijesSinglePionHCALup;
   Float_t         minLeptonJetDrjesSinglePionHCALup;
   ROOT::VecOps::RVec<float> *JetPtjesSinglePionHCALdown;
   ROOT::VecOps::RVec<float> *jetEtajesSinglePionHCALdown;
   ROOT::VecOps::RVec<float> *jetPhijesSinglePionHCALdown;
   ROOT::VecOps::RVec<float> *jetMassjesSinglePionHCALdown;
   Float_t         METPt_jesSinglePionHCALdown;
   Float_t         metPhijesSinglePionHCALdown;
   Int_t           numBtagsjesSinglePionHCALdown;
   Float_t         metleptondelphijesSinglePionHCALdown;
   Float_t         minLeptonJetDrjesSinglePionHCALdown;
   ROOT::VecOps::RVec<float> *JetPtjesTimePtEtaup;
   ROOT::VecOps::RVec<float> *jetEtajesTimePtEtaup;
   ROOT::VecOps::RVec<float> *jetPhijesTimePtEtaup;
   ROOT::VecOps::RVec<float> *jetMassjesTimePtEtaup;
   Float_t         METPt_jesTimePtEtaup;
   Float_t         metPhijesTimePtEtaup;
   Int_t           numBtagsjesTimePtEtaup;
   Float_t         metleptondelphijesTimePtEtaup;
   Float_t         minLeptonJetDrjesTimePtEtaup;
   ROOT::VecOps::RVec<float> *JetPtjesTimePtEtadown;
   ROOT::VecOps::RVec<float> *jetEtajesTimePtEtadown;
   ROOT::VecOps::RVec<float> *jetPhijesTimePtEtadown;
   ROOT::VecOps::RVec<float> *jetMassjesTimePtEtadown;
   Float_t         METPt_jesTimePtEtadown;
   Float_t         metPhijesTimePtEtadown;
   Int_t           numBtagsjesTimePtEtadown;
   Float_t         metleptondelphijesTimePtEtadown;
   Float_t         minLeptonJetDrjesTimePtEtadown;
   Float_t         METPt_unclustenup;
   Float_t         METPt_unclustendown;
   Float_t         metPhiunclustenup;
   Float_t         metPhiunclustendown;
   Float_t         metleptondelphiunclustenup;
   Float_t         metleptondelphiunclustendown;
   Float_t         Pileup_scaleFactor;
   Float_t         Pileup_scaleFactorUp;
   Float_t         Pileup_scaleFactorDown;
   UInt_t          nGenPart;
   Float_t         GenPart_pt[137];   //[nGenPart]
   Float_t         GenPart_eta[137];   //[nGenPart]
   Float_t         GenPart_phi[137];   //[nGenPart]
   Float_t         GenPart_mass[137];   //[nGenPart]
   Int_t           GenPart_pdgId[137];   //[nGenPart]
   Int_t           GenPart_genPartIdxMother[137];   //[nGenPart]
   UInt_t          nGenJet;
   Float_t         GenJet_pt[23];   //[nGenJet]
   Float_t         GenJet_eta[23];   //[nGenJet]
   Float_t         GenJet_phi[23];   //[nGenJet]
   Float_t         GenJet_mass[23];   //[nGenJet]
   ROOT::VecOps::RVec<int> *jetgenjetidx;

  // List of branches
   TBranch        *b_event;   //!
   TBranch        *b_RegionIdentifier;   //!
   TBranch        *b_EventWeight;   //!
   TBranch        *b_EventWeightObjectVariations;   //!
   TBranch        *b_LeptonPt;   //!
   TBranch        *b_LeptonPt_SU;   //!
   TBranch        *b_LeptonPt_SD;   //!
   TBranch        *b_LeptonPt_RU;   //!
   TBranch        *b_LeptonPt_RD;   //!
   TBranch        *b_JetPt;   //!
   TBranch        *b_METPt;   //!
   TBranch        *b_Best_Likelihood_300;   //!
   TBranch        *b_Best_Likelihood_400;   //!
   TBranch        *b_Best_Likelihood_500;   //!
   TBranch        *b_Best_Likelihood_600;   //!
   TBranch        *b_Best_Likelihood_700;   //!
   TBranch        *b_Best_Likelihood_800;   //!
   TBranch        *b_Best_Likelihood_900;   //!
   TBranch        *b_Best_Likelihood_1000;   //!
   TBranch        *b_Best_Likelihood_1100;   //!
   TBranch        *b_Best_WPrimeMass_300;   //!
   TBranch        *b_Best_WPrimeMass_400;   //!
   TBranch        *b_Best_WPrimeMass_500;   //!
   TBranch        *b_Best_WPrimeMass_600;   //!
   TBranch        *b_Best_WPrimeMass_700;   //!
   TBranch        *b_Best_WPrimeMass_800;   //!
   TBranch        *b_Best_WPrimeMass_900;   //!
   TBranch        *b_Best_WPrimeMass_1000;   //!
   TBranch        *b_Best_WPrimeMass_1100;   //!
   TBranch        *b_Best_PbTag_300;   //!
   TBranch        *b_Best_PbTag_400;   //!
   TBranch        *b_Best_PbTag_500;   //!
   TBranch        *b_Best_PbTag_600;   //!
   TBranch        *b_Best_PbTag_700;   //!
   TBranch        *b_Best_PbTag_800;   //!
   TBranch        *b_Best_PbTag_900;   //!
   TBranch        *b_Best_PbTag_1000;   //!
   TBranch        *b_Best_PbTag_1100;   //!
   TBranch        *b_best_perm_300;   //!
   TBranch        *b_best_perm_400;   //!
   TBranch        *b_best_perm_500;   //!
   TBranch        *b_best_perm_600;   //!
   TBranch        *b_best_perm_700;   //!
   TBranch        *b_best_perm_800;   //!
   TBranch        *b_best_perm_900;   //!
   TBranch        *b_best_perm_1000;   //!
   TBranch        *b_best_perm_1100;   //!
   TBranch        *b_best_scales_300;   //!
   TBranch        *b_best_scales_400;   //!
   TBranch        *b_best_scales_500;   //!
   TBranch        *b_best_scales_600;   //!
   TBranch        *b_best_scales_700;   //!
   TBranch        *b_best_scales_800;   //!
   TBranch        *b_best_scales_900;   //!
   TBranch        *b_best_scales_1000;   //!
   TBranch        *b_best_scales_1100;   //!
   TBranch        *b_leptonEta;   //!
   TBranch        *b_leptonEta_SU;   //!
   TBranch        *b_leptonEta_SD;   //!
   TBranch        *b_leptonEta_RU;   //!
   TBranch        *b_leptonEta_RD;   //!
   TBranch        *b_leptonPhi;   //!
   TBranch        *b_leptonPhi_SU;   //!
   TBranch        *b_leptonPhi_SD;   //!
   TBranch        *b_leptonPhi_RU;   //!
   TBranch        *b_leptonPhi_RD;   //!
   TBranch        *b_leptonMass;   //!
   TBranch        *b_leptonMass_SU;   //!
   TBranch        *b_leptonMass_SD;   //!
   TBranch        *b_leptonMass_RU;   //!
   TBranch        *b_leptonMass_RD;   //!
   TBranch        *b_jetEta;   //!
   TBranch        *b_jetPhi;   //!
   TBranch        *b_jetMass;   //!
   TBranch        *b_metPhi;   //!
   TBranch        *b_transverseMass;   //!
   TBranch        *b_transverseMassscaleup;   //!
   TBranch        *b_transverseMassscaledown;   //!
   TBranch        *b_numBtags;   //!
   TBranch        *b_metleptondelphi;   //!
   TBranch        *b_minLeptonJetDr;   //!
   TBranch        *b_PV_npvsGood;   //!
   TBranch        *b_transverseMassresup;   //!
   TBranch        *b_transverseMassresdown;   //!
   TBranch        *b_metleptondelphiscaleup;   //!
   TBranch        *b_metleptondelphiscaledown;   //!
   TBranch        *b_metleptondelphiresup;   //!
   TBranch        *b_metleptondelphiresdown;   //!
   TBranch        *b_minLeptonJetDrscaleup;   //!
   TBranch        *b_minLeptonJetDrscaledown;   //!
   TBranch        *b_minLeptonJetDrresup;   //!
   TBranch        *b_minLeptonJetDrresdown;   //!
   TBranch        *b_JetPtjerup;   //!
   TBranch        *b_jetEtajerup;   //!
   TBranch        *b_jetPhijerup;   //!
   TBranch        *b_jetMassjerup;   //!
   TBranch        *b_METPt_jerup;   //!
   TBranch        *b_metPhijerup;   //!
   TBranch        *b_numBtagsjerup;   //!
   TBranch        *b_metleptondelphijerup;   //!
   TBranch        *b_minLeptonJetDrjerup;   //!
   TBranch        *b_JetPtjerdown;   //!
   TBranch        *b_jetEtajerdown;   //!
   TBranch        *b_jetPhijerdown;   //!
   TBranch        *b_jetMassjerdown;   //!
   TBranch        *b_METPt_jerdown;   //!
   TBranch        *b_metPhijerdown;   //!
   TBranch        *b_numBtagsjerdown;   //!
   TBranch        *b_metleptondelphijerdown;   //!
   TBranch        *b_minLeptonJetDrjerdown;   //!
   TBranch        *b_JetPtjesAbsoluteStatup;   //!
   TBranch        *b_jetEtajesAbsoluteStatup;   //!
   TBranch        *b_jetPhijesAbsoluteStatup;   //!
   TBranch        *b_jetMassjesAbsoluteStatup;   //!
   TBranch        *b_METPt_jesAbsoluteStatup;   //!
   TBranch        *b_metPhijesAbsoluteStatup;   //!
   TBranch        *b_numBtagsjesAbsoluteStatup;   //!
   TBranch        *b_metleptondelphijesAbsoluteStatup;   //!
   TBranch        *b_minLeptonJetDrjesAbsoluteStatup;   //!
   TBranch        *b_JetPtjesAbsoluteStatdown;   //!
   TBranch        *b_jetEtajesAbsoluteStatdown;   //!
   TBranch        *b_jetPhijesAbsoluteStatdown;   //!
   TBranch        *b_jetMassjesAbsoluteStatdown;   //!
   TBranch        *b_METPt_jesAbsoluteStatdown;   //!
   TBranch        *b_metPhijesAbsoluteStatdown;   //!
   TBranch        *b_numBtagsjesAbsoluteStatdown;   //!
   TBranch        *b_metleptondelphijesAbsoluteStatdown;   //!
   TBranch        *b_minLeptonJetDrjesAbsoluteStatdown;   //!
   TBranch        *b_JetPtjesAbsoluteScaleup;   //!
   TBranch        *b_jetEtajesAbsoluteScaleup;   //!
   TBranch        *b_jetPhijesAbsoluteScaleup;   //!
   TBranch        *b_jetMassjesAbsoluteScaleup;   //!
   TBranch        *b_METPt_jesAbsoluteScaleup;   //!
   TBranch        *b_metPhijesAbsoluteScaleup;   //!
   TBranch        *b_numBtagsjesAbsoluteScaleup;   //!
   TBranch        *b_metleptondelphijesAbsoluteScaleup;   //!
   TBranch        *b_minLeptonJetDrjesAbsoluteScaleup;   //!
   TBranch        *b_JetPtjesAbsoluteScaledown;   //!
   TBranch        *b_jetEtajesAbsoluteScaledown;   //!
   TBranch        *b_jetPhijesAbsoluteScaledown;   //!
   TBranch        *b_jetMassjesAbsoluteScaledown;   //!
   TBranch        *b_METPt_jesAbsoluteScaledown;   //!
   TBranch        *b_metPhijesAbsoluteScaledown;   //!
   TBranch        *b_numBtagsjesAbsoluteScaledown;   //!
   TBranch        *b_metleptondelphijesAbsoluteScaledown;   //!
   TBranch        *b_minLeptonJetDrjesAbsoluteScaledown;   //!
   TBranch        *b_JetPtjesAbsoluteMPFBiasup;   //!
   TBranch        *b_jetEtajesAbsoluteMPFBiasup;   //!
   TBranch        *b_jetPhijesAbsoluteMPFBiasup;   //!
   TBranch        *b_jetMassjesAbsoluteMPFBiasup;   //!
   TBranch        *b_METPt_jesAbsoluteMPFBiasup;   //!
   TBranch        *b_metPhijesAbsoluteMPFBiasup;   //!
   TBranch        *b_numBtagsjesAbsoluteMPFBiasup;   //!
   TBranch        *b_metleptondelphijesAbsoluteMPFBiasup;   //!
   TBranch        *b_minLeptonJetDrjesAbsoluteMPFBiasup;   //!
   TBranch        *b_JetPtjesAbsoluteMPFBiasdown;   //!
   TBranch        *b_jetEtajesAbsoluteMPFBiasdown;   //!
   TBranch        *b_jetPhijesAbsoluteMPFBiasdown;   //!
   TBranch        *b_jetMassjesAbsoluteMPFBiasdown;   //!
   TBranch        *b_METPt_jesAbsoluteMPFBiasdown;   //!
   TBranch        *b_metPhijesAbsoluteMPFBiasdown;   //!
   TBranch        *b_numBtagsjesAbsoluteMPFBiasdown;   //!
   TBranch        *b_metleptondelphijesAbsoluteMPFBiasdown;   //!
   TBranch        *b_minLeptonJetDrjesAbsoluteMPFBiasdown;   //!
   TBranch        *b_JetPtjesFlavorQCDup;   //!
   TBranch        *b_jetEtajesFlavorQCDup;   //!
   TBranch        *b_jetPhijesFlavorQCDup;   //!
   TBranch        *b_jetMassjesFlavorQCDup;   //!
   TBranch        *b_METPt_jesFlavorQCDup;   //!
   TBranch        *b_metPhijesFlavorQCDup;   //!
   TBranch        *b_numBtagsjesFlavorQCDup;   //!
   TBranch        *b_metleptondelphijesFlavorQCDup;   //!
   TBranch        *b_minLeptonJetDrjesFlavorQCDup;   //!
   TBranch        *b_JetPtjesFlavorQCDdown;   //!
   TBranch        *b_jetEtajesFlavorQCDdown;   //!
   TBranch        *b_jetPhijesFlavorQCDdown;   //!
   TBranch        *b_jetMassjesFlavorQCDdown;   //!
   TBranch        *b_METPt_jesFlavorQCDdown;   //!
   TBranch        *b_metPhijesFlavorQCDdown;   //!
   TBranch        *b_numBtagsjesFlavorQCDdown;   //!
   TBranch        *b_metleptondelphijesFlavorQCDdown;   //!
   TBranch        *b_minLeptonJetDrjesFlavorQCDdown;   //!
   TBranch        *b_JetPtjesFragmentationup;   //!
   TBranch        *b_jetEtajesFragmentationup;   //!
   TBranch        *b_jetPhijesFragmentationup;   //!
   TBranch        *b_jetMassjesFragmentationup;   //!
   TBranch        *b_METPt_jesFragmentationup;   //!
   TBranch        *b_metPhijesFragmentationup;   //!
   TBranch        *b_numBtagsjesFragmentationup;   //!
   TBranch        *b_metleptondelphijesFragmentationup;   //!
   TBranch        *b_minLeptonJetDrjesFragmentationup;   //!
   TBranch        *b_JetPtjesFragmentationdown;   //!
   TBranch        *b_jetEtajesFragmentationdown;   //!
   TBranch        *b_jetPhijesFragmentationdown;   //!
   TBranch        *b_jetMassjesFragmentationdown;   //!
   TBranch        *b_METPt_jesFragmentationdown;   //!
   TBranch        *b_metPhijesFragmentationdown;   //!
   TBranch        *b_numBtagsjesFragmentationdown;   //!
   TBranch        *b_metleptondelphijesFragmentationdown;   //!
   TBranch        *b_minLeptonJetDrjesFragmentationdown;   //!
   TBranch        *b_JetPtjesPileUpDataMCup;   //!
   TBranch        *b_jetEtajesPileUpDataMCup;   //!
   TBranch        *b_jetPhijesPileUpDataMCup;   //!
   TBranch        *b_jetMassjesPileUpDataMCup;   //!
   TBranch        *b_METPt_jesPileUpDataMCup;   //!
   TBranch        *b_metPhijesPileUpDataMCup;   //!
   TBranch        *b_numBtagsjesPileUpDataMCup;   //!
   TBranch        *b_metleptondelphijesPileUpDataMCup;   //!
   TBranch        *b_minLeptonJetDrjesPileUpDataMCup;   //!
   TBranch        *b_JetPtjesPileUpDataMCdown;   //!
   TBranch        *b_jetEtajesPileUpDataMCdown;   //!
   TBranch        *b_jetPhijesPileUpDataMCdown;   //!
   TBranch        *b_jetMassjesPileUpDataMCdown;   //!
   TBranch        *b_METPt_jesPileUpDataMCdown;   //!
   TBranch        *b_metPhijesPileUpDataMCdown;   //!
   TBranch        *b_numBtagsjesPileUpDataMCdown;   //!
   TBranch        *b_metleptondelphijesPileUpDataMCdown;   //!
   TBranch        *b_minLeptonJetDrjesPileUpDataMCdown;   //!
   TBranch        *b_JetPtjesPileUpPtBBup;   //!
   TBranch        *b_jetEtajesPileUpPtBBup;   //!
   TBranch        *b_jetPhijesPileUpPtBBup;   //!
   TBranch        *b_jetMassjesPileUpPtBBup;   //!
   TBranch        *b_METPt_jesPileUpPtBBup;   //!
   TBranch        *b_metPhijesPileUpPtBBup;   //!
   TBranch        *b_numBtagsjesPileUpPtBBup;   //!
   TBranch        *b_metleptondelphijesPileUpPtBBup;   //!
   TBranch        *b_minLeptonJetDrjesPileUpPtBBup;   //!
   TBranch        *b_JetPtjesPileUpPtBBdown;   //!
   TBranch        *b_jetEtajesPileUpPtBBdown;   //!
   TBranch        *b_jetPhijesPileUpPtBBdown;   //!
   TBranch        *b_jetMassjesPileUpPtBBdown;   //!
   TBranch        *b_METPt_jesPileUpPtBBdown;   //!
   TBranch        *b_metPhijesPileUpPtBBdown;   //!
   TBranch        *b_numBtagsjesPileUpPtBBdown;   //!
   TBranch        *b_metleptondelphijesPileUpPtBBdown;   //!
   TBranch        *b_minLeptonJetDrjesPileUpPtBBdown;   //!
   TBranch        *b_JetPtjesPileUpPtEC1up;   //!
   TBranch        *b_jetEtajesPileUpPtEC1up;   //!
   TBranch        *b_jetPhijesPileUpPtEC1up;   //!
   TBranch        *b_jetMassjesPileUpPtEC1up;   //!
   TBranch        *b_METPt_jesPileUpPtEC1up;   //!
   TBranch        *b_metPhijesPileUpPtEC1up;   //!
   TBranch        *b_numBtagsjesPileUpPtEC1up;   //!
   TBranch        *b_metleptondelphijesPileUpPtEC1up;   //!
   TBranch        *b_minLeptonJetDrjesPileUpPtEC1up;   //!
   TBranch        *b_JetPtjesPileUpPtEC1down;   //!
   TBranch        *b_jetEtajesPileUpPtEC1down;   //!
   TBranch        *b_jetPhijesPileUpPtEC1down;   //!
   TBranch        *b_jetMassjesPileUpPtEC1down;   //!
   TBranch        *b_METPt_jesPileUpPtEC1down;   //!
   TBranch        *b_metPhijesPileUpPtEC1down;   //!
   TBranch        *b_numBtagsjesPileUpPtEC1down;   //!
   TBranch        *b_metleptondelphijesPileUpPtEC1down;   //!
   TBranch        *b_minLeptonJetDrjesPileUpPtEC1down;   //!
   TBranch        *b_JetPtjesPileUpPtEC2up;   //!
   TBranch        *b_jetEtajesPileUpPtEC2up;   //!
   TBranch        *b_jetPhijesPileUpPtEC2up;   //!
   TBranch        *b_jetMassjesPileUpPtEC2up;   //!
   TBranch        *b_METPt_jesPileUpPtEC2up;   //!
   TBranch        *b_metPhijesPileUpPtEC2up;   //!
   TBranch        *b_numBtagsjesPileUpPtEC2up;   //!
   TBranch        *b_metleptondelphijesPileUpPtEC2up;   //!
   TBranch        *b_minLeptonJetDrjesPileUpPtEC2up;   //!
   TBranch        *b_JetPtjesPileUpPtEC2down;   //!
   TBranch        *b_jetEtajesPileUpPtEC2down;   //!
   TBranch        *b_jetPhijesPileUpPtEC2down;   //!
   TBranch        *b_jetMassjesPileUpPtEC2down;   //!
   TBranch        *b_METPt_jesPileUpPtEC2down;   //!
   TBranch        *b_metPhijesPileUpPtEC2down;   //!
   TBranch        *b_numBtagsjesPileUpPtEC2down;   //!
   TBranch        *b_metleptondelphijesPileUpPtEC2down;   //!
   TBranch        *b_minLeptonJetDrjesPileUpPtEC2down;   //!
   TBranch        *b_JetPtjesPileUpPtHFup;   //!
   TBranch        *b_jetEtajesPileUpPtHFup;   //!
   TBranch        *b_jetPhijesPileUpPtHFup;   //!
   TBranch        *b_jetMassjesPileUpPtHFup;   //!
   TBranch        *b_METPt_jesPileUpPtHFup;   //!
   TBranch        *b_metPhijesPileUpPtHFup;   //!
   TBranch        *b_numBtagsjesPileUpPtHFup;   //!
   TBranch        *b_metleptondelphijesPileUpPtHFup;   //!
   TBranch        *b_minLeptonJetDrjesPileUpPtHFup;   //!
   TBranch        *b_JetPtjesPileUpPtHFdown;   //!
   TBranch        *b_jetEtajesPileUpPtHFdown;   //!
   TBranch        *b_jetPhijesPileUpPtHFdown;   //!
   TBranch        *b_jetMassjesPileUpPtHFdown;   //!
   TBranch        *b_METPt_jesPileUpPtHFdown;   //!
   TBranch        *b_metPhijesPileUpPtHFdown;   //!
   TBranch        *b_numBtagsjesPileUpPtHFdown;   //!
   TBranch        *b_metleptondelphijesPileUpPtHFdown;   //!
   TBranch        *b_minLeptonJetDrjesPileUpPtHFdown;   //!
   TBranch        *b_JetPtjesPileUpPtRefup;   //!
   TBranch        *b_jetEtajesPileUpPtRefup;   //!
   TBranch        *b_jetPhijesPileUpPtRefup;   //!
   TBranch        *b_jetMassjesPileUpPtRefup;   //!
   TBranch        *b_METPt_jesPileUpPtRefup;   //!
   TBranch        *b_metPhijesPileUpPtRefup;   //!
   TBranch        *b_numBtagsjesPileUpPtRefup;   //!
   TBranch        *b_metleptondelphijesPileUpPtRefup;   //!
   TBranch        *b_minLeptonJetDrjesPileUpPtRefup;   //!
   TBranch        *b_JetPtjesPileUpPtRefdown;   //!
   TBranch        *b_jetEtajesPileUpPtRefdown;   //!
   TBranch        *b_jetPhijesPileUpPtRefdown;   //!
   TBranch        *b_jetMassjesPileUpPtRefdown;   //!
   TBranch        *b_METPt_jesPileUpPtRefdown;   //!
   TBranch        *b_metPhijesPileUpPtRefdown;   //!
   TBranch        *b_numBtagsjesPileUpPtRefdown;   //!
   TBranch        *b_metleptondelphijesPileUpPtRefdown;   //!
   TBranch        *b_minLeptonJetDrjesPileUpPtRefdown;   //!
   TBranch        *b_JetPtjesRelativeFSRup;   //!
   TBranch        *b_jetEtajesRelativeFSRup;   //!
   TBranch        *b_jetPhijesRelativeFSRup;   //!
   TBranch        *b_jetMassjesRelativeFSRup;   //!
   TBranch        *b_METPt_jesRelativeFSRup;   //!
   TBranch        *b_metPhijesRelativeFSRup;   //!
   TBranch        *b_numBtagsjesRelativeFSRup;   //!
   TBranch        *b_metleptondelphijesRelativeFSRup;   //!
   TBranch        *b_minLeptonJetDrjesRelativeFSRup;   //!
   TBranch        *b_JetPtjesRelativeFSRdown;   //!
   TBranch        *b_jetEtajesRelativeFSRdown;   //!
   TBranch        *b_jetPhijesRelativeFSRdown;   //!
   TBranch        *b_jetMassjesRelativeFSRdown;   //!
   TBranch        *b_METPt_jesRelativeFSRdown;   //!
   TBranch        *b_metPhijesRelativeFSRdown;   //!
   TBranch        *b_numBtagsjesRelativeFSRdown;   //!
   TBranch        *b_metleptondelphijesRelativeFSRdown;   //!
   TBranch        *b_minLeptonJetDrjesRelativeFSRdown;   //!
   TBranch        *b_JetPtjesRelativeJEREC1up;   //!
   TBranch        *b_jetEtajesRelativeJEREC1up;   //!
   TBranch        *b_jetPhijesRelativeJEREC1up;   //!
   TBranch        *b_jetMassjesRelativeJEREC1up;   //!
   TBranch        *b_METPt_jesRelativeJEREC1up;   //!
   TBranch        *b_metPhijesRelativeJEREC1up;   //!
   TBranch        *b_numBtagsjesRelativeJEREC1up;   //!
   TBranch        *b_metleptondelphijesRelativeJEREC1up;   //!
   TBranch        *b_minLeptonJetDrjesRelativeJEREC1up;   //!
   TBranch        *b_JetPtjesRelativeJEREC1down;   //!
   TBranch        *b_jetEtajesRelativeJEREC1down;   //!
   TBranch        *b_jetPhijesRelativeJEREC1down;   //!
   TBranch        *b_jetMassjesRelativeJEREC1down;   //!
   TBranch        *b_METPt_jesRelativeJEREC1down;   //!
   TBranch        *b_metPhijesRelativeJEREC1down;   //!
   TBranch        *b_numBtagsjesRelativeJEREC1down;   //!
   TBranch        *b_metleptondelphijesRelativeJEREC1down;   //!
   TBranch        *b_minLeptonJetDrjesRelativeJEREC1down;   //!
   TBranch        *b_JetPtjesRelativeJEREC2up;   //!
   TBranch        *b_jetEtajesRelativeJEREC2up;   //!
   TBranch        *b_jetPhijesRelativeJEREC2up;   //!
   TBranch        *b_jetMassjesRelativeJEREC2up;   //!
   TBranch        *b_METPt_jesRelativeJEREC2up;   //!
   TBranch        *b_metPhijesRelativeJEREC2up;   //!
   TBranch        *b_numBtagsjesRelativeJEREC2up;   //!
   TBranch        *b_metleptondelphijesRelativeJEREC2up;   //!
   TBranch        *b_minLeptonJetDrjesRelativeJEREC2up;   //!
   TBranch        *b_JetPtjesRelativeJEREC2down;   //!
   TBranch        *b_jetEtajesRelativeJEREC2down;   //!
   TBranch        *b_jetPhijesRelativeJEREC2down;   //!
   TBranch        *b_jetMassjesRelativeJEREC2down;   //!
   TBranch        *b_METPt_jesRelativeJEREC2down;   //!
   TBranch        *b_metPhijesRelativeJEREC2down;   //!
   TBranch        *b_numBtagsjesRelativeJEREC2down;   //!
   TBranch        *b_metleptondelphijesRelativeJEREC2down;   //!
   TBranch        *b_minLeptonJetDrjesRelativeJEREC2down;   //!
   TBranch        *b_JetPtjesRelativeJERHFup;   //!
   TBranch        *b_jetEtajesRelativeJERHFup;   //!
   TBranch        *b_jetPhijesRelativeJERHFup;   //!
   TBranch        *b_jetMassjesRelativeJERHFup;   //!
   TBranch        *b_METPt_jesRelativeJERHFup;   //!
   TBranch        *b_metPhijesRelativeJERHFup;   //!
   TBranch        *b_numBtagsjesRelativeJERHFup;   //!
   TBranch        *b_metleptondelphijesRelativeJERHFup;   //!
   TBranch        *b_minLeptonJetDrjesRelativeJERHFup;   //!
   TBranch        *b_JetPtjesRelativeJERHFdown;   //!
   TBranch        *b_jetEtajesRelativeJERHFdown;   //!
   TBranch        *b_jetPhijesRelativeJERHFdown;   //!
   TBranch        *b_jetMassjesRelativeJERHFdown;   //!
   TBranch        *b_METPt_jesRelativeJERHFdown;   //!
   TBranch        *b_metPhijesRelativeJERHFdown;   //!
   TBranch        *b_numBtagsjesRelativeJERHFdown;   //!
   TBranch        *b_metleptondelphijesRelativeJERHFdown;   //!
   TBranch        *b_minLeptonJetDrjesRelativeJERHFdown;   //!
   TBranch        *b_JetPtjesRelativePtBBup;   //!
   TBranch        *b_jetEtajesRelativePtBBup;   //!
   TBranch        *b_jetPhijesRelativePtBBup;   //!
   TBranch        *b_jetMassjesRelativePtBBup;   //!
   TBranch        *b_METPt_jesRelativePtBBup;   //!
   TBranch        *b_metPhijesRelativePtBBup;   //!
   TBranch        *b_numBtagsjesRelativePtBBup;   //!
   TBranch        *b_metleptondelphijesRelativePtBBup;   //!
   TBranch        *b_minLeptonJetDrjesRelativePtBBup;   //!
   TBranch        *b_JetPtjesRelativePtBBdown;   //!
   TBranch        *b_jetEtajesRelativePtBBdown;   //!
   TBranch        *b_jetPhijesRelativePtBBdown;   //!
   TBranch        *b_jetMassjesRelativePtBBdown;   //!
   TBranch        *b_METPt_jesRelativePtBBdown;   //!
   TBranch        *b_metPhijesRelativePtBBdown;   //!
   TBranch        *b_numBtagsjesRelativePtBBdown;   //!
   TBranch        *b_metleptondelphijesRelativePtBBdown;   //!
   TBranch        *b_minLeptonJetDrjesRelativePtBBdown;   //!
   TBranch        *b_JetPtjesRelativePtEC1up;   //!
   TBranch        *b_jetEtajesRelativePtEC1up;   //!
   TBranch        *b_jetPhijesRelativePtEC1up;   //!
   TBranch        *b_jetMassjesRelativePtEC1up;   //!
   TBranch        *b_METPt_jesRelativePtEC1up;   //!
   TBranch        *b_metPhijesRelativePtEC1up;   //!
   TBranch        *b_numBtagsjesRelativePtEC1up;   //!
   TBranch        *b_metleptondelphijesRelativePtEC1up;   //!
   TBranch        *b_minLeptonJetDrjesRelativePtEC1up;   //!
   TBranch        *b_JetPtjesRelativePtEC1down;   //!
   TBranch        *b_jetEtajesRelativePtEC1down;   //!
   TBranch        *b_jetPhijesRelativePtEC1down;   //!
   TBranch        *b_jetMassjesRelativePtEC1down;   //!
   TBranch        *b_METPt_jesRelativePtEC1down;   //!
   TBranch        *b_metPhijesRelativePtEC1down;   //!
   TBranch        *b_numBtagsjesRelativePtEC1down;   //!
   TBranch        *b_metleptondelphijesRelativePtEC1down;   //!
   TBranch        *b_minLeptonJetDrjesRelativePtEC1down;   //!
   TBranch        *b_JetPtjesRelativePtEC2up;   //!
   TBranch        *b_jetEtajesRelativePtEC2up;   //!
   TBranch        *b_jetPhijesRelativePtEC2up;   //!
   TBranch        *b_jetMassjesRelativePtEC2up;   //!
   TBranch        *b_METPt_jesRelativePtEC2up;   //!
   TBranch        *b_metPhijesRelativePtEC2up;   //!
   TBranch        *b_numBtagsjesRelativePtEC2up;   //!
   TBranch        *b_metleptondelphijesRelativePtEC2up;   //!
   TBranch        *b_minLeptonJetDrjesRelativePtEC2up;   //!
   TBranch        *b_JetPtjesRelativePtEC2down;   //!
   TBranch        *b_jetEtajesRelativePtEC2down;   //!
   TBranch        *b_jetPhijesRelativePtEC2down;   //!
   TBranch        *b_jetMassjesRelativePtEC2down;   //!
   TBranch        *b_METPt_jesRelativePtEC2down;   //!
   TBranch        *b_metPhijesRelativePtEC2down;   //!
   TBranch        *b_numBtagsjesRelativePtEC2down;   //!
   TBranch        *b_metleptondelphijesRelativePtEC2down;   //!
   TBranch        *b_minLeptonJetDrjesRelativePtEC2down;   //!
   TBranch        *b_JetPtjesRelativePtHFup;   //!
   TBranch        *b_jetEtajesRelativePtHFup;   //!
   TBranch        *b_jetPhijesRelativePtHFup;   //!
   TBranch        *b_jetMassjesRelativePtHFup;   //!
   TBranch        *b_METPt_jesRelativePtHFup;   //!
   TBranch        *b_metPhijesRelativePtHFup;   //!
   TBranch        *b_numBtagsjesRelativePtHFup;   //!
   TBranch        *b_metleptondelphijesRelativePtHFup;   //!
   TBranch        *b_minLeptonJetDrjesRelativePtHFup;   //!
   TBranch        *b_JetPtjesRelativePtHFdown;   //!
   TBranch        *b_jetEtajesRelativePtHFdown;   //!
   TBranch        *b_jetPhijesRelativePtHFdown;   //!
   TBranch        *b_jetMassjesRelativePtHFdown;   //!
   TBranch        *b_METPt_jesRelativePtHFdown;   //!
   TBranch        *b_metPhijesRelativePtHFdown;   //!
   TBranch        *b_numBtagsjesRelativePtHFdown;   //!
   TBranch        *b_metleptondelphijesRelativePtHFdown;   //!
   TBranch        *b_minLeptonJetDrjesRelativePtHFdown;   //!
   TBranch        *b_JetPtjesRelativeBalup;   //!
   TBranch        *b_jetEtajesRelativeBalup;   //!
   TBranch        *b_jetPhijesRelativeBalup;   //!
   TBranch        *b_jetMassjesRelativeBalup;   //!
   TBranch        *b_METPt_jesRelativeBalup;   //!
   TBranch        *b_metPhijesRelativeBalup;   //!
   TBranch        *b_numBtagsjesRelativeBalup;   //!
   TBranch        *b_metleptondelphijesRelativeBalup;   //!
   TBranch        *b_minLeptonJetDrjesRelativeBalup;   //!
   TBranch        *b_JetPtjesRelativeBaldown;   //!
   TBranch        *b_jetEtajesRelativeBaldown;   //!
   TBranch        *b_jetPhijesRelativeBaldown;   //!
   TBranch        *b_jetMassjesRelativeBaldown;   //!
   TBranch        *b_METPt_jesRelativeBaldown;   //!
   TBranch        *b_metPhijesRelativeBaldown;   //!
   TBranch        *b_numBtagsjesRelativeBaldown;   //!
   TBranch        *b_metleptondelphijesRelativeBaldown;   //!
   TBranch        *b_minLeptonJetDrjesRelativeBaldown;   //!
   TBranch        *b_JetPtjesRelativeSampleup;   //!
   TBranch        *b_jetEtajesRelativeSampleup;   //!
   TBranch        *b_jetPhijesRelativeSampleup;   //!
   TBranch        *b_jetMassjesRelativeSampleup;   //!
   TBranch        *b_METPt_jesRelativeSampleup;   //!
   TBranch        *b_metPhijesRelativeSampleup;   //!
   TBranch        *b_numBtagsjesRelativeSampleup;   //!
   TBranch        *b_metleptondelphijesRelativeSampleup;   //!
   TBranch        *b_minLeptonJetDrjesRelativeSampleup;   //!
   TBranch        *b_JetPtjesRelativeSampledown;   //!
   TBranch        *b_jetEtajesRelativeSampledown;   //!
   TBranch        *b_jetPhijesRelativeSampledown;   //!
   TBranch        *b_jetMassjesRelativeSampledown;   //!
   TBranch        *b_METPt_jesRelativeSampledown;   //!
   TBranch        *b_metPhijesRelativeSampledown;   //!
   TBranch        *b_numBtagsjesRelativeSampledown;   //!
   TBranch        *b_metleptondelphijesRelativeSampledown;   //!
   TBranch        *b_minLeptonJetDrjesRelativeSampledown;   //!
   TBranch        *b_JetPtjesRelativeStatECup;   //!
   TBranch        *b_jetEtajesRelativeStatECup;   //!
   TBranch        *b_jetPhijesRelativeStatECup;   //!
   TBranch        *b_jetMassjesRelativeStatECup;   //!
   TBranch        *b_METPt_jesRelativeStatECup;   //!
   TBranch        *b_metPhijesRelativeStatECup;   //!
   TBranch        *b_numBtagsjesRelativeStatECup;   //!
   TBranch        *b_metleptondelphijesRelativeStatECup;   //!
   TBranch        *b_minLeptonJetDrjesRelativeStatECup;   //!
   TBranch        *b_JetPtjesRelativeStatECdown;   //!
   TBranch        *b_jetEtajesRelativeStatECdown;   //!
   TBranch        *b_jetPhijesRelativeStatECdown;   //!
   TBranch        *b_jetMassjesRelativeStatECdown;   //!
   TBranch        *b_METPt_jesRelativeStatECdown;   //!
   TBranch        *b_metPhijesRelativeStatECdown;   //!
   TBranch        *b_numBtagsjesRelativeStatECdown;   //!
   TBranch        *b_metleptondelphijesRelativeStatECdown;   //!
   TBranch        *b_minLeptonJetDrjesRelativeStatECdown;   //!
   TBranch        *b_JetPtjesRelativeStatFSRup;   //!
   TBranch        *b_jetEtajesRelativeStatFSRup;   //!
   TBranch        *b_jetPhijesRelativeStatFSRup;   //!
   TBranch        *b_jetMassjesRelativeStatFSRup;   //!
   TBranch        *b_METPt_jesRelativeStatFSRup;   //!
   TBranch        *b_metPhijesRelativeStatFSRup;   //!
   TBranch        *b_numBtagsjesRelativeStatFSRup;   //!
   TBranch        *b_metleptondelphijesRelativeStatFSRup;   //!
   TBranch        *b_minLeptonJetDrjesRelativeStatFSRup;   //!
   TBranch        *b_JetPtjesRelativeStatFSRdown;   //!
   TBranch        *b_jetEtajesRelativeStatFSRdown;   //!
   TBranch        *b_jetPhijesRelativeStatFSRdown;   //!
   TBranch        *b_jetMassjesRelativeStatFSRdown;   //!
   TBranch        *b_METPt_jesRelativeStatFSRdown;   //!
   TBranch        *b_metPhijesRelativeStatFSRdown;   //!
   TBranch        *b_numBtagsjesRelativeStatFSRdown;   //!
   TBranch        *b_metleptondelphijesRelativeStatFSRdown;   //!
   TBranch        *b_minLeptonJetDrjesRelativeStatFSRdown;   //!
   TBranch        *b_JetPtjesRelativeStatHFup;   //!
   TBranch        *b_jetEtajesRelativeStatHFup;   //!
   TBranch        *b_jetPhijesRelativeStatHFup;   //!
   TBranch        *b_jetMassjesRelativeStatHFup;   //!
   TBranch        *b_METPt_jesRelativeStatHFup;   //!
   TBranch        *b_metPhijesRelativeStatHFup;   //!
   TBranch        *b_numBtagsjesRelativeStatHFup;   //!
   TBranch        *b_metleptondelphijesRelativeStatHFup;   //!
   TBranch        *b_minLeptonJetDrjesRelativeStatHFup;   //!
   TBranch        *b_JetPtjesRelativeStatHFdown;   //!
   TBranch        *b_jetEtajesRelativeStatHFdown;   //!
   TBranch        *b_jetPhijesRelativeStatHFdown;   //!
   TBranch        *b_jetMassjesRelativeStatHFdown;   //!
   TBranch        *b_METPt_jesRelativeStatHFdown;   //!
   TBranch        *b_metPhijesRelativeStatHFdown;   //!
   TBranch        *b_numBtagsjesRelativeStatHFdown;   //!
   TBranch        *b_metleptondelphijesRelativeStatHFdown;   //!
   TBranch        *b_minLeptonJetDrjesRelativeStatHFdown;   //!
   TBranch        *b_JetPtjesSinglePionECALup;   //!
   TBranch        *b_jetEtajesSinglePionECALup;   //!
   TBranch        *b_jetPhijesSinglePionECALup;   //!
   TBranch        *b_jetMassjesSinglePionECALup;   //!
   TBranch        *b_METPt_jesSinglePionECALup;   //!
   TBranch        *b_metPhijesSinglePionECALup;   //!
   TBranch        *b_numBtagsjesSinglePionECALup;   //!
   TBranch        *b_metleptondelphijesSinglePionECALup;   //!
   TBranch        *b_minLeptonJetDrjesSinglePionECALup;   //!
   TBranch        *b_JetPtjesSinglePionECALdown;   //!
   TBranch        *b_jetEtajesSinglePionECALdown;   //!
   TBranch        *b_jetPhijesSinglePionECALdown;   //!
   TBranch        *b_jetMassjesSinglePionECALdown;   //!
   TBranch        *b_METPt_jesSinglePionECALdown;   //!
   TBranch        *b_metPhijesSinglePionECALdown;   //!
   TBranch        *b_numBtagsjesSinglePionECALdown;   //!
   TBranch        *b_metleptondelphijesSinglePionECALdown;   //!
   TBranch        *b_minLeptonJetDrjesSinglePionECALdown;   //!
   TBranch        *b_JetPtjesSinglePionHCALup;   //!
   TBranch        *b_jetEtajesSinglePionHCALup;   //!
   TBranch        *b_jetPhijesSinglePionHCALup;   //!
   TBranch        *b_jetMassjesSinglePionHCALup;   //!
   TBranch        *b_METPt_jesSinglePionHCALup;   //!
   TBranch        *b_metPhijesSinglePionHCALup;   //!
   TBranch        *b_numBtagsjesSinglePionHCALup;   //!
   TBranch        *b_metleptondelphijesSinglePionHCALup;   //!
   TBranch        *b_minLeptonJetDrjesSinglePionHCALup;   //!
   TBranch        *b_JetPtjesSinglePionHCALdown;   //!
   TBranch        *b_jetEtajesSinglePionHCALdown;   //!
   TBranch        *b_jetPhijesSinglePionHCALdown;   //!
   TBranch        *b_jetMassjesSinglePionHCALdown;   //!
   TBranch        *b_METPt_jesSinglePionHCALdown;   //!
   TBranch        *b_metPhijesSinglePionHCALdown;   //!
   TBranch        *b_numBtagsjesSinglePionHCALdown;   //!
   TBranch        *b_metleptondelphijesSinglePionHCALdown;   //!
   TBranch        *b_minLeptonJetDrjesSinglePionHCALdown;   //!
   TBranch        *b_JetPtjesTimePtEtaup;   //!
   TBranch        *b_jetEtajesTimePtEtaup;   //!
   TBranch        *b_jetPhijesTimePtEtaup;   //!
   TBranch        *b_jetMassjesTimePtEtaup;   //!
   TBranch        *b_METPt_jesTimePtEtaup;   //!
   TBranch        *b_metPhijesTimePtEtaup;   //!
   TBranch        *b_numBtagsjesTimePtEtaup;   //!
   TBranch        *b_metleptondelphijesTimePtEtaup;   //!
   TBranch        *b_minLeptonJetDrjesTimePtEtaup;   //!
   TBranch        *b_JetPtjesTimePtEtadown;   //!
   TBranch        *b_jetEtajesTimePtEtadown;   //!
   TBranch        *b_jetPhijesTimePtEtadown;   //!
   TBranch        *b_jetMassjesTimePtEtadown;   //!
   TBranch        *b_METPt_jesTimePtEtadown;   //!
   TBranch        *b_metPhijesTimePtEtadown;   //!
   TBranch        *b_numBtagsjesTimePtEtadown;   //!
   TBranch        *b_metleptondelphijesTimePtEtadown;   //!
   TBranch        *b_minLeptonJetDrjesTimePtEtadown;   //!
   TBranch        *b_METPt_unclustenup;   //!
   TBranch        *b_METPt_unclustendown;   //!
   TBranch        *b_metPhiunclustenup;   //!
   TBranch        *b_metPhiunclustendown;   //!
   TBranch        *b_metleptondelphiunclustenup;   //!
   TBranch        *b_metleptondelphiunclustendown;   //!
   TBranch        *b_Pileup_scaleFactor;   //!
   TBranch        *b_Pileup_scaleFactorUp;   //!
   TBranch        *b_Pileup_scaleFactorDown;   //!
   TBranch        *b_nGenPart;   //!
   TBranch        *b_GenPart_pt;   //!
   TBranch        *b_GenPart_eta;   //!
   TBranch        *b_GenPart_phi;   //!
   TBranch        *b_GenPart_mass;   //!
   TBranch        *b_GenPart_pdgId;   //!
   TBranch        *b_GenPart_genPartIdxMother;   //!
   TBranch        *b_nGenJet;   //!
   TBranch        *b_GenJet_pt;   //!
   TBranch        *b_GenJet_eta;   //!
   TBranch        *b_GenJet_phi;   //!
   TBranch        *b_GenJet_mass;   //!
   TBranch        *b_jetgenjetidx;   //!

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
  TString YearType;
  Dataset dset;
  int bin;
  int SFreg;
};

#endif

#ifdef CombineHistogramDumpster_cxx
CombineHistogramDumpster::CombineHistogramDumpster(TChain *tree, unsigned it_, int bin_, TString year_, int SFreg_) : fChain(0) 
{
  /*if(it_>39) {
    std::cout<<"iterator out of range"<<std::endl;
    return;
  }*/
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
  if (tree == 0) {
    dset = dlib.GetDataset(it_);
    TString   FilePath = "/eos/cms/store/group/phys_b2g/wprime/analyzeroutputlatest/"; //"/eos/cms/store/group/phys_b2g/wprime/analyzeroutputjune2025/";
    //if(it_ > 1) FilePath = "/eos/cms/store/group/phys_b2g/wprime/analyzeroutputaugust2025/";
    FilePath.Append(year_);
    FilePath.Append("_");
    FilePath.Append(dset.Name);
    FilePath.Append("/*.root");
    std::cout<<"processing "<<FilePath<<std::endl;
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
   Best_Likelihood_300 = 0;
   Best_Likelihood_400 = 0;
   Best_Likelihood_500 = 0;
   Best_Likelihood_600 = 0;
   Best_Likelihood_700 = 0;
   Best_Likelihood_800 = 0;
   Best_Likelihood_900 = 0;
   Best_Likelihood_1000 = 0;
   Best_Likelihood_1100 = 0;
   Best_WPrimeMass_300 = 0;
   Best_WPrimeMass_400 = 0;
   Best_WPrimeMass_500 = 0;
   Best_WPrimeMass_600 = 0;
   Best_WPrimeMass_700 = 0;
   Best_WPrimeMass_800 = 0;
   Best_WPrimeMass_900 = 0;
   Best_WPrimeMass_1000 = 0;
   Best_WPrimeMass_1100 = 0;
   Best_PbTag_300 = 0;
   Best_PbTag_400 = 0;
   Best_PbTag_500 = 0;
   Best_PbTag_600 = 0;
   Best_PbTag_700 = 0;
   Best_PbTag_800 = 0;
   Best_PbTag_900 = 0;
   Best_PbTag_1000 = 0;
   Best_PbTag_1100 = 0;
   best_perm_300 = 0;
   best_perm_400 = 0;
   best_perm_500 = 0;
   best_perm_600 = 0;
   best_perm_700 = 0;
   best_perm_800 = 0;
   best_perm_900 = 0;
   best_perm_1000 = 0;
   best_perm_1100 = 0;
   best_scales_300 = 0;
   best_scales_400 = 0;
   best_scales_500 = 0;
   best_scales_600 = 0;
   best_scales_700 = 0;
   best_scales_800 = 0;
   best_scales_900 = 0;
   best_scales_1000 = 0;
   best_scales_1100 = 0;
   jetEta = 0;
   jetPhi = 0;
   jetMass = 0;
   if(Iterator > 1){
     JetPtjerup = 0;
     jetEtajerup = 0;
     jetPhijerup = 0;
     jetMassjerup = 0;
     JetPtjerdown = 0;
     jetEtajerdown = 0;
     jetPhijerdown = 0;
     jetMassjerdown = 0;
     JetPtjesAbsoluteStatup = 0;
     jetEtajesAbsoluteStatup = 0;
     jetPhijesAbsoluteStatup = 0;
     jetMassjesAbsoluteStatup = 0;
     JetPtjesAbsoluteStatdown = 0;
     jetEtajesAbsoluteStatdown = 0;
     jetPhijesAbsoluteStatdown = 0;
     jetMassjesAbsoluteStatdown = 0;
     JetPtjesAbsoluteScaleup = 0;
     jetEtajesAbsoluteScaleup = 0;
     jetPhijesAbsoluteScaleup = 0;
     jetMassjesAbsoluteScaleup = 0;
     JetPtjesAbsoluteScaledown = 0;
     jetEtajesAbsoluteScaledown = 0;
     jetPhijesAbsoluteScaledown = 0;
     jetMassjesAbsoluteScaledown = 0;
     JetPtjesAbsoluteMPFBiasup = 0;
     jetEtajesAbsoluteMPFBiasup = 0;
     jetPhijesAbsoluteMPFBiasup = 0;
     jetMassjesAbsoluteMPFBiasup = 0;
     JetPtjesAbsoluteMPFBiasdown = 0;
     jetEtajesAbsoluteMPFBiasdown = 0;
     jetPhijesAbsoluteMPFBiasdown = 0;
     jetMassjesAbsoluteMPFBiasdown = 0;
     JetPtjesFlavorQCDup = 0;
     jetEtajesFlavorQCDup = 0;
     jetPhijesFlavorQCDup = 0;
     jetMassjesFlavorQCDup = 0;
     JetPtjesFlavorQCDdown = 0;
     jetEtajesFlavorQCDdown = 0;
     jetPhijesFlavorQCDdown = 0;
     jetMassjesFlavorQCDdown = 0;
     JetPtjesFragmentationup = 0;
     jetEtajesFragmentationup = 0;
     jetPhijesFragmentationup = 0;
     jetMassjesFragmentationup = 0;
     JetPtjesFragmentationdown = 0;
     jetEtajesFragmentationdown = 0;
     jetPhijesFragmentationdown = 0;
     jetMassjesFragmentationdown = 0;
     JetPtjesPileUpDataMCup = 0;
     jetEtajesPileUpDataMCup = 0;
     jetPhijesPileUpDataMCup = 0;
     jetMassjesPileUpDataMCup = 0;
     JetPtjesPileUpDataMCdown = 0;
     jetEtajesPileUpDataMCdown = 0;
     jetPhijesPileUpDataMCdown = 0;
     jetMassjesPileUpDataMCdown = 0;
     JetPtjesPileUpPtBBup = 0;
     jetEtajesPileUpPtBBup = 0;
     jetPhijesPileUpPtBBup = 0;
     jetMassjesPileUpPtBBup = 0;
     JetPtjesPileUpPtBBdown = 0;
     jetEtajesPileUpPtBBdown = 0;
     jetPhijesPileUpPtBBdown = 0;
     jetMassjesPileUpPtBBdown = 0;
     JetPtjesPileUpPtEC1up = 0;
     jetEtajesPileUpPtEC1up = 0;
     jetPhijesPileUpPtEC1up = 0;
     jetMassjesPileUpPtEC1up = 0;
     JetPtjesPileUpPtEC1down = 0;
     jetEtajesPileUpPtEC1down = 0;
     jetPhijesPileUpPtEC1down = 0;
     jetMassjesPileUpPtEC1down = 0;
     JetPtjesPileUpPtEC2up = 0;
     jetEtajesPileUpPtEC2up = 0;
     jetPhijesPileUpPtEC2up = 0;
     jetMassjesPileUpPtEC2up = 0;
     JetPtjesPileUpPtEC2down = 0;
     jetEtajesPileUpPtEC2down = 0;
     jetPhijesPileUpPtEC2down = 0;
     jetMassjesPileUpPtEC2down = 0;
     JetPtjesPileUpPtHFup = 0;
     jetEtajesPileUpPtHFup = 0;
     jetPhijesPileUpPtHFup = 0;
     jetMassjesPileUpPtHFup = 0;
     JetPtjesPileUpPtHFdown = 0;
     jetEtajesPileUpPtHFdown = 0;
     jetPhijesPileUpPtHFdown = 0;
     jetMassjesPileUpPtHFdown = 0;
     JetPtjesPileUpPtRefup = 0;
     jetEtajesPileUpPtRefup = 0;
     jetPhijesPileUpPtRefup = 0;
     jetMassjesPileUpPtRefup = 0;
     JetPtjesPileUpPtRefdown = 0;
     jetEtajesPileUpPtRefdown = 0;
     jetPhijesPileUpPtRefdown = 0;
     jetMassjesPileUpPtRefdown = 0;
     JetPtjesRelativeFSRup = 0;
     jetEtajesRelativeFSRup = 0;
     jetPhijesRelativeFSRup = 0;
     jetMassjesRelativeFSRup = 0;
     JetPtjesRelativeFSRdown = 0;
     jetEtajesRelativeFSRdown = 0;
     jetPhijesRelativeFSRdown = 0;
     jetMassjesRelativeFSRdown = 0;
     JetPtjesRelativeJEREC1up = 0;
     jetEtajesRelativeJEREC1up = 0;
     jetPhijesRelativeJEREC1up = 0;
     jetMassjesRelativeJEREC1up = 0;
     JetPtjesRelativeJEREC1down = 0;
     jetEtajesRelativeJEREC1down = 0;
     jetPhijesRelativeJEREC1down = 0;
     jetMassjesRelativeJEREC1down = 0;
     JetPtjesRelativeJEREC2up = 0;
     jetEtajesRelativeJEREC2up = 0;
     jetPhijesRelativeJEREC2up = 0;
     jetMassjesRelativeJEREC2up = 0;
     JetPtjesRelativeJEREC2down = 0;
     jetEtajesRelativeJEREC2down = 0;
     jetPhijesRelativeJEREC2down = 0;
     jetMassjesRelativeJEREC2down = 0;
     JetPtjesRelativeJERHFup = 0;
     jetEtajesRelativeJERHFup = 0;
     jetPhijesRelativeJERHFup = 0;
     jetMassjesRelativeJERHFup = 0;
     JetPtjesRelativeJERHFdown = 0;
     jetEtajesRelativeJERHFdown = 0;
     jetPhijesRelativeJERHFdown = 0;
     jetMassjesRelativeJERHFdown = 0;
     JetPtjesRelativePtBBup = 0;
     jetEtajesRelativePtBBup = 0;
     jetPhijesRelativePtBBup = 0;
     jetMassjesRelativePtBBup = 0;
     JetPtjesRelativePtBBdown = 0;
     jetEtajesRelativePtBBdown = 0;
     jetPhijesRelativePtBBdown = 0;
     jetMassjesRelativePtBBdown = 0;
     JetPtjesRelativePtEC1up = 0;
     jetEtajesRelativePtEC1up = 0;
     jetPhijesRelativePtEC1up = 0;
     jetMassjesRelativePtEC1up = 0;
     JetPtjesRelativePtEC1down = 0;
     jetEtajesRelativePtEC1down = 0;
     jetPhijesRelativePtEC1down = 0;
     jetMassjesRelativePtEC1down = 0;
     JetPtjesRelativePtEC2up = 0;
     jetEtajesRelativePtEC2up = 0;
     jetPhijesRelativePtEC2up = 0;
     jetMassjesRelativePtEC2up = 0;
     JetPtjesRelativePtEC2down = 0;
     jetEtajesRelativePtEC2down = 0;
     jetPhijesRelativePtEC2down = 0;
     jetMassjesRelativePtEC2down = 0;
     JetPtjesRelativePtHFup = 0;
     jetEtajesRelativePtHFup = 0;
     jetPhijesRelativePtHFup = 0;
     jetMassjesRelativePtHFup = 0;
     JetPtjesRelativePtHFdown = 0;
     jetEtajesRelativePtHFdown = 0;
     jetPhijesRelativePtHFdown = 0;
     jetMassjesRelativePtHFdown = 0;
     JetPtjesRelativeBalup = 0;
     jetEtajesRelativeBalup = 0;
     jetPhijesRelativeBalup = 0;
     jetMassjesRelativeBalup = 0;
     JetPtjesRelativeBaldown = 0;
     jetEtajesRelativeBaldown = 0;
     jetPhijesRelativeBaldown = 0;
     jetMassjesRelativeBaldown = 0;
     JetPtjesRelativeSampleup = 0;
     jetEtajesRelativeSampleup = 0;
     jetPhijesRelativeSampleup = 0;
     jetMassjesRelativeSampleup = 0;
     JetPtjesRelativeSampledown = 0;
     jetEtajesRelativeSampledown = 0;
     jetPhijesRelativeSampledown = 0;
     jetMassjesRelativeSampledown = 0;
     JetPtjesRelativeStatECup = 0;
     jetEtajesRelativeStatECup = 0;
     jetPhijesRelativeStatECup = 0;
     jetMassjesRelativeStatECup = 0;
     JetPtjesRelativeStatECdown = 0;
     jetEtajesRelativeStatECdown = 0;
     jetPhijesRelativeStatECdown = 0;
     jetMassjesRelativeStatECdown = 0;
     JetPtjesRelativeStatFSRup = 0;
     jetEtajesRelativeStatFSRup = 0;
     jetPhijesRelativeStatFSRup = 0;
     jetMassjesRelativeStatFSRup = 0;
     JetPtjesRelativeStatFSRdown = 0;
     jetEtajesRelativeStatFSRdown = 0;
     jetPhijesRelativeStatFSRdown = 0;
     jetMassjesRelativeStatFSRdown = 0;
     JetPtjesRelativeStatHFup = 0;
     jetEtajesRelativeStatHFup = 0;
     jetPhijesRelativeStatHFup = 0;
     jetMassjesRelativeStatHFup = 0;
     JetPtjesRelativeStatHFdown = 0;
     jetEtajesRelativeStatHFdown = 0;
     jetPhijesRelativeStatHFdown = 0;
     jetMassjesRelativeStatHFdown = 0;
     JetPtjesSinglePionECALup = 0;
     jetEtajesSinglePionECALup = 0;
     jetPhijesSinglePionECALup = 0;
     jetMassjesSinglePionECALup = 0;
     JetPtjesSinglePionECALdown = 0;
     jetEtajesSinglePionECALdown = 0;
     jetPhijesSinglePionECALdown = 0;
     jetMassjesSinglePionECALdown = 0;
     JetPtjesSinglePionHCALup = 0;
     jetEtajesSinglePionHCALup = 0;
     jetPhijesSinglePionHCALup = 0;
     jetMassjesSinglePionHCALup = 0;
     JetPtjesSinglePionHCALdown = 0;
     jetEtajesSinglePionHCALdown = 0;
     jetPhijesSinglePionHCALdown = 0;
     jetMassjesSinglePionHCALdown = 0;
     JetPtjesTimePtEtaup = 0;
     jetEtajesTimePtEtaup = 0;
     jetPhijesTimePtEtaup = 0;
     jetMassjesTimePtEtaup = 0;
     JetPtjesTimePtEtadown = 0;
     jetEtajesTimePtEtadown = 0;
     jetPhijesTimePtEtadown = 0;
     jetMassjesTimePtEtadown = 0;
     jetgenjetidx = 0;
   }


  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);

   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("RegionIdentifier", RegionIdentifier, &b_RegionIdentifier);
   fChain->SetBranchAddress("EventWeight", EventWeight, &b_EventWeight);
   fChain->SetBranchAddress("EventWeightObjectVariations", EventWeightObjectVariations, &b_EventWeightObjectVariations);
   fChain->SetBranchAddress("LeptonPt", &LeptonPt, &b_LeptonPt);
   fChain->SetBranchAddress("LeptonPt_SU", &LeptonPt_SU, &b_LeptonPt_SU);
   fChain->SetBranchAddress("LeptonPt_SD", &LeptonPt_SD, &b_LeptonPt_SD);
   fChain->SetBranchAddress("LeptonPt_RU", &LeptonPt_RU, &b_LeptonPt_RU);
   fChain->SetBranchAddress("LeptonPt_RD", &LeptonPt_RD, &b_LeptonPt_RD);
   fChain->SetBranchAddress("JetPt", &JetPt, &b_JetPt);
   fChain->SetBranchAddress("METPt", &METPt, &b_METPt);
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
   fChain->SetBranchAddress("best_perm_300", &best_perm_300, &b_best_perm_300);
   fChain->SetBranchAddress("best_perm_400", &best_perm_400, &b_best_perm_400);
   fChain->SetBranchAddress("best_perm_500", &best_perm_500, &b_best_perm_500);
   fChain->SetBranchAddress("best_perm_600", &best_perm_600, &b_best_perm_600);
   fChain->SetBranchAddress("best_perm_700", &best_perm_700, &b_best_perm_700);
   fChain->SetBranchAddress("best_perm_800", &best_perm_800, &b_best_perm_800);
   fChain->SetBranchAddress("best_perm_900", &best_perm_900, &b_best_perm_900);
   fChain->SetBranchAddress("best_perm_1000", &best_perm_1000, &b_best_perm_1000);
   fChain->SetBranchAddress("best_perm_1100", &best_perm_1100, &b_best_perm_1100);
   fChain->SetBranchAddress("best_scales_300", &best_scales_300, &b_best_scales_300);
   fChain->SetBranchAddress("best_scales_400", &best_scales_400, &b_best_scales_400);
   fChain->SetBranchAddress("best_scales_500", &best_scales_500, &b_best_scales_500);
   fChain->SetBranchAddress("best_scales_600", &best_scales_600, &b_best_scales_600);
   fChain->SetBranchAddress("best_scales_700", &best_scales_700, &b_best_scales_700);
   fChain->SetBranchAddress("best_scales_800", &best_scales_800, &b_best_scales_800);
   fChain->SetBranchAddress("best_scales_900", &best_scales_900, &b_best_scales_900);
   fChain->SetBranchAddress("best_scales_1000", &best_scales_1000, &b_best_scales_1000);
   fChain->SetBranchAddress("best_scales_1100", &best_scales_1100, &b_best_scales_1100);
   fChain->SetBranchAddress("leptonEta", &leptonEta, &b_leptonEta);
   fChain->SetBranchAddress("leptonEta_SU", &leptonEta_SU, &b_leptonEta_SU);
   fChain->SetBranchAddress("leptonEta_SD", &leptonEta_SD, &b_leptonEta_SD);
   fChain->SetBranchAddress("leptonPhi", &leptonPhi, &b_leptonPhi);
   fChain->SetBranchAddress("leptonPhi_SU", &leptonPhi_SU, &b_leptonPhi_SU);
   fChain->SetBranchAddress("leptonPhi_SD", &leptonPhi_SD, &b_leptonPhi_SD);
   fChain->SetBranchAddress("leptonMass", &leptonMass, &b_leptonMass);
   fChain->SetBranchAddress("leptonMass_SU", &leptonMass_SU, &b_leptonMass_SU);
   fChain->SetBranchAddress("leptonMass_SD", &leptonMass_SD, &b_leptonMass_SD);
   fChain->SetBranchAddress("jetEta", &jetEta, &b_jetEta);
   fChain->SetBranchAddress("jetPhi", &jetPhi, &b_jetPhi);
   fChain->SetBranchAddress("jetMass", &jetMass, &b_jetMass);
   fChain->SetBranchAddress("metPhi", &metPhi, &b_metPhi);
   fChain->SetBranchAddress("transverseMass", &transverseMass, &b_transverseMass);
   fChain->SetBranchAddress("transverseMassscaleup", &transverseMassscaleup, &b_transverseMassscaleup);
   fChain->SetBranchAddress("transverseMassscaledown", &transverseMassscaledown, &b_transverseMassscaledown);
   fChain->SetBranchAddress("numBtags", &numBtags, &b_numBtags);
   fChain->SetBranchAddress("metleptondelphi", &metleptondelphi, &b_metleptondelphi);
   fChain->SetBranchAddress("minLeptonJetDr", &minLeptonJetDr, &b_minLeptonJetDr);
   fChain->SetBranchAddress("PV_npvsGood", &PV_npvsGood, &b_PV_npvsGood);
   if(Iterator > 1){
     fChain->SetBranchAddress("leptonEta_RU", &leptonEta_RU, &b_leptonEta_RU);
     fChain->SetBranchAddress("leptonEta_RD", &leptonEta_RD, &b_leptonEta_RD);
     fChain->SetBranchAddress("leptonPhi_RU", &leptonPhi_RU, &b_leptonPhi_RU);
     fChain->SetBranchAddress("leptonPhi_RD", &leptonPhi_RD, &b_leptonPhi_RD);
     fChain->SetBranchAddress("leptonMass_RU", &leptonMass_RU, &b_leptonMass_RU);
     fChain->SetBranchAddress("leptonMass_RD", &leptonMass_RD, &b_leptonMass_RD);

     fChain->SetBranchAddress("transverseMassresup", &transverseMassresup, &b_transverseMassresup);
     fChain->SetBranchAddress("transverseMassresdown", &transverseMassresdown, &b_transverseMassresdown);
     fChain->SetBranchAddress("metleptondelphiscaleup", &metleptondelphiscaleup, &b_metleptondelphiscaleup);
     fChain->SetBranchAddress("metleptondelphiscaledown", &metleptondelphiscaledown, &b_metleptondelphiscaledown);
     fChain->SetBranchAddress("metleptondelphiresup", &metleptondelphiresup, &b_metleptondelphiresup);
     fChain->SetBranchAddress("metleptondelphiresdown", &metleptondelphiresdown, &b_metleptondelphiresdown);
     fChain->SetBranchAddress("minLeptonJetDrscaleup", &minLeptonJetDrscaleup, &b_minLeptonJetDrscaleup);
     fChain->SetBranchAddress("minLeptonJetDrscaledown", &minLeptonJetDrscaledown, &b_minLeptonJetDrscaledown);
     fChain->SetBranchAddress("minLeptonJetDrresup", &minLeptonJetDrresup, &b_minLeptonJetDrresup);
     fChain->SetBranchAddress("minLeptonJetDrresdown", &minLeptonJetDrresdown, &b_minLeptonJetDrresdown);
     fChain->SetBranchAddress("JetPtjerup", &JetPtjerup, &b_JetPtjerup);
     fChain->SetBranchAddress("jetEtajerup", &jetEtajerup, &b_jetEtajerup);
     fChain->SetBranchAddress("jetPhijerup", &jetPhijerup, &b_jetPhijerup);
     fChain->SetBranchAddress("jetMassjerup", &jetMassjerup, &b_jetMassjerup);
     fChain->SetBranchAddress("METPt_jerup", &METPt_jerup, &b_METPt_jerup);
     fChain->SetBranchAddress("metPhijerup", &metPhijerup, &b_metPhijerup);
     fChain->SetBranchAddress("numBtagsjerup", &numBtagsjerup, &b_numBtagsjerup);
     fChain->SetBranchAddress("metleptondelphijerup", &metleptondelphijerup, &b_metleptondelphijerup);
     fChain->SetBranchAddress("minLeptonJetDrjerup", &minLeptonJetDrjerup, &b_minLeptonJetDrjerup);
     fChain->SetBranchAddress("JetPtjerdown", &JetPtjerdown, &b_JetPtjerdown);
     fChain->SetBranchAddress("jetEtajerdown", &jetEtajerdown, &b_jetEtajerdown);
     fChain->SetBranchAddress("jetPhijerdown", &jetPhijerdown, &b_jetPhijerdown);
     fChain->SetBranchAddress("jetMassjerdown", &jetMassjerdown, &b_jetMassjerdown);
     fChain->SetBranchAddress("METPt_jerdown", &METPt_jerdown, &b_METPt_jerdown);
     fChain->SetBranchAddress("metPhijerdown", &metPhijerdown, &b_metPhijerdown);
     fChain->SetBranchAddress("numBtagsjerdown", &numBtagsjerdown, &b_numBtagsjerdown);
     fChain->SetBranchAddress("metleptondelphijerdown", &metleptondelphijerdown, &b_metleptondelphijerdown);
     fChain->SetBranchAddress("minLeptonJetDrjerdown", &minLeptonJetDrjerdown, &b_minLeptonJetDrjerdown);
     fChain->SetBranchAddress("JetPtjesAbsoluteStatup", &JetPtjesAbsoluteStatup, &b_JetPtjesAbsoluteStatup);
     fChain->SetBranchAddress("jetEtajesAbsoluteStatup", &jetEtajesAbsoluteStatup, &b_jetEtajesAbsoluteStatup);
     fChain->SetBranchAddress("jetPhijesAbsoluteStatup", &jetPhijesAbsoluteStatup, &b_jetPhijesAbsoluteStatup);
     fChain->SetBranchAddress("jetMassjesAbsoluteStatup", &jetMassjesAbsoluteStatup, &b_jetMassjesAbsoluteStatup);
     fChain->SetBranchAddress("METPt_jesAbsoluteStatup", &METPt_jesAbsoluteStatup, &b_METPt_jesAbsoluteStatup);
     fChain->SetBranchAddress("metPhijesAbsoluteStatup", &metPhijesAbsoluteStatup, &b_metPhijesAbsoluteStatup);
     fChain->SetBranchAddress("numBtagsjesAbsoluteStatup", &numBtagsjesAbsoluteStatup, &b_numBtagsjesAbsoluteStatup);
     fChain->SetBranchAddress("metleptondelphijesAbsoluteStatup", &metleptondelphijesAbsoluteStatup, &b_metleptondelphijesAbsoluteStatup);
     fChain->SetBranchAddress("minLeptonJetDrjesAbsoluteStatup", &minLeptonJetDrjesAbsoluteStatup, &b_minLeptonJetDrjesAbsoluteStatup);
     fChain->SetBranchAddress("JetPtjesAbsoluteStatdown", &JetPtjesAbsoluteStatdown, &b_JetPtjesAbsoluteStatdown);
     fChain->SetBranchAddress("jetEtajesAbsoluteStatdown", &jetEtajesAbsoluteStatdown, &b_jetEtajesAbsoluteStatdown);
     fChain->SetBranchAddress("jetPhijesAbsoluteStatdown", &jetPhijesAbsoluteStatdown, &b_jetPhijesAbsoluteStatdown);
     fChain->SetBranchAddress("jetMassjesAbsoluteStatdown", &jetMassjesAbsoluteStatdown, &b_jetMassjesAbsoluteStatdown);
     fChain->SetBranchAddress("METPt_jesAbsoluteStatdown", &METPt_jesAbsoluteStatdown, &b_METPt_jesAbsoluteStatdown);
     fChain->SetBranchAddress("metPhijesAbsoluteStatdown", &metPhijesAbsoluteStatdown, &b_metPhijesAbsoluteStatdown);
     fChain->SetBranchAddress("numBtagsjesAbsoluteStatdown", &numBtagsjesAbsoluteStatdown, &b_numBtagsjesAbsoluteStatdown);
     fChain->SetBranchAddress("metleptondelphijesAbsoluteStatdown", &metleptondelphijesAbsoluteStatdown, &b_metleptondelphijesAbsoluteStatdown);
     fChain->SetBranchAddress("minLeptonJetDrjesAbsoluteStatdown", &minLeptonJetDrjesAbsoluteStatdown, &b_minLeptonJetDrjesAbsoluteStatdown);
     fChain->SetBranchAddress("JetPtjesAbsoluteScaleup", &JetPtjesAbsoluteScaleup, &b_JetPtjesAbsoluteScaleup);
     fChain->SetBranchAddress("jetEtajesAbsoluteScaleup", &jetEtajesAbsoluteScaleup, &b_jetEtajesAbsoluteScaleup);
     fChain->SetBranchAddress("jetPhijesAbsoluteScaleup", &jetPhijesAbsoluteScaleup, &b_jetPhijesAbsoluteScaleup);
     fChain->SetBranchAddress("jetMassjesAbsoluteScaleup", &jetMassjesAbsoluteScaleup, &b_jetMassjesAbsoluteScaleup);
     fChain->SetBranchAddress("METPt_jesAbsoluteScaleup", &METPt_jesAbsoluteScaleup, &b_METPt_jesAbsoluteScaleup);
     fChain->SetBranchAddress("metPhijesAbsoluteScaleup", &metPhijesAbsoluteScaleup, &b_metPhijesAbsoluteScaleup);
     fChain->SetBranchAddress("numBtagsjesAbsoluteScaleup", &numBtagsjesAbsoluteScaleup, &b_numBtagsjesAbsoluteScaleup);
     fChain->SetBranchAddress("metleptondelphijesAbsoluteScaleup", &metleptondelphijesAbsoluteScaleup, &b_metleptondelphijesAbsoluteScaleup);
     fChain->SetBranchAddress("minLeptonJetDrjesAbsoluteScaleup", &minLeptonJetDrjesAbsoluteScaleup, &b_minLeptonJetDrjesAbsoluteScaleup);
     fChain->SetBranchAddress("JetPtjesAbsoluteScaledown", &JetPtjesAbsoluteScaledown, &b_JetPtjesAbsoluteScaledown);
     fChain->SetBranchAddress("jetEtajesAbsoluteScaledown", &jetEtajesAbsoluteScaledown, &b_jetEtajesAbsoluteScaledown);
     fChain->SetBranchAddress("jetPhijesAbsoluteScaledown", &jetPhijesAbsoluteScaledown, &b_jetPhijesAbsoluteScaledown);
     fChain->SetBranchAddress("jetMassjesAbsoluteScaledown", &jetMassjesAbsoluteScaledown, &b_jetMassjesAbsoluteScaledown);
     fChain->SetBranchAddress("METPt_jesAbsoluteScaledown", &METPt_jesAbsoluteScaledown, &b_METPt_jesAbsoluteScaledown);
     fChain->SetBranchAddress("metPhijesAbsoluteScaledown", &metPhijesAbsoluteScaledown, &b_metPhijesAbsoluteScaledown);
     fChain->SetBranchAddress("numBtagsjesAbsoluteScaledown", &numBtagsjesAbsoluteScaledown, &b_numBtagsjesAbsoluteScaledown);
     fChain->SetBranchAddress("metleptondelphijesAbsoluteScaledown", &metleptondelphijesAbsoluteScaledown, &b_metleptondelphijesAbsoluteScaledown);
     fChain->SetBranchAddress("minLeptonJetDrjesAbsoluteScaledown", &minLeptonJetDrjesAbsoluteScaledown, &b_minLeptonJetDrjesAbsoluteScaledown);
     fChain->SetBranchAddress("JetPtjesAbsoluteMPFBiasup", &JetPtjesAbsoluteMPFBiasup, &b_JetPtjesAbsoluteMPFBiasup);
     fChain->SetBranchAddress("jetEtajesAbsoluteMPFBiasup", &jetEtajesAbsoluteMPFBiasup, &b_jetEtajesAbsoluteMPFBiasup);
     fChain->SetBranchAddress("jetPhijesAbsoluteMPFBiasup", &jetPhijesAbsoluteMPFBiasup, &b_jetPhijesAbsoluteMPFBiasup);
     fChain->SetBranchAddress("jetMassjesAbsoluteMPFBiasup", &jetMassjesAbsoluteMPFBiasup, &b_jetMassjesAbsoluteMPFBiasup);
     fChain->SetBranchAddress("METPt_jesAbsoluteMPFBiasup", &METPt_jesAbsoluteMPFBiasup, &b_METPt_jesAbsoluteMPFBiasup);
     fChain->SetBranchAddress("metPhijesAbsoluteMPFBiasup", &metPhijesAbsoluteMPFBiasup, &b_metPhijesAbsoluteMPFBiasup);
     fChain->SetBranchAddress("numBtagsjesAbsoluteMPFBiasup", &numBtagsjesAbsoluteMPFBiasup, &b_numBtagsjesAbsoluteMPFBiasup);
     fChain->SetBranchAddress("metleptondelphijesAbsoluteMPFBiasup", &metleptondelphijesAbsoluteMPFBiasup, &b_metleptondelphijesAbsoluteMPFBiasup);
     fChain->SetBranchAddress("minLeptonJetDrjesAbsoluteMPFBiasup", &minLeptonJetDrjesAbsoluteMPFBiasup, &b_minLeptonJetDrjesAbsoluteMPFBiasup);
     fChain->SetBranchAddress("JetPtjesAbsoluteMPFBiasdown", &JetPtjesAbsoluteMPFBiasdown, &b_JetPtjesAbsoluteMPFBiasdown);
     fChain->SetBranchAddress("jetEtajesAbsoluteMPFBiasdown", &jetEtajesAbsoluteMPFBiasdown, &b_jetEtajesAbsoluteMPFBiasdown);
     fChain->SetBranchAddress("jetPhijesAbsoluteMPFBiasdown", &jetPhijesAbsoluteMPFBiasdown, &b_jetPhijesAbsoluteMPFBiasdown);
     fChain->SetBranchAddress("jetMassjesAbsoluteMPFBiasdown", &jetMassjesAbsoluteMPFBiasdown, &b_jetMassjesAbsoluteMPFBiasdown);
     fChain->SetBranchAddress("METPt_jesAbsoluteMPFBiasdown", &METPt_jesAbsoluteMPFBiasdown, &b_METPt_jesAbsoluteMPFBiasdown);
     fChain->SetBranchAddress("metPhijesAbsoluteMPFBiasdown", &metPhijesAbsoluteMPFBiasdown, &b_metPhijesAbsoluteMPFBiasdown);
     fChain->SetBranchAddress("numBtagsjesAbsoluteMPFBiasdown", &numBtagsjesAbsoluteMPFBiasdown, &b_numBtagsjesAbsoluteMPFBiasdown);
     fChain->SetBranchAddress("metleptondelphijesAbsoluteMPFBiasdown", &metleptondelphijesAbsoluteMPFBiasdown, &b_metleptondelphijesAbsoluteMPFBiasdown);
     fChain->SetBranchAddress("minLeptonJetDrjesAbsoluteMPFBiasdown", &minLeptonJetDrjesAbsoluteMPFBiasdown, &b_minLeptonJetDrjesAbsoluteMPFBiasdown);
     fChain->SetBranchAddress("JetPtjesFlavorQCDup", &JetPtjesFlavorQCDup, &b_JetPtjesFlavorQCDup);
     fChain->SetBranchAddress("jetEtajesFlavorQCDup", &jetEtajesFlavorQCDup, &b_jetEtajesFlavorQCDup);
     fChain->SetBranchAddress("jetPhijesFlavorQCDup", &jetPhijesFlavorQCDup, &b_jetPhijesFlavorQCDup);
     fChain->SetBranchAddress("jetMassjesFlavorQCDup", &jetMassjesFlavorQCDup, &b_jetMassjesFlavorQCDup);
     fChain->SetBranchAddress("METPt_jesFlavorQCDup", &METPt_jesFlavorQCDup, &b_METPt_jesFlavorQCDup);
     fChain->SetBranchAddress("metPhijesFlavorQCDup", &metPhijesFlavorQCDup, &b_metPhijesFlavorQCDup);
     fChain->SetBranchAddress("numBtagsjesFlavorQCDup", &numBtagsjesFlavorQCDup, &b_numBtagsjesFlavorQCDup);
     fChain->SetBranchAddress("metleptondelphijesFlavorQCDup", &metleptondelphijesFlavorQCDup, &b_metleptondelphijesFlavorQCDup);
     fChain->SetBranchAddress("minLeptonJetDrjesFlavorQCDup", &minLeptonJetDrjesFlavorQCDup, &b_minLeptonJetDrjesFlavorQCDup);
     fChain->SetBranchAddress("JetPtjesFlavorQCDdown", &JetPtjesFlavorQCDdown, &b_JetPtjesFlavorQCDdown);
     fChain->SetBranchAddress("jetEtajesFlavorQCDdown", &jetEtajesFlavorQCDdown, &b_jetEtajesFlavorQCDdown);
     fChain->SetBranchAddress("jetPhijesFlavorQCDdown", &jetPhijesFlavorQCDdown, &b_jetPhijesFlavorQCDdown);
     fChain->SetBranchAddress("jetMassjesFlavorQCDdown", &jetMassjesFlavorQCDdown, &b_jetMassjesFlavorQCDdown);
     fChain->SetBranchAddress("METPt_jesFlavorQCDdown", &METPt_jesFlavorQCDdown, &b_METPt_jesFlavorQCDdown);
     fChain->SetBranchAddress("metPhijesFlavorQCDdown", &metPhijesFlavorQCDdown, &b_metPhijesFlavorQCDdown);
     fChain->SetBranchAddress("numBtagsjesFlavorQCDdown", &numBtagsjesFlavorQCDdown, &b_numBtagsjesFlavorQCDdown);
     fChain->SetBranchAddress("metleptondelphijesFlavorQCDdown", &metleptondelphijesFlavorQCDdown, &b_metleptondelphijesFlavorQCDdown);
     fChain->SetBranchAddress("minLeptonJetDrjesFlavorQCDdown", &minLeptonJetDrjesFlavorQCDdown, &b_minLeptonJetDrjesFlavorQCDdown);
     fChain->SetBranchAddress("JetPtjesFragmentationup", &JetPtjesFragmentationup, &b_JetPtjesFragmentationup);
     fChain->SetBranchAddress("jetEtajesFragmentationup", &jetEtajesFragmentationup, &b_jetEtajesFragmentationup);
     fChain->SetBranchAddress("jetPhijesFragmentationup", &jetPhijesFragmentationup, &b_jetPhijesFragmentationup);
     fChain->SetBranchAddress("jetMassjesFragmentationup", &jetMassjesFragmentationup, &b_jetMassjesFragmentationup);
     fChain->SetBranchAddress("METPt_jesFragmentationup", &METPt_jesFragmentationup, &b_METPt_jesFragmentationup);
     fChain->SetBranchAddress("metPhijesFragmentationup", &metPhijesFragmentationup, &b_metPhijesFragmentationup);
     fChain->SetBranchAddress("numBtagsjesFragmentationup", &numBtagsjesFragmentationup, &b_numBtagsjesFragmentationup);
     fChain->SetBranchAddress("metleptondelphijesFragmentationup", &metleptondelphijesFragmentationup, &b_metleptondelphijesFragmentationup);
     fChain->SetBranchAddress("minLeptonJetDrjesFragmentationup", &minLeptonJetDrjesFragmentationup, &b_minLeptonJetDrjesFragmentationup);
     fChain->SetBranchAddress("JetPtjesFragmentationdown", &JetPtjesFragmentationdown, &b_JetPtjesFragmentationdown);
     fChain->SetBranchAddress("jetEtajesFragmentationdown", &jetEtajesFragmentationdown, &b_jetEtajesFragmentationdown);
     fChain->SetBranchAddress("jetPhijesFragmentationdown", &jetPhijesFragmentationdown, &b_jetPhijesFragmentationdown);
     fChain->SetBranchAddress("jetMassjesFragmentationdown", &jetMassjesFragmentationdown, &b_jetMassjesFragmentationdown);
     fChain->SetBranchAddress("METPt_jesFragmentationdown", &METPt_jesFragmentationdown, &b_METPt_jesFragmentationdown);
     fChain->SetBranchAddress("metPhijesFragmentationdown", &metPhijesFragmentationdown, &b_metPhijesFragmentationdown);
     fChain->SetBranchAddress("numBtagsjesFragmentationdown", &numBtagsjesFragmentationdown, &b_numBtagsjesFragmentationdown);
     fChain->SetBranchAddress("metleptondelphijesFragmentationdown", &metleptondelphijesFragmentationdown, &b_metleptondelphijesFragmentationdown);
     fChain->SetBranchAddress("minLeptonJetDrjesFragmentationdown", &minLeptonJetDrjesFragmentationdown, &b_minLeptonJetDrjesFragmentationdown);
     fChain->SetBranchAddress("JetPtjesPileUpDataMCup", &JetPtjesPileUpDataMCup, &b_JetPtjesPileUpDataMCup);
     fChain->SetBranchAddress("jetEtajesPileUpDataMCup", &jetEtajesPileUpDataMCup, &b_jetEtajesPileUpDataMCup);
     fChain->SetBranchAddress("jetPhijesPileUpDataMCup", &jetPhijesPileUpDataMCup, &b_jetPhijesPileUpDataMCup);
     fChain->SetBranchAddress("jetMassjesPileUpDataMCup", &jetMassjesPileUpDataMCup, &b_jetMassjesPileUpDataMCup);
     fChain->SetBranchAddress("METPt_jesPileUpDataMCup", &METPt_jesPileUpDataMCup, &b_METPt_jesPileUpDataMCup);
     fChain->SetBranchAddress("metPhijesPileUpDataMCup", &metPhijesPileUpDataMCup, &b_metPhijesPileUpDataMCup);
     fChain->SetBranchAddress("numBtagsjesPileUpDataMCup", &numBtagsjesPileUpDataMCup, &b_numBtagsjesPileUpDataMCup);
     fChain->SetBranchAddress("metleptondelphijesPileUpDataMCup", &metleptondelphijesPileUpDataMCup, &b_metleptondelphijesPileUpDataMCup);
     fChain->SetBranchAddress("minLeptonJetDrjesPileUpDataMCup", &minLeptonJetDrjesPileUpDataMCup, &b_minLeptonJetDrjesPileUpDataMCup);
     fChain->SetBranchAddress("JetPtjesPileUpDataMCdown", &JetPtjesPileUpDataMCdown, &b_JetPtjesPileUpDataMCdown);
     fChain->SetBranchAddress("jetEtajesPileUpDataMCdown", &jetEtajesPileUpDataMCdown, &b_jetEtajesPileUpDataMCdown);
     fChain->SetBranchAddress("jetPhijesPileUpDataMCdown", &jetPhijesPileUpDataMCdown, &b_jetPhijesPileUpDataMCdown);
     fChain->SetBranchAddress("jetMassjesPileUpDataMCdown", &jetMassjesPileUpDataMCdown, &b_jetMassjesPileUpDataMCdown);
     fChain->SetBranchAddress("METPt_jesPileUpDataMCdown", &METPt_jesPileUpDataMCdown, &b_METPt_jesPileUpDataMCdown);
     fChain->SetBranchAddress("metPhijesPileUpDataMCdown", &metPhijesPileUpDataMCdown, &b_metPhijesPileUpDataMCdown);
     fChain->SetBranchAddress("numBtagsjesPileUpDataMCdown", &numBtagsjesPileUpDataMCdown, &b_numBtagsjesPileUpDataMCdown);
     fChain->SetBranchAddress("metleptondelphijesPileUpDataMCdown", &metleptondelphijesPileUpDataMCdown, &b_metleptondelphijesPileUpDataMCdown);
     fChain->SetBranchAddress("minLeptonJetDrjesPileUpDataMCdown", &minLeptonJetDrjesPileUpDataMCdown, &b_minLeptonJetDrjesPileUpDataMCdown);
     fChain->SetBranchAddress("JetPtjesPileUpPtBBup", &JetPtjesPileUpPtBBup, &b_JetPtjesPileUpPtBBup);
     fChain->SetBranchAddress("jetEtajesPileUpPtBBup", &jetEtajesPileUpPtBBup, &b_jetEtajesPileUpPtBBup);
     fChain->SetBranchAddress("jetPhijesPileUpPtBBup", &jetPhijesPileUpPtBBup, &b_jetPhijesPileUpPtBBup);
     fChain->SetBranchAddress("jetMassjesPileUpPtBBup", &jetMassjesPileUpPtBBup, &b_jetMassjesPileUpPtBBup);
     fChain->SetBranchAddress("METPt_jesPileUpPtBBup", &METPt_jesPileUpPtBBup, &b_METPt_jesPileUpPtBBup);
     fChain->SetBranchAddress("metPhijesPileUpPtBBup", &metPhijesPileUpPtBBup, &b_metPhijesPileUpPtBBup);
     fChain->SetBranchAddress("numBtagsjesPileUpPtBBup", &numBtagsjesPileUpPtBBup, &b_numBtagsjesPileUpPtBBup);
     fChain->SetBranchAddress("metleptondelphijesPileUpPtBBup", &metleptondelphijesPileUpPtBBup, &b_metleptondelphijesPileUpPtBBup);
     fChain->SetBranchAddress("minLeptonJetDrjesPileUpPtBBup", &minLeptonJetDrjesPileUpPtBBup, &b_minLeptonJetDrjesPileUpPtBBup);
     fChain->SetBranchAddress("JetPtjesPileUpPtBBdown", &JetPtjesPileUpPtBBdown, &b_JetPtjesPileUpPtBBdown);
     fChain->SetBranchAddress("jetEtajesPileUpPtBBdown", &jetEtajesPileUpPtBBdown, &b_jetEtajesPileUpPtBBdown);
     fChain->SetBranchAddress("jetPhijesPileUpPtBBdown", &jetPhijesPileUpPtBBdown, &b_jetPhijesPileUpPtBBdown);
     fChain->SetBranchAddress("jetMassjesPileUpPtBBdown", &jetMassjesPileUpPtBBdown, &b_jetMassjesPileUpPtBBdown);
     fChain->SetBranchAddress("METPt_jesPileUpPtBBdown", &METPt_jesPileUpPtBBdown, &b_METPt_jesPileUpPtBBdown);
     fChain->SetBranchAddress("metPhijesPileUpPtBBdown", &metPhijesPileUpPtBBdown, &b_metPhijesPileUpPtBBdown);
     fChain->SetBranchAddress("numBtagsjesPileUpPtBBdown", &numBtagsjesPileUpPtBBdown, &b_numBtagsjesPileUpPtBBdown);
     fChain->SetBranchAddress("metleptondelphijesPileUpPtBBdown", &metleptondelphijesPileUpPtBBdown, &b_metleptondelphijesPileUpPtBBdown);
     fChain->SetBranchAddress("minLeptonJetDrjesPileUpPtBBdown", &minLeptonJetDrjesPileUpPtBBdown, &b_minLeptonJetDrjesPileUpPtBBdown);
     fChain->SetBranchAddress("JetPtjesPileUpPtEC1up", &JetPtjesPileUpPtEC1up, &b_JetPtjesPileUpPtEC1up);
     fChain->SetBranchAddress("jetEtajesPileUpPtEC1up", &jetEtajesPileUpPtEC1up, &b_jetEtajesPileUpPtEC1up);
     fChain->SetBranchAddress("jetPhijesPileUpPtEC1up", &jetPhijesPileUpPtEC1up, &b_jetPhijesPileUpPtEC1up);
     fChain->SetBranchAddress("jetMassjesPileUpPtEC1up", &jetMassjesPileUpPtEC1up, &b_jetMassjesPileUpPtEC1up);
     fChain->SetBranchAddress("METPt_jesPileUpPtEC1up", &METPt_jesPileUpPtEC1up, &b_METPt_jesPileUpPtEC1up);
     fChain->SetBranchAddress("metPhijesPileUpPtEC1up", &metPhijesPileUpPtEC1up, &b_metPhijesPileUpPtEC1up);
     fChain->SetBranchAddress("numBtagsjesPileUpPtEC1up", &numBtagsjesPileUpPtEC1up, &b_numBtagsjesPileUpPtEC1up);
     fChain->SetBranchAddress("metleptondelphijesPileUpPtEC1up", &metleptondelphijesPileUpPtEC1up, &b_metleptondelphijesPileUpPtEC1up);
     fChain->SetBranchAddress("minLeptonJetDrjesPileUpPtEC1up", &minLeptonJetDrjesPileUpPtEC1up, &b_minLeptonJetDrjesPileUpPtEC1up);
     fChain->SetBranchAddress("JetPtjesPileUpPtEC1down", &JetPtjesPileUpPtEC1down, &b_JetPtjesPileUpPtEC1down);
     fChain->SetBranchAddress("jetEtajesPileUpPtEC1down", &jetEtajesPileUpPtEC1down, &b_jetEtajesPileUpPtEC1down);
     fChain->SetBranchAddress("jetPhijesPileUpPtEC1down", &jetPhijesPileUpPtEC1down, &b_jetPhijesPileUpPtEC1down);
     fChain->SetBranchAddress("jetMassjesPileUpPtEC1down", &jetMassjesPileUpPtEC1down, &b_jetMassjesPileUpPtEC1down);
     fChain->SetBranchAddress("METPt_jesPileUpPtEC1down", &METPt_jesPileUpPtEC1down, &b_METPt_jesPileUpPtEC1down);
     fChain->SetBranchAddress("metPhijesPileUpPtEC1down", &metPhijesPileUpPtEC1down, &b_metPhijesPileUpPtEC1down);
     fChain->SetBranchAddress("numBtagsjesPileUpPtEC1down", &numBtagsjesPileUpPtEC1down, &b_numBtagsjesPileUpPtEC1down);
     fChain->SetBranchAddress("metleptondelphijesPileUpPtEC1down", &metleptondelphijesPileUpPtEC1down, &b_metleptondelphijesPileUpPtEC1down);
     fChain->SetBranchAddress("minLeptonJetDrjesPileUpPtEC1down", &minLeptonJetDrjesPileUpPtEC1down, &b_minLeptonJetDrjesPileUpPtEC1down);
     fChain->SetBranchAddress("JetPtjesPileUpPtEC2up", &JetPtjesPileUpPtEC2up, &b_JetPtjesPileUpPtEC2up);
     fChain->SetBranchAddress("jetEtajesPileUpPtEC2up", &jetEtajesPileUpPtEC2up, &b_jetEtajesPileUpPtEC2up);
     fChain->SetBranchAddress("jetPhijesPileUpPtEC2up", &jetPhijesPileUpPtEC2up, &b_jetPhijesPileUpPtEC2up);
     fChain->SetBranchAddress("jetMassjesPileUpPtEC2up", &jetMassjesPileUpPtEC2up, &b_jetMassjesPileUpPtEC2up);
     fChain->SetBranchAddress("METPt_jesPileUpPtEC2up", &METPt_jesPileUpPtEC2up, &b_METPt_jesPileUpPtEC2up);
     fChain->SetBranchAddress("metPhijesPileUpPtEC2up", &metPhijesPileUpPtEC2up, &b_metPhijesPileUpPtEC2up);
     fChain->SetBranchAddress("numBtagsjesPileUpPtEC2up", &numBtagsjesPileUpPtEC2up, &b_numBtagsjesPileUpPtEC2up);
     fChain->SetBranchAddress("metleptondelphijesPileUpPtEC2up", &metleptondelphijesPileUpPtEC2up, &b_metleptondelphijesPileUpPtEC2up);
     fChain->SetBranchAddress("minLeptonJetDrjesPileUpPtEC2up", &minLeptonJetDrjesPileUpPtEC2up, &b_minLeptonJetDrjesPileUpPtEC2up);
     fChain->SetBranchAddress("JetPtjesPileUpPtEC2down", &JetPtjesPileUpPtEC2down, &b_JetPtjesPileUpPtEC2down);
     fChain->SetBranchAddress("jetEtajesPileUpPtEC2down", &jetEtajesPileUpPtEC2down, &b_jetEtajesPileUpPtEC2down);
     fChain->SetBranchAddress("jetPhijesPileUpPtEC2down", &jetPhijesPileUpPtEC2down, &b_jetPhijesPileUpPtEC2down);
     fChain->SetBranchAddress("jetMassjesPileUpPtEC2down", &jetMassjesPileUpPtEC2down, &b_jetMassjesPileUpPtEC2down);
     fChain->SetBranchAddress("METPt_jesPileUpPtEC2down", &METPt_jesPileUpPtEC2down, &b_METPt_jesPileUpPtEC2down);
     fChain->SetBranchAddress("metPhijesPileUpPtEC2down", &metPhijesPileUpPtEC2down, &b_metPhijesPileUpPtEC2down);
     fChain->SetBranchAddress("numBtagsjesPileUpPtEC2down", &numBtagsjesPileUpPtEC2down, &b_numBtagsjesPileUpPtEC2down);
     fChain->SetBranchAddress("metleptondelphijesPileUpPtEC2down", &metleptondelphijesPileUpPtEC2down, &b_metleptondelphijesPileUpPtEC2down);
     fChain->SetBranchAddress("minLeptonJetDrjesPileUpPtEC2down", &minLeptonJetDrjesPileUpPtEC2down, &b_minLeptonJetDrjesPileUpPtEC2down);
     fChain->SetBranchAddress("JetPtjesPileUpPtHFup", &JetPtjesPileUpPtHFup, &b_JetPtjesPileUpPtHFup);
     fChain->SetBranchAddress("jetEtajesPileUpPtHFup", &jetEtajesPileUpPtHFup, &b_jetEtajesPileUpPtHFup);
     fChain->SetBranchAddress("jetPhijesPileUpPtHFup", &jetPhijesPileUpPtHFup, &b_jetPhijesPileUpPtHFup);
     fChain->SetBranchAddress("jetMassjesPileUpPtHFup", &jetMassjesPileUpPtHFup, &b_jetMassjesPileUpPtHFup);
     fChain->SetBranchAddress("METPt_jesPileUpPtHFup", &METPt_jesPileUpPtHFup, &b_METPt_jesPileUpPtHFup);
     fChain->SetBranchAddress("metPhijesPileUpPtHFup", &metPhijesPileUpPtHFup, &b_metPhijesPileUpPtHFup);
     fChain->SetBranchAddress("numBtagsjesPileUpPtHFup", &numBtagsjesPileUpPtHFup, &b_numBtagsjesPileUpPtHFup);
     fChain->SetBranchAddress("metleptondelphijesPileUpPtHFup", &metleptondelphijesPileUpPtHFup, &b_metleptondelphijesPileUpPtHFup);
     fChain->SetBranchAddress("minLeptonJetDrjesPileUpPtHFup", &minLeptonJetDrjesPileUpPtHFup, &b_minLeptonJetDrjesPileUpPtHFup);
     fChain->SetBranchAddress("JetPtjesPileUpPtHFdown", &JetPtjesPileUpPtHFdown, &b_JetPtjesPileUpPtHFdown);
     fChain->SetBranchAddress("jetEtajesPileUpPtHFdown", &jetEtajesPileUpPtHFdown, &b_jetEtajesPileUpPtHFdown);
     fChain->SetBranchAddress("jetPhijesPileUpPtHFdown", &jetPhijesPileUpPtHFdown, &b_jetPhijesPileUpPtHFdown);
     fChain->SetBranchAddress("jetMassjesPileUpPtHFdown", &jetMassjesPileUpPtHFdown, &b_jetMassjesPileUpPtHFdown);
     fChain->SetBranchAddress("METPt_jesPileUpPtHFdown", &METPt_jesPileUpPtHFdown, &b_METPt_jesPileUpPtHFdown);
     fChain->SetBranchAddress("metPhijesPileUpPtHFdown", &metPhijesPileUpPtHFdown, &b_metPhijesPileUpPtHFdown);
     fChain->SetBranchAddress("numBtagsjesPileUpPtHFdown", &numBtagsjesPileUpPtHFdown, &b_numBtagsjesPileUpPtHFdown);
     fChain->SetBranchAddress("metleptondelphijesPileUpPtHFdown", &metleptondelphijesPileUpPtHFdown, &b_metleptondelphijesPileUpPtHFdown);
     fChain->SetBranchAddress("minLeptonJetDrjesPileUpPtHFdown", &minLeptonJetDrjesPileUpPtHFdown, &b_minLeptonJetDrjesPileUpPtHFdown);
     fChain->SetBranchAddress("JetPtjesPileUpPtRefup", &JetPtjesPileUpPtRefup, &b_JetPtjesPileUpPtRefup);
     fChain->SetBranchAddress("jetEtajesPileUpPtRefup", &jetEtajesPileUpPtRefup, &b_jetEtajesPileUpPtRefup);
     fChain->SetBranchAddress("jetPhijesPileUpPtRefup", &jetPhijesPileUpPtRefup, &b_jetPhijesPileUpPtRefup);
     fChain->SetBranchAddress("jetMassjesPileUpPtRefup", &jetMassjesPileUpPtRefup, &b_jetMassjesPileUpPtRefup);
     fChain->SetBranchAddress("METPt_jesPileUpPtRefup", &METPt_jesPileUpPtRefup, &b_METPt_jesPileUpPtRefup);
     fChain->SetBranchAddress("metPhijesPileUpPtRefup", &metPhijesPileUpPtRefup, &b_metPhijesPileUpPtRefup);
     fChain->SetBranchAddress("numBtagsjesPileUpPtRefup", &numBtagsjesPileUpPtRefup, &b_numBtagsjesPileUpPtRefup);
     fChain->SetBranchAddress("metleptondelphijesPileUpPtRefup", &metleptondelphijesPileUpPtRefup, &b_metleptondelphijesPileUpPtRefup);
     fChain->SetBranchAddress("minLeptonJetDrjesPileUpPtRefup", &minLeptonJetDrjesPileUpPtRefup, &b_minLeptonJetDrjesPileUpPtRefup);
     fChain->SetBranchAddress("JetPtjesPileUpPtRefdown", &JetPtjesPileUpPtRefdown, &b_JetPtjesPileUpPtRefdown);
     fChain->SetBranchAddress("jetEtajesPileUpPtRefdown", &jetEtajesPileUpPtRefdown, &b_jetEtajesPileUpPtRefdown);
     fChain->SetBranchAddress("jetPhijesPileUpPtRefdown", &jetPhijesPileUpPtRefdown, &b_jetPhijesPileUpPtRefdown);
     fChain->SetBranchAddress("jetMassjesPileUpPtRefdown", &jetMassjesPileUpPtRefdown, &b_jetMassjesPileUpPtRefdown);
     fChain->SetBranchAddress("METPt_jesPileUpPtRefdown", &METPt_jesPileUpPtRefdown, &b_METPt_jesPileUpPtRefdown);
     fChain->SetBranchAddress("metPhijesPileUpPtRefdown", &metPhijesPileUpPtRefdown, &b_metPhijesPileUpPtRefdown);
     fChain->SetBranchAddress("numBtagsjesPileUpPtRefdown", &numBtagsjesPileUpPtRefdown, &b_numBtagsjesPileUpPtRefdown);
     fChain->SetBranchAddress("metleptondelphijesPileUpPtRefdown", &metleptondelphijesPileUpPtRefdown, &b_metleptondelphijesPileUpPtRefdown);
     fChain->SetBranchAddress("minLeptonJetDrjesPileUpPtRefdown", &minLeptonJetDrjesPileUpPtRefdown, &b_minLeptonJetDrjesPileUpPtRefdown);
     fChain->SetBranchAddress("JetPtjesRelativeFSRup", &JetPtjesRelativeFSRup, &b_JetPtjesRelativeFSRup);
     fChain->SetBranchAddress("jetEtajesRelativeFSRup", &jetEtajesRelativeFSRup, &b_jetEtajesRelativeFSRup);
     fChain->SetBranchAddress("jetPhijesRelativeFSRup", &jetPhijesRelativeFSRup, &b_jetPhijesRelativeFSRup);
     fChain->SetBranchAddress("jetMassjesRelativeFSRup", &jetMassjesRelativeFSRup, &b_jetMassjesRelativeFSRup);
     fChain->SetBranchAddress("METPt_jesRelativeFSRup", &METPt_jesRelativeFSRup, &b_METPt_jesRelativeFSRup);
     fChain->SetBranchAddress("metPhijesRelativeFSRup", &metPhijesRelativeFSRup, &b_metPhijesRelativeFSRup);
     fChain->SetBranchAddress("numBtagsjesRelativeFSRup", &numBtagsjesRelativeFSRup, &b_numBtagsjesRelativeFSRup);
     fChain->SetBranchAddress("metleptondelphijesRelativeFSRup", &metleptondelphijesRelativeFSRup, &b_metleptondelphijesRelativeFSRup);
     fChain->SetBranchAddress("minLeptonJetDrjesRelativeFSRup", &minLeptonJetDrjesRelativeFSRup, &b_minLeptonJetDrjesRelativeFSRup);
     fChain->SetBranchAddress("JetPtjesRelativeFSRdown", &JetPtjesRelativeFSRdown, &b_JetPtjesRelativeFSRdown);
     fChain->SetBranchAddress("jetEtajesRelativeFSRdown", &jetEtajesRelativeFSRdown, &b_jetEtajesRelativeFSRdown);
     fChain->SetBranchAddress("jetPhijesRelativeFSRdown", &jetPhijesRelativeFSRdown, &b_jetPhijesRelativeFSRdown);
     fChain->SetBranchAddress("jetMassjesRelativeFSRdown", &jetMassjesRelativeFSRdown, &b_jetMassjesRelativeFSRdown);
     fChain->SetBranchAddress("METPt_jesRelativeFSRdown", &METPt_jesRelativeFSRdown, &b_METPt_jesRelativeFSRdown);
     fChain->SetBranchAddress("metPhijesRelativeFSRdown", &metPhijesRelativeFSRdown, &b_metPhijesRelativeFSRdown);
     fChain->SetBranchAddress("numBtagsjesRelativeFSRdown", &numBtagsjesRelativeFSRdown, &b_numBtagsjesRelativeFSRdown);
     fChain->SetBranchAddress("metleptondelphijesRelativeFSRdown", &metleptondelphijesRelativeFSRdown, &b_metleptondelphijesRelativeFSRdown);
     fChain->SetBranchAddress("minLeptonJetDrjesRelativeFSRdown", &minLeptonJetDrjesRelativeFSRdown, &b_minLeptonJetDrjesRelativeFSRdown);
     fChain->SetBranchAddress("JetPtjesRelativeJEREC1up", &JetPtjesRelativeJEREC1up, &b_JetPtjesRelativeJEREC1up);
     fChain->SetBranchAddress("jetEtajesRelativeJEREC1up", &jetEtajesRelativeJEREC1up, &b_jetEtajesRelativeJEREC1up);
     fChain->SetBranchAddress("jetPhijesRelativeJEREC1up", &jetPhijesRelativeJEREC1up, &b_jetPhijesRelativeJEREC1up);
     fChain->SetBranchAddress("jetMassjesRelativeJEREC1up", &jetMassjesRelativeJEREC1up, &b_jetMassjesRelativeJEREC1up);
     fChain->SetBranchAddress("METPt_jesRelativeJEREC1up", &METPt_jesRelativeJEREC1up, &b_METPt_jesRelativeJEREC1up);
     fChain->SetBranchAddress("metPhijesRelativeJEREC1up", &metPhijesRelativeJEREC1up, &b_metPhijesRelativeJEREC1up);
     fChain->SetBranchAddress("numBtagsjesRelativeFSRdown", &numBtagsjesRelativeFSRdown, &b_numBtagsjesRelativeFSRdown);
     fChain->SetBranchAddress("metleptondelphijesRelativeFSRdown", &metleptondelphijesRelativeFSRdown, &b_metleptondelphijesRelativeFSRdown);
     fChain->SetBranchAddress("minLeptonJetDrjesRelativeFSRdown", &minLeptonJetDrjesRelativeFSRdown, &b_minLeptonJetDrjesRelativeFSRdown);
     fChain->SetBranchAddress("JetPtjesRelativeJEREC1up", &JetPtjesRelativeJEREC1up, &b_JetPtjesRelativeJEREC1up);
     fChain->SetBranchAddress("jetEtajesRelativeJEREC1up", &jetEtajesRelativeJEREC1up, &b_jetEtajesRelativeJEREC1up);
     fChain->SetBranchAddress("jetPhijesRelativeJEREC1up", &jetPhijesRelativeJEREC1up, &b_jetPhijesRelativeJEREC1up);
     fChain->SetBranchAddress("jetMassjesRelativeJEREC1up", &jetMassjesRelativeJEREC1up, &b_jetMassjesRelativeJEREC1up);
     fChain->SetBranchAddress("METPt_jesRelativeJEREC1up", &METPt_jesRelativeJEREC1up, &b_METPt_jesRelativeJEREC1up);
     fChain->SetBranchAddress("metPhijesRelativeJEREC1up", &metPhijesRelativeJEREC1up, &b_metPhijesRelativeJEREC1up);
     fChain->SetBranchAddress("numBtagsjesRelativeJEREC1up", &numBtagsjesRelativeJEREC1up, &b_numBtagsjesRelativeJEREC1up);
     fChain->SetBranchAddress("metleptondelphijesRelativeJEREC1up", &metleptondelphijesRelativeJEREC1up, &b_metleptondelphijesRelativeJEREC1up);
     fChain->SetBranchAddress("minLeptonJetDrjesRelativeJEREC1up", &minLeptonJetDrjesRelativeJEREC1up, &b_minLeptonJetDrjesRelativeJEREC1up);
     fChain->SetBranchAddress("JetPtjesRelativeJEREC1down", &JetPtjesRelativeJEREC1down, &b_JetPtjesRelativeJEREC1down);
     fChain->SetBranchAddress("jetEtajesRelativeJEREC1down", &jetEtajesRelativeJEREC1down, &b_jetEtajesRelativeJEREC1down);
     fChain->SetBranchAddress("jetPhijesRelativeJEREC1down", &jetPhijesRelativeJEREC1down, &b_jetPhijesRelativeJEREC1down);
     fChain->SetBranchAddress("jetMassjesRelativeJEREC1down", &jetMassjesRelativeJEREC1down, &b_jetMassjesRelativeJEREC1down);
     fChain->SetBranchAddress("METPt_jesRelativeJEREC1down", &METPt_jesRelativeJEREC1down, &b_METPt_jesRelativeJEREC1down);
     fChain->SetBranchAddress("metPhijesRelativeJEREC1down", &metPhijesRelativeJEREC1down, &b_metPhijesRelativeJEREC1down);
     fChain->SetBranchAddress("numBtagsjesRelativeJEREC1down", &numBtagsjesRelativeJEREC1down, &b_numBtagsjesRelativeJEREC1down);
     fChain->SetBranchAddress("metleptondelphijesRelativeJEREC1down", &metleptondelphijesRelativeJEREC1down, &b_metleptondelphijesRelativeJEREC1down);
     fChain->SetBranchAddress("minLeptonJetDrjesRelativeJEREC1down", &minLeptonJetDrjesRelativeJEREC1down, &b_minLeptonJetDrjesRelativeJEREC1down);
     fChain->SetBranchAddress("JetPtjesRelativeJEREC2up", &JetPtjesRelativeJEREC2up, &b_JetPtjesRelativeJEREC2up);
     fChain->SetBranchAddress("jetEtajesRelativeJEREC2up", &jetEtajesRelativeJEREC2up, &b_jetEtajesRelativeJEREC2up);
     fChain->SetBranchAddress("jetPhijesRelativeJEREC2up", &jetPhijesRelativeJEREC2up, &b_jetPhijesRelativeJEREC2up);
     fChain->SetBranchAddress("jetMassjesRelativeJEREC2up", &jetMassjesRelativeJEREC2up, &b_jetMassjesRelativeJEREC2up);
     fChain->SetBranchAddress("METPt_jesRelativeJEREC2up", &METPt_jesRelativeJEREC2up, &b_METPt_jesRelativeJEREC2up);
     fChain->SetBranchAddress("metPhijesRelativeJEREC2up", &metPhijesRelativeJEREC2up, &b_metPhijesRelativeJEREC2up);
     fChain->SetBranchAddress("numBtagsjesRelativeJEREC2up", &numBtagsjesRelativeJEREC2up, &b_numBtagsjesRelativeJEREC2up);
     fChain->SetBranchAddress("metleptondelphijesRelativeJEREC2up", &metleptondelphijesRelativeJEREC2up, &b_metleptondelphijesRelativeJEREC2up);
     fChain->SetBranchAddress("minLeptonJetDrjesRelativeJEREC2up", &minLeptonJetDrjesRelativeJEREC2up, &b_minLeptonJetDrjesRelativeJEREC2up);
     fChain->SetBranchAddress("JetPtjesRelativeJEREC2down", &JetPtjesRelativeJEREC2down, &b_JetPtjesRelativeJEREC2down);
     fChain->SetBranchAddress("jetEtajesRelativeJEREC2down", &jetEtajesRelativeJEREC2down, &b_jetEtajesRelativeJEREC2down);
     fChain->SetBranchAddress("jetPhijesRelativeJEREC2down", &jetPhijesRelativeJEREC2down, &b_jetPhijesRelativeJEREC2down);
     fChain->SetBranchAddress("jetMassjesRelativeJEREC2down", &jetMassjesRelativeJEREC2down, &b_jetMassjesRelativeJEREC2down);
     fChain->SetBranchAddress("METPt_jesRelativeJEREC2down", &METPt_jesRelativeJEREC2down, &b_METPt_jesRelativeJEREC2down);
     fChain->SetBranchAddress("metPhijesRelativeJEREC2down", &metPhijesRelativeJEREC2down, &b_metPhijesRelativeJEREC2down);
     fChain->SetBranchAddress("numBtagsjesRelativeJEREC2down", &numBtagsjesRelativeJEREC2down, &b_numBtagsjesRelativeJEREC2down);
     fChain->SetBranchAddress("metleptondelphijesRelativeJEREC2down", &metleptondelphijesRelativeJEREC2down, &b_metleptondelphijesRelativeJEREC2down);
     fChain->SetBranchAddress("minLeptonJetDrjesRelativeJEREC2down", &minLeptonJetDrjesRelativeJEREC2down, &b_minLeptonJetDrjesRelativeJEREC2down);
     fChain->SetBranchAddress("JetPtjesRelativeJERHFup", &JetPtjesRelativeJERHFup, &b_JetPtjesRelativeJERHFup);
     fChain->SetBranchAddress("jetEtajesRelativeJERHFup", &jetEtajesRelativeJERHFup, &b_jetEtajesRelativeJERHFup);
     fChain->SetBranchAddress("jetPhijesRelativeJERHFup", &jetPhijesRelativeJERHFup, &b_jetPhijesRelativeJERHFup);
     fChain->SetBranchAddress("jetMassjesRelativeJERHFup", &jetMassjesRelativeJERHFup, &b_jetMassjesRelativeJERHFup);
     fChain->SetBranchAddress("METPt_jesRelativeJERHFup", &METPt_jesRelativeJERHFup, &b_METPt_jesRelativeJERHFup);
     fChain->SetBranchAddress("metPhijesRelativeJERHFup", &metPhijesRelativeJERHFup, &b_metPhijesRelativeJERHFup);
     fChain->SetBranchAddress("numBtagsjesRelativeJERHFup", &numBtagsjesRelativeJERHFup, &b_numBtagsjesRelativeJERHFup);
     fChain->SetBranchAddress("metleptondelphijesRelativeJERHFup", &metleptondelphijesRelativeJERHFup, &b_metleptondelphijesRelativeJERHFup);
     fChain->SetBranchAddress("minLeptonJetDrjesRelativeJERHFup", &minLeptonJetDrjesRelativeJERHFup, &b_minLeptonJetDrjesRelativeJERHFup);
     fChain->SetBranchAddress("JetPtjesRelativeJERHFdown", &JetPtjesRelativeJERHFdown, &b_JetPtjesRelativeJERHFdown);
     fChain->SetBranchAddress("jetEtajesRelativeJERHFdown", &jetEtajesRelativeJERHFdown, &b_jetEtajesRelativeJERHFdown);
     fChain->SetBranchAddress("jetPhijesRelativeJERHFdown", &jetPhijesRelativeJERHFdown, &b_jetPhijesRelativeJERHFdown);
     fChain->SetBranchAddress("jetMassjesRelativeJERHFdown", &jetMassjesRelativeJERHFdown, &b_jetMassjesRelativeJERHFdown);
     fChain->SetBranchAddress("METPt_jesRelativeJERHFdown", &METPt_jesRelativeJERHFdown, &b_METPt_jesRelativeJERHFdown);
     fChain->SetBranchAddress("metPhijesRelativeJERHFdown", &metPhijesRelativeJERHFdown, &b_metPhijesRelativeJERHFdown);
     fChain->SetBranchAddress("numBtagsjesRelativeJERHFdown", &numBtagsjesRelativeJERHFdown, &b_numBtagsjesRelativeJERHFdown);
     fChain->SetBranchAddress("metleptondelphijesRelativeJERHFdown", &metleptondelphijesRelativeJERHFdown, &b_metleptondelphijesRelativeJERHFdown);
     fChain->SetBranchAddress("minLeptonJetDrjesRelativeJERHFdown", &minLeptonJetDrjesRelativeJERHFdown, &b_minLeptonJetDrjesRelativeJERHFdown);
     fChain->SetBranchAddress("JetPtjesRelativePtBBup", &JetPtjesRelativePtBBup, &b_JetPtjesRelativePtBBup);
     fChain->SetBranchAddress("jetEtajesRelativePtBBup", &jetEtajesRelativePtBBup, &b_jetEtajesRelativePtBBup);
     fChain->SetBranchAddress("jetPhijesRelativePtBBup", &jetPhijesRelativePtBBup, &b_jetPhijesRelativePtBBup);
     fChain->SetBranchAddress("jetMassjesRelativePtBBup", &jetMassjesRelativePtBBup, &b_jetMassjesRelativePtBBup);
     fChain->SetBranchAddress("METPt_jesRelativePtBBup", &METPt_jesRelativePtBBup, &b_METPt_jesRelativePtBBup);
     fChain->SetBranchAddress("metPhijesRelativePtBBup", &metPhijesRelativePtBBup, &b_metPhijesRelativePtBBup);
     fChain->SetBranchAddress("numBtagsjesRelativePtBBup", &numBtagsjesRelativePtBBup, &b_numBtagsjesRelativePtBBup);
     fChain->SetBranchAddress("metleptondelphijesRelativePtBBup", &metleptondelphijesRelativePtBBup, &b_metleptondelphijesRelativePtBBup);
     fChain->SetBranchAddress("minLeptonJetDrjesRelativePtBBup", &minLeptonJetDrjesRelativePtBBup, &b_minLeptonJetDrjesRelativePtBBup);
     fChain->SetBranchAddress("JetPtjesRelativePtBBdown", &JetPtjesRelativePtBBdown, &b_JetPtjesRelativePtBBdown);
     fChain->SetBranchAddress("jetEtajesRelativePtBBdown", &jetEtajesRelativePtBBdown, &b_jetEtajesRelativePtBBdown);
     fChain->SetBranchAddress("jetPhijesRelativePtBBdown", &jetPhijesRelativePtBBdown, &b_jetPhijesRelativePtBBdown);
     fChain->SetBranchAddress("jetMassjesRelativePtBBdown", &jetMassjesRelativePtBBdown, &b_jetMassjesRelativePtBBdown);
     fChain->SetBranchAddress("METPt_jesRelativePtBBdown", &METPt_jesRelativePtBBdown, &b_METPt_jesRelativePtBBdown);
     fChain->SetBranchAddress("metPhijesRelativePtBBdown", &metPhijesRelativePtBBdown, &b_metPhijesRelativePtBBdown);
     fChain->SetBranchAddress("numBtagsjesRelativePtBBdown", &numBtagsjesRelativePtBBdown, &b_numBtagsjesRelativePtBBdown);
     fChain->SetBranchAddress("metleptondelphijesRelativePtBBdown", &metleptondelphijesRelativePtBBdown, &b_metleptondelphijesRelativePtBBdown);
     fChain->SetBranchAddress("minLeptonJetDrjesRelativePtBBdown", &minLeptonJetDrjesRelativePtBBdown, &b_minLeptonJetDrjesRelativePtBBdown);
     fChain->SetBranchAddress("JetPtjesRelativePtEC1up", &JetPtjesRelativePtEC1up, &b_JetPtjesRelativePtEC1up);
     fChain->SetBranchAddress("jetEtajesRelativePtEC1up", &jetEtajesRelativePtEC1up, &b_jetEtajesRelativePtEC1up);
     fChain->SetBranchAddress("jetPhijesRelativePtEC1up", &jetPhijesRelativePtEC1up, &b_jetPhijesRelativePtEC1up);
     fChain->SetBranchAddress("jetMassjesRelativePtEC1up", &jetMassjesRelativePtEC1up, &b_jetMassjesRelativePtEC1up);
     fChain->SetBranchAddress("METPt_jesRelativePtEC1up", &METPt_jesRelativePtEC1up, &b_METPt_jesRelativePtEC1up);
     fChain->SetBranchAddress("metPhijesRelativePtEC1up", &metPhijesRelativePtEC1up, &b_metPhijesRelativePtEC1up);
     fChain->SetBranchAddress("numBtagsjesRelativePtEC1up", &numBtagsjesRelativePtEC1up, &b_numBtagsjesRelativePtEC1up);
     fChain->SetBranchAddress("metleptondelphijesRelativePtEC1up", &metleptondelphijesRelativePtEC1up, &b_metleptondelphijesRelativePtEC1up);
     fChain->SetBranchAddress("minLeptonJetDrjesRelativePtEC1up", &minLeptonJetDrjesRelativePtEC1up, &b_minLeptonJetDrjesRelativePtEC1up);
     fChain->SetBranchAddress("JetPtjesRelativePtEC1down", &JetPtjesRelativePtEC1down, &b_JetPtjesRelativePtEC1down);
     fChain->SetBranchAddress("jetEtajesRelativePtEC1down", &jetEtajesRelativePtEC1down, &b_jetEtajesRelativePtEC1down);
     fChain->SetBranchAddress("jetPhijesRelativePtEC1down", &jetPhijesRelativePtEC1down, &b_jetPhijesRelativePtEC1down);
     fChain->SetBranchAddress("jetMassjesRelativePtEC1down", &jetMassjesRelativePtEC1down, &b_jetMassjesRelativePtEC1down);
     fChain->SetBranchAddress("METPt_jesRelativePtEC1down", &METPt_jesRelativePtEC1down, &b_METPt_jesRelativePtEC1down);
     fChain->SetBranchAddress("metPhijesRelativePtEC1down", &metPhijesRelativePtEC1down, &b_metPhijesRelativePtEC1down);
     fChain->SetBranchAddress("numBtagsjesRelativePtEC1down", &numBtagsjesRelativePtEC1down, &b_numBtagsjesRelativePtEC1down);
     fChain->SetBranchAddress("metleptondelphijesRelativePtEC1down", &metleptondelphijesRelativePtEC1down, &b_metleptondelphijesRelativePtEC1down);
     fChain->SetBranchAddress("minLeptonJetDrjesRelativePtEC1down", &minLeptonJetDrjesRelativePtEC1down, &b_minLeptonJetDrjesRelativePtEC1down);
     fChain->SetBranchAddress("JetPtjesRelativePtEC2up", &JetPtjesRelativePtEC2up, &b_JetPtjesRelativePtEC2up);
     fChain->SetBranchAddress("jetEtajesRelativePtEC2up", &jetEtajesRelativePtEC2up, &b_jetEtajesRelativePtEC2up);
     fChain->SetBranchAddress("jetPhijesRelativePtEC2up", &jetPhijesRelativePtEC2up, &b_jetPhijesRelativePtEC2up);
     fChain->SetBranchAddress("jetMassjesRelativePtEC2up", &jetMassjesRelativePtEC2up, &b_jetMassjesRelativePtEC2up);
     fChain->SetBranchAddress("METPt_jesRelativePtEC2up", &METPt_jesRelativePtEC2up, &b_METPt_jesRelativePtEC2up);
     fChain->SetBranchAddress("metPhijesRelativePtEC2up", &metPhijesRelativePtEC2up, &b_metPhijesRelativePtEC2up);
     fChain->SetBranchAddress("numBtagsjesRelativePtEC2up", &numBtagsjesRelativePtEC2up, &b_numBtagsjesRelativePtEC2up);
     fChain->SetBranchAddress("metleptondelphijesRelativePtEC2up", &metleptondelphijesRelativePtEC2up, &b_metleptondelphijesRelativePtEC2up);
     fChain->SetBranchAddress("minLeptonJetDrjesRelativePtEC2up", &minLeptonJetDrjesRelativePtEC2up, &b_minLeptonJetDrjesRelativePtEC2up);
     fChain->SetBranchAddress("JetPtjesRelativePtEC2down", &JetPtjesRelativePtEC2down, &b_JetPtjesRelativePtEC2down);
     fChain->SetBranchAddress("jetEtajesRelativePtEC2down", &jetEtajesRelativePtEC2down, &b_jetEtajesRelativePtEC2down);
     fChain->SetBranchAddress("jetPhijesRelativePtEC2down", &jetPhijesRelativePtEC2down, &b_jetPhijesRelativePtEC2down);
     fChain->SetBranchAddress("jetMassjesRelativePtEC2down", &jetMassjesRelativePtEC2down, &b_jetMassjesRelativePtEC2down);
     fChain->SetBranchAddress("METPt_jesRelativePtEC2down", &METPt_jesRelativePtEC2down, &b_METPt_jesRelativePtEC2down);
     fChain->SetBranchAddress("metPhijesRelativePtEC2down", &metPhijesRelativePtEC2down, &b_metPhijesRelativePtEC2down);
     fChain->SetBranchAddress("numBtagsjesRelativePtEC2down", &numBtagsjesRelativePtEC2down, &b_numBtagsjesRelativePtEC2down);
     fChain->SetBranchAddress("metleptondelphijesRelativePtEC2down", &metleptondelphijesRelativePtEC2down, &b_metleptondelphijesRelativePtEC2down);
     fChain->SetBranchAddress("minLeptonJetDrjesRelativePtEC2down", &minLeptonJetDrjesRelativePtEC2down, &b_minLeptonJetDrjesRelativePtEC2down);
     fChain->SetBranchAddress("JetPtjesRelativePtHFup", &JetPtjesRelativePtHFup, &b_JetPtjesRelativePtHFup);
     fChain->SetBranchAddress("jetEtajesRelativePtHFup", &jetEtajesRelativePtHFup, &b_jetEtajesRelativePtHFup);
     fChain->SetBranchAddress("jetPhijesRelativePtHFup", &jetPhijesRelativePtHFup, &b_jetPhijesRelativePtHFup);
     fChain->SetBranchAddress("jetMassjesRelativePtHFup", &jetMassjesRelativePtHFup, &b_jetMassjesRelativePtHFup);
     fChain->SetBranchAddress("METPt_jesRelativePtHFup", &METPt_jesRelativePtHFup, &b_METPt_jesRelativePtHFup);
     fChain->SetBranchAddress("metPhijesRelativePtHFup", &metPhijesRelativePtHFup, &b_metPhijesRelativePtHFup);
     fChain->SetBranchAddress("numBtagsjesRelativePtHFup", &numBtagsjesRelativePtHFup, &b_numBtagsjesRelativePtHFup);
     fChain->SetBranchAddress("metleptondelphijesRelativePtHFup", &metleptondelphijesRelativePtHFup, &b_metleptondelphijesRelativePtHFup);
     fChain->SetBranchAddress("minLeptonJetDrjesRelativePtHFup", &minLeptonJetDrjesRelativePtHFup, &b_minLeptonJetDrjesRelativePtHFup);
     fChain->SetBranchAddress("JetPtjesRelativePtHFdown", &JetPtjesRelativePtHFdown, &b_JetPtjesRelativePtHFdown);
     fChain->SetBranchAddress("jetEtajesRelativePtHFdown", &jetEtajesRelativePtHFdown, &b_jetEtajesRelativePtHFdown);
     fChain->SetBranchAddress("jetPhijesRelativePtHFdown", &jetPhijesRelativePtHFdown, &b_jetPhijesRelativePtHFdown);
     fChain->SetBranchAddress("jetMassjesRelativePtHFdown", &jetMassjesRelativePtHFdown, &b_jetMassjesRelativePtHFdown);
     fChain->SetBranchAddress("METPt_jesRelativePtHFdown", &METPt_jesRelativePtHFdown, &b_METPt_jesRelativePtHFdown);
     fChain->SetBranchAddress("metPhijesRelativePtHFdown", &metPhijesRelativePtHFdown, &b_metPhijesRelativePtHFdown);
     fChain->SetBranchAddress("numBtagsjesRelativePtHFdown", &numBtagsjesRelativePtHFdown, &b_numBtagsjesRelativePtHFdown);
     fChain->SetBranchAddress("metleptondelphijesRelativePtHFdown", &metleptondelphijesRelativePtHFdown, &b_metleptondelphijesRelativePtHFdown);
     fChain->SetBranchAddress("minLeptonJetDrjesRelativePtHFdown", &minLeptonJetDrjesRelativePtHFdown, &b_minLeptonJetDrjesRelativePtHFdown);
     fChain->SetBranchAddress("JetPtjesRelativeBalup", &JetPtjesRelativeBalup, &b_JetPtjesRelativeBalup);
     fChain->SetBranchAddress("jetEtajesRelativeBalup", &jetEtajesRelativeBalup, &b_jetEtajesRelativeBalup);
     fChain->SetBranchAddress("jetPhijesRelativeBalup", &jetPhijesRelativeBalup, &b_jetPhijesRelativeBalup);
     fChain->SetBranchAddress("jetMassjesRelativeBalup", &jetMassjesRelativeBalup, &b_jetMassjesRelativeBalup);
     fChain->SetBranchAddress("METPt_jesRelativeBalup", &METPt_jesRelativeBalup, &b_METPt_jesRelativeBalup);
     fChain->SetBranchAddress("metPhijesRelativeBalup", &metPhijesRelativeBalup, &b_metPhijesRelativeBalup);
     fChain->SetBranchAddress("numBtagsjesRelativeBalup", &numBtagsjesRelativeBalup, &b_numBtagsjesRelativeBalup);
     fChain->SetBranchAddress("metleptondelphijesRelativeBalup", &metleptondelphijesRelativeBalup, &b_metleptondelphijesRelativeBalup);
     fChain->SetBranchAddress("minLeptonJetDrjesRelativeBalup", &minLeptonJetDrjesRelativeBalup, &b_minLeptonJetDrjesRelativeBalup);
     fChain->SetBranchAddress("JetPtjesRelativeBaldown", &JetPtjesRelativeBaldown, &b_JetPtjesRelativeBaldown);
     fChain->SetBranchAddress("jetEtajesRelativeBaldown", &jetEtajesRelativeBaldown, &b_jetEtajesRelativeBaldown);
     fChain->SetBranchAddress("jetPhijesRelativeBaldown", &jetPhijesRelativeBaldown, &b_jetPhijesRelativeBaldown);
     fChain->SetBranchAddress("jetMassjesRelativeBaldown", &jetMassjesRelativeBaldown, &b_jetMassjesRelativeBaldown);
     fChain->SetBranchAddress("METPt_jesRelativeBaldown", &METPt_jesRelativeBaldown, &b_METPt_jesRelativeBaldown);
     fChain->SetBranchAddress("metPhijesRelativeBaldown", &metPhijesRelativeBaldown, &b_metPhijesRelativeBaldown);
     fChain->SetBranchAddress("numBtagsjesRelativeBaldown", &numBtagsjesRelativeBaldown, &b_numBtagsjesRelativeBaldown);
     fChain->SetBranchAddress("metleptondelphijesRelativeBaldown", &metleptondelphijesRelativeBaldown, &b_metleptondelphijesRelativeBaldown);
     fChain->SetBranchAddress("minLeptonJetDrjesRelativeBaldown", &minLeptonJetDrjesRelativeBaldown, &b_minLeptonJetDrjesRelativeBaldown);
     fChain->SetBranchAddress("JetPtjesRelativeSampleup", &JetPtjesRelativeSampleup, &b_JetPtjesRelativeSampleup);
     fChain->SetBranchAddress("jetEtajesRelativeSampleup", &jetEtajesRelativeSampleup, &b_jetEtajesRelativeSampleup);
     fChain->SetBranchAddress("jetPhijesRelativeSampleup", &jetPhijesRelativeSampleup, &b_jetPhijesRelativeSampleup);
     fChain->SetBranchAddress("jetMassjesRelativeSampleup", &jetMassjesRelativeSampleup, &b_jetMassjesRelativeSampleup);
     fChain->SetBranchAddress("METPt_jesRelativeSampleup", &METPt_jesRelativeSampleup, &b_METPt_jesRelativeSampleup);
     fChain->SetBranchAddress("metPhijesRelativeSampleup", &metPhijesRelativeSampleup, &b_metPhijesRelativeSampleup);
     fChain->SetBranchAddress("numBtagsjesRelativeSampleup", &numBtagsjesRelativeSampleup, &b_numBtagsjesRelativeSampleup);
     fChain->SetBranchAddress("metleptondelphijesRelativeSampleup", &metleptondelphijesRelativeSampleup, &b_metleptondelphijesRelativeSampleup);
     fChain->SetBranchAddress("minLeptonJetDrjesRelativeSampleup", &minLeptonJetDrjesRelativeSampleup, &b_minLeptonJetDrjesRelativeSampleup);
     fChain->SetBranchAddress("JetPtjesRelativeSampledown", &JetPtjesRelativeSampledown, &b_JetPtjesRelativeSampledown);
     fChain->SetBranchAddress("jetEtajesRelativeSampledown", &jetEtajesRelativeSampledown, &b_jetEtajesRelativeSampledown);
     fChain->SetBranchAddress("jetPhijesRelativeSampledown", &jetPhijesRelativeSampledown, &b_jetPhijesRelativeSampledown);
     fChain->SetBranchAddress("jetMassjesRelativeSampledown", &jetMassjesRelativeSampledown, &b_jetMassjesRelativeSampledown);
     fChain->SetBranchAddress("METPt_jesRelativeSampledown", &METPt_jesRelativeSampledown, &b_METPt_jesRelativeSampledown);
     fChain->SetBranchAddress("metPhijesRelativeSampledown", &metPhijesRelativeSampledown, &b_metPhijesRelativeSampledown);
     fChain->SetBranchAddress("numBtagsjesRelativeSampledown", &numBtagsjesRelativeSampledown, &b_numBtagsjesRelativeSampledown);
     fChain->SetBranchAddress("metleptondelphijesRelativeSampledown", &metleptondelphijesRelativeSampledown, &b_metleptondelphijesRelativeSampledown);
     fChain->SetBranchAddress("minLeptonJetDrjesRelativeSampledown", &minLeptonJetDrjesRelativeSampledown, &b_minLeptonJetDrjesRelativeSampledown);
     fChain->SetBranchAddress("JetPtjesRelativeStatECup", &JetPtjesRelativeStatECup, &b_JetPtjesRelativeStatECup);
     fChain->SetBranchAddress("jetEtajesRelativeStatECup", &jetEtajesRelativeStatECup, &b_jetEtajesRelativeStatECup);
     fChain->SetBranchAddress("jetPhijesRelativeStatECup", &jetPhijesRelativeStatECup, &b_jetPhijesRelativeStatECup);
     fChain->SetBranchAddress("jetMassjesRelativeStatECup", &jetMassjesRelativeStatECup, &b_jetMassjesRelativeStatECup);
     fChain->SetBranchAddress("METPt_jesRelativeStatECup", &METPt_jesRelativeStatECup, &b_METPt_jesRelativeStatECup);
     fChain->SetBranchAddress("metPhijesRelativeStatECup", &metPhijesRelativeStatECup, &b_metPhijesRelativeStatECup);
     fChain->SetBranchAddress("numBtagsjesRelativeStatECup", &numBtagsjesRelativeStatECup, &b_numBtagsjesRelativeStatECup);
     fChain->SetBranchAddress("metleptondelphijesRelativeStatECup", &metleptondelphijesRelativeStatECup, &b_metleptondelphijesRelativeStatECup);
     fChain->SetBranchAddress("minLeptonJetDrjesRelativeStatECup", &minLeptonJetDrjesRelativeStatECup, &b_minLeptonJetDrjesRelativeStatECup);
     fChain->SetBranchAddress("JetPtjesRelativeStatECdown", &JetPtjesRelativeStatECdown, &b_JetPtjesRelativeStatECdown);
     fChain->SetBranchAddress("jetEtajesRelativeStatECdown", &jetEtajesRelativeStatECdown, &b_jetEtajesRelativeStatECdown);
     fChain->SetBranchAddress("jetPhijesRelativeStatECdown", &jetPhijesRelativeStatECdown, &b_jetPhijesRelativeStatECdown);
     fChain->SetBranchAddress("jetMassjesRelativeStatECdown", &jetMassjesRelativeStatECdown, &b_jetMassjesRelativeStatECdown);
     fChain->SetBranchAddress("METPt_jesRelativeStatECdown", &METPt_jesRelativeStatECdown, &b_METPt_jesRelativeStatECdown);
     fChain->SetBranchAddress("metPhijesRelativeStatECdown", &metPhijesRelativeStatECdown, &b_metPhijesRelativeStatECdown);
     fChain->SetBranchAddress("numBtagsjesRelativeStatECdown", &numBtagsjesRelativeStatECdown, &b_numBtagsjesRelativeStatECdown);
     fChain->SetBranchAddress("metleptondelphijesRelativeStatECdown", &metleptondelphijesRelativeStatECdown, &b_metleptondelphijesRelativeStatECdown);
     fChain->SetBranchAddress("minLeptonJetDrjesRelativeStatECdown", &minLeptonJetDrjesRelativeStatECdown, &b_minLeptonJetDrjesRelativeStatECdown);
     fChain->SetBranchAddress("JetPtjesRelativeStatFSRup", &JetPtjesRelativeStatFSRup, &b_JetPtjesRelativeStatFSRup);
     fChain->SetBranchAddress("jetEtajesRelativeStatFSRup", &jetEtajesRelativeStatFSRup, &b_jetEtajesRelativeStatFSRup);
     fChain->SetBranchAddress("jetPhijesRelativeStatFSRup", &jetPhijesRelativeStatFSRup, &b_jetPhijesRelativeStatFSRup);
     fChain->SetBranchAddress("jetMassjesRelativeStatFSRup", &jetMassjesRelativeStatFSRup, &b_jetMassjesRelativeStatFSRup);
     fChain->SetBranchAddress("METPt_jesRelativeStatFSRup", &METPt_jesRelativeStatFSRup, &b_METPt_jesRelativeStatFSRup);
     fChain->SetBranchAddress("metPhijesRelativeStatFSRup", &metPhijesRelativeStatFSRup, &b_metPhijesRelativeStatFSRup);
     fChain->SetBranchAddress("numBtagsjesRelativeStatFSRup", &numBtagsjesRelativeStatFSRup, &b_numBtagsjesRelativeStatFSRup);
     fChain->SetBranchAddress("metleptondelphijesRelativeStatFSRup", &metleptondelphijesRelativeStatFSRup, &b_metleptondelphijesRelativeStatFSRup);
     fChain->SetBranchAddress("minLeptonJetDrjesRelativeStatFSRup", &minLeptonJetDrjesRelativeStatFSRup, &b_minLeptonJetDrjesRelativeStatFSRup);
     fChain->SetBranchAddress("JetPtjesRelativeStatFSRdown", &JetPtjesRelativeStatFSRdown, &b_JetPtjesRelativeStatFSRdown);
     fChain->SetBranchAddress("jetEtajesRelativeStatFSRdown", &jetEtajesRelativeStatFSRdown, &b_jetEtajesRelativeStatFSRdown);
     fChain->SetBranchAddress("jetPhijesRelativeStatFSRdown", &jetPhijesRelativeStatFSRdown, &b_jetPhijesRelativeStatFSRdown);
     fChain->SetBranchAddress("jetMassjesRelativeStatFSRdown", &jetMassjesRelativeStatFSRdown, &b_jetMassjesRelativeStatFSRdown);
     fChain->SetBranchAddress("METPt_jesRelativeStatFSRdown", &METPt_jesRelativeStatFSRdown, &b_METPt_jesRelativeStatFSRdown);
     fChain->SetBranchAddress("metPhijesRelativeStatFSRdown", &metPhijesRelativeStatFSRdown, &b_metPhijesRelativeStatFSRdown);
     fChain->SetBranchAddress("numBtagsjesRelativeStatFSRdown", &numBtagsjesRelativeStatFSRdown, &b_numBtagsjesRelativeStatFSRdown);
     fChain->SetBranchAddress("metleptondelphijesRelativeStatFSRdown", &metleptondelphijesRelativeStatFSRdown, &b_metleptondelphijesRelativeStatFSRdown);
     fChain->SetBranchAddress("minLeptonJetDrjesRelativeStatFSRdown", &minLeptonJetDrjesRelativeStatFSRdown, &b_minLeptonJetDrjesRelativeStatFSRdown);
     fChain->SetBranchAddress("JetPtjesRelativeStatHFup", &JetPtjesRelativeStatHFup, &b_JetPtjesRelativeStatHFup);
     fChain->SetBranchAddress("jetEtajesRelativeStatHFup", &jetEtajesRelativeStatHFup, &b_jetEtajesRelativeStatHFup);
     fChain->SetBranchAddress("jetPhijesRelativeStatHFup", &jetPhijesRelativeStatHFup, &b_jetPhijesRelativeStatHFup);
     fChain->SetBranchAddress("jetMassjesRelativeStatHFup", &jetMassjesRelativeStatHFup, &b_jetMassjesRelativeStatHFup);
     fChain->SetBranchAddress("METPt_jesRelativeStatHFup", &METPt_jesRelativeStatHFup, &b_METPt_jesRelativeStatHFup);
     fChain->SetBranchAddress("metPhijesRelativeStatHFup", &metPhijesRelativeStatHFup, &b_metPhijesRelativeStatHFup);
     fChain->SetBranchAddress("numBtagsjesRelativeStatHFup", &numBtagsjesRelativeStatHFup, &b_numBtagsjesRelativeStatHFup);
     fChain->SetBranchAddress("metleptondelphijesRelativeStatHFup", &metleptondelphijesRelativeStatHFup, &b_metleptondelphijesRelativeStatHFup);
     fChain->SetBranchAddress("minLeptonJetDrjesRelativeStatHFup", &minLeptonJetDrjesRelativeStatHFup, &b_minLeptonJetDrjesRelativeStatHFup);
     fChain->SetBranchAddress("JetPtjesRelativeStatHFdown", &JetPtjesRelativeStatHFdown, &b_JetPtjesRelativeStatHFdown);
     fChain->SetBranchAddress("jetEtajesRelativeStatHFdown", &jetEtajesRelativeStatHFdown, &b_jetEtajesRelativeStatHFdown);
     fChain->SetBranchAddress("jetPhijesRelativeStatHFdown", &jetPhijesRelativeStatHFdown, &b_jetPhijesRelativeStatHFdown);
     fChain->SetBranchAddress("jetMassjesRelativeStatHFdown", &jetMassjesRelativeStatHFdown, &b_jetMassjesRelativeStatHFdown);
     fChain->SetBranchAddress("METPt_jesRelativeStatHFdown", &METPt_jesRelativeStatHFdown, &b_METPt_jesRelativeStatHFdown);
     fChain->SetBranchAddress("metPhijesRelativeStatHFdown", &metPhijesRelativeStatHFdown, &b_metPhijesRelativeStatHFdown);
     fChain->SetBranchAddress("numBtagsjesRelativeStatHFdown", &numBtagsjesRelativeStatHFdown, &b_numBtagsjesRelativeStatHFdown);
     fChain->SetBranchAddress("metleptondelphijesRelativeStatHFdown", &metleptondelphijesRelativeStatHFdown, &b_metleptondelphijesRelativeStatHFdown);
     fChain->SetBranchAddress("minLeptonJetDrjesRelativeStatHFdown", &minLeptonJetDrjesRelativeStatHFdown, &b_minLeptonJetDrjesRelativeStatHFdown);
     fChain->SetBranchAddress("JetPtjesSinglePionECALup", &JetPtjesSinglePionECALup, &b_JetPtjesSinglePionECALup);
     fChain->SetBranchAddress("jetEtajesSinglePionECALup", &jetEtajesSinglePionECALup, &b_jetEtajesSinglePionECALup);
     fChain->SetBranchAddress("jetPhijesSinglePionECALup", &jetPhijesSinglePionECALup, &b_jetPhijesSinglePionECALup);
     fChain->SetBranchAddress("jetMassjesSinglePionECALup", &jetMassjesSinglePionECALup, &b_jetMassjesSinglePionECALup);
     fChain->SetBranchAddress("METPt_jesSinglePionECALup", &METPt_jesSinglePionECALup, &b_METPt_jesSinglePionECALup);
     fChain->SetBranchAddress("metPhijesSinglePionECALup", &metPhijesSinglePionECALup, &b_metPhijesSinglePionECALup);
     fChain->SetBranchAddress("numBtagsjesSinglePionECALup", &numBtagsjesSinglePionECALup, &b_numBtagsjesSinglePionECALup);
     fChain->SetBranchAddress("metleptondelphijesSinglePionECALup", &metleptondelphijesSinglePionECALup, &b_metleptondelphijesSinglePionECALup);
     fChain->SetBranchAddress("minLeptonJetDrjesSinglePionECALup", &minLeptonJetDrjesSinglePionECALup, &b_minLeptonJetDrjesSinglePionECALup);
     fChain->SetBranchAddress("JetPtjesSinglePionECALdown", &JetPtjesSinglePionECALdown, &b_JetPtjesSinglePionECALdown);
     fChain->SetBranchAddress("jetEtajesSinglePionECALdown", &jetEtajesSinglePionECALdown, &b_jetEtajesSinglePionECALdown);
     fChain->SetBranchAddress("jetPhijesSinglePionECALdown", &jetPhijesSinglePionECALdown, &b_jetPhijesSinglePionECALdown);
     fChain->SetBranchAddress("jetMassjesSinglePionECALdown", &jetMassjesSinglePionECALdown, &b_jetMassjesSinglePionECALdown);
     fChain->SetBranchAddress("METPt_jesSinglePionECALdown", &METPt_jesSinglePionECALdown, &b_METPt_jesSinglePionECALdown);
     fChain->SetBranchAddress("metPhijesSinglePionECALdown", &metPhijesSinglePionECALdown, &b_metPhijesSinglePionECALdown);
     fChain->SetBranchAddress("numBtagsjesSinglePionECALdown", &numBtagsjesSinglePionECALdown, &b_numBtagsjesSinglePionECALdown);
     fChain->SetBranchAddress("metleptondelphijesSinglePionECALdown", &metleptondelphijesSinglePionECALdown, &b_metleptondelphijesSinglePionECALdown);
     fChain->SetBranchAddress("minLeptonJetDrjesSinglePionECALdown", &minLeptonJetDrjesSinglePionECALdown, &b_minLeptonJetDrjesSinglePionECALdown);
     fChain->SetBranchAddress("JetPtjesSinglePionHCALup", &JetPtjesSinglePionHCALup, &b_JetPtjesSinglePionHCALup);
     fChain->SetBranchAddress("jetEtajesSinglePionHCALup", &jetEtajesSinglePionHCALup, &b_jetEtajesSinglePionHCALup);
     fChain->SetBranchAddress("jetPhijesSinglePionHCALup", &jetPhijesSinglePionHCALup, &b_jetPhijesSinglePionHCALup);
     fChain->SetBranchAddress("jetMassjesSinglePionHCALup", &jetMassjesSinglePionHCALup, &b_jetMassjesSinglePionHCALup);
     fChain->SetBranchAddress("METPt_jesSinglePionHCALup", &METPt_jesSinglePionHCALup, &b_METPt_jesSinglePionHCALup);
     fChain->SetBranchAddress("metPhijesSinglePionHCALup", &metPhijesSinglePionHCALup, &b_metPhijesSinglePionHCALup);
     fChain->SetBranchAddress("numBtagsjesSinglePionHCALup", &numBtagsjesSinglePionHCALup, &b_numBtagsjesSinglePionHCALup);
     fChain->SetBranchAddress("metleptondelphijesSinglePionHCALup", &metleptondelphijesSinglePionHCALup, &b_metleptondelphijesSinglePionHCALup);
     fChain->SetBranchAddress("minLeptonJetDrjesSinglePionHCALup", &minLeptonJetDrjesSinglePionHCALup, &b_minLeptonJetDrjesSinglePionHCALup);
     fChain->SetBranchAddress("JetPtjesSinglePionHCALdown", &JetPtjesSinglePionHCALdown, &b_JetPtjesSinglePionHCALdown);
     fChain->SetBranchAddress("jetEtajesSinglePionHCALdown", &jetEtajesSinglePionHCALdown, &b_jetEtajesSinglePionHCALdown);
     fChain->SetBranchAddress("jetPhijesSinglePionHCALdown", &jetPhijesSinglePionHCALdown, &b_jetPhijesSinglePionHCALdown);
     fChain->SetBranchAddress("jetMassjesSinglePionHCALdown", &jetMassjesSinglePionHCALdown, &b_jetMassjesSinglePionHCALdown);
     fChain->SetBranchAddress("METPt_jesSinglePionHCALdown", &METPt_jesSinglePionHCALdown, &b_METPt_jesSinglePionHCALdown);
     fChain->SetBranchAddress("metPhijesSinglePionHCALdown", &metPhijesSinglePionHCALdown, &b_metPhijesSinglePionHCALdown);
     fChain->SetBranchAddress("numBtagsjesSinglePionHCALdown", &numBtagsjesSinglePionHCALdown, &b_numBtagsjesSinglePionHCALdown);
     fChain->SetBranchAddress("metleptondelphijesSinglePionHCALdown", &metleptondelphijesSinglePionHCALdown, &b_metleptondelphijesSinglePionHCALdown);
     fChain->SetBranchAddress("minLeptonJetDrjesSinglePionHCALdown", &minLeptonJetDrjesSinglePionHCALdown, &b_minLeptonJetDrjesSinglePionHCALdown);
     fChain->SetBranchAddress("JetPtjesTimePtEtaup", &JetPtjesTimePtEtaup, &b_JetPtjesTimePtEtaup);
     fChain->SetBranchAddress("jetEtajesTimePtEtaup", &jetEtajesTimePtEtaup, &b_jetEtajesTimePtEtaup);
     fChain->SetBranchAddress("jetPhijesTimePtEtaup", &jetPhijesTimePtEtaup, &b_jetPhijesTimePtEtaup);
     fChain->SetBranchAddress("jetMassjesTimePtEtaup", &jetMassjesTimePtEtaup, &b_jetMassjesTimePtEtaup);
     fChain->SetBranchAddress("METPt_jesTimePtEtaup", &METPt_jesTimePtEtaup, &b_METPt_jesTimePtEtaup);
     fChain->SetBranchAddress("metPhijesTimePtEtaup", &metPhijesTimePtEtaup, &b_metPhijesTimePtEtaup);
     fChain->SetBranchAddress("numBtagsjesTimePtEtaup", &numBtagsjesTimePtEtaup, &b_numBtagsjesTimePtEtaup);
     fChain->SetBranchAddress("metleptondelphijesTimePtEtaup", &metleptondelphijesTimePtEtaup, &b_metleptondelphijesTimePtEtaup);
     fChain->SetBranchAddress("minLeptonJetDrjesTimePtEtaup", &minLeptonJetDrjesTimePtEtaup, &b_minLeptonJetDrjesTimePtEtaup);
     fChain->SetBranchAddress("JetPtjesTimePtEtadown", &JetPtjesTimePtEtadown, &b_JetPtjesTimePtEtadown);
     fChain->SetBranchAddress("jetEtajesTimePtEtadown", &jetEtajesTimePtEtadown, &b_jetEtajesTimePtEtadown);
     fChain->SetBranchAddress("jetPhijesTimePtEtadown", &jetPhijesTimePtEtadown, &b_jetPhijesTimePtEtadown);
     fChain->SetBranchAddress("jetMassjesTimePtEtadown", &jetMassjesTimePtEtadown, &b_jetMassjesTimePtEtadown);
     fChain->SetBranchAddress("METPt_jesTimePtEtadown", &METPt_jesTimePtEtadown, &b_METPt_jesTimePtEtadown);
     fChain->SetBranchAddress("metPhijesTimePtEtadown", &metPhijesTimePtEtadown, &b_metPhijesTimePtEtadown);
     fChain->SetBranchAddress("numBtagsjesTimePtEtadown", &numBtagsjesTimePtEtadown, &b_numBtagsjesTimePtEtadown);
     fChain->SetBranchAddress("metleptondelphijesTimePtEtadown", &metleptondelphijesTimePtEtadown, &b_metleptondelphijesTimePtEtadown);
     fChain->SetBranchAddress("minLeptonJetDrjesTimePtEtadown", &minLeptonJetDrjesTimePtEtadown, &b_minLeptonJetDrjesTimePtEtadown);
     fChain->SetBranchAddress("METPt_unclustenup", &METPt_unclustenup, &b_METPt_unclustenup);
     fChain->SetBranchAddress("METPt_unclustendown", &METPt_unclustendown, &b_METPt_unclustendown);
     fChain->SetBranchAddress("metPhiunclustenup", &metPhiunclustenup, &b_metPhiunclustenup);
     fChain->SetBranchAddress("metPhiunclustendown", &metPhiunclustendown, &b_metPhiunclustendown);
     fChain->SetBranchAddress("metleptondelphiunclustenup", &metleptondelphiunclustenup, &b_metleptondelphiunclustenup);
     fChain->SetBranchAddress("metleptondelphiunclustendown", &metleptondelphiunclustendown, &b_metleptondelphiunclustendown);
     fChain->SetBranchAddress("Pileup_scaleFactor", &Pileup_scaleFactor, &b_Pileup_scaleFactor);
     fChain->SetBranchAddress("Pileup_scaleFactorUp", &Pileup_scaleFactorUp, &b_Pileup_scaleFactorUp);
     fChain->SetBranchAddress("Pileup_scaleFactorDown", &Pileup_scaleFactorDown, &b_Pileup_scaleFactorDown);
     /*fChain->SetBranchAddress("nGenPart", &nGenPart, &b_nGenPart);
     fChain->SetBranchAddress("GenPart_pt", GenPart_pt, &b_GenPart_pt);
     fChain->SetBranchAddress("GenPart_eta", GenPart_eta, &b_GenPart_eta);
     fChain->SetBranchAddress("GenPart_phi", GenPart_phi, &b_GenPart_phi);
     fChain->SetBranchAddress("GenPart_mass", GenPart_mass, &b_GenPart_mass);
     fChain->SetBranchAddress("GenPart_pdgId", GenPart_pdgId, &b_GenPart_pdgId);
     fChain->SetBranchAddress("GenPart_genPartIdxMother", GenPart_genPartIdxMother, &b_GenPart_genPartIdxMother);
     fChain->SetBranchAddress("nGenJet", &nGenJet, &b_nGenJet);
     fChain->SetBranchAddress("GenJet_pt", GenJet_pt, &b_GenJet_pt);
     fChain->SetBranchAddress("GenJet_eta", GenJet_eta, &b_GenJet_eta);
     fChain->SetBranchAddress("GenJet_phi", GenJet_phi, &b_GenJet_phi);
     fChain->SetBranchAddress("GenJet_mass", GenJet_mass, &b_GenJet_mass);
     fChain->SetBranchAddress("jetgenjetidx", &jetgenjetidx, &b_jetgenjetidx);*/
  }

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
