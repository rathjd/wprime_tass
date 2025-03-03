import os,sys
from ROOT import TH1F, TChain, TMath
import numpy as np
import math

#This is a small macro to determine a binning with the following requirements:
nStatMinMC = 100.  #minimum number of events in a given bin
nStatMinST = 1000. #minimum number of events in a given bin for ST
JetMult    = 5     #jet multiplicity required
bTagMult   = 3     #b tag multiplicity required

#Macro loads all ttbar events in desired region (electron variant, lower stats)
chainTtbar = TChain("t")
chainTtbar.Add("/eos/cms/store/group/phys_b2g/wprime/analyzednewskims/2016_ttbar_semileptonic/*.root")
chainTtbar.Add("/eos/cms/store/group/phys_b2g/wprime/analyzednewskims/2016_ttbar_leptonic/*.root")
chainTtbar.Add("/eos/cms/store/group/phys_b2g/wprime/analyzednewskims/2016_ttbar_hadronic/*.root")
chainTtbar.SetBranchStatus("*",0)
chainTtbar.SetBranchStatus("EventWeight",1)
chainTtbar.SetBranchStatus("RegionIdentifier",1)
chainTtbar.SetBranchStatus("JetPt",1)
chainTtbar.SetBranchStatus("Best_WPrimeMass_*",1)
chainTtbar.SetBranchStatus("Best_Likelihood_*",1)

HThist  = TH1F("HThist" ,"", 2000, 0., 2000.)

NLLhists = []
FitHists = []
for i in range(3,12):
    NLLhists.append(TH1F("NLLhist"+str(i),"", 300,   0., 30.))
    FitHists.append(TH1F("FitHist"+str(i),"", 2000,  0., 2000.))

for entry in range(0, chainTtbar.GetEntries()):
    chainTtbar.GetEntry(entry)
    evWeight = getattr(chainTtbar,"EventWeight")
    regId    = getattr(chainTtbar,"RegionIdentifier")
    if evWeight[0] > 0. and regId[0] == (2100+JetMult*10+bTagMult):
        isValidInAllFits = True
        for i in range(3,12):
            LL   = getattr(chainTtbar,"Best_Likelihood_"+str(i*100))
            try:
                if -math.log(LL[0]) < 0:
                    isValidInAllFits = False
                    break
            except:
                isValidInAllFits = False
                break
        if isValidInAllFits:
            JetPt = getattr(chainTtbar,"JetPt")
            if JetMult == 5:
                HThist.Fill(JetPt[0]+JetPt[1]+JetPt[2]+JetPt[3]+JetPt[4])
            elif JetMult == 6:
                HThist.Fill(JetPt[0]+JetPt[1]+JetPt[2]+JetPt[3]+JetPt[4]+JetPt[5])

            for i in range(3,12):
                Mfit = getattr(chainTtbar,"Best_WPrimeMass_"+str(i*100))
                LL   = getattr(chainTtbar,"Best_Likelihood_"+str(i*100))
                FitHists[i-3].Fill(Mfit[0])
                NLLhists[i-3].Fill(LL[0])

#Get the quantiles and print
print("Monte Carlo ttbar electron region with jet multiplicity",JetMult,"and b tag multiplicity",bTagMult,"considering minimum statistics per bin",nStatMinMC)
nqHT = math.floor(HThist.Integral()/nStatMinMC)
xHT = []
yHT = []
for i in range(0, nqHT+1):
    xHT.append(i*1./nqHT)
    yHT.append(0.)

AxHT = np.array(xHT)
AyHT = np.array(yHT)

print("HT",xHT,yHT)
print(HThist.GetQuantiles(nqHT+1, AyHT, AxHT))
print(AyHT)

for i in range(3,12):
    nqFit = math.floor(FitHists[i-3].Integral()/nStatMinMC)
    xFit  = []
    yFit  = []
    for j in range(0, nqFit+1):
        xFit.append(j*1./nqFit)
        yFit.append(0.)

    AxFit = np.array(xFit)
    AyFit = np.array(yFit)

    print("Fit for",i*100,xFit,yFit)
    print(FitHists[i-3].GetQuantiles(nqFit+1, AyFit, AxFit))
    print(AyFit)

    nqNLL = math.floor(NLLhists[i-3].Integral()/nStatMinMC)
    xNLL = []
    yNLL = []
    for j in range(0, nqNLL+1):
        xNLL.append(j*1/nqNLL)
        yNLL.append(0.)

    AxNLL = np.array(xNLL)
    AyNLL = np.array(yNLL)

    print("NLL for",i*100,xNLL,yNLL)
    print(NLLhists[i-3].GetQuantiles(nqNLL+1, AyNLL, AxNLL))
    print(AyNLL)


#Macro loads all data events in desired control region for ST binning (electron variant, lower stats)
chainData = TChain("t")
chainData.Add("/eos/cms/store/group/phys_b2g/wprime/analyzednewskims/2016_SingleElectron/*.root")
chainData.SetBranchStatus("*",0)
chainData.SetBranchStatus("EventWeight",1)
chainData.SetBranchStatus("RegionIdentifier",1)
chainData.SetBranchStatus("JetPt",1)
chainData.SetBranchStatus("Best_Likelihood_*",1)
chainData.SetBranchStatus("LeptonPt",1)
chainData.SetBranchStatus("METPt",1)

STmin = 180.
STbins = 1820
if JetMult == 6:
    STmin = 210.
    STbins = 1790

SThist = TH1F("SThist","", STbins, STmin, 2000.)

for entry in range(0, chainData.GetEntries()):
    chainData.GetEntry(entry)

    evWeight = getattr(chainData,"EventWeight")
    regId    = getattr(chainData,"RegionIdentifier")
    if evWeight[0] > 0. and regId[0] == (2100+JetMult*10+bTagMult):
        isValidInAllFits = True
        for i in range(3,12):
            LL   = getattr(chainData,"Best_Likelihood_"+str(i*100))
            try:
                if -math.log(LL[0]) < 0:
                    isValidInAllFits = False
                    break
            except:
                print(LL[0])
                isValidInAllFits = False
                break
        if isValidInAllFits:
            JetPt = getattr(chainData,"JetPt")
            if JetMult == 5:
                SThist.Fill(JetPt[0]+JetPt[1]+JetPt[2]+JetPt[3]+JetPt[4]+getattr(chainData,"LeptonPt")+getattr(chainData,"METPt"))
            elif JetMult == 6:
                SThist.Fill(JetPt[0]+JetPt[1]+JetPt[2]+JetPt[3]+JetPt[4]+JetPt[5]+getattr(chainData,"LeptonPt")+getattr(chainData,"METPt"))

#Get the quantiles and print
print("Data electron region for ST histograms with jet multiplicity",JetMult,"and b tag multiplicity",bTagMult,"considering minimum statistics per bin",nStatMinMC)
nqST = math.floor(SThist.Integral()/nStatMinST)
xST = []
yST = []
for i in range(0, nqST+1):
    xST.append(i*1./nqST)
    yST.append(0.)

AxHT = np.array(xST)
AyHT = np.array(yST)

print("ST",xST,yST)
print(HThist.GetQuantiles(nqST+1, AyST, AxST))
print(AyST)
