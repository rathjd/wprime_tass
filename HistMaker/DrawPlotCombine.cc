#include "TFile.h"
#include "Utilities/RatioPlot.cc"
#include "Utilities/Regions.cc" 

void DrawPlotCombine(int it = 0, int ir = 0, string nb = "1", int iy = 0) {
  vector<string> Regions = {"115", "116", "215", "216"};
  vector<string> years = {"2016","2017","2018"};
  string Region = Regions[ir];
  Region.append(nb);
  string subfolder = "BinW20/";

  string InputPath = "CombinedFiles/" + subfolder;
  string InputFile = "SimpleWpMassFL_Wprime";

  string OutputPath = "Brazil/" + subfolder;

  string year = years[iy];
  if (it == 1) {
    // InputPath = "../SifuCombine/" + subfolder;
    InputPath = "/eos/cms/store/group/phys_b2g/wprime/DenisIntermediateHistogramsForCombine/ttbarRewFixed/";
    //year = "2016";
    InputFile = "Combined_Bin" + Region + "_" + year + ".root";

  }
  if (it == 2) {
    InputPath = "/afs/cern.ch/user/s/siluo/b2g/wprime/SifuCombine/2018/v4/";
    year = "2018";
  }
  //TString InputFileName = InputPath + InputFile + Region + "_" + year + ".root";
  TString InputFileName = InputPath + InputFile;
  cout << "Reading from file: " << InputFileName << endl;
  TFile *f = new TFile(InputFileName,"READ");
  vector<string> Groups = {"_ttbar", "_wjets", "_single_top", "_diboson", "_M300", "_M400", "_M500", "_M600", "_M700", "_M800", "_M900", "_M1000", "_M1100"};
  // vector<string> Groups = {"_ttbar", "_wjets", "_single_top", "_diboson"};
  if(nb == "2") Groups[0] = "rew_ttbar";
  rm.MakeCombineVariations(year, Region == "1152" || "2152" || "1162" || "2162" ? true : false);
  if (it == 0) rm.AddVariationSource("RwStat2018" + Region);
  vector<double> LegendPos = {0.65,0.65,0.9,0.9};

  RatioPlot *rp = new RatioPlot("wprime", false, "S_{T} [GeV/c]", "Number of weighted events / bin width");
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
  vector<string> DataGroups = {"ST_data_obs_Wprime"+Region};
  for (unsigned ig = 0; ig < DataGroups.size(); ++ig) {
    int Type = 0; // Data
    //for (unsigned iv = 0; iv < rm.Variations.size(); ++iv) {
      TString hn = DataGroups[ig] + "_" + year + "_";// + rm.Variations[iv];
      cout << "Getting " << hn << endl;
      TH1F* h = (TH1F*) f->Get(hn);
      
      //if (h == nullptr) continue;
      cout << "HistName = " << h->GetName() << " , Integral = " << h->Integral() << endl;
      //Hists.push_back(h);
      h->GetXaxis()->SetRangeUser(20,2000);
      h->Scale(1.0, "width");
      rp->AddHist(DataGroups[ig], h, 0, 0);
    //}
  }
 

  dlib.AddLegend(rp->leg,1);
  // Change plot Y title

  rp->PrepHists();
  rp->CreateRatioPlots();
  rp->CreateErrorGraphs();



  TCanvas *c1 = new TCanvas("c1","c1", 800,800);
  rp->SetPad(c1);
  rp->DrawPlot(!iy?0:iy+1); // 0 =  2016, 2 = 2017, 3 = 2018

  TString PlotName = OutputPath + "MyPlots" + Region;
  if (it == 1) PlotName = OutputPath + "DenisPlots" + Region + "_" + year;
  c1->SaveAs(PlotName + ".pdf");

  //rp->SaveInfos(PlotName);


}
