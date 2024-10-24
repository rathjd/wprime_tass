#include <TROOT.h>
#include "../Utilities/Dataset.cc"
#include <TFile.h>
#include <TF1.h>
#include <TH1.h>
#include <vector>
#include <TString.h>
#include <TFitResult.h> 
#include <TMatrixD.h>
#include <TMath.h>

//derive ttbar SF from 2-tag regions of same multiplicity and lepton flavour, then propagate stat uncertainty envelope bin-by-bin and do syst variation histograms
void ScaleFactorTTbarCalc(int bin=1152, int year=2018){
 
  TString sYear = TString::Format("%d",year);
  TString B2Gn = "xxyyy"; //placeholder, until we get a cadi line number
 
  /*vector<TString> variations = {"" // 0
    , "electronScale"+sYear+ "Up", "electronScale"+sYear+ "Down", "electronRes"+sYear+ "Up", "electronRes"+sYear+ "Down", "JES"+sYear+ "Up", "JES"+sYear+ "Down", "JER"+sYear+ "Up", "JER"+sYear+ "Down" // 1 - 8
    , "electron"+sYear+ "Up", "electron"+sYear+ "Down", "muonTrigger"+sYear+ "Up", "muonTrigger"+sYear+ "Down", "muonId"+sYear+ "Up", "muonId"+sYear+ "Down", "muonIso"+sYear+ "Up", "muonIso"+sYear+ "Down" // 9 - 16
    , "BjetTagCorrUp", "BjetTagCorrDown", "BjetTagUncorr"+sYear+ "Up", "BjetTagUncorr"+sYear+ "Down", "PUID"+sYear+ "Up", "PUID"+sYear+ "Down", "L1PreFiring"+sYear+ "Up", "L1PreFiring"+sYear+ "Down" // 17 - 24
    , "PUreweight"+sYear+ "Up", "PUreweight"+sYear+ "Down", "PDFUp", "PDFDown", "LHEScaleUp", "LHEScaleDown", // 25 - 30
      "LumiCorrUp", "LumiCorrDown", "LumiStat"+sYear+"Up", "LumiStat"+sYear+"Down" //31-34
  };*/

  TH1F dataHist;
  vector<TH1F> ttbarHists, NonTtbarHists;

  //also derive negative log likelihood residuals
  vector<TH1F> dataHistNLL, ttbarHistNLL, NonTtbarHistNLL;

  //loop over samples, organizing data, ttbar, and non-ttbar with variations
  for(unsigned i = 0; i < 22; ++i){
    if(bin/1000 == 1 && i == 0) continue;
    if(bin/2000 == 1 && i == 1) continue;

    //load dataset information
    Dataset dset = dlib.GetDataset(i);
    TString gn = dset.GroupName;

    //version with CMS standard names
    vector<TString> variations = {"", //0: nominal
          "CMS_scale_e_"                  +sYear+"Up",  "CMS_scale_e"                   +sYear+"Down",  //1-2:   electron energy scale pT variation (on data)
          "CMS_res_e_"                    +sYear+"Up",  "CMS_res_e_"                    +sYear+"Down",  //3-4:   electron energy resolution pT variation
          "CMS_scale_j_"                  +sYear+"Up",  "CMS_scale_j_"                  +sYear+"Down",  //5-6:   jet energy scale pT variation
          "CMS_res_j_"                    +sYear+"Up",  "CMS_res_j_"                    +sYear+"Down",  //7-8:   jet energy resolution pT variation
          "CMS_eff_e_trigger_"            +sYear+"Up",  "CMS_eff_e_trigger_"            +sYear+"Down",  //?-?:   FIXME: electron trigger efficiency variation, including HLT Zvtx for 2017
          "CMS_eff_e_reco_"               +sYear+"Up",  "CMS_eff_e_reco_"               +sYear+"Down",  //?-?:   FIXME: electron reconstruction efficiency variation
          "CMS_eff_e_"                    +sYear+"Up",  "CMS_eff_e_"                    +sYear+"Down",  //9-10:  electron ID (including ISO) variation
          "CMS_eff_m_trigger_"            +sYear+"Up",  "CMS_eff_m_trigger_"            +sYear+"Down",  //11-12: muon trigger efficiency variation
          "CMS_eff_m_id_"                 +sYear+"Up",  "CMS_eff_m_id_"                 +sYear+"Down",  //13-14: muon ID efficiency variation
          "CMS_eff_m_iso_"                +sYear+"Up",  "CMS_eff_m_iso_"                +sYear+"Down",  //15-16: muon ISO efficiency variation
          "CMS_btag_comb"                       +"Up",  "CMS_btag_comb"                       +"Down",  //17-18: correlated component of b-tagging efficiency combined across all flavours
          "CMS_eff_b_"                    +sYear+"Up",  "CMS_eff_b_"                    +sYear+"Down",  //19-20: uncorrelated component across years of b-tagging efficiency combined across all flavours
          "CMS_eff_j_PUJET_id_"           +sYear+"Up",  "CMS_eff_j_PUJET_id_"           +sYear+"Down",  //21-22: uncertaintiy of PU jet ID efficiency
          "CMS_l1_ecal_prefiring_"        +sYear+"Up",  "CMS_l1_ecal_prefiring_"        +sYear+"Down",  //23-24: L1 ECAL prefiring issue in 2016 and 2017 only
          "CMS_pileup"                          +"Up",  "CMS_pileup"                          +"Down",  //25-26: CMS pileup reweighting uncertainty, correlated for Run2
          "pdf_B2G"+B2Gn+"_envelope_"+gn	+"Up",  "pdf_B2G"+B2Gn+"_envelope_"+gn	      +"Down",  //27-28: Envelope of largest variations of 100 PDF variations
          "QCDscale_"+gn	                +"Up",  "QCDscale_"+gn	                      +"Down",  //29-30: ISR/FSR uncertainties
          "lumi_13TeV_correlated"               +"Up",  "lumi_13TeV_correlated"               +"Down",  //31-32: correlated luminosity variation for 13 TeV
          "lumi_"                         +sYear+"Up",  "lumi_"                         +sYear+"Down"   //33-34: uncorrelated luminosity variation by year
    };

    //need to combine 2016 and 2016apv before determining SFs
    if(year == 2016){
      TString command = TString::Format("hadd -f TestHistograms/SimpleShapes_Bin%d_2016all_%d.root TestHistograms/SimpleShapes_Bin%d_2016_%d.root TestHistograms/SimpleShapes_Bin%d_2016apv_%d.root",bin,i,bin,i,bin,i);
      system(command);
    }
    TFile *infile;
    if(year != 2016) infile = new TFile(TString::Format("TestHistograms/SimpleShapes_Bin%d_%d_%d.root",bin,year,i),"READ");
    else	     infile = new TFile(TString::Format("TestHistograms/SimpleShapes_Bin%d_2016all_%d.root",bin,i),"READ");
    if(i<=1){
      dataHist = *(TH1F*)(infile->Get(TString::Format("ST_data_obs_Wprime%d_%d_",bin,year)))->Clone("dataHist");
      for(unsigned mass = 300; mass < 1200; mass+=100) dataHistNLL.push_back( *(TH1F*)(infile->Get(TString::Format("NegLogLnoB_Data_Wprime%d_%d_M%d_",bin,year,mass)))->Clone(TString::Format("dataHistNLL_M%d",mass)));
    }
    else if(i==2){
      for(unsigned j = 0; j < variations.size(); ++j){
	      std::cout<<TString::Format("ST_"+gn+"_Wprime%d_%d_"+variations[j],bin,year)<<std::endl;
	      ttbarHists.push_back(*(TH1F*)(infile->Get(TString::Format("ST_"+gn+"_Wprime%d_%d_"+variations[j],bin,year)))->Clone(TString::Format("ttbar_%d",j)));
      }
      for(unsigned mass = 300; mass < 1200; mass+=100) ttbarHistNLL.push_back( *(TH1F*)(infile->Get(TString::Format("NegLogLnoB_ttbar_Wprime%d_%d_M%d_",bin,year,mass)))->Clone(TString::Format("ttbarNLL_M%d",mass)));
    }
   else if (i==3){
     for(unsigned j = 0; j < variations.size(); ++j) NonTtbarHists.push_back(*(TH1F*)(infile->Get(TString::Format("ST_"+gn+"_Wprime%d_%d_"+variations[j],bin,year)))->Clone(TString::Format("nonTtbar_%d",j)));
     for(unsigned mass = 300; mass < 1200; mass+=100) NonTtbarHistNLL.push_back( *(TH1F*)(infile->Get(TString::Format("NegLogLnoB_"+gn+"_Wprime%d_%d_M%d_",bin,year,mass)))->Clone(TString::Format("nonTtbarNLL_M%d",mass)));
   }
   else{
     for(unsigned j = 0; j < variations.size(); ++j) NonTtbarHists[j].Add((TH1F*)(infile->Get(TString::Format("ST_"+gn+"_Wprime%d_%d_"+variations[j],bin,year))));
     for(unsigned mass = 300; mass < 1200; mass+=100) NonTtbarHistNLL[mass/100-3].Add((TH1F*)(infile->Get(TString::Format("NegLogLnoB_"+gn+"_Wprime%d_%d_M%d_",bin,year,mass))));
   }
  }

  //use variations gathered to calculate central SF histogram and variation SF histograms
  vector<TH1F> SFhists, SFs;
  for(unsigned i = 0; i < variations.size(); ++i){
    SFhists.push_back(*(TH1F*)dataHist.Clone("SF_"+variations[i]));
    SFhists[i].Add(&NonTtbarHists[i],-1.);
    SFhists[i].Divide(&ttbarHists[i]);
    std::cout<<SFhists[i].Integral()<<std::endl;

    //cleaning function
    for(unsigned x = 0; x < dataHist.GetNbinsX(); ++x){
      if(dataHist.GetBinContent(x+1) < 10.) {SFhists[i].SetBinContent(x+1,0.); SFhists[i].SetBinError(x+1,0.);}
      if(dataHist.GetBinContent(x+1) < NonTtbarHists[i].GetBinContent(x+1)) {SFhists[i].SetBinContent(x+1,0.); SFhists[i].SetBinError(x+1,0.);}
    }

    SFs.push_back(*(TH1F*)SFhists[i].Clone("SFcalc_"+variations[i]));

  }

  TFile *savefile = new TFile(TString::Format("TestHistograms/SF_Bin%d_%d.root",bin,year),"RECREATE");

  //make NLL residual distribution
  for(unsigned m = 3; m < 12; ++m){
    TH1F NLLresidual = *(TH1F*)dataHistNLL[m-3].Clone(TString::Format("NLLresidual_%d_%d_M%d",bin,year,m*100));
    ttbarHistNLL[m-3].Add(&NonTtbarHistNLL[m-3]);
    NLLresidual.Divide(&ttbarHistNLL[m-3]);
    NLLresidual.Write();
  }

  //define fit function
  TF1 *fitFunction;
  if(bin % 100 < 60) fitFunction = new TF1("fitFunction","[0]/x/x/x+[1]/x/x+[2]/x+[3]+[4]*x+[5]*x*x", 150., 2000.);
  else 		     fitFunction = new TF1("fitFunction","[0]/x+[1]+[2]*x+[3]*x*x", 150., 2000.);
  //fit nominal variant with statistical uncertainties only, get covariance matrix, calculate statistical envelope
  TFitResultPtr fr = SFhists[0].Fit(fitFunction,"SRF");
  TMatrixD cov = fr->GetCovarianceMatrix();
  for(unsigned i = 0; i < SFhists[0].GetNbinsX(); ++i){
    double bc = SFhists[0].GetBinCenter(i+1);
    vector<double> Derivatives;
    if(bin % 100 < 60) Derivatives = {1./bc/bc/bc, 1./bc/bc, 1./bc, 1., bc, bc*bc};
    else 	       Derivatives = {1./bc, 1., bc, bc*bc};
    float FinalEnvelope = 0.;
    //scan covariance matrix
    for(unsigned x = 0; x < Derivatives.size(); ++x){
      for(unsigned y = 0; y < Derivatives.size(); ++y){
	FinalEnvelope += Derivatives[x] * Derivatives[y] * cov(x,y);
      }
    }
    SFs[0].SetBinContent(i+1, fitFunction->Eval(bc));
    SFs[0].SetBinError(i+1, TMath::Sqrt(FinalEnvelope));
  }

  //fit systematic variations
  for(unsigned i = 1; i < SFhists.size(); ++i){
    SFhists[i].Fit(fitFunction,"R");
    for(unsigned j = 0; j < SFhists[i].GetNbinsX(); ++j){
      SFs[i].SetBinContent(j+1, fitFunction->Eval(SFhists[i].GetBinCenter(j+1)));
      SFs[i].SetBinError(j+1, 0.);
    }
  }

  for(unsigned i = 0; i < SFhists.size(); ++i){
    SFhists[i].Write();
    SFs[i].Write();
  }
  savefile->Close();
} 
