import os,sys
from ROOT import TFile, TTree, TH3F, TH2F

binNr = "2163"
year = "2018"

savefile = TFile("LimitSplits_"+binNr+"_"+year+".root","RECREATE")

for mass in range(0,9):
    current = 0
    massString = str((3+mass)*100)
    savefile = TFile("LimitSplits_M"+massString+"_"+binNr+"_"+year+".root","RECREATE")

    #HThist = TH3F("HThist_"+massString,"H_{T} limits;binSplit;binEnd;limit category", 11, 4.5, 15.5, 19, 5.5, 24.5, 6, -0.5, 5.5)
    #FitHist = TH3F("FitHist_"+massString,"fitted mass limits;binSplit;binEnd;limit category", 11, 4.5, 15.5, 19, 5.5, 24.5, 6, -0.5, 5.5)
    CombHist = TH3F("CombHist_"+massString,"Combination limits;binSplit;binEnd;limit category", 11, 4.5, 15.5, 19, 5.5, 24.5, 6, -0.5, 5.5)
    for binSplit in range(5,15):
        for binEnd in range(binSplit+1,25):
            #run the splitter
            os.system("python3 SliceHTvsFitMass.py "+year+" "+binNr+" "+str(binSplit)+" "+str(binEnd))

            #run the limit on HT
            #os.system("combine -M AsymptoticLimits -m "+massString+" HTslice_Wprime"+binNr+"_"+year+".txt")

            #extract the information on HT
            #infile = TFile("higgsCombineTest.AsymptoticLimits.mH"+massString+".root","READ")
            #counter = 0
            #for event in infile.limit:
            #    HThist.SetBinContent(binSplit-4, binEnd-5, counter+1, event.limit)
            #    counter+=1
            
            #clean up
            #infile.Close()
            #os.system("rm higgsCombineTest.AsymptoticLimits.mH"+massString+".root")

            #run the limit on the fit
            #os.system("combine -M AsymptoticLimits -m "+massString+" FitSlice_Wprime"+binNr+"_"+year+".txt")

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
            os.system("combine -M AsymptoticLimits -m "+massString+" CombinationSlices_Wprime"+binNr+"_"+year+".txt")

            #extract the information on the fit
            infile = TFile("higgsCombineTest.AsymptoticLimits.mH"+massString+".root","READ")
            counter = 0
            for event in infile.limit:
                CombHist.SetBinContent(binSplit-4, binEnd-5, counter+1, event.limit)
                counter+=1

            #clean up
            infile.Close()
            os.system("rm higgsCombineTest.AsymptoticLimits.mH"+massString+".root")

    savefile.cd()
    #HThist.Write()
    #FitHist.Write()
    CombHist.Write()
    savefile.Close()
