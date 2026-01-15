import os,sys
from ROOT import TGraph, TFile, TTree, TCanvas, TColor, TLegend, TLatex, kBlack, TMath
from array import array
import cmsstyle as CMS
import ctypes

#configuration for comparing all signal regions combination of slices
ConfigSigComb = [["1153", "CombinationSlices", "all", 1, "H_{T}/m_{fit} #mu 5j 3b Run2"],
                 ["2153", "CombinationSlices", "all", 2, "H_{T}/m_{fit} e 5j 3b Run2"],
                 ["1163", "CombinationSlices", "all", 3, "H_{T}/m_{fit} #mu 6j 3b Run2"],
                 ["1164", "CombinationSlices", "all", 4, "H_{T}/m_{fit} #mu 6j 4b Run2"],
                 ["2163", "CombinationSlices", "all", 5, "H_{T}/m_{fit} e 6j 3b Run2"],
                 ["2164", "CombinationSlices", "all", 6, "H_{T}/m_{fit} e 6j 4b Run2"]]

#configuration to compare all slices in 5j3b regions
ConfigHTvsFit5j = [["1153", "HTslice",  "all", 1, "H_{T} #mu 5j 3b Run2"],
                   ["2153", "HTslice",  "all", 2, "H_{T} e 5j 3b Run2"],
                   ["1153", "FitSlice", "all", 3, "m_{fit} #mu 5j 3b Run2"],
                   ["2153", "FitSlice", "all", 4, "m_{fit} e 5j 3b Run2"]]

#configuration to compare all slices in 6j3b regions
ConfigHTvsFit6j3b = [["1163", "HTslice",  "all", 1, "H_{T} #mu 6j 3b Run2"],
                     ["2163", "HTslice",  "all", 2, "H_{T} e 6j 3b Run2"],
                     ["1163", "FitSlice", "all", 3, "m_{fit} #mu 6j 3b Run2"],
                     ["2163", "FitSlice", "all", 4, "m_{fit} e 6j 3b Run2"]]

#configuration to compare all slices in 6j4b regions
ConfigHTvsFit6j4b = [["1164", "HTslice",  "all", 1, "H_{T} #mu 6j 4b Run2"],
                     ["2164", "HTslice",  "all", 2, "H_{T} e 6j 4b Run2"]]
                     #["1164", "FitSlice", "all", 3, "m_{fit} #mu 6j 4b Run2"],
                     #["2164", "FitSlice", "all", 4, "m_{fit} e 6j 4b Run2"]]

#configuration to compare slice combinations for each era
ConfigEraComparison = [["All", "CombinationSlices", "2016",    1, "H_{T}/m_{fit} #mu/e 5-6j 3-4b 2016"],
                       ["All", "CombinationSlices", "2016apv", 2, "H_{T}/m_{fit} #mu/e 5-6j 3-4b 2016apv"],
                       ["All", "CombinationSlices", "2017",    3, "H_{T}/m_{fit} #mu/e 5-6j 3-4b 2017"],
                       ["All", "CombinationSlices", "2018",    4, "H_{T}/m_{fit} #mu/e 5-6j 3-4b 2018"]]

#set of plots to be made
PlotsToBeMade = [[ConfigSigComb,       "CompareAllSRcombinationsRun2"],
                 [ConfigHTvsFit5j,     "CompareAll5jSlicesRun2"],
                 [ConfigHTvsFit6j3b,   "CompareAll6j3bSlicesRun2"],
                 [ConfigHTvsFit6j4b,   "CompareAll6j4bSlicesRun2"],
                 [ConfigEraComparison, "CompareAllErasSRcombinations"]]

CMS.SetLumi("varies")
CMS.SetEnergy("13")

CMS.SetExtraText("Simulation Preliminary")

#loop over configuration entries and store expected lines with line style settings for overview plot
for plot in PlotsToBeMade:
    GraphList = []
    leg = CMS.cmsLeg(0.35, 0.89-0.08*(len(plot)+1), 0.89, 0.89, textSize=0.04)
    leg.SetHeader("Median expected per region")
    MinY = ctypes.c_double(999.)
    MaxY = ctypes.c_double(0.)
    #load all graphs and configure style and legend
    for GraphIn in plot[0]:
        InFile = TFile("Combination/CombinedLimit_"+GraphIn[1]+"_"+GraphIn[0]+"_"+GraphIn[2]+".root","READ");
        GraphList.append(InFile.Get("Expected"))
        GraphList[len(GraphList)-1].SetLineColor(GraphIn[3])
        leg.AddEntry(GraphList[len(GraphList)-1], GraphIn[4], "l")

        #determine the minima and maxima of y-axis across all loaded graphs
        minX = ctypes.c_double(0.)
        maxX = ctypes.c_double(0.)
        minY = ctypes.c_double(0.)
        maxY = ctypes.c_double(0.)
        GraphList[len(GraphList)-1].ComputeRange(minX, minY, maxX, maxY)
        MinY = TMath.Min(minY, MinY)
        MaxY = TMath.Max(maxY, MaxY)

    #draw CMS style comparison plot
    canvas = CMS.cmsCanvas(plot[1], 300., 1100., MinY*0.8, MaxY*1.4, "m_{W'} [GeV]", "#sigma#font[12]{B}(pp #rightarrow tW' #rightarrow ttb) [fb]", square = CMS.kSquare, extraSpace = 0.01, iPos = 0)
    canvas.cd(1)

    #draw all graphs
    for i, graph in enumerate(GraphList):
        if i == 0:
            graph.Draw("L")
        else:
            graph.Draw("L,same")

    #add legend
    leg.Draw()

    #update, then save output
    canvas.Update()
    CMS.SaveCanvas(canvas, plot[1]+".pdf")
