#include <TROOT.h>
#include "Dataset.cc"
#include <TFile.h>
#include <TF1.h>
#include <TH1.h>
#include <vector>
#include <TString.h>
#include <TFitResult.h> 
#include <TMatrixD.h>
#include <TMath.h>
//#include "Systematics.C" 

//derive ttbar SF from 2-tag regions of same multiplicity and lepton flavour, then propagate stat uncertainty envelope bin-by-bin and do syst variation histograms
void ScaleFactorTTbarCalc(int bin=1152, TString year="2018"){
 
  TString B2Gn = "25008"; //placeholder, until we get a cadi line number
  TString YearS = year;
  if(year == "2016_APV") YearS="2016apv";
 
  TH1F dataHist;
  vector<vector<TH1F> > SimHists;

  //also derive negative log likelihood residuals
  vector<TH1F> dataHistNLL;
  vector<vector<TH1F> > SimHistsNLL;

  //sample-type-specific variations need special treatment
  vector<TString> SampleTypes = {"ttbar", "wjets", "single_top", "diboson", "qcd"}; //list of background sample types
  for(unsigned s = 0; s < SampleTypes.size(); ++s){
    vector<TH1F> dummy;
    SimHists.push_back(dummy);
    SimHistsNLL.push_back(dummy);
  }

  //set variation size
  unsigned varSize = 45; //note that this is hardcoded as a crosscheck
  unsigned qcdExtra = 2; //two more added to produce QCD systematic variations

  //loop over samples, organizing data, ttbar, and non-ttbar with variations
  //for(unsigned sam = 0; sam < 27; ++sam){//FIXME
  for(unsigned sam = 0; sam < 33; ++sam){
    if(bin/1000 == 1 && sam == 0) continue;
    if(bin/2000 == 1 && sam == 1) continue;

    //load dataset information
    Dataset dset = dlib.GetDataset(sam);
    TString gn = dset.GroupName;
    TString sampleType = gn;

    //load infile per sample
    TFile *infile;
    TString FileLoc = TString::Format("/eos/cms/store/group/phys_b2g/wprime/temp/SimpleShapes_Bin%d_",bin) + YearS + TString::Format("_%d.root",sam);
    infile = new TFile(FileLoc, "READ");

    //sort the samples into histograms with all variations
    if(sam<=1){//data
      TString dataName = TString::Format("ST_data_obs_Wprime%d_",bin)+ YearS + "_";
      //std::cout<<"loading "<<dataName<<" from "<<FileLoc<<std::endl;
      dataHist = *(TH1F*)(infile->Get(dataName))->Clone("dataHist");
      for(unsigned mass = 300; mass < 1200; mass+=100){
	TString MassDataName = TString::Format("NegLogLnoB_Data_Wprime%d_",bin) + YearS + TString::Format("_M%d_",mass);
	dataHistNLL.push_back( *(TH1F*)(infile->Get(MassDataName))->Clone(TString::Format("dataHistNLL_M%d",mass)));
      }
    }
    else if(sam==2 || sam==8 || sam==16 || sam==21 || sam==27){//first sample in each sample set
      for(unsigned var = 0; var < varSize; ++var){
	TString variation = Systematics(var, YearS, sampleType, B2Gn);
        TString TmpName = TString("ST_") + gn + TString::Format("_Wprime%d_",bin) + YearS + "_" + variation;
        //std::cout<<"creating "<<TmpName<<std::endl;
        SimHists[dset.Color-2].push_back(*(TH1F*)(infile->Get(TmpName)->Clone(gn + TString::Format("_%d",var))));
      }
      for(unsigned mass = 300; mass < 1200; mass+=100){
        TString TmpName = TString("NegLogLnoB_") + gn + TString::Format("_Wprime%d_",bin) + YearS + TString::Format("_M%d_",mass);
	SimHistsNLL[dset.Color-2].push_back(*(TH1F*)(infile->Get(TmpName)->Clone(gn + TString::Format("NLL_M%d",mass))));
      }
    }
    else{//additional samples in each sample set
      for(unsigned var = 0; var < varSize; ++var){
	TString variation = Systematics(var, YearS, sampleType, B2Gn);
        TString TmpName = TString("ST_") + gn + TString::Format("_Wprime%d_",bin) + YearS + "_" + variation;
        //std::cout<<"adding "<<TmpName<<std::endl;
        SimHists[dset.Color-2][var].Add((TH1F*)(infile->Get(TmpName)));
      }
      for(unsigned mass = 300; mass < 1200; mass+=100){
        TString TmpName = TString("NegLogLnoB_") + gn + TString::Format("_Wprime%d_",bin) + YearS + TString::Format("_M%d_",mass);
        SimHistsNLL[dset.Color-2][mass/100-3].Add((TH1F*)(infile->Get(TmpName)));
      }
    }
  }

  //use variations gathered to calculate central SF histogram and variation SF histograms
  vector<TH1F> SFhists, SFs;
  for(unsigned var = 0; var < varSize + qcdExtra; ++var){

    //except sample-dependent uncertainties for extra logic
    //at this stage, variation i departs from being concurrent with SF histogram iterator, so we just keep appending each different variation
    if(var >= 35 && var <= 40) for(unsigned sam = 0; sam < SampleTypes.size(); ++sam){ //loop over different samples{ 
      TString variation = Systematics(var, YearS, SampleTypes[sam], B2Gn);
      SFhists.push_back(*(TH1F*)dataHist.Clone("SF_"+variation));
      unsigned currentPos = SFhists.size()-1;
      for(unsigned sh = 1; sh < SimHists.size(); ++sh){
	if(sam == sh) SFhists[currentPos].Add(&SimHists[sh][var],-1); //if this is the variation sample, add variation
	else          SFhists[currentPos].Add(&SimHists[sh][0]  ,-1); //else use nominal variation
      }
      if(sam == 0) SFhists[currentPos].Divide(&SimHists[0][var]);   //if this is the ttbar variation, divide by variation
      else         SFhists[currentPos].Divide(&SimHists[0][0]);     //else divide by nominal ttbar

      //cleaning function for bins with no data or negative values
      for(unsigned x = 0; x < dataHist.GetNbinsX(); ++x){
        if(dataHist.GetBinContent(x+1) < 10. || SFhists[currentPos].GetBinContent(x+1) < 0.){
     	  SFhists[currentPos].SetBinContent(x+1,0.);
	  SFhists[currentPos].SetBinError(x+1,0.);
	}
      }
      //std::cout<<variation<<" integrated SF yields per bin "<<SFhists[currentPos].Integral()/SFhists[currentPos].GetNbinsX()<<std::endl;

      SFs.push_back(*(TH1F*)SFhists[currentPos].Clone("SFcalc_"+variation));
    }
    else if(var >= varSize){ //QCD normalization variations special case
      if(var == varSize) SFhists.push_back(*(TH1F*)dataHist.Clone("SF_STfitQCDUp"));
      else		 SFhists.push_back(*(TH1F*)dataHist.Clone("SF_STfitQCDDown"));
      unsigned currentPos = SFhists.size()-1;
      for(unsigned sh = 1; sh < SimHists.size(); ++sh){
	double multiplier = -1.;
	if(sh == SimHists.size()-1 ){//only vary QCD contribution size
	  if(var==varSize) multiplier = -1.25 ; //QCD up
	  else		   multiplier = -0.75;  //QCD down
	} 
	SFhists[currentPos].Add(&SimHists[sh][0], multiplier); //substract non-ttbar simulation
      }
      SFhists[currentPos].Divide(&SimHists[0][0]); //divide by nominal ttbar

      //cleaning function for bins with no data or negative values
      for(unsigned x = 0; x < dataHist.GetNbinsX(); ++x){
        if(dataHist.GetBinContent(x+1) < 10. || SFhists[currentPos].GetBinContent(x+1) < 0.){
          SFhists[currentPos].SetBinContent(x+1,0.);
          SFhists[currentPos].SetBinError(x+1,0.);
        }
      }

      if(var == varSize) SFs.push_back(*(TH1F*)SFhists[currentPos].Clone("SFcalc_STfitQCDUp"));
      else               SFs.push_back(*(TH1F*)SFhists[currentPos].Clone("SFcalc_STfitQCDDown"));
    }
    else{//standard case
      TString variation = Systematics(var, YearS, "", B2Gn);//sampleType is empty, because no sample-dependent uncertainties are covered her
      SFhists.push_back(*(TH1F*)dataHist.Clone("SF_"+variation));
      unsigned currentPos = SFhists.size()-1;
      for(unsigned sh = 1; sh < SimHists.size(); ++sh) SFhists[currentPos].Add(&SimHists[sh][var],-1); //substract non-ttbar simulation
      SFhists[currentPos].Divide(&SimHists[0][var]); //divide by ttbar simulation

      //cleaning function for bins with no data or negative values
      for(unsigned x = 0; x < dataHist.GetNbinsX(); ++x){
        if(dataHist.GetBinContent(x+1) < 10. || SFhists[currentPos].GetBinContent(x+1) < 0.){
          SFhists[currentPos].SetBinContent(x+1,0.);
          SFhists[currentPos].SetBinError(x+1,0.);
        }
      }
      //std::cout<<variation<<" integrated SF yields per bin "<<SFhists[currentPos].Integral()/SFhists[currentPos].GetNbinsX()<<std::endl;

      SFs.push_back(*(TH1F*)SFhists[currentPos].Clone("SFcalc_"+variation));
    }
  }

  TString SaveName = TString::Format("/eos/cms/store/group/phys_b2g/wprime/temp/SF_Bin%d_",bin) + YearS + ".root";
  TFile *savefile = new TFile(SaveName,"RECREATE");

  //make NLL residual distribution
  //This includes data / all MC components
  //ttbar in the 2-tag regions is supposed to be ST-reweighted in this workflow within the CombineHistogramDumpster
  for(unsigned m = 3; m < 12; ++m){
    TString TmpName = TString::Format("NLLresidual_%d_",bin) + YearS + TString::Format("_M%d",m*100);
    TH1F NLLresidual = *(TH1F*)dataHistNLL[m-3].Clone(TmpName);
    for(unsigned shn = 1; shn < SimHistsNLL.size(); ++shn) SimHistsNLL[0][m-3].Add(&SimHistsNLL[shn][m-3]);
    NLLresidual.Divide(&SimHistsNLL[0][m-3]);
    NLLresidual.Write();
  }

  //define fit function
  TF1 *fitFunction;
  if(bin % 100 < 60) fitFunction = new TF1("fitFunction","[0]/x/x+[1]/x+[2]+[3]*x+[4]*x*x", 180., 2000.);
  else 		     fitFunction = new TF1("fitFunction","[0]/x/x+[1]/x+[2]+[3]*x"        , 210., 2000.);
  //fit nominal variant with statistical uncertainties only, get covariance matrix, calculate statistical envelope
  TFitResultPtr fr = SFhists[0].Fit(fitFunction,"SRF");
  TMatrixD cov = fr->GetCovarianceMatrix();
  for(unsigned i = 0; i < SFhists[0].GetNbinsX(); ++i){
    double bc = SFhists[0].GetBinCenter(i+1);
    vector<double> Derivatives;
    if(bin % 100 < 60) Derivatives = {1./bc/bc, 1./bc, 1., bc, bc*bc};
    else 	       Derivatives = {1./bc/bc, 1./bc, 1., bc};
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
