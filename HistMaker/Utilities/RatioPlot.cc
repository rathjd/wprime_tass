#ifndef RATIOPLOT_CC
#define RATIOPLOT_CC

#include "TString.h"
#include "TH1.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLegend.h"
#include "TGraph.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm> // max,min

#include "CMSStyle.cc"

using namespace std;

class RatioPlot {
public:
  RatioPlot(TString pn, bool IsSR_ = false, TString xt = "", TString yt = "Number of Entries")
  {
    Init();
    PlotName = pn;
    Logy = false;
    IsSR = IsSR_;
    UTitle = ";;" + yt;
    LTitle = ";" + xt + ";Obs./Exp.";
    TString stackname = PlotName + (TString)"_MCStack";
    MCStack = new THStack(stackname,UTitle);
  };

  void Init() {
    MCHists.clear();
    MCNames.clear();
    MCSummed.clear();
    latex.SetNDC();
    latex.SetTextSize(0.035);
    latex.SetTextAlign(23);
  }

  void SetLogy(bool l = true) {
    Logy = l;
    //UPad->SetLogy(l);
  }

  void ReverseXAxis(TH1F* h) {
    // int nbins = h->GetNbinsX();
    // TH1F *hr = (TH1F*)h->Clone("hr");
    // // Reverse the bin contents
    // for (int i=1; i<=nbins; i++) {
    //   h->SetBinContent(i, hr->GetBinContent(nbins-i+1));
    //   h->SetBinError(i, hr->GetBinError(nbins-i+1));
    // }
    //cout << "Not reversing X axis" << std::endl;
  }

  void SetTitles(TString xt, TString yt = "Number of Entries") {
    UTitle = ";;" + yt;
    LTitle = ";" + xt + ";Data/MC";
    MCStack->SetTitle(UTitle);
  }

  void SetVariations(vector<string> vars) {
    Variations = vars;
    VarSize = Variations.size();
    MCSummed.clear();
    SigHists.clear();
    MCSummed.resize(VarSize);
    //SigHists will be dynamically sized since it is [iSig][iVar] structure.
  }

  void YEnlarge(double ye) {
    TrueMaximumScale *= ye;
  }

  void AddData(TH1F* h_) {
    DataHist = (TH1F*)h_->Clone();
    cout << "Data name = " << DataHist->GetName() << endl;
    ReverseXAxis(DataHist);
    DataHist->SetDirectory(0);
    DataHist->SetBinErrorOption(TH1::kPoisson);
    DataHist->SetLineStyle(1);
    DataHist->SetLineColor(1);
    DataHist->SetMarkerStyle(20);
    DataHist->SetTitle(UTitle); // Not necessary because Stack will be drawn first
    if (DataHist->GetMaximum() > TrueMaximum) {
      TrueMaximum = DataHist->GetMaximum();
    }
    HasData = true;
  }

  void AddMC(TString n_, TH1F* h_, int iv) {
    if (iv == 0) {
      TH1F* h = (TH1F*) h_->Clone();
      h->SetDirectory(0);
      h->SetLineStyle(1);

      MCHists.push_back(h); // Saved into vector, since THStack class doesn't own the histograms.
      MCNames.push_back(n_);

      MCStack->Add(h);
      HasMC = true;
      // print integral of h
      cout << n_ << " integral: " << h->Integral() << endl;
    }
    else {
      for (unsigned inm = 0; inm < MCNames.size(); ++inm) {
        if (MCNames[inm] != n_) continue;
        if (fabs(h_->Integral(0,-1) / MCHists[inm]->Integral(0,-1) - 1.0) > 0.5) {
        }
      }
    }
    if (MCSummed[iv] == nullptr) {
      MCSummed[iv] = (TH1F*) h_->Clone();
      MCSummed[iv]->SetDirectory(0);
      MCSummed[iv]->SetLineColor(1);
    }
    else {
      MCSummed[iv]->Add(h_);
    }
    if (MCSummed[iv]->GetMaximum() > TrueMaximum) TrueMaximum = MCSummed[iv]->GetMaximum();
  }

  void AddSig(TString n_, TH1F* h_, int iv, int style) {
    TH1F* h = (TH1F*) h_->Clone();
    h->SetDirectory(0);
    ReverseXAxis(h);
    if (iv == 0) {
      if (style == -1) h->SetLineStyle(2);
      else h->SetLineStyle(style);
      h->SetLineWidth(2);
      HasSig = true;
    }
    int idx = -1;
    for (unsigned i = 0; i < SigNames.size(); ++i) {
      if (SigNames[i] == n_) idx = i;
    }
    if (idx == -1) {
      idx = SigNames.size();
      SigNames.push_back(n_);
      SigHists.push_back(vector<TH1F*>(VarSize)); // Potentially result in unfilled slot being nullptr
    }
    // Signal index in RatioPlot is dynamically incremented,
    // thus is not always the same as it is in Dataset.cc
    SigHists[idx][iv] = h;
    if (h->GetMaximum() > TrueMaximum) TrueMaximum = h->GetMaximum();
  }

  void AddHist(TString n_, TH1F* h_, int type_, int iv, int style = -1) {
    if (h_ == nullptr) return;
    if (type_ == 0 && iv == 0) AddData(h_);
    else if (type_ == 1) AddMC(n_, h_, iv);
    else if (type_ == 2) AddSig(n_, h_, iv, style);
    if (h_->GetNbinsX() <= 0) cout << n_ << " nbins = " << h_->GetNbinsX()<< endl;
    if (nbins == 0) {
      nbins = h_->GetNbinsX();
      xlow = h_->GetXaxis()->GetXmin();
      xup = h_->GetXaxis()->GetXmax();
    }
    else if ((unsigned) h_->GetNbinsX() != nbins) cout << "Inconsistent histogram nbins for " << n_ <<endl;
    else if (h_->GetXaxis()->GetXmin() != xlow) cout << "Inconsistent histogram xlow for " << n_ <<endl;
    else if (h_->GetXaxis()->GetXmax() != xup) cout << "Inconsistent histogram xup for " << n_ <<endl;
    // cout << "Adding Histogram " << n_  << " of variation " << iv <<endl;
  }

  void PrepHists() {
    LowerDummy = new TH1F("","", nbins, xlow, xup);
    if (!HasMC) {
      StackDummy = new TH1F("","",nbins,xlow, xup);
      // StackDummy->SetLineWidth(0);
      MCStack->Add(StackDummy);
      double x[2] = {xlow, xup};
      double y[2] = {1.,1.};
      MCErrorRatioGraph = new TGraph(2,x,y);
    }
    if (!HasMC && !HasData && HasSig) {
      for (unsigned i = 0; i < SigNames.size(); ++i) {
        if (SigHists[i][0]->GetLineStyle() == 2) SigHists[i][0]->SetLineStyle(1);
      }
    }
  }


  vector<double> StatError(TH1F* hcentral, vector<double>& errup, vector<double>& errlow) {
    if (hcentral == nullptr) return {};
    bool doreport = false;
    bool reported = false;
    double StatErrorIntegralNom(0), StatErrorIntegralUp(0), StatErrorIntegralDown(0);
    hcentral->SetBinErrorOption(TH1::kPoisson);
    for (unsigned i = 0; i < nbins; ++i) {
      if (hcentral->GetBinErrorUp(i + 1) != hcentral->GetBinErrorUp(i + 1)) {
        if (!reported && doreport) {
          cout << "Stat has val of nan for " << hcentral->GetName() << endl;
          reported = true;
        }
        continue;
      }
      errup[i] = errup[i] + hcentral->GetBinErrorUp(i + 1) * hcentral->GetBinErrorUp(i + 1);
      errlow[i] = errlow[i] + hcentral->GetBinErrorLow(i + 1) * hcentral->GetBinErrorLow(i + 1);
      StatErrorIntegralNom += hcentral->GetBinContent(i + 1);
      StatErrorIntegralUp += (hcentral->GetBinContent(i + 1) + hcentral->GetBinErrorUp(i + 1));
      StatErrorIntegralDown += (hcentral->GetBinContent(i + 1) - hcentral->GetBinErrorLow(i + 1));
    }
    if (StatErrorIntegralNom != hcentral->Integral()) cout << "StatErrorIntegralNom = " << StatErrorIntegralNom << ", Integral = " << hcentral->Integral() << endl;
    vector<double> out = {StatErrorIntegralNom, StatErrorIntegralUp, StatErrorIntegralDown};
    return out;
  }

  vector<double> SystError(TH1F* hcentral, TH1F* hvarup, TH1F* hvarlow, vector<double>& errup, vector<double>& errlow) {
    if (hcentral == nullptr) return {};
    // cout << "Start Sys Error" << endl;
    // cout << "hcentral: " << hcentral->GetName() << ", hvarup: " << hvarup->GetName() << ", hvarlow: " << hvarlow->GetName() <<endl;
    // cout << "Integrals: " << hcentral->Integral() << ", " << hvarup->Integral() << ", " << hvarlow->Integral() << endl;
    if (hvarup->Integral() / hcentral->Integral() > 2. || hvarup->Integral() / hcentral->Integral() < 0.5) cout << hvarup->GetName() << " ratio to central " << hcentral->GetName() << " = " <<hvarup->Integral() / hcentral->Integral() << endl;
    if (hvarlow->Integral() / hcentral->Integral() > 2. || hvarlow->Integral() / hcentral->Integral() < 0.5) cout << hvarlow->GetName() << " ratio to central " << hcentral->GetName() << " = " <<hvarlow->Integral() / hcentral->Integral() << endl;
    // cout << "Sys error 2"<< endl;
    
    bool doreport = false;
    bool reportedup = false;
    bool reportedlow = false;
    double SystErrorIntegralNom(0), SystErrorIntegralUp(0), SystErrorIntegralDown(0);
    // cout << errup[5] <<"," << errlow[5] <<endl;
    for (unsigned i = 0; i < nbins; ++i) {
      double diffup(0), difflow(0);
      if (hvarup != nullptr) diffup = hvarup->GetBinContent(i + 1) - hcentral->GetBinContent(i + 1);
      if (hvarlow != nullptr) difflow = hvarlow->GetBinContent(i + 1) - hcentral->GetBinContent(i + 1);
      if (diffup != diffup) {
        diffup = 0;
        if (!reportedup && doreport) {
          cout << "Diff up has  val of nan for " << hvarup->GetName() << " and " << hcentral->GetName() << endl;
          reportedup = true;
        }
      }
      if (difflow != difflow) {
        difflow = 0;
        if (!reportedlow && doreport) {
          cout << "Diff low has  val of nan for " << hvarlow->GetName() << " and " << hcentral->GetName() << endl;
          reportedlow = true;
        }
      }
      double eu = max(max(diffup,difflow),0.0);
      double el = min(min(diffup,difflow),0.0);
      // if (i == 5) cout << "diffup = " << diffup << ", difflow = " << difflow <<endl;
      // if (i == 5) cout << "eu = " << eu << ", el = " << el <<endl;
      errup[i] = errup[i] + eu * eu;
      errlow[i] = errlow[i] + el * el;
      SystErrorIntegralNom += hcentral->GetBinContent(i + 1);
      SystErrorIntegralUp += eu;
      SystErrorIntegralDown += el;
    }
    if (doreport) cout << errup[5] <<"," << errlow[5] <<endl;
    vector<double> out = {SystErrorIntegralNom, SystErrorIntegralUp, SystErrorIntegralDown};
    return out;
  }

  void ErrorCalc() {
    MCErrUp.clear();
    MCErrLow.clear();
    SigErrUp.clear();
    SigErrLow.clear();
    MCStatUncertIntegral.clear();
    MCSystUncertIntegral.clear();
    MCErrUp.resize(nbins, 0);
    MCErrLow.resize(nbins, 0);
    SigErrUp.resize(SigNames.size());
    SigErrLow.resize(SigNames.size());
    for (unsigned isig = 0; isig < SigNames.size(); ++isig) {
      SigErrUp[isig].resize(nbins, 0);
      SigErrLow[isig].resize(nbins, 0);
    }
    for (unsigned iv = 0; iv < VarSize; ++iv) {
      if (iv == 0) {
        if (HasMC) {
          MCStatUncertIntegral = StatError(MCSummed[0],MCErrUp,MCErrLow);
        }
        for (unsigned isig = 0; isig < SigNames.size(); ++isig) {
          // cout << "Stat:" <<endl;
          StatError(SigHists[isig][0], SigErrUp[isig], SigErrLow[isig]);
        }
      }
      else if (iv % 2 == 1) continue;
      else {
        if (HasMC) {
          vector<double> errint = SystError(MCSummed[0], MCSummed[iv-1],MCSummed[iv], MCErrUp, MCErrLow);
          if (MCSystUncertIntegral.size() == 0) MCSystUncertIntegral.push_back(errint[0]);
          MCSystUncertIntegral.push_back(errint[1]);
          MCSystUncertIntegral.push_back(errint[2]);
        }
        // cout <<"Iv = " << iv <<endl;
        // cout <<"Print Sig Names" <<endl;
        for (unsigned isig = 0; isig < SigNames.size(); ++isig) {

          
          // cout << "-SigNames: " << SigNames[isig] <<endl;
        }

        for (unsigned isig = 0; isig < SigNames.size(); ++isig) {
          // cout << "isig = " << isig <<"var size = " << VarSize <<endl;
          // cout << "SigErrUp size = " << SigErrUp[isig].size() <<endl;
          // cout << "SigErrLow size = " << SigErrLow[isig].size() <<endl;
          
          // cout << "SigNames: " << SigNames[isig] <<endl;
          SystError(SigHists[isig][0], SigHists[isig][iv-1], SigHists[isig][iv], SigErrUp[isig], SigErrLow[isig]);
        }
      }
    }
    if (HasMC) {
      if (MCSystUncertIntegral[0] != MCStatUncertIntegral[0]) cout << "Different nominal integrals. In stat = " << MCStatUncertIntegral[0] << ", In syst = " << MCSystUncertIntegral[0] << endl;
    }
    for (unsigned ib = 0; ib < nbins; ++ib) {
      if (HasMC) {
        MCErrUp[ib] = sqrt(MCErrUp[ib]);
        MCErrLow[ib] = sqrt(MCErrLow[ib]);
      }
      for (unsigned isig = 0; isig < SigNames.size(); ++isig) {
        SigErrUp[isig][ib] = sqrt(SigErrUp[isig][ib]);
        SigErrLow[isig][ib] = sqrt(SigErrLow[isig][ib]);
      }
      
    }
  }

  void CreateErrorGraphs() {
    ErrorCalc();
    double bw = (xup - xlow) / (nbins + 0.0); // Ensure nbins double
    // cout << "nbins" << nbins <<endl;
    int lp = nbins * 4 - 1;
    // cout << "lp " << lp <<endl;
    double x[200000]; // A large enough size larger than any nbins
    double y[200000];


    for (unsigned i = 0; i < nbins; ++i) {
      // x[2*i] = x[lp-2*i] = xlow + (i + 0.0) * bw;
      // x[2*i+1] = x[lp-2*i-1] = xlow + (i + 1.0) * bw;
      // cout << "MC Bin Low Edge = " << MCSummed[0]->GetXaxis()->GetBinLowEdge(i+1) <<endl;
      // cout << "MC Bin Up Edge = " << MCSummed[0]->GetXaxis()->GetBinUpEdge(i+1) <<endl;

      x[2*i] = x[lp-2*i] = MCSummed[0]->GetXaxis()->GetBinLowEdge(i+1);
      x[2*i+1] = x[lp-2*i-1] = MCSummed[0]->GetXaxis()->GetBinUpEdge(i+1);
    }
    cout << "Error Graphs Debug #1" <<endl;
    if (HasMC) {
      for (unsigned i = 0; i < nbins; ++i) {
        y[2*i] = y[2*i+1] = y[lp-2*i] = y[lp-2*i-1] = MinY;
        double cent = MCSummed[0]->GetBinContent(i+1);
        if (cent <= MinY) continue;
        y[2*i] = y[2*i+1] = cent + MCErrUp[i];
        y[lp-2*i] = y[lp-2*i-1] = cent - MCErrLow[i];
      }
      // cout << "Error Graphs Debug #2" <<endl;
      MCErrorGraph = new TGraph(nbins * 4, x, y);
      MCErrorGraph->SetLineWidth(0);
      MCErrorGraph->SetLineColor(0);
      MCErrorGraph->SetFillColor(1);
      MCErrorGraph->SetFillStyle(ErrorBandFillStyle);
    }
    
    SigErrorGraphs.resize(SigNames.size()); // SigNames size will prevent signal absent case crash the code
    // cout << "SigNames size = " << SigNames.size() <<endl;
    for (unsigned isig = 0; isig < SigNames.size(); ++isig) {
      for (unsigned i = 0; i < nbins; ++i) {
        y[2*i] = y[2*i+1] = y[lp-2*i] = y[lp-2*i-1] = MinY;
        double cent = SigHists[isig][0]->GetBinContent(i+1);
        if (cent <= MinY) continue;
        y[2*i] = y[2*i+1] = cent + SigErrUp[isig][i];
        y[lp-2*i] = y[lp-2*i-1] = cent - SigErrLow[isig][i];
      }
      SigErrorGraphs[isig] = new TGraph(nbins * 4, x, y);
      SigErrorGraphs[isig]->SetLineWidth(0);
      SigErrorGraphs[isig]->SetFillColor(SigHists[isig][0]->GetLineColor());
      SigErrorGraphs[isig]->SetFillStyle(ErrorBandFillStyle);
    }
    if (HasMC) {
      for (unsigned i = 0; i < nbins; ++i) {
        y[2*i] = y[2*i+1] = y[lp-2*i] = y[lp-2*i-1] = 1.0;
        double cent = MCSummed[0]->GetBinContent(i+1);
        // cout << "Cent = " << cent <<endl;
        // cout << "i = " << i <<endl;
        // cout << "MinY" << MinY <<endl;
        if (cent <= MinY) continue;  // Prevent -nan value in graph
        y[2*i] = y[2*i+1] = (cent + MCErrUp[i]) / cent;
        // cout << "y[2*i] = " << y[2*i] <<endl;
        y[lp-2*i] = y[lp-2*i-1] = (cent - MCErrLow[i]) / cent;
        // cout << "y[lp-2*i] = " << y[lp-2*i] <<endl;
      }
      // cout << "nbins: " << nbins <<endl;
      MCErrorRatioGraph = new TGraph(nbins * 4, x, y);
    }
    // cout<< "MCSummed Size" << MCSummed.size() <<endl;
    // cout << "MCErrUp Size" << MCErrUp.size() <<endl;
    // cout << "MCErrLow Size" << MCErrLow.size() <<endl;
    MCErrorRatioGraph->SetLineWidth(0);
    MCErrorRatioGraph->SetLineColor(0);
    MCErrorRatioGraph->SetFillColor(1);
    MCErrorRatioGraph->SetFillStyle(ErrorBandFillStyle);
  }

  void CreateRatioPlots() {
    if (!HasMC) return;
    ExpOverMC.resize(SigNames.size());
    if (HasSig) {
      for (unsigned isig = 0; isig < SigNames.size(); ++isig) {
        ExpOverMC[isig] = (TH1F*)SigHists[isig][0]->Clone();
        ReverseXAxis(ExpOverMC[isig]);
        ExpOverMC[isig]->SetDirectory(0);
        ExpOverMC[isig]->Add(MCSummed[0]);
        ExpOverMC[isig]->Divide(MCSummed[0]);
        ExpOverMC[isig]->SetFillColor(0);
        ReverseXAxis(ExpOverMC[isig]);
      }
    }
    if (!IsSR && HasData) {
      DataOverMC = (TH1F*)DataHist->Clone();
      ReverseXAxis(DataOverMC);
      DataOverMC->SetDirectory(0);
      DataOverMC->SetTitle(LTitle); // Not necessary because ErrorGraph will be drawn first
      TH1F* hmc = (TH1F*)MCSummed[0]->Clone();
      //ReverseXAxis(hmc);
      DataOverMC->Divide(hmc);
      ReverseXAxis(DataOverMC);
      
    }
  }

  void Legend(vector<double> lpos) { // Todo: Dynamic legend position and compress the yaxis if necessary
    double x1 = lpos[0];
    double y1 = lpos[1];
    double x2 = lpos[2];
    double y2 = lpos[3];
    leg = new TLegend(x1,y1,x2,y2,"","NDC");
    leg->SetBorderSize(0);
    leg->SetNColumns(2);
  }

  void CreateLegendEntries() {
    if (HasMC) {
      for (unsigned i = 0; i < MCNames.size(); ++i) {
        leg->AddEntry(MCHists[i],MCNames[i],"f");
      }
    }
    if (HasSig) {
      for (unsigned i = 0; i < SigNames.size(); ++i) {
        leg->AddEntry(SigHists[i][0],SigNames[i],"l");
      }
    }
    if (HasData) {
      leg->AddEntry(DataHist, "Data", "p");
    }
    
  }

  double GetMaximum() {
    return TrueMaximum;
  }

  void SetMaximum(double max) {
    CanvasMaximum = max;
  }
  TString GetSensitivityLatex(TString SensSig = "M500") {
    TString sens = Form("#sqrt{#Sigma_{ibin} Sig ^ 2 / (Sig + BG)} = %f", GetSensitivity(SensSig));
    return sens;
  }

  double GetSensitivity(TString SensSig = "M500") {
    double SensSum = 0;
    for (unsigned i = 0; i < SigNames.size(); ++i) {
      if (SigNames[i] != SensSig) continue;
      TH1F* hsig = SigHists[i][0];
      TH1F* hbg = MCSummed[0];
      for (unsigned i = 1; i <= nbins; ++i) {
        if (hbg->GetBinContent(i) < 10.) continue;
        SensSum = hsig->GetBinContent(i) * hsig->GetBinContent(i) / (hsig->GetBinContent(i) + hbg->GetBinContent(i));
      }
    }
    double sens = sqrt(SensSum);
    return sens;
  }

  TString GetMCPurityLatex() {
    TString out = "";
    for (unsigned i = 0; i < MCNames.size(); ++i) {
      out += Form("#color[%i]{(%.2f%%)} ", MCHists[i]->GetLineColor(), MCHists[i]->Integral(0,-1) / MCSummed[0]->Integral(0,-1) * 100. );
    }
    return out;
  }

  double GetMCPurity(TString sn) {
    for (unsigned i = 0; i < MCNames.size(); ++i) {
      if (MCNames[i] == sn) return MCHists[i]->Integral(0,-1) / MCSummed[0]->Integral(0,-1);
    }
    return 0;
  }

  void SetPad(TVirtualPad* p_) {
    setTDRStyle();
    Pad = p_;
    Pad->cd();
    Pad->UseCurrentStyle();
    TString uppadname = PlotName + (TString)"_upper";
    TString lowpadname = PlotName + (TString)"_lower";
    UPad = new TPad(uppadname,uppadname,0,0.3,1,1);
    UPad->SetTopMargin(gStyle->GetPadTopMargin()/0.7);
    UPad->SetBottomMargin(0.0);
    UPad->SetLogy(Logy);
    UPad->Draw("");
    LPad = new TPad(lowpadname,lowpadname,0,0,1,0.3);
    LPad->Draw("");
    LPad->SetTopMargin(0.1);
    LPad->SetTopMargin(gStyle->GetPadTopMargin()*0.3);
    LPad->SetBottomMargin(gStyle->GetPadBottomMargin()/0.3);
    LPad->SetGridy();
  }

  bool DrawPlot(int year, double cxmin = -1, double cxmax = -1) { // return false if nothing to draw
    cout << "Start DrawPlot RatioPlot" <<endl;
    if (!HasData && !HasMC && !SigNames.size()) return false;
    if (cxmin < 0) cxmin = xlow;
    if (cxmax < 0) cxmax = xup;

    UPad->cd();
    if (CanvasMaximum > 0) MCStack->SetMaximum(CanvasMaximum);
    else MCStack->SetMaximum(TrueMaximum * TrueMaximumScale);
    MCStack->Draw("");
    // cout << "DrawPlot Debug 0" <<endl;
    MCStack->GetXaxis()->SetRangeUser(cxmin, cxmax);
    MCStack->Draw("hist");
    // cout << "DrawPlot Debug 1" <<endl;
    if (MCErrorGraph != nullptr && HasMC) {
      MCErrorGraph->Draw("samef"); // MKR
      leg->AddEntry(MCErrorGraph,"Bkg. Unc.","f");
    }
    // cout << "DrawPlot Debug 2" <<endl;
    if (!IsSR && HasData) DataHist->Draw("E1same ");
    // cout << "DrawPlot Debug 3" <<endl;
    for (unsigned isig = 0; isig < SigNames.size(); ++isig) {
      // cout << "SigNames: " << SigNames[isig] <<endl;
      if (SigHists[isig][0]->GetEntries() == 0) continue;
      SigHists[isig][0]->Draw("samehist ");
      if (SigErrorGraphs.size() == SigNames.size()) SigErrorGraphs[isig]->Draw("f");
    }
    cout << "DrawPlot Debug 4" <<endl;
    // The coefficients are tried out and tested to be placed at same location on canvas
    MCStack->GetYaxis()->SetTitleSize(gStyle->GetTitleSize());
    MCStack->GetYaxis()->SetTitleOffset(gStyle->GetTitleOffset());
    MCStack->GetYaxis()->SetLabelSize(gStyle->GetLabelSize());
    MCStack->GetYaxis()->SetLabelOffset(gStyle->GetLabelOffset());

    // Pad->cd();
    leg->Draw("");

    LPad->cd();
    LowerDummy->SetTitle(LTitle);
    LowerDummy->GetXaxis()->SetRangeUser(cxmin, cxmax);
    LowerDummy->GetYaxis()->SetRangeUser(0.5, 1.4);
    LowerDummy->GetYaxis()->SetNdivisions(505);
    // XBinLabels = {"0", "-1", "-2", "-3", "-4", "-5", "-6", "-7", "-8", "-9", "-10"}; // MKR
    if (XBinLabels.size() != 0) {
      int nb_ = nbins;
      if (XBinLabels.size() < nbins) nb_ = XBinLabels.size();
      //for (int ib = 1; ib <=  11  ; ib++) LowerDummy->GetXaxis()->SetBinLabel(ib*nbins/10 , XBinLabels[10-ib]);
     // for (int ib = 1; ib <= nb_; ib+=nbins/10) LowerDummy->GetXaxis()->ChangeLabel(ib ,-1,-1,-1,-1,-1, XBinLabels[ib - 1]);
      for(int ib = 0; ib<11; ib++) LowerDummy->GetXaxis()->ChangeLabel(1+ib, -1,-1,-1,-1,-1, XBinLabels[ib]);
    }
    // LowerDummy->GetXaxis()->CenterTitle();
    LowerDummy->GetXaxis()->SetTitleSize(gStyle->GetTitleSize() / 0.3 * 0.7);
    LowerDummy->GetXaxis()->SetTitleOffset(gStyle->GetTitleOffset());
    LowerDummy->GetXaxis()->SetLabelSize(gStyle->GetLabelSize() / 0.3 * 0.7);
    LowerDummy->GetXaxis()->SetLabelOffset(gStyle->GetLabelOffset());

    LowerDummy->GetYaxis()->CenterTitle();
    LowerDummy->GetYaxis()->SetTitleSize(gStyle->GetTitleSize() / 0.3 * 0.7);
    LowerDummy->GetYaxis()->SetTitleOffset(gStyle->GetTitleOffset() * 0.5 );
    LowerDummy->GetYaxis()->SetLabelSize(gStyle->GetLabelSize()/ 0.3 * 0.7);
    LowerDummy->GetYaxis()->SetLabelOffset(gStyle->GetLabelOffset());
    LowerDummy->Draw("");


    if (HasMC) {  
      cout << "MCErrorRatioGraph->Draw" <<endl;
      MCErrorRatioGraph->Draw("f same "); //MKR
      cout << "MCErrorRatioGraph->Draw Done" <<endl;
      if (!IsSR && HasData) DataOverMC->Draw("same ");
      cout << "MCErrorRatioGraph->Draw Done2" <<endl;
      for (unsigned isig = 0; isig < SigNames.size(); ++isig) {
        ExpOverMC[isig]->Draw("same hist ][ ");
      }
    }
    
    CMSFrame(UPad,year,IsSR);
    return true;
  }

  void DrawLatex(TString st, int iline = 0, double x = -1., double y = -1., int al = 23) {
    UPad->cd(); // Remember to reverse to previous pad after drawing latex!
    latex.SetTextAlign(al);
    if (x == -1) x = (leg->GetX1() + leg->GetX2()) / 2.;
    if (y == -1) y = leg->GetY1() - 0.025;
    y -= 1.15 * latex.GetTextSize() * iline;
    latex.DrawLatex(x,y,st);
  }

  void SaveInfos(TString pn) {
    TString OutFileName = pn + ".txt";
    ofstream f(OutFileName);
    f << "Central BinContent Summation = " << MCStatUncertIntegral[0] << ", Central Integral = " << MCSummed[0]->Integral();
    f << ", Maximum = " << MCSummed[0]->GetMaximum() << "\n";
    f << "Uncertainties Contributions:" << "\n";
    f << "StatUp = " << MCStatUncertIntegral[1] / MCStatUncertIntegral[0] - 1. << "\n";
    f << "StatDown = " << MCStatUncertIntegral[2] / MCStatUncertIntegral[0] - 1. << "\n";
    for (unsigned i = 1; i < VarSize; ++i) {
      f << Variations[i] << " = " << MCSystUncertIntegral[i] / MCSystUncertIntegral[0] << "\n";
    }
    f << "\nMC Purity:\n";
    for (unsigned i = 0; i < MCNames.size(); ++i) {
      f << Form("[%s]: %.2f%% \n", MCNames[i].Data(), MCHists[i]->Integral(0,-1) / MCSummed[0]->Integral(0,-1) * 100. );
    }
    f.close();
  }

  bool Logy, IsSR;

  TVirtualPad* Pad;
  TVirtualPad* UPad;
  TVirtualPad* LPad;
  TLegend* leg = nullptr;
  TLatex latex;

  TString PlotName, UTitle, LTitle;

  vector<string> Variations;
  unsigned VarSize = 0;
  unsigned nbins = 0;
  double xlow, xup;
  double MinY = 0;
  double CanvasMaximum = 0;
  double TrueMaximum = 0;
  // double TrueMaximumScale = 5.0;
  double TrueMaximumScale = 1.2;

  bool HasData = false;
  bool HasMC = false;
  bool HasSig = false;

  TH1F* DataHist = nullptr;

  vector<TH1F*> MCHists;
  vector<TString> MCNames;
  vector<TH1F*> MCSummed; // [iVariation]
  THStack* MCStack;
  TH1F* StackDummy = nullptr;
  TH1F* DataOverMC = nullptr;

  vector<vector<TH1F*> > SigHists; // [iSig][iVariation]
  vector<TH1F*> SigHistsHolder; // Container for SigHists so this class own it's own Signal Hists
  vector<TString> SigNames;
  vector<TH1F*> ExpOverMC; // [iSig];

  vector<double> MCErrUp, MCErrLow; // [nBins]
  vector< vector<double> > SigErrUp, SigErrLow; //[iSig][nBins]

  vector<double> MCStatUncertIntegral; //[Nom;Up;Down]
  vector<double> MCSystUncertIntegral; //[iVariation]

  int ErrorBandFillStyle = 3002;
  TGraph* MCErrorGraph = nullptr;
  vector<TGraph*> SigErrorGraphs;
  TGraph* MCErrorRatioGraph = nullptr;
  TH1F* LowerDummy;
  vector<TString> XBinLabels;
 

};

struct PlotObservable {
  string Observable;
  string XTitle;
  string YTitle = "";
  double YEnlarge = 1.;
  double xmin = -1;
  double xmax = -1;
  int LegPos = 0;
  vector<double> LegendPos = {0.65,0.65,0.9,0.9};
};


#endif
