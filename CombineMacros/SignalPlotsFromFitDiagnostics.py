import os,sys

from ROOT import TH1F, TFile, THStack, TRatioPlot, TCanvas, TLegend, TLine, TF1, TMatrixD, TFitResultPtr, TGraph, TColor, TPaveStats
import cmsstyle as CMS
from array import array
import math

#settings for what to plot
WpMass = "500"
channel = "ch3_ch1_ch1" 

#accept shell inputs
try:
    if int(sys.argv[1]) > 0:
        WpMass = sys.argv[1]
        print("W' mass set to", WpMass)
except:
    print("W' mass defaults to", WpMass)

#configure naming based on input channel:
year = ""
if channel[2] == "1":
    year = "2016"
elif channel[2] == "2":
    year = "2016apv"
elif channel[2] == "3":
    year = "2017"
elif channel[2] == "4":
    year = "2018"

print("year determined to be", year)

if channel[6] == "1" and len(channel) == 11:
    code = "1152"
elif channel[6] == "2":
    code = "2152"
elif channel[6] == "3":
    code = "1153"
elif channel[6] == "4":
    code = "2153"
elif channel[6] == "5":
    code = "1162"
elif channel[6] == "6":
    code = "2162"
elif channel[6] == "7":
    code = "1163"
elif channel[6] == "8":
    code = "2163"
elif channel[6] == "9":
    code = "1164"
elif len(channel) > 11:
    code = "2164"

#set basic region name from code
if code[0] == "1":
    name = "#mu"
else:
    name = "e"
name += " + " + code[2] + "jets and " + code[3] + " b tags"

if channel[10] == "1":
    var = "m_{fit} [GeV/c]"
else:
    var = "H_{T} [GeV/c]"

#CMS style setup
CMS.SetEnergy("13")
if year == "2016":
    CMS.SetLumi("16.8")
elif year == "2016apv":
    CMS.SetLumi("19.52")
elif year == "2017":
    CMS.SetLumi("41.5")
elif year == "2018":
    CMS.SetLumi("59.8")

#set color options and names for contributions
backgrounds = [["ttbar", 2],
               ["wjets", 3],
               ["single_top", 4],
               ["diboson", 5]]

signal = ["M"+WpMass, int(WpMass)/100+3]

#set input directory
path = "/afs/cern.ch/user/r/rathjd/work/private/TakeOverWprime/wprime/CombineMacros/Combination/"

#configure legend
leg_prefit = CMS.cmsLeg(0.51,0.89-0.05*6, 0.8, 0.89, textSize=0.05)
leg_postfit = CMS.cmsLeg(0.51,0.89-0.05*6, 0.8, 0.89, textSize=0.05)

#read root file with fit diagnostics
infile = TFile("Combination/fitDiagnosticsTest.root","read")

#read prefit histograms
h_bkg_prefit = {}
for bkg in backgrounds:
    temp = infile.Get("shapes_prefit/"+channel+"/"+bkg[0])
    if type(temp) == TH1F:
        #FIXME: Needs to be molded into right binning, then scaled by bin width
        h_bkg_prefit[bkg[0]] = temp

h_sig_prefit = infile.Get("shapes_prefit/"+channel+"/M"+WpMass)
h_bkgErr_prefit = infile.Get("shapes_prefit/"+channel+"/total_background")
h_dat_prefit = infile.Get("shapes_prefit/"+channel+"/data")

#make prefit plot
max_prefit = max(h_bkgErr_prefit.GetMaximum(), h_dat_prefit.GetMaximum(), h_sig_prefit.GetMaximum())
canv_prefit = CMS.cmsDiCanvas("prefit_"+channel, 0, h_bkgErr_prefit.GetNbinsX(), 0, max_prefit, 0.5, 1.5, var, "Events", "Data/Pred.", square=CMS.kSquare, extraSpace=0.1, iPos=0)
canv_prefit.cd(1)

leg_prefit.AddEntry(h_dat_prefit, "Data", "lp")
Stack_prefit = THStack("Stack_prefit_"+channel,"")
CMS.cmsDrawStack(Stack_prefit, leg_prefit, h_bkg_prefit)
h_bkgErr_prefit.SetFillColor(17)
h_bkgErr_prefit.SetFillStyle(3008)
h_bkgErr_prefit.Draw("E2SAME")
CMS.cmsDraw(h_dat_prefit, "P", mcolor=1)
leg_prefit.Draw()

CMS.fixOverlay()

canv_prefit.cd(2)

Ratio_band_prefit = h_bkgErr_prefit.Clone("Ratio_band_prefit")
for i in range(0, Ratio_band_prefit.GetNbinsX()+1):
    if Ratio_band_prefit.GetBinContent(i+1) > 0.:
        Ratio_band_prefit.SetBinError(i+1, Ratio_band_prefit.GetBinError(i+1) / Ratio_band_prefit.GetBinContent(i+1))
        Ratio_band_prefit.SetBinContent(i+1, 1.)

Ratio_band_prefit.Draw("E2")

ref_line = TLine(0, 1, h_bkgErr_prefit.GetNbinsX(), 1)
CMS.cmsDrawLine(ref_line, lcolor=1, lstyle=3)

Ratio_prefit = h_dat_prefit.Clone("Ratio_prefit")
#this is a TGraphAsymmErrors, soset point coordinates anew...
for i in range(0, Ratio_prefit.GetN()):
    expected = h_bkgErr_prefit.GetBinContent(i+1)
    if expected > 0.:
        Ratio_prefit.SetPointY(i, Ratio_prefit.GetPointY(i)/expected)
        Ratio_prefit.SetPointEYhigh(i, Ratio_prefit.GetErrorYhigh(i)/expected)
        Ratio_prefit.SetPointEYlow(i, Ratio_prefit.GetErrorYlow(i)/expected)
    else:
        Ratio_prefit.SetPointY(i, 0.)
        Ratio_prefit.SetPointEYhigh(i, 0.)
        Ratio_prefit.SetPointEYlow(i, 0.)


CMS.cmsDraw(Ratio_prefit, "P,same", mcolor=1)

CMS.SaveCanvas(canv_prefit, "prefit_"+channel+".pdf")

#read postfit histograms
h_bkg_postfit = {}
for bkg in backgrounds:
    temp = infile.Get("shapes_fit_b/"+channel+"/"+bkg[0])
    if type(temp) == TH1F:
        #FIXME: Needs to be molded into right binning, then scaled by bin width
        h_bkg_postfit[bkg[0]] = temp

h_sig_postfit = infile.Get("shapes_fit_b/"+channel+"/M"+WpMass)
h_bkgErr_postfit = infile.Get("shapes_fit_b/"+channel+"/total_background")
h_dat_postfit = infile.Get("shapes_fit_b/"+channel+"/data")

#make postfit plot
max_postfit = max(h_bkgErr_postfit.GetMaximum(), h_dat_postfit.GetMaximum(), h_sig_postfit.GetMaximum())
canv_postfit = CMS.cmsDiCanvas("postfit_"+channel, 0, h_bkgErr_postfit.GetNbinsX(), 0, max_postfit, 0.5, 1.5, var, "Events", "Data/Pred.", square=CMS.kSquare, extraSpace=0.1, iPos=0)
canv_postfit.cd(1)

leg_postfit.AddEntry(h_dat_postfit, "Data", "lp")
Stack_postfit = THStack("Stack_postfit_"+channel,"")
CMS.cmsDrawStack(Stack_postfit, leg_postfit, h_bkg_postfit)
h_bkgErr_postfit.SetFillColor(17)
h_bkgErr_postfit.SetFillStyle(3008)
h_bkgErr_postfit.Draw("E2SAME")
CMS.cmsDraw(h_dat_postfit, "P", mcolor=1)
leg_postfit.Draw()

CMS.fixOverlay()

canv_postfit.cd(2)

Ratio_band_postfit = h_bkgErr_postfit.Clone("Ratio_band_postfit")
for i in range(0, Ratio_band_postfit.GetNbinsX()+1):
    if Ratio_band_postfit.GetBinContent(i+1) > 0.:
        Ratio_band_postfit.SetBinError(i+1, Ratio_band_postfit.GetBinError(i+1) / Ratio_band_postfit.GetBinContent(i+1))
        Ratio_band_postfit.SetBinContent(i+1, 1.)

Ratio_band_postfit.Draw("E2")

ref_line = TLine(0, 1, h_bkgErr_postfit.GetNbinsX(), 1)
CMS.cmsDrawLine(ref_line, lcolor=1, lstyle=3)

Ratio_postfit = h_dat_postfit.Clone("Ratio_postfit")
#this is a TGraphAsymmErrors, soset point coordinates anew...
for i in range(0, Ratio_postfit.GetN()):
    expected = h_bkgErr_postfit.GetBinContent(i+1)
    if expected > 0.:
        Ratio_postfit.SetPointY(i, Ratio_postfit.GetPointY(i)/expected)
        Ratio_postfit.SetPointEYhigh(i, Ratio_postfit.GetErrorYhigh(i)/expected)
        Ratio_postfit.SetPointEYlow(i, Ratio_postfit.GetErrorYlow(i)/expected)
    else:
        Ratio_postfit.SetPointY(i, 0.)
        Ratio_postfit.SetPointEYhigh(i, 0.)
        Ratio_postfit.SetPointEYlow(i, 0.)
CMS.cmsDraw(Ratio_postfit, "P,same", mcolor=1)

CMS.SaveCanvas(canv_postfit, "postfit_"+channel+".pdf")

