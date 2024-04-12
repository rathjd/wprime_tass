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
  TString InputFileName = InputPath + InputFile;
  cout << "Reading from file: " << InputFileName << endl;
  TFile *f = new TFile(InputFileName,"READ");
  //vector<string> Groups = {"ttbar", "wjets", "single_top", "diboson", "M300", "M400", "M500", "M600", "M700", "M800", "M900", "M1000", "M1100"};
  vector<string> Groups = {"rew_ttbar", "_wjets", "_single_top", "_diboson"};
  vector<string> GroupsNames = {"ttbar", "wjets", "single top", "diboson"};
  rm.MakeCombineVariations();
  if (it == 0) rm.AddVariationSource("RwStat2018" + Region);
  vector<double> LegendPos = {0.65,0.65,0.9,0.9};

  RatioPlot *rp = new RatioPlot("wprime", true, "ST", "Number of weighted events / bin width");
  rp->SetVariations(rm.Variations);
  rp->Legend(LegendPos);
  
  
  //double FitLimits[36] = {85., 220., 230., 239., 247., 255., 263., 270., 276., 282., 288., 294., 301., 308., 315., 322., 329., 336., 343., 350., 360., 372., 384., 396., 410., 425., 445., 465., 490., 515., 550., 590., 650., 730., 880., 2000.};
  vector<TH1F*> Hists(50);
  for (unsigned ig = 0; ig < Groups.size(); ++ig) {
    int Type = 1; // MC 
    if (ig > 2) Type = 2;
    for (unsigned iv = 0; iv < rm.Variations.size(); ++iv) {
      TString hn = "ST"+Groups[ig] + "_Wprime" + Region + "_" + year + "_" + rm.Variations[iv];
      std::cout << "Getting " << hn << std::endl;
      TH1F* h = (TH1F*) f->Get(hn);
      //h->Rebin(36, "rebin", FitLimits);
      if (h == nullptr) continue;
      Hists.push_back(h);
      h->GetXaxis()->SetRangeUser(20,2000);
      //h->Scale(1.0, "width");
      cout << "HistName = " << h->GetName() << " , Integral = " << h->Integral() << endl;
      // Set histogram color "#color[%i]{(%.2f%%)}
      // h->SetFillColor(ig);
      if (ig == 0) { h->SetFillColor(kRed); h->SetLineColor(kRed);}
      if (ig == 1) {h->SetFillColor(kGreen); h->SetLineColor(kGreen);}
      if (ig == 2) {h->SetFillColor(kBlue); h->SetLineColor(kBlue);}
      if (ig == 3) {h->SetFillColor(kYellow); h->SetLineColor(kYellow);}
      // if (ig == 4) h->SetLineColor(kBlue);
      // if (ig == 5) h->SetLineColor(kYellow);
      // if (ig == 6) h->SetLineColor(kMagenta);
      // if (ig == 7) h->SetLineColor(kCyan);
      // if (ig == 8) h->SetLineColor(kGreen);
      // if (ig == 9) h->SetLineColor(kBlue -5);
      // if (ig == 10) h->SetLineColor(kGreen - 5);
      cout << "variations = " << rm.Variations[iv] << endl;
      h->SetLineWidth(2);
      rp->AddHist(Groups[ig], h, Type, ig);
    }
  }
  // rp -> AddHist("Data", (TH1F*) f->Get("ST_data_obs_Wprime1152_2016_"), 1, 5);
  // Rebin plot to new limits
  //Add data 
  // vector<string> DataGroups = {"ST_data_obs_Wprime1152_2016_", "ST_data_obs_Wprime1152_2016_electronScale2016Up", "ST_data_obs_Wprime1152_2016_electronScale2016Down"};
  // for (unsigned ig = 0; ig < DataGroups.size(); ++ig) {
  //   cout << "Getting " << DataGroups[ig] << endl;
  //   TH1F* h = (TH1F*) f->Get(DataGroups[ig].c_str());
  //   if (h == nullptr) continue;
  //   h->GetXaxis()->SetRangeUser(20,2000);
  //   h->Scale(1.0, "width");
  //   cout << "HistName = " << h->GetName() << " , Integral = " << h->Integral() << endl;
  //   if (ig == 0) { h->SetMarkerStyle(20); h->SetMarkerSize(1.2); h->SetLineColor(kBlack);}
  //   if (ig == 1) { h->SetMarkerStyle(21); h->SetMarkerSize(1.2); h->SetLineColor(kRed);}
  //   if (ig == 2) { h->SetMarkerStyle(22); h->SetMarkerSize(1.2); h->SetLineColor(kBlue);}
  //   rp->AddHist("Data", h, 1, 5);
  // }

  std::cout << "Debug 1" << std::endl;
  dlib.AddLegend(rp->leg,1);
  std::cout << "Debug 2" << std::endl;
  // Change plot Y title

  rp->PrepHists();
  std::cout << "Debug 3" << std::endl;
  rp->CreateRatioPlots();
  std::cout << "Debug 4" << std::endl;
  rp->CreateErrorGraphs();
  std::cout << "Debug 5" << std::endl;
 


  TCanvas *c1 = new TCanvas("c1","c1", 800,800);
  rp->SetPad(c1);
  rp->DrawPlot(1);

  TString PlotName = OutputPath + "MyPlots" + Region;
  if (it == 1) PlotName = OutputPath + "DenisPlots" + Region;
  c1->SaveAs(PlotName + ".pdf");
 
  //rp->SaveInfos(PlotName);


}