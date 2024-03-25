import os,sys

from ROOT import TFile, TTree, TH3F, TH2F, TGraph, TCanvas, gStyle
from array import array
import math

#macro to study splits between fit mass and HT along the negative log likelihood
#needs to set up a cmsenv capable of running Combine to function

year = "2018"
binNr = "2163"

try:
    if int(sys.argv[1]) > 0:
        year = sys.argv[1]
        print("year set to "+ year)
except:
    print("year defaults to "+ year)

try:
    if int(sys.argv[2]) > 0:
        binNr = sys.argv[2]
        print("bin number set to "+ binNr)
except:
    print("bin number defaults to "+ binNr)

if binNr[2:] == "54":
    print("No possible split for 5 jets 4 b-tags, as fit output nonsensical, aborting")
    exit

#maximum number of NLL bins depending on jet multiplicity
maxNLL = 0
if binNr[2] == "5":
    maxNLL = 52
elif binNr[2] == "6":
    maxNLL = 23

#run over signal mass points in 100 GeV steps between 300 and 1100
for mass in range(0,9):
    current = 0
    massString = str((3+mass)*100)

    #configure output file for analysis of splits
    savefile = TFile("LimitSplits_M"+massString+"_"+binNr+"_"+year+".root","RECREATE")

    #HThist = TH3F("HThist_"+massString,"H_{T} limits;binSplit;binEnd;limit category", 11, 4.5, 15.5, 19, 5.5, 24.5, 6, -0.5, 5.5)
    #FitHist = TH3F("FitHist_"+massString,"fitted mass limits;binSplit;binEnd;limit category", 11, 4.5, 15.5, 19, 5.5, 24.5, 6, -0.5, 5.5)
    CombHist = TH3F("CombHist_"+massString,"Combination limits;binSplit;binEnd;limit category", maxNLL, -0.5, -0.5+maxNLL, maxNLL, -0.5, -0.5+maxNLL, 6, -0.5, 5.5)
    CombHistExp = TH2F("CombHistExp_"+massString,"Combination limits expected;binSplit;binEnd", maxNLL, -0.5, -0.5+maxNLL, maxNLL, -0.5, -0.5+maxNLL)
    expLimits = array( 'd' )
    LimitUncAve = array( 'd' )

    for binSplit in range(1, maxNLL):
        for binEnd in range(binSplit+1, maxNLL):
            #run the splitter
            os.system("python3 SliceHTvsFitMass.py "+year+" "+binNr+" "+str(binSplit)+" "+str(binEnd))

            #run the limit on HT
            #try:
            #    os.system("combine -M AsymptoticLimits -m "+massString+" HTslice_Wprime"+binNr+"_"+year+".txt")
            #except:
            #    print("HT failed for ",binSplit,":",binEnd)
            #    continue

            #extract the information on HT
            #infile = TFile("higgsCombineTest.AsymptoticLimits.mH"+massString+".root","READ")
            #counter = 0
            #for event in infile.limit:
            #    HThist.SetBinContent(binSplit, binEnd, counter+1, event.limit)
            #    counter+=1
            
            #clean up
            #infile.Close()
            #os.system("rm higgsCombineTest.AsymptoticLimits.mH"+massString+".root")

            #run the limit on the fit
            #try:
            #    os.system("combine -M AsymptoticLimits -m "+massString+" FitSlice_Wprime"+binNr+"_"+year+".txt")
            #except:
            #    print"Fit mass failed for ",binSplit,":",binEnd)
            #    continue

            #extract the information on the fit
            #infile = TFile("higgsCombineTest.AsymptoticLimits.mH"+massString+".root","READ")
            #counter = 0
            #for event in infile.limit:
            #    FitHist.SetBinContent(binSplit-4, binEnd-5, counter+1, event.limit)
            #    counter+=1

            #clean up
            #infile.Close()
            #os.system("rm higgsCombineTest.AsymptoticLimits.mH"+massString+".root")

            #run the limit on the combination
            try:
                os.system("combine -M AsymptoticLimits -m "+massString+" CombinationAll/CombinationSlices_Wprime"+binNr+"_"+year+".txt")
            except:
                print("Combination failed for ",binSplit,":",binEnd)
                continue

            #extract the information on the fit
            infile = TFile("higgsCombineTest.AsymptoticLimits.mH"+massString+".root","READ")
            counter = 0
            LimitUnc = []
            currLimit = 0.
            for event in infile.limit:
                CombHist.SetBinContent(binSplit+1, binEnd+1, counter+1, event.limit)
                if counter == 2: #expected limit
                    currLimit = event.limit
                    CombHistExp.SetBinContent(binSplit+1, binEnd+1, event.limit)
                elif counter == 1 or counter == 3: #+/- 1 sigma
                    LimitUnc.append(event.limit)
                counter+=1
            LimitUncAve.append((math.fabs(LimitUnc[0]-currLimit) + math.fabs(LimitUnc[1]-currLimit))/2.)
            expLimits.append(currLimit)

            #clean up
            infile.Close()
            os.system("rm higgsCombineTest.AsymptoticLimits.mH"+massString+".root")

    savefile.cd()
    #HThist.Write()
    #FitHist.Write()
    CombHist.Write()

    #make a TGraph that shows the space of uncertainties and limits that is covered
    Graph = TGraph(len(expLimits), expLimits, LimitUncAve)
    canvas = TCanvas("LimitGraph_"+massString,year+" "+ binNr+" m(W')="+massString,1000,1000)
    Graph.SetMarkerStyle(47)
    Graph.SetMarkerColor(1)
    Graph.SetLineColor(1)
    Graph.Draw("A,P,L")
    canvas.Write()
    canvas.SaveAs("LimitGraph_"+year+"_"+ binNr+"_mWp"+massString+".pdf")

    canvas2 = TCanvas("ExpLimits_"+massString,year+" "+ binNr+" m(W')="+massString,1000,1000)
    gStyle.SetPaintTextFormat("0.3f")
    CombHistExp.Draw("colz,text")
    canvas2.SaveAs("ExpLimits_"+year+"_"+ binNr+"_mWp"+massString+".pdf")
    savefile.Close()
