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
void ScaleFactorTTbarCalc(int bin=1152, TString year="2018"){
 
  TString B2Gn = "xxyyy"; //placeholder, until we get a cadi line number
  TString YearS = year;
  if(year == "2016_APV") YearS="2016apv";
 
  /*vector<TString> variations = {"" // 0
    , "electronScale"+sYear+ "Up", "electronScale"+sYear+ "Down", "electronRes"+sYear+ "Up", "electronRes"+sYear+ "Down", "JES"+sYear+ "Up", "JES"+sYear+ "Down", "JER"+sYear+ "Up", "JER"+sYear+ "Down" // 1 - 8
    , "electron"+sYear+ "Up", "electron"+sYear+ "Down", "muonTrigger"+sYear+ "Up", "muonTrigger"+sYear+ "Down", "muonId"+sYear+ "Up", "muonId"+sYear+ "Down", "muonIso"+sYear+ "Up", "muonIso"+sYear+ "Down" // 9 - 16
    , "BjetTagCorrUp", "BjetTagCorrDown", "BjetTagUncorr"+sYear+ "Up", "BjetTagUncorr"+sYear+ "Down", "PUID"+sYear+ "Up", "PUID"+sYear+ "Down", "L1PreFiring"+sYear+ "Up", "L1PreFiring"+sYear+ "Down" // 17 - 24
    , "PUreweight"+sYear+ "Up", "PUreweight"+sYear+ "Down", "PDFUp", "PDFDown", "LHEScaleUp", "LHEScaleDown", // 25 - 30
      "LumiCorrUp", "LumiCorrDown", "LumiStat"+sYear+"Up", "LumiStat"+sYear+"Down" //31-34
  };*/

  TH1F dataHist;
  vector<vector<TH1F> > SimHists;

  //also derive negative log likelihood residuals
  vector<TH1F> dataHistNLL;
  vector<vector<TH1F> > SimHistsNLL;

  //sample-type-specific variations need special treatment
  vector<TString> SampleTypes = {"ttbar", "wjets", "single_top", "diboson"}; //list of background sample types
  for(unsigned s = 0; s < SampleTypes.size(); ++s){
    vector<TH1F> dummy;
    SimHists.push_back(dummy);
    SimHistsNLL.push_back(dummy);
  }

  //set variation size
  unsigned varSize = 47;

  //loop over samples, organizing data, ttbar, and non-ttbar with variations
  for(unsigned i = 0; i < 24; ++i){
    if(bin/1000 == 1 && i == 0) continue;
    if(bin/2000 == 1 && i == 1) continue;

    //load dataset information
    Dataset dset = dlib.GetDataset(i);
    TString gn = dset.GroupName;
    TString sampleType = gn;

    //version with CMS standard names
    vector<TString> variations = {"", //0: nominal
          TString("CMS_scale_e_")                  +YearS+"Up", TString("CMS_scale_e")                   +YearS+"Down",  //1-2:   electron energy scale pT variation (on data)
          TString("CMS_res_e_")                    +YearS+"Up", TString("CMS_res_e_")                    +YearS+"Down",  //3-4:   electron energy resolution pT variation
          TString("CMS_scale_j_")                  +YearS+"Up", TString("CMS_scale_j_")                  +YearS+"Down",  //5-6:   jet energy scale pT variation
          TString("CMS_res_j_")                    +YearS+"Up", TString("CMS_res_j_")                    +YearS+"Down",  //7-8:   jet energy resolution pT variation
          TString("CMS_eff_e_trigger_")            +YearS+"Up", TString("CMS_eff_e_trigger_")            +YearS+"Down",  //9-10:         electron trigger efficiency variation, including HLT Zvtx for 2017
          TString("CMS_eff_e_reco_")               +YearS+"Up", TString("CMS_eff_e_reco_")               +YearS+"Down",  //11-12: electron reconstruction efficiency variation
          TString("CMS_eff_e_")                    +YearS+"Up", TString("CMS_eff_e_")                    +YearS+"Down",  //13-14:  electron ID (including ISO) variation
          TString("CMS_eff_m_trigger_")            +YearS+"Up", TString("CMS_eff_m_trigger_")            +YearS+"Down",  //15-16: muon trigger efficiency variation
          TString("CMS_eff_m_id_")                 +YearS+"Up", TString("CMS_eff_m_id_")                 +YearS+"Down",  //17-18: muon ID efficiency variation
          TString("CMS_eff_m_iso_")                +YearS+"Up", TString("CMS_eff_m_iso_")                +YearS+"Down",  //19-20: muon ISO efficiency variation
          TString("CMS_btag_light")                      +"Up", TString("CMS_btag_light")                      +"Down",  //21-22: correlated component of b-tagging efficiency
          TString("CMS_btag_heavy")                      +"Up", TString("CMS_btag_heavy")                      +"Down",  //23-24: correlated component of b-tagging efficiency
          TString("CMS_btag_light_")               +YearS+"Up", TString("CMS_btag_light_")               +YearS+"Down",  //25-26: uncorrelated component of b-tagging efficiency
          TString("CMS_btag_heavy_")               +YearS+"Up", TString("CMS_btag_heavy_")               +YearS+"Down",  //27-28: cunorrelated component of b-tagging efficiency
          TString("CMS_eff_j_PUJET_id_")           +YearS+"Up", TString("CMS_eff_j_PUJET_id_")           +YearS+"Down",  //29-30: uncertaintiy of PU jet ID efficiency
          TString("CMS_l1_ecal_prefiring_")        +YearS+"Up", TString("CMS_l1_ecal_prefiring_")        +YearS+"Down",  //31-32: L1 ECAL prefiring issue in 2016 and 2017 only
          TString("CMS_pileup")                          +"Up", TString("CMS_pileup")                          +"Down",  //33-34: CMS pileup reweighting uncertainty, correlated for Run2
          TString("pdf_B2G")+B2Gn+"_envelope_"+sampleType+"Up", TString("pdf_B2G")+B2Gn+"_envelope_"+sampleType+"Down",  //35-36: Envelope of largest variations of 100 PDF variations
          TString("QCDscale_")+sampleType                +"Up", TString("QCDscale_")+sampleType                +"Down",  //37-38: ISR/FSR uncertainties
          TString("lumi_13TeV_correlated")               +"Up", TString("lumi_13TeV_correlated")               +"Down",  //39-40: correlated luminosity variation for 13 TeV
          TString("lumi_13TeV_1718")                     +"Up", TString("lumi_13TeV_1718")                     +"Down",  //41-42: correlation luminosity variation for 2017 and 2018
          TString("lumi_")                         +YearS+"Up", TString("lumi_")                         +YearS+"Down",  //43-44: uncorrelated luminosity variation by year
          TString("CMS_eff_e_HLTzvtx_17")                +"Up", TString("CMS_eff_e_HLTzvtx_17")                +"Down"   //45-46: 2017 only electron Z vtx window of HLT inefficiency uncertainty
    };
    if(variations.size() != varSize) std::cout<<"!!!WARNING: Variations differ in size: "<<variations.size()<<" in vector vs "<<varSize<<" set."<<std::endl;

    //load infile per sample
    TFile *infile;
    TString FileLoc = TString::Format("TestHistograms/SimpleShapes_Bin%d_",bin) + YearS + TString::Format("_%d.root",i);
    infile = new TFile(FileLoc, "READ");

    //sort the samples into histograms with all variations
    if(i<=1){//data
      TString dataName = TString::Format("ST_data_obs_Wprime%d_",bin)+ YearS + "_";
      dataHist = *(TH1F*)(infile->Get(dataName))->Clone("dataHist");
      for(unsigned mass = 300; mass < 1200; mass+=100){
	TString MassDataName = TString::Format("NegLogLnoB_Data_Wprime%d_",bin) + YearS + TString::Format("_M%d_",mass);
	dataHistNLL.push_back( *(TH1F*)(infile->Get(MassDataName))->Clone(TString::Format("dataHistNLL_M%d",mass)));
      }
    }
    else if(i==2 || i==5 || i==13 || i==18){//first sample in each sample set
      for(unsigned j = 0; j < variations.size(); ++j){
        TString TmpName = TString("ST_") + gn + TString::Format("_Wprime%d_",bin) + YearS + "_" + variations[j];
        std::cout<<"creating "<<TmpName<<std::endl;
        SimHists[dset.Color-2].push_back(*(TH1F*)(infile->Get(TmpName)->Clone(gn + TString::Format("_%d",j))));
      }
      for(unsigned mass = 300; mass < 1200; mass+=100){
        TString TmpName = TString("NegLogLnoB_") + gn + TString::Format("_Wprime%d_",bin) + YearS + TString::Format("_M%d_",mass);
	SimHistsNLL[dset.Color-2].push_back(*(TH1F*)(infile->Get(TmpName)->Clone(gn + TString::Format("NLL_M%d",mass))));
      }
    }
    else{//additional samples in each sample set
      for(unsigned j = 0; j < variations.size(); ++j){
        TString TmpName = TString("ST_") + gn + TString::Format("_Wprime%d_",bin) + YearS + "_" + variations[j];
        std::cout<<"adding "<<TmpName<<std::endl;
        SimHists[dset.Color-2][i].Add((TH1F*)(infile->Get(TmpName)));
      }
      for(unsigned mass = 300; mass < 1200; mass+=100){
        TString TmpName = TString("NegLogLnoB_") + gn + TString::Format("_Wprime%d_",bin) + YearS + TString::Format("_M%d_",mass);
        SimHistsNLL[dset.Color-2][mass/100-3].Add((TH1F*)(infile->Get(TmpName)));
      }
    }
  }

  //use variations gathered to calculate central SF histogram and variation SF histograms
  vector<TH1F> SFhists, SFs;
  for(unsigned i = 0; i < varSize; ++i){

    //version with CMS standard names
    vector<TString> variations = {"", //0: nominal
          TString("CMS_scale_e_")                  +YearS+"Up", TString("CMS_scale_e")                   +YearS+"Down",  //1-2:   electron energy scale pT variation (on data)
          TString("CMS_res_e_")                    +YearS+"Up", TString("CMS_res_e_")                    +YearS+"Down",  //3-4:   electron energy resolution pT variation
          TString("CMS_scale_j_")                  +YearS+"Up", TString("CMS_scale_j_")                  +YearS+"Down",  //5-6:   jet energy scale pT variation
          TString("CMS_res_j_")                    +YearS+"Up", TString("CMS_res_j_")                    +YearS+"Down",  //7-8:   jet energy resolution pT variation
          TString("CMS_eff_e_trigger_")            +YearS+"Up", TString("CMS_eff_e_trigger_")            +YearS+"Down",  //9-10:         electron trigger efficiency variation, including HLT Zvtx for 2017
          TString("CMS_eff_e_reco_")               +YearS+"Up", TString("CMS_eff_e_reco_")               +YearS+"Down",  //11-12: electron reconstruction efficiency variation
          TString("CMS_eff_e_")                    +YearS+"Up", TString("CMS_eff_e_")                    +YearS+"Down",  //13-14:  electron ID (including ISO) variation
          TString("CMS_eff_m_trigger_")            +YearS+"Up", TString("CMS_eff_m_trigger_")            +YearS+"Down",  //15-16: muon trigger efficiency variation
          TString("CMS_eff_m_id_")                 +YearS+"Up", TString("CMS_eff_m_id_")                 +YearS+"Down",  //17-18: muon ID efficiency variation
          TString("CMS_eff_m_iso_")                +YearS+"Up", TString("CMS_eff_m_iso_")                +YearS+"Down",  //19-20: muon ISO efficiency variation
          TString("CMS_btag_light")                      +"Up", TString("CMS_btag_light")                      +"Down",  //21-22: correlated component of b-tagging efficiency
          TString("CMS_btag_heavy")                      +"Up", TString("CMS_btag_heavy")                      +"Down",  //23-24: correlated component of b-tagging efficiency
          TString("CMS_btag_light_")               +YearS+"Up", TString("CMS_btag_light_")               +YearS+"Down",  //25-26: uncorrelated component of b-tagging efficiency
          TString("CMS_btag_heavy_")               +YearS+"Up", TString("CMS_btag_heavy_")               +YearS+"Down",  //27-28: cunorrelated component of b-tagging efficiency
          TString("CMS_eff_j_PUJET_id_")           +YearS+"Up", TString("CMS_eff_j_PUJET_id_")           +YearS+"Down",  //29-30: uncertaintiy of PU jet ID efficiency
          TString("CMS_l1_ecal_prefiring_")        +YearS+"Up", TString("CMS_l1_ecal_prefiring_")        +YearS+"Down",  //31-32: L1 ECAL prefiring issue in 2016 and 2017 only
          TString("CMS_pileup")                          +"Up", TString("CMS_pileup")                          +"Down",  //33-34: CMS pileup reweighting uncertainty, correlated for Run2
          TString("dummy"), TString("dummy"),  //35-36: Envelope of largest variations of 100 PDF variations
          TString("dummy"), TString("dummy"),  //37-38: ISR/FSR uncertainties
          TString("lumi_13TeV_correlated")               +"Up", TString("lumi_13TeV_correlated")               +"Down",  //39-40: correlated luminosity variation for 13 TeV
          TString("lumi_13TeV_1718")                     +"Up", TString("lumi_13TeV_1718")                     +"Down",  //41-42: correlation luminosity variation for 2017 and 2018
          TString("lumi_")                         +YearS+"Up", TString("lumi_")                         +YearS+"Down",  //43-44: uncorrelated luminosity variation by year
          TString("CMS_eff_e_HLTzvtx_17")                +"Up", TString("CMS_eff_e_HLTzvtx_17")                +"Down"   //45-46: 2017 only electron Z vtx window of HLT inefficiency uncertainty
    };

    //except sample-dependent uncertainties for extra logic
    //at this stage, variation i departs from being concurrent with SF histogram iterator, so we just keep appending each different variation
    if(i >= 35 && i <= 38) for(unsigned j = 0; j < SampleTypes.size(); ++j){ //loop over different samples{
      TString sampleType = SampleTypes[j];
      vector<TString> variationsSpecial = {
        "pdf_B2G"+B2Gn+"_envelope_"+sampleType+"Up",  "pdf_B2G"+B2Gn+"_envelope_"+sampleType+"Down",  //35-36: Envelope of largest variations of 100 PDF variations
        "QCDscale_"+sampleType                +"Up",  "QCDscale_"+sampleType                +"Down",  //37-38: ISR/FSR uncertainties
      };
      SFhists.push_back(*(TH1F*)dataHist.Clone("SF_"+variationsSpecial[i]));
      unsigned currentPos = SFhists.size()-1;
      for(unsigned k = 1; k < SimHists.size(); ++k){
	if(j == k) SFhists[currentPos].Add(&SimHists[k][i],-1); //if this is the variation sample, add variation
	else       SFhists[currentPos].Add(&SimHists[k][0],-1); //else use nominal variation
      }
      if(j == 0) SFhists[currentPos].Divide(&SimHists[0][i]);   //if this is the ttbar variation, divide by variation
      else       SFhists[currentPos].Divide(&SimHists[0][0]);   //else divide by nominal ttbar

      //cleaning function for bins with no data or negative values
      for(unsigned x = 0; x < dataHist.GetNbinsX(); ++x){
        if(dataHist.GetBinContent(x+1) < 10. || SFhists[currentPos].GetBinContent(x+1) < 0.){
     	  SFhists[currentPos].SetBinContent(x+1,0.);
	  SFhists[currentPos].SetBinError(x+1,0.);
	}
      }
      std::cout<<variationsSpecial[i]<<" integrated SF yields per bin "<<SFhists[currentPos].Integral()/SFhists[currentPos].GetNbinsX()<<std::endl;

      SFs.push_back(*(TH1F*)SFhists[currentPos].Clone("SFcalc_"+variationsSpecial[i]));
    }
    else{//standard case
      if(variations[i]=="dummy") std::cout<<"Fail!!! Dummy variations mistakenly loaded, instead of specially treating them!"<<std::endl;
      SFhists.push_back(*(TH1F*)dataHist.Clone("SF_"+variations[i]));
      unsigned currentPos = SFhists.size()-1;
      for(unsigned k = 1; k < SimHists.size(); ++k) SFhists[currentPos].Add(&SimHists[k][i],-1);
      SFhists[currentPos].Divide(&SimHists[0][i]);

      //cleaning function for bins with no data or negative values
      for(unsigned x = 0; x < dataHist.GetNbinsX(); ++x){
        if(dataHist.GetBinContent(x+1) < 10. || SFhists[currentPos].GetBinContent(x+1) < 0.){
          SFhists[currentPos].SetBinContent(x+1,0.);
          SFhists[currentPos].SetBinError(x+1,0.);
        }
      }
      std::cout<<variations[i]<<" integrated SF yields per bin "<<SFhists[currentPos].Integral()/SFhists[currentPos].GetNbinsX()<<std::endl;

      SFs.push_back(*(TH1F*)SFhists[currentPos].Clone("SFcalc_"+variations[i]));
    }
  }

  TString SaveName = TString::Format("TestHistograms/SF_Bin%d_",bin) + YearS + ".root";
  TFile *savefile = new TFile(SaveName,"RECREATE");

  //make NLL residual distribution
  //This includes data / all MC components
  //ttbar in the 2-tag regions is supposed to be ST-reweighted in this workflow within the CombineHistogramDumpster
  for(unsigned m = 3; m < 12; ++m){
    TString TmpName = TString::Format("NLLresidual_%d_",bin) + YearS + TString::Format("_M%d",m*100);
    TH1F NLLresidual = *(TH1F*)dataHistNLL[m-3].Clone(TmpName);
    for(unsigned i = 1; i < SimHistsNLL.size(); ++i) SimHistsNLL[0][m-3].Add(&SimHistsNLL[i][m-3]);
    NLLresidual.Divide(&SimHistsNLL[0][m-3]);
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
