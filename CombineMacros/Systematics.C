#include <TROOT.h>
#include <TString.h>
#include <vector>

//archive of systematics sources and their indexing, returns the systematics name string

TString Systematics(unsigned index, TString YearS, TString sampleType, TString B2Gn, bool offdiagonal = false){
  vector<TString> variations = {"", //0: nominal
          //object pT variation uncertainties
          TString("CMS_scale_e")                         +"Up", TString("CMS_scale_e")                         +"Down",  //1-2:   electron energy scale pT variation (on data)
          TString("CMS_res_e")                           +"Up", TString("CMS_res_e")                           +"Down",  //3-4:   electron energy resolution pT variation

          TString("CMS_res_j_")                    +YearS+"Up", TString("CMS_res_j_")                    +YearS+"Down",  //5-6:   jet energy resolution pT variation
	  TString("CMS_scale_j_AbsoluteStat_")     +YearS+"Up", TString("CMS_scale_j_AbsoluteStat_")     +YearS+"Down",  //7-8:   jet energy scale Absolute stat
	  TString("CMS_scale_j_AbsoluteScale")           +"Up", TString("CMS_scale_j_AbsoluteScale")           +"Down",  //9-10:  jet energy scale Absolute scale
	  TString("CMS_scale_j_AbsoluteMPFBias")         +"Up", TString("CMS_scale_j_AbsoluteMPFBias")         +"Down",  //11-12: jet energy scale Absolute MPF Bias
	  TString("CMS_scale_j_FlavorQCD")               +"Up", TString("CMS_scale_j_FlavorQCD")	       +"Down",  //13-14: jet energy scale Flavor QCD
	  TString("CMS_scale_j_Fragmentation")           +"Up", TString("CMS_scale_j_Fragmentation")	       +"Down",  //15-16: jet energy scale Fragmentation
	  TString("CMS_scale_j_PileUpDataMC")		 +"Up", TString("CMS_scale_j_PileUpDataMC")	       +"Down",  //17-18: jet energy scale PileUp Data/MC
	  TString("CMS_scale_j_PileUpPtBB")		 +"Up", TString("CMS_scale_j_PileUpPtBB")	       +"Down",  //19-20: jet energy scale PileUp PT BB
	  TString("CMS_scale_j_PileUpPtEC1")             +"Up", TString("CMS_scale_j_PileUpPtEC1")             +"Down",  //21-22: jet energy scale PileUp PT EC1
	  TString("CMS_scale_j_PileUpPtEC2")             +"Up", TString("CMS_scale_j_PileUpPtEC2")             +"Down",  //23-24: jet energy scale PileUp PT EC2
	  TString("CMS_scale_j_PileUpPtHF")              +"Up", TString("CMS_scale_j_PileUpPtHF")              +"Down",  //25-26: jet energy scale PileUp PT HF
	  TString("CMS_scale_j_PileUpPtRef")             +"Up", TString("CMS_scale_j_PileUpPtRef")             +"Down",  //27-28: jet energy scale PileUp PT Ref
	  TString("CMS_scale_j_RelativeFSR")		 +"Up", TString("CMS_scale_j_RelativeFSR")	       +"Down",  //29-30: jet energy scale Relative FSR
	  TString("CMS_scale_j_RelativeJEREC1_")   +YearS+"Up", TString("CMS_scale_j_RelativeJEREC1_")	 +YearS+"Down",  //31-32: jet energy scale Relative JER EC1
          TString("CMS_scale_j_RelativeJEREC2_")   +YearS+"Up", TString("CMS_scale_j_RelativeJEREC_2")   +YearS+"Down",  //33-34: jet energy scale Relative JER EC2
          TString("CMS_scale_j_RelativeJERHF")           +"Up", TString("CMS_scale_j_RelativeJERHF")           +"Down",  //35-36: jet energy scale Relative JER HF
	  TString("CMS_scale_j_RelativePtBB")		 +"Up", TString("CMS_scale_j_RelativePtBB")	       +"Down",  //37-38: jet energy scale Relative Pt BB
	  TString("CMS_scale_j_RelativePtEC1_")    +YearS+"Up", TString("CMS_scale_j_RelativePtEC1_")    +YearS+"Down",  //39-40: jet energy scale Relative Pt EC1
	  TString("CMS_scale_j_RelativePtEC2_")    +YearS+"Up", TString("CMS_scale_j_RelativePtEC2_")    +YearS+"Down",  //41-42: jet energy scale Relative Pt EC2
	  TString("CMS_scale_j_RelativePtHF")            +"Up", TString("CMS_scale_j_RelativePtHF")            +"Down",  //43-44: jet energy scale Relative Pt HF
	  TString("CMS_scale_j_RelativeBal")		 +"Up", TString("CMS_scale_j_RelativeBal") 	       +"Down",  //45-46: jet energy scale Relative Balance
	  TString("CMS_scale_j_RelativeSample_")   +YearS+"Up", TString("CMS_scale_j_RelativeSample_")   +YearS+"Down",  //47-48: jet energy scale Relative Sample
	  TString("CMS_scale_j_RelativeStatEC_")   +YearS+"Up", TString("CMS_scale_j_RelativeStatEC_")   +YearS+"Down",  //49-50: jet energy scale Relative Stat EC
	  TString("CMS_scale_j_RelativeStatFSR_")  +YearS+"Up", TString("CMS_scale_j_RelativeStatFSR_")  +YearS+"Down",  //51-52: jet energy scale Relative Stat FSR
	  TString("CMS_scale_j_RelativeStatHF_")   +YearS+"Up", TString("CMS_scale_j_RelativeStatHF_")   +YearS+"Down",  //53-54: jet energy scale Relative Stat HF
	  TString("CMS_scale_j_PionECAL")                +"Up", TString("CMS_scale_j_PionECAL")                +"Down",  //55-56: jet energy scale Single Pion ECAL
	  TString("CMS_scale_j_PionHCAL")                +"Up", TString("CMS_scale_j_PionHCAL")                +"Down",  //57-58: jet energy scale Single Pion HCAL
	  TString("CMS_scale_j_TimePtEta_")	   +YearS+"Up", TString("CMS_scale_j_TimePtEta_")	 +YearS+"Down",  //59-60: jet energy scale TimePtEta
	  TString("CMS_scale_met_unclustered_energy_")+YearS+"Up", TString("CMS_scale_met_unclustered_energy_")+YearS+"Down", //61-62: MET unclustered energy uncertainty

          //event weight variation uncertainties
          TString("CMS_eff_e_trigger")                   +"Up", TString("CMS_eff_e_trigger")                   +"Down",  //63-64:  electron trigger efficiency variation, including HLT Zvtx for 2017
          TString("CMS_eff_e_reco")                      +"Up", TString("CMS_eff_e_reco")                      +"Down",  //65-66: electron reconstruction efficiency variation
          TString("CMS_eff_e")                           +"Up", TString("CMS_eff_e")                           +"Down",  //67-68: electron ID (including ISO) variation
          TString("CMS_eff_m_trigger_")            +YearS+"Up", TString("CMS_eff_m_trigger_")            +YearS+"Down",  //69-70: muon trigger efficiency variation
          TString("CMS_eff_m_id_")                 +YearS+"Up", TString("CMS_eff_m_id_")                 +YearS+"Down",  //71-72: muon ID efficiency variation
          TString("CMS_eff_m_iso_")                +YearS+"Up", TString("CMS_eff_m_iso_")                +YearS+"Down",  //73-74: muon ISO efficiency variation
          TString("CMS_btag_light")                      +"Up", TString("CMS_btag_light")                      +"Down",  //75-76: correlated component across years of b-tagging efficiency
          TString("CMS_btag_heavy")                      +"Up", TString("CMS_btag_heavy")                      +"Down",  //77-78: correlated component across years of b-tagging efficiency
          TString("CMS_btag_light_")               +YearS+"Up", TString("CMS_btag_light_")               +YearS+"Down",  //79-80: uncorrelated component across years of b-tagging efficiency
          TString("CMS_btag_heavy_")               +YearS+"Up", TString("CMS_btag_heavy_")               +YearS+"Down",  //81-82: uncorrelated component across years of b-tagging efficiency             
          TString("CMS_eff_j_PUJET_id_")           +YearS+"Up", TString("CMS_eff_j_PUJET_id_")           +YearS+"Down",  //83-84: uncertaintiy of PU jet ID efficiency
          TString("CMS_l1_ecal_prefiring_")        +YearS+"Up", TString("CMS_l1_ecal_prefiring_")        +YearS+"Down",  //85-86: L1 ECAL prefiring issue in 2016 and 2017 only
          TString("CMS_pileup")                          +"Up", TString("CMS_pileup")                          +"Down",  //87-88: CMS pileup reweighting uncertainty, correlated for Run2
          TString("pdf_B2G")+B2Gn+"_envelope_"+sampleType+"Up", TString("pdf_B2G")+B2Gn+"_envelope_"+sampleType+"Down",  //89-90: 16th and 84th percentile of 103 PDF variations
          TString("QCDscale_ren_")+sampleType            +"Up", TString("QCDscale_ren_")+sampleType            +"Down",  //91-92: PDF renormalization scale uncertainty by sample
          TString("QCDscale_fac_")+sampleType            +"Up", TString("QCDscale_fac_")+sampleType            +"Down",  //93-94: PDF factorization scale uncertainty by sample
          TString("ps_isr")                              +"Up", TString("ps_isr")                              +"Down",  //95-96: PS ISR uncertainty
          TString("ps_fsr")                              +"Up", TString("ps_fsr")                              +"Down",  //97-98: PS FSR uncertainty

          //uncertainties CombineHistogramDumpster takes care of, since they are hardcoded numbers
          /*TString("lumi_13TeV_correlated")               +"Up", TString("lumi_13TeV_correlated")               +"Down",  //45-46: correlated luminosity variation for 13 TeV
          TString("lumi_13TeV_1718")                     +"Up", TString("lumi_13TeV_1718")                     +"Down",  //47-48: correlation luminosity variation for 2017 and 2018
          TString("lumi_")                         +YearS+"Up", TString("lumi_")                         +YearS+"Down",  //49-50: uncorrelated luminosity variation by year
          TString("CMS_eff_e_HLTzvtx_17")                +"Up", TString("CMS_eff_e_HLTzvtx_17")                +"Down"   //51-52: 2017 only electron Z vtx window of HLT inefficiency uncertainty*/
  };

  vector<TString> offvariations = {
  	TString("pdf_B2G")+B2Gn+"_envelope_wjetsUp",		TString("pdf_B2G")+B2Gn+"_envelope_wjetsDown",  	//0-1:   offdiagonal PDF variations: wjets
	TString("pdf_B2G")+B2Gn+"_envelope_single_topUp",	TString("pdf_B2G")+B2Gn+"_envelope_single_topDown",  	//2-3:   offdiagonal PDF variations: single top
	TString("pdf_B2G")+B2Gn+"_envelope_dibosonUp",		TString("pdf_B2G")+B2Gn+"_envelope_dibosonDown",  	//4-5:   offdiagonal PDF variations: diboson
	TString("QCDscale_ren_wjetsUp"),			TString("QCDscale_ren_wjetsDown"),  			//6-7:   offdiagonal QCD scale variations: wjets
	TString("QCDscale_ren_single_topUp"),                   TString("QCDscale_ren_single_topDown"),                 //8-9:   offdiagonal QCD scale variations: single top
	TString("QCDscale_ren_dibosonUp"),                      TString("QCDscale_ren_dibosonDown"),                    //10-11: offdiagonal QCD scale variations: diboson
	TString("QCDscale_fac_wjetsUp"),                        TString("QCDscale_fac_wjetsDown"),                      //12-13: offdiagonal QCD scale variations: wjets
        TString("QCDscale_fac_single_topUp"),                   TString("QCDscale_fac_single_topDown"),                 //14-15: offdiagonal QCD scale variations: single top
        TString("QCDscale_fac_dibosonUp"),                      TString("QCDscale_fac_dibosonDown"),                    //16-17: offdiagonal QCD scale variations: diboson
  };
  
  if(!offdiagonal) return variations[index];
  else             return offvariations[index];
}
