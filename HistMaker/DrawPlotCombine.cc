#include "TFile.h"
#include "Utilities/RatioPlot.cc"
#include "Utilities/Regions.cc" 

void DrawPlotCombine(int it = 0, int ir = 0) {
  vector<string> Regions = {"1153", "1163", "2153", "2163"};
  string Region = Regions[ir];
  string subfolder = "BinW20/";

  string InputPath = "CombinedFiles/" + subfolder;
  string InputFile = "SimpleWpMassFL_Wprime";

  string OutputPath = "Brazil/" + subfolder;

  string year;
  if (it == 1) {
    // InputPath = "../SifuCombine/" + subfolder;
    InputPath = "/eos/cms/store/group/phys_b2g/wprime/DenisIntermediateHistogramsForCombine/";
    year = "2016";
    InputFile = "Combined_Bin1152_2016all.root";
    Region = "1152";

  }
  if (it == 2) {
    InputPath = "/afs/cern.ch/user/s/siluo/b2g/wprime/SifuCombine/2018/v4/";
    year = "2018";
  }
  //TString InputFileName = InputPath + InputFile + Region + "_" + year + ".root";
  TString InputFileName = InputPath + InputFile;
  cout << "Reading from file: " << InputFileName << endl;
  TFile *f = new TFile(InputFileName,"READ");
  //vector<string> Groups = {"ttbar", "wjets", "single_top", "diboson", "M300", "M400", "M500", "M600", "M700", "M800", "M900", "M1000", "M1100"};
   vector<string> Groups = {"rew_ttbar", "_wjets", "_single_top", "_diboson"};
  rm.MakeCombineVariations();
  if (it == 0) rm.AddVariationSource("RwStat2018" + Region);
  vector<double> LegendPos = {0.65,0.65,0.9,0.9};

  RatioPlot *rp = new RatioPlot("wprime", true, "ST", "Number of weighted events / bin width");
  rp->SetVariations(rm.Variations);
  rp->Legend(LegendPos);
  
  int i = 0;
  vector<TH1F*> Hists(50);
  for (unsigned ig = 0; ig < Groups.size(); ++ig) {
    int Type = 1; // MC
    if (ig > 2) Type = 2;
    for (unsigned iv = 0; iv < rm.Variations.size(); ++iv) {
      //TString hn = Groups[ig] + "_Wprime" + Region + "_" + year + "_" + rm.Variations[iv];
      TString hn = "ST"+Groups[ig] + "_Wprime" + Region + "_" + year + "_" + rm.Variations[iv];
      std::cout << "Getting " << hn << std::endl;
      TH1F* h = (TH1F*) f->Get(hn);
      //h->Rebin(36, "rebin", FitLimits);
      if (h == nullptr) continue;
      Hists.push_back(h);
      h->GetXaxis()->SetRangeUser(20,2000);
      cout << "HistName = " << h->GetName() << " , Integral = " << h->Integral() << endl;
      // Set histogram color "#color[%i]{(%.2f%%)}
      // h->SetFillColor(ig);
      h->Scale(1.0, "width");
      if (ig == 0) { h->SetFillColor(kRed); h->SetLineColor(kRed);}
      if (ig == 1) {h->SetFillColor(kGreen); h->SetLineColor(kGreen);}
      if (ig == 2) {h->SetFillColor(kBlue); h->SetLineColor(kBlue);}
      if (ig == 3) {h->SetFillColor(kYellow); h->SetLineColor(kYellow);}
      if (ig == 4) h->SetLineColor(kBlue);
      if (ig == 5) h->SetLineColor(kYellow);
      if (ig == 6) h->SetLineColor(kMagenta);
      if (ig == 7) h->SetLineColor(kCyan);
      if (ig == 8) h->SetLineColor(kGreen);
      if (ig == 9) h->SetLineColor(kBlue -5);
      if (ig == 10) h->SetLineColor(kGreen - 5);
      i++;
      h->SetLineWidth(2);
      rp->AddHist(Groups[ig], h, Type, iv);
    }
  }
  

  // Add data 
  vector<string> DataGroups = {"ST_data_obs_Wprime1152"};
  for (unsigned ig = 0; ig < DataGroups.size(); ++ig) {
    int Type = 0; // Data
    for (unsigned iv = 0; iv < rm.Variations.size(); ++iv) {
      TString hn = DataGroups[ig] + "_" + year + "_" + rm.Variations[iv];
      cout << "Getting " << hn << endl;
      TH1F* h = (TH1F*) f->Get(hn);
      
      if (h == nullptr) continue;
      cout << "HistName = " << h->GetName() << " , Integral = " << h->Integral() << endl;
      Hists.push_back(h);
      h->GetXaxis()->SetRangeUser(20,2000);
      h->Scale(1.0, "width");
      rp->AddHist(DataGroups[ig], h, Type, iv);
    }
  }
 

  dlib.AddLegend(rp->leg,1);
  // Change plot Y title

  rp->PrepHists();
  rp->CreateRatioPlots();
  rp->CreateErrorGraphs();



  TCanvas *c1 = new TCanvas("c1","c1", 800,800);
  rp->SetPad(c1);
  rp->DrawPlot(0); // 0 =  2016, 2 = 2017, 3 = 2018

  TString PlotName = OutputPath + "MyPlots" + Region;
  if (it == 1) PlotName = OutputPath + "DenisPlots" + Region;
  c1->SaveAs(PlotName + ".pdf");

  //rp->SaveInfos(PlotName);


}