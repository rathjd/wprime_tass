import os,sys

from ROOT import TH1F, TFile, THStack, TRatioPlot, TCanvas, TLegend, TLine, TF1, TMatrixD, TFitResultPtr, TGraph, TColor, TPaveStats, TGraphAsymmErrors
import cmsstyle as CMS
from array import array
import math

#settings for what to plot
WpMass = "500"
channel = "ch1_ch1_ch1" 

#accept shell inputs
try:
    if int(sys.argv[1]) > 0:
        WpMass = sys.argv[1]
        print("W' mass set to", WpMass)
except:
    print("W' mass defaults to", WpMass)

try:
    if sys.argv[2].find("ch") > -1:
        channel = sys.argv[2]
        print("channel set to", channel)
except:
    print("channel defaults to", channel)

#configure naming based on input channel:
year = ""
if channel[6] == "1":
    year = "2016apv"
elif channel[6] == "2":
    year = "2016"
elif channel[6] == "3":
    year = "2017"
elif channel[6] == "4":
    year = "2018"

print("year determined to be", year)

if channel[2] == "1" and len(channel) == 11:
    code = "1152"
elif channel[2] == "2":
    code = "1162"
elif channel[2] == "3":
    code = "2152"
elif channel[2] == "4":
    code = "2162"

#set basic region name from code
if code[0] == "1":
    name = "#mu"
else:
    name = "e"
name += " + " + code[2] + "jets and " + code[3] + " b tags"

if channel[10] == "1":
    var = "m_{fit} [GeV]"
else:
    var = "H_{T} [GeV]"

#CMS style setup
CMS.SetEnergy("13")
if year == "2016":
    CMS.SetLumi("16.8")
elif year == "2016apv":
    CMS.SetLumi("19.52")
elif year == "2017":
    CMS.SetLumi("42.1")
elif year == "2018":
    CMS.SetLumi("59.6")

#set color options and names for contributions
backgrounds = [["ttbar", 2],
               ["wjets", 3],
               ["single_top", 4],
               ["diboson", 5]]

signal = ["M"+WpMass, int(WpMass)/100+3]

#set input directory
path = "/afs/cern.ch/user/r/rathjd/work/private/TakeOverWprime/wprime/CombineMacros/Combination/"

#configure legend
leg = CMS.cmsLeg(0.51,0.89-0.05*7, 0.8, 0.89, textSize=0.05)

#configure header
CMS.cmsHeader(leg, name, textSize=0.05)

#read root file with fit diagnostics
infile = TFile("fitDiagnosticsTest.root","read")

#define binnings
binnings = {"5_1": ["nFitLimits53_500", "FitLimits53_500", 0, array('d')],
            "5_2": ["nHTlimits53", "HTlimits53", 0, array('d')],
            "6_1": ["nFitLimits63_500", "FitLimits63_500", 0, array('d')],
            "6_2": ["nHTlimits63", "HTlimits63", 0, array('d')]}

binFile = open("BinTables.C", "r")
binLines = binFile.readlines()
for line in binLines:
    if line.find(binnings[code[2]+"_"+channel[10]][0]) > -1:
        splits = line.split()
        binnings[code[2]+"_"+channel[10]][2] = int(splits[3][0:len(splits[3])-1])
    if line.find(binnings[code[2]+"_"+channel[10]][1]) == 7:
        splits = line.split()
        for i, split in enumerate(splits):
            if i==3:
                binnings[code[2]+"_"+channel[10]][3].append(float(split[1:len(split)-1]))
            elif split.find("}") > -1:
                binnings[code[2]+"_"+channel[10]][3].append(float(split[0:len(split)-2]))
            elif i > 3:
                binnings[code[2]+"_"+channel[10]][3].append(float(split[0:len(split)-1]))

#read histograms and convert into proper binning
h_bkg_prefit = {}
for bkg in backgrounds:
    temp = infile.Get("shapes_prefit/"+channel+"/"+bkg[0])
    if type(temp) == TH1F:
        h_bkg_prefit[bkg[0]] = TH1F(bkg[0], "", binnings[code[2]+"_"+channel[10]][2], binnings[code[2]+"_"+channel[10]][3])
        for b in range(1, binnings[code[2]+"_"+channel[10]][2]+2):
            h_bkg_prefit[bkg[0]].SetBinContent(b, temp.GetBinContent(b))
        h_bkg_prefit[bkg[0]].SetFillColor(bkg[1])
        h_bkg_prefit[bkg[0]].Scale(1.,"width")

temp = infile.Get("shapes_prefit/"+channel+"/total_background")
h_bkgErr_prefit = TH1F("bkgErr_prefit", "", binnings[code[2]+"_"+channel[10]][2], binnings[code[2]+"_"+channel[10]][3])
for b in range(1, binnings[code[2]+"_"+channel[10]][2]+2):
        h_bkgErr_prefit.SetBinContent(b, temp.GetBinContent(b))
        h_bkgErr_prefit.SetBinError(b, temp.GetBinError(b))
h_bkgErr_prefit.SetFillColor(17)
h_bkgErr_prefit.SetFillStyle(3008)
h_bkgErr_prefit.Scale(1.,"width")

temp = infile.Get("shapes_prefit/"+channel+"/data")
points = [array('d'), array('d'), array('d'), array('d'), array('d'), array('d')] #x, y, exl, exh, eyl, eyh
ratioPoints = [array('d'), array('d'), array('d')] #y, eyl, eyh
for p in range(0, binnings[code[2]+"_"+channel[10]][2]+1):
    points[0].append(h_bkgErr_prefit.GetBinCenter(p+1))
    points[1].append(temp.GetPointY(p)/h_bkgErr_prefit.GetBinWidth(p+1))
    points[2].append(h_bkgErr_prefit.GetBinLowEdge(p+1)-h_bkgErr_prefit.GetBinCenter(p+1))
    points[3].append(h_bkgErr_prefit.GetBinLowEdge(p+1)-h_bkgErr_prefit.GetBinCenter(p+1))
    points[4].append(temp.GetErrorYlow(p)/h_bkgErr_prefit.GetBinWidth(p+1))
    points[5].append(temp.GetErrorYhigh(p)/h_bkgErr_prefit.GetBinWidth(p+1))

    if h_bkgErr_prefit.GetBinContent(p+1) > 0.:
        ratioPoints[0].append(temp.GetPointY(p)/h_bkgErr_prefit.GetBinWidth(p+1)/h_bkgErr_prefit.GetBinContent(p+1))
        ratioPoints[1].append(temp.GetErrorYlow(p)/h_bkgErr_prefit.GetBinWidth(p+1)/h_bkgErr_prefit.GetBinContent(p+1))
        ratioPoints[2].append(temp.GetErrorYhigh(p)/h_bkgErr_prefit.GetBinWidth(p+1)/h_bkgErr_prefit.GetBinContent(p+1))

h_dat = TGraphAsymmErrors(binnings[code[2]+"_"+channel[10]][2], points[0], points[1], points[2], points[3], points[4], points[5])
h_dat.SetLineColor(1)

h_dat_ratio = TGraphAsymmErrors(binnings[code[2]+"_"+channel[10]][2], points[0], ratioPoints[0], points[2], points[3], ratioPoints[1], ratioPoints[2])
h_dat_ratio.SetLineColor(1)

temp = infile.Get("shapes_fit_b/"+channel+"/total_background")
h_bkg_postfit = TH1F("bkg_postfit", "", binnings[code[2]+"_"+channel[10]][2], binnings[code[2]+"_"+channel[10]][3])
for b in range(1, binnings[code[2]+"_"+channel[10]][2]+2):
        h_bkg_postfit.SetBinContent(b, temp.GetBinContent(b))
        h_bkg_postfit.SetBinError(b, temp.GetBinError(b))
h_bkg_postfit.SetLineColor(2)
h_bkg_postfit.SetMarkerStyle(1)
h_bkg_postfit.Scale(1.,"width")

#make prefit to postfit comparison plot
max_prefit = max(h_bkgErr_prefit.GetMaximum()*1.2, h_dat.GetMaximum()*1.2)
canv = CMS.cmsDiCanvas(channel, 0, binnings[code[2]+"_"+channel[10]][3][len(binnings[code[2]+"_"+channel[10]][3])-1], 0, max_prefit, 0.5, 1.5, var, "Events/GeV", "Data/Prefit", square=CMS.kSquare, extraSpace=0.1, iPos=0)
canv.cd(1)

leg.AddEntry(h_dat, "Data", "lp")
Stack_prefit = THStack("Stack_prefit_"+channel,"")
CMS.cmsDrawStack(Stack_prefit, leg, h_bkg_prefit)
h_bkgErr_prefit.Draw("E2,same")
CMS.cmsDraw(h_dat, "P", mcolor=1)
leg.AddEntry(h_bkgErr_prefit,"prefit unc.","F")
leg.AddEntry(h_bkg_postfit,"postfit","L")
leg.Draw()

h_bkg_postfit.Draw("P,same")

CMS.fixOverlay()

canv.cd(2)

Ratio_band_prefit = h_bkgErr_prefit.Clone("Ratio_band_prefit")
for i in range(0, Ratio_band_prefit.GetNbinsX()+1):
    if Ratio_band_prefit.GetBinContent(i+1) > 0.:
        Ratio_band_prefit.SetBinError(i+1, Ratio_band_prefit.GetBinError(i+1) / Ratio_band_prefit.GetBinContent(i+1))
        Ratio_band_prefit.SetBinContent(i+1, 1.)

Ratio_band_prefit.Draw("E2,same")

#ref_line = TLine(0, 1, h_bkgErr_prefit.GetNbinsX(), 1)
#CMS.cmsDrawLine(ref_line, lcolor=1, lstyle=3)

CMS.cmsDraw(h_dat_ratio, "P", mcolor=1)

Ratio_postfit = h_bkg_postfit.Clone("Ratio_postfit")
prefitNoErr = h_bkgErr_prefit.Clone("prefitNoErr")
Ratio_postfit.Divide(h_bkgErr_prefit)

for i in range(0, Ratio_postfit.GetNbinsX()+1):
    if h_bkgErr_prefit.GetBinContent(i+1) > 0:
        Ratio_postfit.SetBinError(i+1, h_bkg_postfit.GetBinError(i+1)/h_bkgErr_prefit.GetBinContent(i+1))
    else:
        Ratio_postfit.SetBinError(i+1, 0.)

Ratio_postfit.Draw("P,same")

CMS.SaveCanvas(canv, "PrePostFitComparison_"+channel+".pdf")

