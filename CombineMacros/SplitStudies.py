import os,sys

from ROOT import TFile, TTree, TH3F, TH2F, TGraph, TCanvas, gStyle
from array import array
import math

#macro to study splits between fit mass and HT along the negative log likelihood
#needs to set up a cmsenv capable of running Combine to function

year = "2018"
binNr = "2163"
mass = 300

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

try:
    if int(sys.argv[3]) > -1:
        mass = (int(sys.argv[3])+3)*100
        print("mass set to", mass)
except:
    print("mass defaults to", mass)

#maximum number of NLL bins depending on jet multiplicity
maxNLL = 0
if binNr[2] == "5":
    maxNLL = 52
elif binNr[2] == "6":
    maxNLL = 23

massString = str(mass)

#configure output file for analysis of splits
if not os.path.isdir(year+"_"+binNr+"_"+massString+"_combineTmp"):
    os.system("mkdir "+year+"_"+binNr+"_"+massString+"_combineTmp")
os.chdir(year+"_"+binNr+"_"+massString+"_combineTmp")
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
        os.system("python3 ../SliceHTvsFitMass.py "+year+" "+binNr+" "+str(binSplit)+" "+str(binEnd)+" "+massString)

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
        skip = False

        #check for empty bins
        if binNr[0]=="1":
            FitRm = TFile("../CombinationAll/FitSlices_Wprime"+binNr+"_"+year+"_M"+massString+".root", "READ")
            BgrMfit = FitRm.Get("Fit_ttbar_WprimeFit"+binNr+"_"+year+"_M"+massString+"_")
            SigMfit = FitRm.Get("Fit_M"+massString+"_WprimeFit"+binNr+"_"+year+"_M"+massString+"_")

            HTRm = TFile("../CombinationAll/HTslices_Wprime"+binNr+"_"+year+"_M"+massString+".root", "READ")
            BgrMHT = HTRm.Get("HT_ttbar_WprimeHT"+binNr+"_"+year+"_M"+massString+"_")
            SigMHT = HTRm.Get("HT_M"+massString+"_WprimeHT"+binNr+"_"+year+"_M"+massString+"_")

            for binN in range(0, BgrMfit.GetNbinsX()):
                if BgrMfit.GetBinContent(binN+1) <= 0 and SigMfit.GetBinContent(binN+1) > 0:
                    skip = True
                    print("fit slice had empty background bin",binN+1)
            if skip:
                break #skip this entire row of scans, as the fit slice size doesn't change and will be empty

            for binN in range(0, BgrMHT.GetNbinsX()):
                if BgrMHT.GetBinContent(binN+1) <= 0 and SigMHT.GetBinContent(binN+1) > 0:
                    skip = True
                    print("HT slice had empty background bin",binN+1)
        elif binNr[0]=="2":
            FitRe = TFile("../CombinationAll/FitSlices_Wprime"+binNr+"_"+year+"_M"+massString+".root", "READ")
            BgrEfit = FitRe.Get("Fit_ttbar_WprimeFit"+binNr+"_"+year+"_M"+massString+"_")
            SigEfit = FitRe.Get("Fit_M"+massString+"_WprimeFit"+binNr+"_"+year+"_M"+massString+"_")

            HTRe = TFile("../CombinationAll/HTslices_Wprime"+binNr+"_"+year+"_M"+massString+".root", "READ")
            BgrEHT = HTRe.Get("HT_ttbar_WprimeHT"+binNr+"_"+year+"_M"+massString+"_")
            SigEHT = HTRe.Get("HT_M"+massString+"_WprimeHT"+binNr+"_"+year+"_M"+massString+"_")

            for binN in range(0, BgrEfit.GetNbinsX()):
                if BgrEfit.GetBinContent(binN+1) <= 0 and SigEfit.GetBinContent(binN+1) > 0:
                    skip = True
                    print("fit slice had empty background bin",binN+1)
            if skip:
                break #skip this entire row of scans, as the fit slice size doesn't change and will be empty

            for binN in range(0, BgrEHT.GetNbinsX()):
                if BgrEHT.GetBinContent(binN+1) <= 0 and SigEHT.GetBinContent(binN+1) > 0:
                    skip = True
                    print("HT slice had empty background bin",binN+1)

        if skip:
            continue

        try:
            os.system("combine -M AsymptoticLimits -m "+massString+" ../CombinationAll/CombinationSlices_Wprime"+binNr+"_"+year+"_M"+massString+".txt")
        except:
            print("Combination failed for ",binSplit,":",binEnd)
            skip = True

        if skip:
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
print(len(expLimits),expLimits,LimitUncAve)
Graph = TGraph(len(expLimits), expLimits, LimitUncAve)
canvas = TCanvas("LimitGraph_"+massString,year+" "+ binNr+" m(W')="+massString,1000,1000)
Graph.SetMarkerStyle(47)
Graph.SetMarkerColor(1)
Graph.SetLineColor(1)
Graph.Draw("A,P")
canvas.Write()
canvas.SaveAs("LimitGraph_"+year+"_"+ binNr+"_mWp"+massString+".pdf")

canvas2 = TCanvas("ExpLimits_"+massString,year+" "+ binNr+" m(W')="+massString,1000,1000)
gStyle.SetPaintTextFormat("0.3f")
CombHistExp.Draw("colz,text")
canvas2.SaveAs("ExpLimits_"+year+"_"+ binNr+"_mWp"+massString+".pdf")
savefile.Close()

os.chdir("/afs/cern.ch/user/r/rathjd/work/private/TakeOverWprime/wprime/CombineMacros")
