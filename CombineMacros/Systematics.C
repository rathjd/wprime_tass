#include <TROOT.h>
#include <TString.h>
#include <vector>

//archive of systematics sources and their indexing, returns the systematics name string

TString Systematics(unsigned index, TString YearS, TString sampleType, TString B2Gn, bool offdiagonal = false){
  vector<TString> variations = {"", //0: nominal
          //object pT variation uncertainties
          TString("CMS_scale_e_")                        +"Up", TString("CMS_scale_e_")                        +"Down",  //1-2:   electron energy scale pT variation (on data)
          TString("CMS_res_e_")                          +"Up", TString("CMS_res_e_")                          +"Down",  //3-4:   electron energy resolution pT variation
          TString("CMS_scale_j_")                  +YearS+"Up", TString("CMS_scale_j_")                  +YearS+"Down",  //5-6:   jet energy scale pT variation
          TString("CMS_res_j_")                    +YearS+"Up", TString("CMS_res_j_")                    +YearS+"Down",  //7-8:   jet energy resolution pT variation

          //event weight variation uncertainties
          TString("CMS_eff_e_trigger_")                  +"Up", TString("CMS_eff_e_trigger_")                  +"Down",  //9-10:  electron trigger efficiency variation, including HLT Zvtx for 2017
          TString("CMS_eff_e_reco_")                     +"Up", TString("CMS_eff_e_reco_")                     +"Down",  //11-12: electron reconstruction efficiency variation
          TString("CMS_eff_e_")                          +"Up", TString("CMS_eff_e_")                          +"Down",  //13-14: electron ID (including ISO) variation
          TString("CMS_eff_m_trigger_")            +YearS+"Up", TString("CMS_eff_m_trigger_")            +YearS+"Down",  //15-16: muon trigger efficiency variation
          TString("CMS_eff_m_id_")                 +YearS+"Up", TString("CMS_eff_m_id_")                 +YearS+"Down",  //17-18: muon ID efficiency variation
          TString("CMS_eff_m_iso_")                +YearS+"Up", TString("CMS_eff_m_iso_")                +YearS+"Down",  //19-20: muon ISO efficiency variation
          TString("CMS_btag_light")                      +"Up", TString("CMS_btag_light")                      +"Down",  //21-22: correlated component across years of b-tagging efficiency
          TString("CMS_btag_heavy")                      +"Up", TString("CMS_btag_heavy")                      +"Down",  //23-24: correlated component across years of b-tagging efficiency
          TString("CMS_btag_light_")               +YearS+"Up", TString("CMS_btag_light_")               +YearS+"Down",  //25-26: uncorrelated component across years of b-tagging efficiency
          TString("CMS_btag_heavy_")               +YearS+"Up", TString("CMS_btag_heavy_")               +YearS+"Down",  //27-28: uncorrelated component across years of b-tagging efficiency             
          TString("CMS_eff_j_PUJET_id_")           +YearS+"Up", TString("CMS_eff_j_PUJET_id_")           +YearS+"Down",  //29-30: uncertaintiy of PU jet ID efficiency
          TString("CMS_l1_ecal_prefiring_")        +YearS+"Up", TString("CMS_l1_ecal_prefiring_")        +YearS+"Down",  //31-32: L1 ECAL prefiring issue in 2016 and 2017 only
          TString("CMS_pileup")                          +"Up", TString("CMS_pileup")                          +"Down",  //33-34: CMS pileup reweighting uncertainty, correlated for Run2
          TString("pdf_B2G")+B2Gn+"_envelope_"+sampleType+"Up", TString("pdf_B2G")+B2Gn+"_envelope_"+sampleType+"Down",  //35-36: 16th and 84th percentile of 103 PDF variations
          TString("QCDscale_ren_")+sampleType            +"Up", TString("QCDscale_ren_")+sampleType            +"Down",  //37-38: PDF renormalization scale uncertainty by sample
          TString("QCDscale_fac_")+sampleType            +"Up", TString("QCDscale_fac_")+sampleType            +"Down",  //39-40: PDF factorization scale uncertainty by sample
          TString("ps_isr")                              +"Up", TString("ps_isr")                              +"Down",  //41-42: PS ISR uncertainty
          TString("ps_fsr")                              +"Up", TString("ps_fsr")                              +"Down",  //43-44: PS FSR uncertainty

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
