import os,sys
from ROOT import TH1F, TChain, TMath
import numpy as np
import math

#function for determining the binning from desired minimum unweighted stats and input histogram in fine binning, as well as desired precision
def determineQuantiles(hist, nStatMin, precision):
    #determine desired number of quantiles, initiate empty quantiles lists
    nq = math.floor(hist.Integral()/nStatMin)
    x = []
    y = []

    for i in range(0, nq+1):
        x.append(i*1./nq) #desired quantile
        y.append(0.) #empty actual bin edge

    #ROOT-friendly format conversion
    Ax = np.array(x) 
    Ay = np.array(y)

    #quantile function from ROOT
    hist.GetQuantiles(nq+1, Ay, Ax)

    #print("start determineQuantiles",nStatMin,precision)
    #print("target",nq,"bins")

    #round all elements in list to desired precision
    for i in range(0, len(Ay)):
        Ay[i] = round(Ay[i], precision)

    #print(Ay)

    #find smallest bin size
    minimumIndex = -1
    minimum = 999.
    for i in range(0, len(Ay)-1):
        size = Ay[i+1] - Ay[i]
        if size == 0.:
            size = math.pow(10., -precision)
        if size < minimum:
            minimum = size
            minimumIndex = i

    #print(minimumIndex,minimum)

    #then determine from that minimum point to the left-side bin edges
    newBinEdgeList = [0] * len(Ay)
    newBinEdgeList[minimumIndex] = Ay[minimumIndex]
    currentSize = minimum
    for i in range(minimumIndex-1, 0, -1):
        size = newBinEdgeList[i+1] - Ay[i]
        if size < currentSize:
            size = currentSize
        elif size > currentSize:
            currentSize = size

        newBinEdgeList[i] = round(newBinEdgeList[i+1] - size, precision)

    newBinEdgeList[0] = Ay[0]

    #print(newBinEdgeList)

    #determine the right-side bin edges
    currentSize = minimum
    for i in range(minimumIndex+1, len(Ay)-1):
        size = Ay[i] - newBinEdgeList[i-1]
        if size < currentSize:
            size = currentSize
        elif size > currentSize:
            currentSize = size

        newBinEdgeList[i] = round(newBinEdgeList[i-1] + size, precision)

    newBinEdgeList[len(Ay)-1] = Ay[len(Ay)-1]

   # print(newBinEdgeList)

    return newBinEdgeList


#This is a small macro to determine a binning with the following requirements:
nStatMinMC5  = 200.  #minimum number of unweighted events in 5 jet regions
nStatMinMC63 = 100.  #minimum number of unweighted events in 6 jet regions with 3 tags
nStatMinMC64 = 25.   #minimum number of unweighted events in 6 jet regions with 4 tags
nStatMinST5  = 500. #minimum number of data events in a given bin for ST in a 2 tag region for 5 jets
nStatMinST6  = 250.  #minimum number of data events in a given bin for ST in a 2 tag region for 6 jets

print("Starting bin derivation with settings nStatMinMC5 =",nStatMinMC5,", nStatMinMC63 =",nStatMinMC63,", nStatMinMC64 =",nStatMinMC64,", nStatMinST5 =",nStatMinST5,"and nStatMinST6 =",nStatMinST6)

#LaTeX tables and bin edge number and configuration files will be dumped into BinTables.tex and BinTables.C files
fLatex = open("BinTables.tex", "w")

fBins  = open("BinTables.C"  , "w")
fBins.write("#include <TROOT.h>\n")

#Macro loads all ttbar events in desired region (electron variant, lower stats)
chainTtbar = TChain("t")
chainTtbar.Add("/eos/cms/store/group/phys_b2g/wprime/analyzednewskims/2016_ttbar_semileptonic/*.root")
chainTtbar.Add("/eos/cms/store/group/phys_b2g/wprime/analyzednewskims/2016_ttbar_leptonic/*.root")
chainTtbar.Add("/eos/cms/store/group/phys_b2g/wprime/analyzednewskims/2016_ttbar_hadronic/*.root")

#activate necessary branches only, for speed
chainTtbar.SetBranchStatus("*",0)
chainTtbar.SetBranchStatus("EventWeight",1)
chainTtbar.SetBranchStatus("RegionIdentifier",1)
chainTtbar.SetBranchStatus("JetPt",1)
chainTtbar.SetBranchStatus("Best_WPrimeMass_*",1)
chainTtbar.SetBranchStatus("Best_Likelihood_*",1)

#declare MC histograms in very fine binning for binning determination
HThist53  = TH1F("HThist53" ,"", 2000, 0., 2000.)
HThist63  = TH1F("HThist63" ,"", 2000, 0., 2000.)
HThist64  = TH1F("HThist64" ,"", 2000, 0., 2000.)

NLLhists53 = []
FitHists53 = []
NLLhists63 = []
FitHists63 = []
NLLhists64 = []
FitHists64 = []

for i in range(3,12):
    NLLhists53.append(TH1F("NLLhist53_"+str(i),"", 300,   0., 30.))
    FitHists53.append(TH1F("FitHist53_"+str(i),"", 2000,  0., 2000.))
    NLLhists63.append(TH1F("NLLhist63_"+str(i),"", 300,   0., 30.))
    FitHists63.append(TH1F("FitHist63_"+str(i),"", 2000,  0., 2000.))
    NLLhists64.append(TH1F("NLLhist64_"+str(i),"", 300,   0., 30.))
    FitHists64.append(TH1F("FitHist64_"+str(i),"", 2000,  0., 2000.))

nentriesMC = chainTtbar.GetEntries()

print("Starting loop over",nentriesMC,"events in MC files")

#loop over MC events in ttbar samples
for entry in range(0, nentriesMC):
    chainTtbar.GetEntry(entry)
    evWeight = getattr(chainTtbar,"EventWeight")
    regId    = getattr(chainTtbar,"RegionIdentifier")
    #check for a valid event
    if evWeight[0] > 0. :
        #check whether the event is in any region of interest
        if regId[0] == 2153 or regId[0] == 2164 or regId[0] == 2163:
            isValidInAllFits = True
            #select events which are valid for all mass variants, only (acceptance loss is percent level individually, so this is worst case)
            for i in range(3,12):
                LL   = getattr(chainTtbar,"Best_Likelihood_"+str(i*100))
                try:
                    if -math.log(LL[0]) < 0:
                        isValidInAllFits = False
                        break
                except:
                    isValidInAllFits = False
                    break

            #fill MC fine-binned histograms for the correct regions
            if isValidInAllFits:
                JetPt = getattr(chainTtbar,"JetPt")
                if   regId[0] == 2153:
                    HThist53.Fill(JetPt[0]+JetPt[1]+JetPt[2]+JetPt[3]+JetPt[4])
                elif regId[0] == 2163:
                    HThist63.Fill(JetPt[0]+JetPt[1]+JetPt[2]+JetPt[3]+JetPt[4]+JetPt[5])
                elif regId[0] == 2164:
                    HThist64.Fill(JetPt[0]+JetPt[1]+JetPt[2]+JetPt[3]+JetPt[4]+JetPt[5])

                for i in range(3,12):
                    Mfit = getattr(chainTtbar,"Best_WPrimeMass_"+str(i*100))
                    LL   = getattr(chainTtbar,"Best_Likelihood_"+str(i*100))
                    if   regId[0] == 2153:
                        FitHists53[i-3].Fill(Mfit[0])
                        NLLhists53[i-3].Fill(-math.log(LL[0]))
                    elif regId[0] == 2163:
                        FitHists63[i-3].Fill(Mfit[0])
                        NLLhists63[i-3].Fill(-math.log(LL[0]))
                    elif regId[0] == 2164:
                        FitHists64[i-3].Fill(Mfit[0])
                        NLLhists64[i-3].Fill(-math.log(LL[0]))

#Get the quantiles and print
print("Reached quantile determination stage for MC histograms")

HTbins53 = determineQuantiles(HThist53, nStatMinMC5, 0)
HTbins63 = determineQuantiles(HThist63, nStatMinMC63, 0)
HTbins64 = determineQuantiles(HThist64, nStatMinMC64, 0)

FitBins53 = []
FitBins63 = []
FitBins64 = []

NLLbins53 = []
NLLbins63 = []
NLLbins64 = []

for i in range(3,12):
    FitBins53.append(determineQuantiles(FitHists53[i-3], nStatMinMC5, 0))
    FitBins63.append(determineQuantiles(FitHists63[i-3], nStatMinMC63, 0))
    FitBins64.append(determineQuantiles(FitHists64[i-3], nStatMinMC64, 0))

    NLLbins53.append(determineQuantiles(NLLhists53[i-3], nStatMinMC5, 1))
    NLLbins63.append(determineQuantiles(NLLhists63[i-3], nStatMinMC63, 1))
    NLLbins64.append(determineQuantiles(NLLhists64[i-3], nStatMinMC64, 1))

#write the tables
fLatex.write("5 jets 3 \PQb tags \HT bin edges:\n")
fBins.write("int nHTlimits53 = "+str(len(HTbins53)-1)+";\n")
fBins.write("double HTlimits53["+str(len(HTbins53))+"] = {")
for i in range(0, len(HTbins53)):
    b = HTbins53[i]
    fLatex.write(str(b))
    fBins.write(str(b))
    if i != len(HTbins53) -1:
        fLatex.write(", ")
        fBins.write(", ")
    else:
        fLatex.write(" .")
fLatex.write("\n")
fBins.write("};\n")

fLatex.write("6 jets 3 \PQb tags \HT bin edges:\n")
fBins.write("int nHTlimits63 = "+str(len(HTbins63)-1)+";\n")
fBins.write("double HTlimits63["+str(len(HTbins63))+"] = {")
for i in range(0, len(HTbins63)):
    b = HTbins63[i]
    fLatex.write(str(b))
    fBins.write(str(b))
    if i != len(HTbins63) -1:
        fLatex.write(", ")
        fBins.write(", ")
    else:
        fLatex.write(" .")
fLatex.write("\n")
fBins.write("};\n")

fLatex.write("6 jets 4 \PQb tags \HT bin edges:\n")
fBins.write("int nHTlimits64 = "+str(len(HTbins64)-1)+";\n")
fBins.write("double HTlimits64["+str(len(HTbins64))+"] = {")
for i in range(0, len(HTbins64)):
    b = HTbins64[i]
    fLatex.write(str(b))
    fBins.write(str(b))
    if i != len(HTbins64) -1:
        fLatex.write(", ")
        fBins.write(", ")
    else:
        fLatex.write(" .")
fLatex.write("\n")
fBins.write("};\n")

for i in range(3,12):
    fLatex.write("5 jets 3 \Pqb tags $m_{\PWpr}$ bin edges at $m_{\PWpr}="+str(i*100)+"$ \GeV:\n")
    fBins.write("int nFitLimits53_"+str(i*100)+" = "+str(len(FitBins53[i-3])-1)+";\n")
    fBins.write("double FitLimits53_"+str(i*100)+"["+str(len(FitBins53[i-3]))+"] = {")
    for j in range(0, len(FitBins53[i-3])):
        b = FitBins53[i-3][j]
        fLatex.write(str(b))
        fBins.write(str(b))
        if j != len(FitBins53[i-3]) -1:
            fLatex.write(", ")
            fBins.write(", ")
        else:
            fLatex.write(" .")
    fLatex.write("\n")
    fBins.write("};\n")

    fLatex.write("6 jets 3 \Pqb tags $m_{\PWpr}$ bin edges at $m_{\PWpr}="+str(i*100)+"$ \GeV:\n")
    fBins.write("int nFitLimits63_"+str(i*100)+" = "+str(len(FitBins63[i-3])-1)+";\n")
    fBins.write("double FitLimits63_"+str(i*100)+"["+str(len(FitBins63[i-3]))+"] = {")
    for j in range(0, len(FitBins63[i-3])):
        b = FitBins63[i-3][j]
        fLatex.write(str(b))
        fBins.write(str(b))
        if j != len(FitBins63[i-3]) -1:
            fLatex.write(", ")
            fBins.write(", ")
        else:
            fLatex.write(" .")
    fLatex.write("\n")
    fBins.write("};\n")

    fLatex.write("6 jets 4 \Pqb tags $m_{\PWpr}$ bin edges at $m_{\PWpr}="+str(i*100)+"$ \GeV:\n")
    fBins.write("int nFitLimits64_"+str(i*100)+" = "+str(len(FitBins64[i-3])-1)+";\n")
    fBins.write("double FitLimits64_"+str(i*100)+"["+str(len(FitBins64[i-3]))+"] = {")
    for j in range(0, len(FitBins64[i-3])):
        b = FitBins64[i-3][j]
        fLatex.write(str(b))
        fBins.write(str(b))
        if j != len(FitBins64[i-3]) -1:
            fLatex.write(", ")
            fBins.write(", ")
        else:
            fLatex.write(" .")
    fLatex.write("\n")
    fBins.write("};\n")

    fLatex.write("5 jets 3 \Pqb tags -log(L) bin edges at $m_{\PWpr}="+str(i*100)+"$ \GeV:\n")
    fBins.write("int nNLLlimits53_"+str(i*100)+" = "+str(len(NLLbins53[i-3])-1)+";\n")
    fBins.write("double NLLlimits53_"+str(i*100)+"["+str(len(NLLbins53[i-3]))+"] = {")
    for j in range(0, len(NLLbins53[i-3])):
        b = NLLbins53[i-3][j]
        fLatex.write(str(b))
        fBins.write(str(b))
        if j != len(NLLbins53[i-3]) -1:
            fLatex.write(", ")
            fBins.write(", ")
        else:
            fLatex.write(" .")
    fLatex.write("\n")
    fBins.write("};\n")

    fLatex.write("6 jets 3 \Pqb tags -log(L) bin edges at $m_{\PWpr}="+str(i*100)+"$ \GeV:\n")
    fBins.write("int nNLLlimits63_"+str(i*100)+" = "+str(len(NLLbins63[i-3])-1)+";\n")
    fBins.write("double NLLlimits63_"+str(i*100)+"["+str(len(NLLbins63[i-3]))+"] = {")
    for j in range(0, len(NLLbins63[i-3])):
        b = NLLbins63[i-3][j]
        fLatex.write(str(b))
        fBins.write(str(b))
        if j != len(NLLbins63[i-3]) -1:
            fLatex.write(", ")
            fBins.write(", ")
        else:
            fLatex.write(" .")
    fLatex.write("\n")
    fBins.write("};\n")

    fLatex.write("6 jets 4 \Pqb tags -log(L) bin edges at $m_{\PWpr}="+str(i*100)+"$ \GeV:\n")
    fBins.write("int nNLLlimits64_"+str(i*100)+" = "+str(len(NLLbins64[i-3])-1)+";\n")
    fBins.write("double NLLlimits64_"+str(i*100)+"["+str(len(NLLbins64[i-3]))+"] = {")
    for j in range(0, len(NLLbins64[i-3])):
        b = NLLbins64[i-3][j]
        fLatex.write(str(b))
        fBins.write(str(b))
        if j != len(NLLbins64[i-3]) -1:
            fLatex.write(", ")
            fBins.write(", ")
        else:
            fLatex.write(" .")
    fLatex.write("\n")
    fBins.write("};\n")


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

SThist5 = TH1F("SThist5","", 1820, 180., 2000.)
SThist6 = TH1F("SThist6","", 1790, 210., 2000.)

nentriesData = chainData.GetEntries()

print("Starting loop over",nentriesData,"events in data files")

for entry in range(0, nentriesData):
    chainData.GetEntry(entry)

    regId    = getattr(chainData,"RegionIdentifier")
    if regId[0] == 2152 or regId[0] == 2162: #3 or 4 b tag regions are blinded, take data from least populated ST-relevant region
        isValidInAllFits = True
        for i in range(3,12):
            LL   = getattr(chainData,"Best_Likelihood_"+str(i*100))
            try:
                if -math.log(LL[0]) < 0:
                    isValidInAllFits = False
                    break
            except:
                isValidInAllFits = False
                break
        if isValidInAllFits:
            JetPt = getattr(chainData,"JetPt")
            if regId[0] == 2152:
                SThist5.Fill(JetPt[0]+JetPt[1]+JetPt[2]+JetPt[3]+JetPt[4]+getattr(chainData,"LeptonPt")+getattr(chainData,"METPt"))
            elif regId[0] == 2162:
                SThist6.Fill(JetPt[0]+JetPt[1]+JetPt[2]+JetPt[3]+JetPt[4]+JetPt[5]+getattr(chainData,"LeptonPt")+getattr(chainData,"METPt"))

#Get the quantiles
print("Reached quantile determination stage for ST histograms")

STbins5 = determineQuantiles(SThist5, nStatMinST5, 0)
STbins6 = determineQuantiles(SThist6, nStatMinST6, 0)

#write the tables
fLatex.write("5 jets 2 \PQb tags $S_{\textrm{T}}$ bin edges:\n")
fBins.write("int nSTlimits5 = "+str(len(STbins5)-1)+";\n")
fBins.write("double STlimits5["+str(len(STbins5))+"] = {")
for i in range(0, len(STbins5)):
    b = STbins5[i]
    fLatex.write(str(b))
    fBins.write(str(b))
    if i != len(STbins5) -1:
        fLatex.write(", ")
        fBins.write(", ")
    else:
        fLatex.write(" .")
fLatex.write("\n")
fBins.write("};\n")

fLatex.write("6 jets 2 \PQb tags $S_{\textrm{T}}$ bin edges:\n")
fBins.write("int nSTlimits6 = "+str(len(STbins6)-1)+";\n")
fBins.write("double STlimits6["+str(len(STbins6))+"] = {")
for i in range(0, len(STbins6)):
    b = STbins6[i]
    fLatex.write(str(b))
    fBins.write(str(b))
    if i != len(STbins6) -1:
        fLatex.write(", ")
        fBins.write(", ")
    else:
        fLatex.write(" .")
fLatex.write("\n")
fBins.write("};\n")

#write the files, this in python NEEDS closing the files
fLatex.close()
fBins.close()

