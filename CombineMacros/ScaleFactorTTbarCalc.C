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
 
  vector<TString> variations = {"" // 0
    , "electronScale"+sYear+ "Up", "electronScale"+sYear+ "Down", "electronRes"+sYear+ "Up", "electronRes"+sYear+ "Down", "JES"+sYear+ "Up", "JES"+sYear+ "Down", "JER"+sYear+ "Up", "JER"+sYear+ "Down" // 1 - 8
    , "electron"+sYear+ "Up", "electron"+sYear+ "Down", "muonTrigger"+sYear+ "Up", "muonTrigger"+sYear+ "Down", "muonId"+sYear+ "Up", "muonId"+sYear+ "Down", "muonIso"+sYear+ "Up", "muonIso"+sYear+ "Down" // 9 - 16
    , "BjetTagCorrUp", "BjetTagCorrDown", "BjetTagUncorr"+sYear+ "Up", "BjetTagUncorr"+sYear+ "Down", "PUID"+sYear+ "Up", "PUID"+sYear+ "Down", "L1PreFiring"+sYear+ "Up", "L1PreFiring"+sYear+ "Down" // 17 - 24
    , "PUreweight"+sYear+ "Up", "PUreweight"+sYear+ "Down", "PDFUp", "PDFDown", "LHEScaleUp", "LHEScaleDown", // 25 - 30
      "LumiCorrUp", "LumiCorrDown", "LumiStat"+sYear+"Up", "LumiStat"+sYear+"Down" //31-34
  };

  TH1F dataHist;
  vector<TH1F> ttbarHists, NonTtbarHists;

  //also derive negative log likelihood residuals
  TH1F dataHistNLL, ttbarHistNLL, NonTtbarHistNLL;

  //loop over samples, organizing data, ttbar, and non-ttbar with variations
  for(unsigned i = 0; i < 22; ++i){
    if(bin/1000 == 1 && i == 0) continue;
    if(bin/2000 == 1 && i == 1) continue;
    //if(year == 2017 && i == 17) continue; //FIXME: skip ZZ sample that can't be processed

    //need to combine 2016 and 2016apv before determining SFs
    if(year == 2016){
      TString command = TString::Format("hadd -f TestHistograms/SimpleShapes_Bin%d_2016all_%d.root TestHistograms/SimpleShapes_Bin%d_2016_%d.root TestHistograms/SimpleShapes_Bin%d_2016apv_%d.root",bin,i,bin,i,bin,i);
      system(command);
    }
    TFile *infile;
    if(year != 2016) infile = new TFile(TString::Format("TestHistograms/SimpleShapes_Bin%d_%d_%d.root",bin,year,i),"READ");
    else	     infile = new TFile(TString::Format("TestHistograms/SimpleShapes_Bin%d_2016all_%d.root",bin,i),"READ");
    Dataset dset = dlib.GetDataset(i);
    TString gn = dset.GroupName;
    if(i<=1){
      dataHist = *(TH1F*)(infile->Get(TString::Format("ST_data_obs_Wprime%d_%d_",bin,year)))->Clone("dataHist");
      dataHistNLL = *(TH1F*)(infile->Get(TString::Format("NegLogLnoB_Data_Wprime%d_%d_",bin,year)))->Clone("dataHistNLL");
    }
    else if(i==2){
      for(unsigned j = 0; j < variations.size(); ++j){
	      //std::cout<<TString::Format("ST_"+gn+"_Wprime%d_%d_"+variations[j],bin,year)<<std::endl;
	      ttbarHists.push_back(*(TH1F*)(infile->Get(TString::Format("ST_"+gn+"_Wprime%d_%d_"+variations[j],bin,year)))->Clone(TString::Format("ttbar_%d",j)));
      }
      ttbarHistNLL = *(TH1F*)(infile->Get(TString::Format("NegLogLnoB_ttbar_Wprime%d_%d_",bin,year)))->Clone("ttbarNLL");
    }
   else if (i==3){
     for(unsigned j = 0; j < variations.size(); ++j) NonTtbarHists.push_back(*(TH1F*)(infile->Get(TString::Format("ST_"+gn+"_Wprime%d_%d_"+variations[j],bin,year)))->Clone(TString::Format("nonTtbar_%d",j)));
     NonTtbarHistNLL = *(TH1F*)(infile->Get(TString::Format("NegLogLnoB_"+gn+"_Wprime%d_%d_",bin,year)))->Clone("nonTtbarNLL");
   }
   else{
     for(unsigned j = 0; j < variations.size(); ++j) NonTtbarHists[j].Add((TH1F*)(infile->Get(TString::Format("ST_"+gn+"_Wprime%d_%d_"+variations[j],bin,year))));
     NonTtbarHistNLL.Add((TH1F*)(infile->Get(TString::Format("NegLogLnoB_"+gn+"_Wprime%d_%d_",bin,year))));
   }
  }

  //use variations gathered to calculate central SF histogram and variation SF histograms
  vector<TH1F> SFhists, SFs;
  for(unsigned i = 0; i < variations.size(); ++i){
    SFhists.push_back(*(TH1F*)dataHist.Clone("SF_"+variations[i]));
    SFhists[i].Add(&NonTtbarHists[i],-1.);
    SFhists[i].Divide(&ttbarHists[i]);

    //cleaning function
    for(unsigned x = 0; x < dataHist.GetNbinsX(); ++x){
      if(dataHist.GetBinContent(x+1) < 10.) {SFhists[i].SetBinContent(x+1,0.); SFhists[i].SetBinError(x+1,0.);}
      if(dataHist.GetBinContent(x+1) < NonTtbarHists[i].GetBinContent(x+1)) {SFhists[i].SetBinContent(x+1,0.); SFhists[i].SetBinError(x+1,0.);}
    }

    SFs.push_back(*(TH1F*)SFhists[i].Clone("SFcalc_"+variations[i]));

  }

  //make NLL residual distribution
  TH1F NLLresidual = *(TH1F*)dataHistNLL.Clone(TString::Format("NLLresidual_%d_%d",bin,year));
  ttbarHistNLL.Add(&NonTtbarHistNLL);
  NLLresidual.Divide(&ttbarHistNLL);

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

  TFile *savefile = new TFile(TString::Format("TestHistograms/SF_Bin%d_%d.root",bin,year),"RECREATE");
  for(unsigned i = 0; i < SFhists.size(); ++i){
    SFhists[i].Write();
    SFs[i].Write();
  }
  NLLresidual.Write();
  savefile->Close();
} 
