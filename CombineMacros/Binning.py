import os,sys
from ROOT import TH1F, TChain, TMath
import numpy as np
import math

#Optimization of NLL cuts October 2025 version pre-ST cut after b tag weight fix
Optimization =       [[[13, 61], [11, 24]],
                      [[ 7, 59], [ 9, 22]],
                      [[ 8, 65], [ 7, 24]],
                      [[10, 76], [ 7, 24]],
                      [[11, 75], [ 7, 24]],
                      [[12, 67], [ 7, 24]],
                      [[12, 67], [ 7, 24]],
                      [[12, 67], [ 7, 24]],
                      [[12, 74], [ 7, 24]]]

NLLlimits53_500 = [0.0, 1.3, 1.7, 2.0, 2.3, 2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0, 4.2, 4.4, 4.5, 4.6, 4.8, 5.0, 5.2, 5.4, 5.6, 5.8, 6.0, 6.2, 6.4, 6.6, 6.8, 7.0, 7.2, 7.4, 7.6, 7.8, 8.0, 8.2, 8.4, 8.6, 8.8, 9.0, 9.2, 9.4, 9.6, 9.8, 10.0, 10.2, 10.4, 10.6, 10.8, 11.0, 11.2, 11.4, 11.6, 11.8, 12.0, 12.2, 12.4, 12.6, 12.8, 13.0, 13.2, 13.4, 13.6, 13.8, 14.0, 14.2, 14.4, 14.6, 14.8, 15.0, 15.2, 15.4, 15.6, 15.8, 16.0, 16.2, 16.4, 16.6, 16.8, 29.9]
NLLlimits63_500 = [0.0, 0.8, 1.0, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9, 4.0, 4.1, 4.2, 4.3, 4.4, 4.5, 4.6, 4.7, 4.8, 4.9, 5.0, 5.1, 5.2, 5.3, 5.4, 5.5, 5.6, 5.7, 5.8, 5.9, 6.0, 6.1, 6.2, 6.3, 6.4, 6.5, 6.6, 6.7, 6.8, 6.9, 7.0, 7.1, 7.2, 7.3, 7.4, 7.5, 7.6, 7.7, 7.8, 7.9, 8.0, 8.1, 8.2, 8.3, 8.4, 8.5, 8.6, 8.7, 8.8, 8.9, 9.0, 9.1, 9.2, 9.3, 9.4, 9.5, 9.6, 9.7, 9.8, 9.9, 10.0, 10.1, 10.2, 10.3, 10.4, 10.5, 10.8, 11.1, 11.6, 12.1, 12.7, 13.6, 15.0, 29.9]
NLLlimits64_500 = [0.2, 1.0, 1.6, 2.2, 2.8, 3.4, 4.0, 4.5, 4.8, 5.0, 5.3, 5.6, 5.9, 6.2, 6.5, 6.8, 7.2, 7.8, 8.4, 9.0, 9.6, 10.2, 10.8, 11.5, 13.0, 29.9]

#function for determining the binning from desired minimum unweighted stats and input histogram in fine binning, as well as desired precision
def determineQuantiles(hist, nStatMin, precision, wHist):
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
    Ay_clean = []
    for i in range(0, len(Ay)):
        member = round(Ay[i], precision)
        if member not in Ay_clean:
            Ay_clean.append(member)

    print(Ay_clean,"bare cleaned quantiles")

    #find smallest bin size
    minimumIndex = -1
    minimum = 999.
    for i in range(0, len(Ay_clean)-1):
        size = Ay_clean[i+1] - Ay_clean[i]
        if size == 0.:
            size = math.pow(10., -precision)
        if size < minimum:
            minimum = size
            minimumIndex = i

    print(minimumIndex,minimum, "minimum")

    #then determine from that minimum point to the left-side bin edges
    newBinEdgeList = [0] * len(Ay_clean)
    newBinEdgeList[minimumIndex] = Ay_clean[minimumIndex]
    currentSize = minimum
    for i in range(minimumIndex-1, 0, -1):
        size = newBinEdgeList[i+1] - Ay_clean[i]
        if size < currentSize:
            size = currentSize
        elif size > currentSize:
            currentSize = size

        newBinEdgeList[i] = round(newBinEdgeList[i+1] - size, precision)

    newBinEdgeList[0] = Ay_clean[0]

    print(newBinEdgeList, "after left side operation")

    #determine the right-side bin edges
    currentSize = minimum
    for i in range(minimumIndex+1, len(Ay_clean)-1):
        size = Ay_clean[i] - newBinEdgeList[i-1]
        if size < currentSize:
            size = currentSize
        elif size > currentSize:
            currentSize = size

        newBinEdgeList[i] = round(newBinEdgeList[i-1] + size, precision)

    newBinEdgeList[len(Ay_clean)-1] = Ay_clean[len(Ay_clean)-1]

    print("newBinEdgeList",newBinEdgeList)

    #do another run over the weighted histograms to find the equivalent weighted bin contents and ensure at least 10 predicted weighted events
    SecondNewBinEdgeList = [newBinEdgeList[0]]
    FoundSolutionList = []
    step = -1
    for i in range(0, len(newBinEdgeList)-1):
        if i < step:
            continue
        for j in range(i+1, len(newBinEdgeList)):
            if wHist.Integral(wHist.FindBin(newBinEdgeList[i]), wHist.FindBin(newBinEdgeList[j])-1) > 9.: #3 sigma stat limit with respect to 0
                SecondNewBinEdgeList.append(newBinEdgeList[j])
                FoundSolutionList.append(True)
                step = j
                break

    if len(SecondNewBinEdgeList)-1 != len(FoundSolutionList): #case that the last bin on the right edge is too small
        SecondNewBinEdgeList[len(SecondNewBinEdgeList-1)] = newBinEdgeList[len(newBinEdgeList)-1]

    print("SecondNewBinEdgeList",SecondNewBinEdgeList)

    return SecondNewBinEdgeList


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

#definition of 2016 ttbar samples
#ttbarSamples = [["ttbar_semileptonic"      , 366300., 144722000.],
#                ["ttbar_leptonic"          ,  88500.,  43546000.],
#                ["ttbar_hadronic"          , 378900., 107067000.],
#                ["ttbar_bbbar_semileptonic",  19200.,   3223118.],
#                ["ttbar_bbbar_leptonic"    ,   4600.,   1571799.],
#                ["ttbar_bbbar_hadronic"    ,  19900.,   2597868.]]
#2017 ttbar samples
ttbarSamples = [["ttbar_semileptonic"      , 366300., 346052000.],
                ["ttbar_leptonic"          ,  88500., 106724000.],
                ["ttbar_hadronic"          , 378900., 232999999.],
                ["ttbar_bbbar_semileptonic",  19200.,   7318891.],
                ["ttbar_bbbar_leptonic"    ,   4600.,   3472503.],
                ["ttbar_bbbar_hadronic"    ,  19900.,   5694656.]]
#lumi = 16.81 #2016
lumi = 41.48 #2017

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

#declare weighted MC histograms in very fine binning for binning determination
wHThist53  = TH1F("wHThist53" ,"", 2000, 0., 2000.)
wHThist63  = TH1F("wHThist63" ,"", 2000, 0., 2000.)
wHThist64  = TH1F("wHThist64" ,"", 2000, 0., 2000.)

wNLLhists53 = []
wFitHists53 = []
wNLLhists63 = []
wFitHists63 = []
wNLLhists64 = []
wFitHists64 = []

for i in range(3,12):
    wNLLhists53.append(TH1F("wNLLhist53_"+str(i),"", 300,   0., 30.))
    wFitHists53.append(TH1F("wFitHist53_"+str(i),"", 2000,  0., 2000.))
    wNLLhists63.append(TH1F("wNLLhist63_"+str(i),"", 300,   0., 30.))
    wFitHists63.append(TH1F("wFitHist63_"+str(i),"", 2000,  0., 2000.))
    wNLLhists64.append(TH1F("wNLLhist64_"+str(i),"", 300,   0., 30.))
    wFitHists64.append(TH1F("wFitHist64_"+str(i),"", 2000,  0., 2000.))

for sample in ttbarSamples:
    #Macro loads all ttbar events in desired region (electron variant, lower stats)
    chainTtbar = TChain("t")
    chainTtbar.Add("/eos/cms/store/group/phys_b2g/wprime/analyzeroutputjune2025/2017_"+sample[0]+"/*.root")

    #activate necessary branches only, for speed
    chainTtbar.SetBranchStatus("*",0)
    chainTtbar.SetBranchStatus("EventWeight",1)
    chainTtbar.SetBranchStatus("RegionIdentifier",1)
    chainTtbar.SetBranchStatus("JetPt",1)
    chainTtbar.SetBranchStatus("Best_WPrimeMass_*",1)
    chainTtbar.SetBranchStatus("Best_Likelihood_*",1)

    nentriesMC = chainTtbar.GetEntries()

    print("Starting loop over",nentriesMC,"events in MC file",sample[0])

    MCweight = sample[1]/sample[2]*lumi

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
                        if LL[0] < 0. or LL[0] >= 1.:
                            isValidInAllFits = False
                            break
                    except:
                        isValidInAllFits = False
                        break

                #fill MC fine-binned histograms for the correct regions
                if isValidInAllFits:
                    JetPt = getattr(chainTtbar,"JetPt")
                    LL500 = getattr(chainTtbar,"Best_Likelihood_"+str(500))
                    if   regId[0] == 2153:
                        if -math.log(LL500[0]) > NLLlimits53_500[Optimization[2][0][0]+1] and -math.log(LL500[0]) < NLLlimits53_500[Optimization[2][0][1]+1]:
                            HThist53.Fill(JetPt[0]+JetPt[1]+JetPt[2]+JetPt[3]+JetPt[4])
                            wHThist53.Fill(JetPt[0]+JetPt[1]+JetPt[2]+JetPt[3]+JetPt[4], evWeight[0]*MCweight)
                    elif regId[0] == 2163:
                        if -math.log(LL500[0]) > NLLlimits63_500[Optimization[2][1][0]+1] and -math.log(LL500[0]) < NLLlimits63_500[Optimization[2][1][1]+1]:
                            HThist63.Fill(JetPt[0]+JetPt[1]+JetPt[2]+JetPt[3]+JetPt[4]+JetPt[5])
                            wHThist63.Fill(JetPt[0]+JetPt[1]+JetPt[2]+JetPt[3]+JetPt[4]+JetPt[5], evWeight[0]*MCweight)
                    elif regId[0] == 2164:
                        if -math.log(LL500[0]) > NLLlimits64_500[Optimization[2][1][0]+1] and -math.log(LL500[0]) < NLLlimits64_500[Optimization[2][1][1]+1]:
                            HThist64.Fill(JetPt[0]+JetPt[1]+JetPt[2]+JetPt[3]+JetPt[4]+JetPt[5])
                            wHThist64.Fill(JetPt[0]+JetPt[1]+JetPt[2]+JetPt[3]+JetPt[4]+JetPt[5], evWeight[0]*MCweight)

                    for i in range(3,12):
                        Mfit = getattr(chainTtbar,"Best_WPrimeMass_"+str(i*100))
                        LL   = getattr(chainTtbar,"Best_Likelihood_"+str(i*100))
                        if -math.log(LL[0]) < 0.:
                            print(LL[0], "less than zero NLL")
                            continue
                        if   regId[0] == 2153:
                            if -math.log(LL500[0]) < NLLlimits53_500[Optimization[2][0][0]+1]:
                                FitHists53[i-3].Fill(Mfit[0])
                                wFitHists53[i-3].Fill(Mfit[0], evWeight[0]*MCweight)
                            NLLhists53[i-3].Fill(-math.log(LL[0]))
                            wNLLhists53[i-3].Fill(-math.log(LL[0]), evWeight[0]*MCweight)
                        elif regId[0] == 2163:
                            if -math.log(LL500[0]) < NLLlimits63_500[Optimization[2][1][0]+1]:
                                FitHists63[i-3].Fill(Mfit[0])
                                wFitHists63[i-3].Fill(Mfit[0], evWeight[0]*MCweight)
                            NLLhists63[i-3].Fill(-math.log(LL[0]))
                            wNLLhists63[i-3].Fill(-math.log(LL[0]), evWeight[0]*MCweight)
                        elif regId[0] == 2164:
                            if -math.log(LL500[0]) < NLLlimits64_500[Optimization[2][1][0]+1]:
                                FitHists64[i-3].Fill(Mfit[0])
                                wFitHists64[i-3].Fill(Mfit[0], evWeight[0]*MCweight)
                            NLLhists64[i-3].Fill(-math.log(LL[0]))
                            wNLLhists64[i-3].Fill(-math.log(LL[0]), evWeight[0]*MCweight)

#Get the quantiles and print
print("Reached quantile determination stage for MC histograms")

HTbins53 = determineQuantiles(HThist53, nStatMinMC5,  0, wHThist53)
HTbins63 = determineQuantiles(HThist63, nStatMinMC63, 0, wHThist63)
HTbins64 = determineQuantiles(HThist64, nStatMinMC64, 0, wHThist64)

FitBins53 = []
FitBins63 = []
FitBins64 = []

NLLbins53 = []
NLLbins63 = []
NLLbins64 = []

for i in range(3,12):
    FitBins53.append(determineQuantiles(FitHists53[i-3], nStatMinMC5,  0, wFitHists53[i-3]))
    FitBins63.append(determineQuantiles(FitHists63[i-3], nStatMinMC63, 0, wFitHists63[i-3]))
    FitBins64.append(determineQuantiles(FitHists64[i-3], nStatMinMC64, 0, wFitHists64[i-3]))

    NLLbins53.append(determineQuantiles(NLLhists53[i-3], nStatMinMC5,  1, wNLLhists53[i-3]))
    NLLbins63.append(determineQuantiles(NLLhists63[i-3], nStatMinMC63, 1, wNLLhists63[i-3]))
    NLLbins64.append(determineQuantiles(NLLhists64[i-3], nStatMinMC64, 1, wNLLhists64[i-3]))

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
chainData.Add("/eos/cms/store/group/phys_b2g/wprime/analyzeroutputjune2025/2016_SingleElectron/*.root")
chainData.SetBranchStatus("*",0)
chainData.SetBranchStatus("EventWeight",1)
chainData.SetBranchStatus("RegionIdentifier",1)
chainData.SetBranchStatus("JetPt",1)
chainData.SetBranchStatus("Best_Likelihood_*",1)
chainData.SetBranchStatus("LeptonPt",1)
chainData.SetBranchStatus("METPt",1)

SThist5 = TH1F("SThist5","", 1600, 400., 2000.)
SThist6 = TH1F("SThist6","", 1600, 400., 2000.)

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

STbins5 = determineQuantiles(SThist5, nStatMinST5, 0, SThist5)
STbins6 = determineQuantiles(SThist6, nStatMinST6, 0, SThist6)

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

