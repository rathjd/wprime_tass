import os,sys

from ROOT import TH1F, TFile, THStack, TRatioPlot, TCanvas, TLegend, TLine, TF1, TMatrixD, TFitResultPtr, TGraph, TColor
import cmsstyle as CMS
from array import array
import math

#settings for what to plot
LeptonFlav = 1
JetMult = 5
year = 2017

#accept shell inputs
try:
    if int(sys.argv[1]) > 0:
        LeptonFlav = int(sys.argv[1])
        print("lepton flavour set to "+str(LeptonFlav))
except:
    print("lepton flavour defaults to "+str(LeptonFlav))

try:
    if int(sys.argv[2]) > 0:
        JetMult = int(sys.argv[2])
        print("jet multiplicity set to "+str(JetMult))
except:
    print("jet multiplicity defaults to "+str(JetMult))

try:
    if int(sys.argv[3]) > 0:
        year = int(sys.argv[3])
        print("year set to "+str(year))
except:
    print("year defaults to "+str(year))

#CMS style setup
if year == 2016:
    CMS.SetLumi("36.3")
elif year == 2017:
    CMS.SetLumi("41.5")
elif year == 2018:
    CMS.SetLumi("59.8")
CMS.SetEnergy("13")

yearName = str(year)
binString = str(LeptonFlav)+"1"+str(JetMult)

STstart = 180.
STend = 2000.
if JetMult == 6:
    STstart = 210.

#all backgrounds considered in stacks and ratios
backgrounds = [["ttbar", 2], 
               ["wjets", 3],
               ["single_top", 4],
               ["diboson", 5]]

signals = [["M300", 6],
           ["M600", 7],
           ["M900", 8]]

baseSystematics = ["LumiCorr",
                   "LumiStat"+yearName,
                   "electron"+yearName,
                   "muonTrigger"+yearName,
                   "muonId"+yearName,
                   "muonIso"+yearName,
                   "BjetTagCorr",
                   "BjetTagUncorr"+yearName,
                   "PUID"+yearName,
                   "L1PreFiring"+yearName,
                   "PUreweight"+yearName,
                   "PDF",
                   "LHEScale",
                   "electronRes"+yearName,
                   "JES"+yearName,
                   "JER"+yearName]

SRsystematics = ["electronScale"+yearName,
                 "NLLnonClosure"+yearName+"_"+binString[0:3]+"2"]

#input files
binS = str(LeptonFlav)+"1"+str(JetMult)

inOrigin = TFile(binS+"1_"+str(year)+"/SimpleShapes_Wprime"+binS+"1_"+str(year)+".root","READ")
inSF     = TFile(binS+"2_"+str(year)+"/SF_Bin"+binS+"1_"+str(year)+".root","READ")
inResult = TFile(binS+"2_"+str(year)+"/SimpleShapes_Wprime"+binS+"2_"+str(year)+".root","READ")

#extract data histograms
Data1b = inOrigin.Get("ST_data_obs_Wprime"+binS+"1_"+str(year)+"_")
Data1b.Scale(1.,"width")
Data2b = inResult.Get("ST_data_obs_Wprime"+binS+"2_"+str(year)+"_")
Data2b.Scale(1.,"width")
Fit1b = inSF.Get("SF_")

#make empty stacks
Stack1b = THStack("Stack_"+binS+"1_"+str(year),"")
Stack2b = THStack("Stack_"+binS+"2_"+str(year),"")

#configure legends
leg1b = CMS.cmsLeg(0.6,0.89-0.05*6, 0.89, 0.89, textSize=0.05)
leg2b = CMS.cmsLeg(0.6,0.89-0.05*6, 0.89, 0.89, textSize=0.05)
if LeptonFlav == 1:
    if JetMult == 5:
        CMS.cmsHeader(leg1b, "#mu + 5 jets and 1 b-tag", textSize=0.05)
        CMS.cmsHeader(leg2b, "#mu + 5 jets and 2 b-tags", textSize=0.05)
    elif JetMult == 6:
        CMS.cmsHeader(leg1b, "#mu + 6 jets and 1 b-tag", textSize=0.05)
        CMS.cmsHeader(leg2b, "#mu + 6 jets and 2 b-tags", textSize=0.05)
elif LeptonFlav == 2:
    if JetMult == 5:
        CMS.cmsHeader(leg1b, "e + 5 jets and 1 b-tag", textSize=0.05)
        CMS.cmsHeader(leg2b, "e + 5 jets and 2 b-tags", textSize=0.05)
    elif JetMult == 6:
        CMS.cmsHeader(leg1b, "e + 6 jets and 1 b-tag", textSize=0.05)
        CMS.cmsHeader(leg2b, "e + 6 jets and 2 b-tags", textSize=0.05)

#determine number of bins
BinOrigin = inOrigin.Get("ST_ttbar_Wprime"+binS+"1_"+str(year)+"_")

#fill background dictionaries
Bgr1b = {}
Bgr1bSystUp = [0] * BinOrigin.GetNbinsX()
Bgr1bSystDown = [0] * BinOrigin.GetNbinsX()
Bgr2b = {}
Bgr2bSystUp = [0] * BinOrigin.GetNbinsX()
Bgr2bSystDown = [0] * BinOrigin.GetNbinsX()
for background in backgrounds:
    BgrPart1b = inOrigin.Get("ST_"+background[0]+"_Wprime"+binS+"1_"+str(year)+"_")
    BgrPart1b.Scale(1.,"width")
    Bgr1b[background[0]] = BgrPart1b
    
    if background[1]==2:
        BgrPart2b = inResult.Get("STrew_"+background[0]+"_Wprime"+binS+"2_"+str(year)+"_")
        BgrPart2b.Scale(1.,"width")
        BgrTotal1b = BgrPart1b.Clone("BgrTotal1b")
        BgrTotal2b = BgrPart2b.Clone("BgrTotal2b")
    else:
        BgrPart2b = inResult.Get("ST_"+background[0]+"_Wprime"+binS+"2_"+str(year)+"_")
        BgrPart2b.Scale(1.,"width")
        BgrTotal1b.Add(BgrPart1b)
        BgrTotal2b.Add(BgrPart2b)

    Bgr2b[background[0]] = BgrPart2b

    for bin in range(0,BgrPart1b.GetNbinsX()):
        Bgr1bSystUp[bin] += pow(BgrPart1b.GetBinError(bin+1),2)
        Bgr1bSystDown[bin] += pow(BgrPart1b.GetBinError(bin+1),2)
        Bgr2bSystUp[bin] += pow(BgrPart2b.GetBinError(bin+1),2)
        Bgr2bSystDown[bin] += pow(BgrPart2b.GetBinError(bin+1),2)

    #determine the uncertainties per bin
    for syst in baseSystematics:
        STstr = "ST_"
        if background[1]==2:
            STstr = "STrew_"
        Up1b   = inOrigin.Get("ST_"+background[0]+"_Wprime"+binS+"1_"+str(year)+"_"+syst+"Up")
        Down1b = inOrigin.Get("ST_"+background[0]+"_Wprime"+binS+"1_"+str(year)+"_"+syst+"Down")
        Up2b   = inResult.Get(STstr+background[0]+"_Wprime"+binS+"2_"+str(year)+"_"+syst+"Up")
        Down2b = inResult.Get(STstr+background[0]+"_Wprime"+binS+"2_"+str(year)+"_"+syst+"Down")
        Up1b.Scale(1.,"width")
        Down1b.Scale(1.,"width")
        Up2b.Scale(1.,"width")
        Down2b.Scale(1.,"width")

        for bin in range(0,BgrPart1b.GetNbinsX()):
            Bgr1bSystUp[bin] += pow(max(Up1b.GetBinContent(bin+1) - BgrPart1b.GetBinContent(bin+1),
                                    Down1b.GetBinContent(bin+1) - BgrPart1b.GetBinContent(bin+1),
                                    0.), 2)
            Bgr1bSystDown[bin] += pow(min(Up1b.GetBinContent(bin+1) - BgrPart1b.GetBinContent(bin+1),
                                    Down1b.GetBinContent(bin+1) - BgrPart1b.GetBinContent(bin+1),
                                    0.), 2)
            Bgr2bSystUp[bin] += pow(max(Up2b.GetBinContent(bin+1) - BgrPart2b.GetBinContent(bin+1),
                                    Down1b.GetBinContent(bin+1) - BgrPart2b.GetBinContent(bin+1),
                                    0.), 2)
            Bgr1bSystDown[bin] += pow(min(Up2b.GetBinContent(bin+1) - BgrPart2b.GetBinContent(bin+1),
                                    Down2b.GetBinContent(bin+1) - BgrPart2b.GetBinContent(bin+1),
                                    0.), 2)
    #add special systematic for ttbar
#    if background[1]==2:
#        STstr = "STrew_"
#        Up2b   = inResult.Get(STstr+background[0]+"_Wprime"+binS+"2_"+str(year)+"_"+"STfit_"+yearName+"_"+binString[0:3]+"2_STfit"+"Up")
#        Nom2b  = inResult.Get(STstr+background[0]+"_Wprime"+binS+"2_"+str(year)+"_")
#        Down2b = inResult.Get(STstr+background[0]+"_Wprime"+binS+"2_"+str(year)+"_"+"STfit_"+yearName+"_"+binString[0:3]+"2_STfit"+"Down")
#        Up2b.Scale(1.,"width")
#        Nom2b.Scale(1.,"width")
#        Down2b.Scale(1.,"width")
#        for bin in range(0,Nom1b.GetNbinsX()):
#            Bgr2bSystUp[bin] += pow(max(Up2b.GetBinContent(bin+1) - Nom2b.GetBinContent(bin+1),
#                                    Down1b.GetBinContent(bin+1) - Nom2b.GetBinContent(bin+1),
#                                    0.), 2)
#            Bgr1bSystDown[bin] += pow(min(Up2b.GetBinContent(bin+1) - Nom2b.GetBinContent(bin+1),
#                                    Down2b.GetBinContent(bin+1) - Nom2b.GetBinContent(bin+1),
#                                    0.), 2)

#get all the uncertainty values
for bin in range(0, len(Bgr1bSystUp)):
    Bgr1bSystUp[bin] = math.sqrt(Bgr1bSystUp[bin])
    Bgr1bSystDown[bin] = math.sqrt(Bgr1bSystDown[bin])
    Bgr2bSystUp[bin] = math.sqrt(Bgr1bSystUp[bin])
    Bgr2bSystDown[bin] = math.sqrt(Bgr1bSystDown[bin])

#fill error bands
Err1bX, Err1bY, Err2bX, Err2bY, Err1bR, Err2bR = array('d'), array('d'), array('d'), array('d'), array('d'), array('d')
for bin in range(0, len(Bgr1bSystUp)): #upper band
    Err1bX.append(BgrTotal1b.GetBinLowEdge(bin+1))
    Err1bY.append(Bgr1bSystUp[bin]+BgrTotal1b.GetBinContent(bin+1))
    Err1bR.append(BgrTotal1b.GetBinContent(bin+1) and Bgr1bSystUp[bin]/BgrTotal1b.GetBinContent(bin+1)+1. or 0.)
    Err1bX.append(BgrTotal1b.GetBinLowEdge(bin+2))
    Err1bY.append(Bgr1bSystUp[bin]+BgrTotal1b.GetBinContent(bin+1))
    Err1bR.append(BgrTotal1b.GetBinContent(bin+1) and Bgr1bSystUp[bin]/BgrTotal1b.GetBinContent(bin+1)+1. or 0.)

    Err2bX.append(BgrTotal2b.GetBinLowEdge(bin+1))
    Err2bY.append(Bgr2bSystUp[bin]+BgrTotal2b.GetBinContent(bin+1))
    Err2bR.append(BgrTotal2b.GetBinContent(bin+1) and Bgr2bSystUp[bin]/BgrTotal2b.GetBinContent(bin+1)+1. or 0.)
    Err2bX.append(BgrTotal2b.GetBinLowEdge(bin+2))
    Err2bY.append(Bgr2bSystUp[bin]+BgrTotal2b.GetBinContent(bin+1))
    Err2bR.append(BgrTotal2b.GetBinContent(bin+1) and Bgr2bSystUp[bin]/BgrTotal2b.GetBinContent(bin+1)+1. or 0.)
for bin in range(len(Bgr1bSystUp)-1,-2,-1): #lower band
    Err1bX.append(BgrTotal1b.GetBinLowEdge(bin+2))
    Err1bY.append(-Bgr1bSystDown[bin]+BgrTotal1b.GetBinContent(bin+1))
    Err1bR.append(BgrTotal1b.GetBinContent(bin+1) and -Bgr1bSystDown[bin]/BgrTotal1b.GetBinContent(bin+1)+1. or 0.)    
    Err1bX.append(BgrTotal1b.GetBinLowEdge(bin+1))
    Err1bY.append(-Bgr1bSystDown[bin]+BgrTotal1b.GetBinContent(bin+1))
    Err1bR.append(BgrTotal1b.GetBinContent(bin+1) and -Bgr1bSystDown[bin]/BgrTotal1b.GetBinContent(bin+1)+1. or 0.)

    Err2bX.append(BgrTotal2b.GetBinLowEdge(bin+2))
    Err2bY.append(-Bgr2bSystDown[bin]+BgrTotal2b.GetBinContent(bin+1))
    Err2bR.append(BgrTotal2b.GetBinContent(bin+1) and -Bgr2bSystDown[bin]/BgrTotal2b.GetBinContent(bin+1)+1. or 0.)
    Err2bX.append(BgrTotal2b.GetBinLowEdge(bin+1))
    Err2bY.append(-Bgr2bSystDown[bin]+BgrTotal2b.GetBinContent(bin+1))
    Err2bR.append(BgrTotal2b.GetBinContent(bin+1) and -Bgr2bSystDown[bin]/BgrTotal2b.GetBinContent(bin+1)+1. or 0.)

#make 1 b-tag region plot
STmax1b = max(Data1b.GetMaximum(), BgrTotal1b.GetMaximum())*1.1
canv1b = CMS.cmsDiCanvas("STSF_"+binS+"_"+str(year)+"_1b",STstart,STend,0,STmax1b,0.5,1.5,"S_{T} [GeV/c]", "Events/bin width", "Data/Pred.", square=CMS.kSquare, extraSpace=0.1, iPos=0)
canv1b.cd(1)

leg1b.AddEntry(Data1b, "Data", "lp")
CMS.cmsDrawStack(Stack1b, leg1b, Bgr1b)

grErr1b = TGraph(len(Err1bX), Err1bX, Err1bY)
grErr1b.SetFillColor(17)
grErr1b.SetFillStyle(3008)

grErr1b.Draw("F, same")

CMS.cmsDraw(Data1b, "P", mcolor=1)
leg1b.Draw()

CMS.fixOverlay()

canv1b.cd(2)

Ratio1b = Data1b.Clone("Ratio1b")
Ratio1b.Divide(BgrTotal1b)

grErr1bR = TGraph(len(Err1bX), Err1bX, Err1bR)
grErr1bR.SetFillColor(17)
grErr1bR.SetFillStyle(3008)
grErr1bR.Draw("F, same")

CMS.cmsDraw(Ratio1b, "P", mcolor=1)

ref_line = TLine(STstart, 1, STend, 1)
CMS.cmsDrawLine(ref_line, lcolor=1, lstyle=3)

CMS.SaveCanvas(canv1b, "ST_Wprime"+binS+"1_"+str(year)+".pdf")

#make 2 b-tag region plot
STmax2b = max(Data2b.GetMaximum(), BgrTotal2b.GetMaximum())*1.1
canv2b = CMS.cmsDiCanvas("STSF_"+binS+"_"+str(year)+"_2b",STstart,STend,0,STmax2b,0.5,1.5,"S_{T} [GeV/c]", "Events/bin width", "Data/Pred.", square=CMS.kSquare, extraSpace=0.1, iPos=0)
canv2b.cd(1)

leg2b.AddEntry(Data2b, "Data", "lp")
CMS.cmsDrawStack(Stack2b, leg2b, Bgr2b)
grErr2b = TGraph(len(Err2bX), Err2bX, Err2bY)
grErr2b.SetFillColor(17)
grErr2b.SetFillStyle(3008)
grErr2b.Draw("F, same")
CMS.cmsDraw(Data2b, "P", mcolor=1)
leg2b.Draw()

CMS.fixOverlay()

canv2b.cd(2)

Ratio2b = Data2b.Clone("Ratio2b")
Ratio2b.Divide(BgrTotal2b)

grErr2bR = TGraph(len(Err2bX), Err2bX, Err2bR)
grErr2bR.SetFillColor(17)
grErr2bR.SetFillStyle(3008)
grErr2bR.Draw("F")

CMS.cmsDraw(Ratio2b, "P", mcolor=1)

ref_line = TLine(STstart, 1, STend, 1)
CMS.cmsDrawLine(ref_line, lcolor=1, lstyle=3)

CMS.SaveCanvas(canv2b, "ST_Wprime"+binS+"2_"+str(year)+".pdf")

#make 1b-tag ST correction fit plot
SF = inSF.Get("SF_")
if JetMult == 5:
  SFfit = TF1("fitFunction","[0]/x/x/x + [1]/x/x + [2]/x + [3] + [4]*x + [5]*x*x", 180., 2000.)
  rangelow = 180.
  rangehigh = 2000.
else:
    SFfit = TF1("fitFunction","[0]/x + [1] + [2]*x + [3]*x*x", 210., 2000.)
    rangelow = 180.
    rangehigh = 2000.
fr = SF.Fit(SFfit,"SRF")
cov = fr.GetCovarianceMatrix()
if JetMult == 5:
  SFfitUp = TF1("fitFunctionUp", "[0]/x/x/x/x/x/x + [1]/x/x/x/x/x + [2]/x/x/x/x + [3]/x/x/x + [4]/x/x + [5]/x + [6] + [7]*x + [8]*x*x + [9]*x*x*x + [10]*x*x*x*x", 180., 2000.)
  SFfitUp.SetParameters(cov(0,0), cov(0,1)+cov(1,0), cov(0,2)+cov(1,1)+cov(2,0), cov(0,3)+cov(1,2)+cov(2,1)+cov(3,0)+SFfit.GetParameter(0), cov(0,4)+cov(1,3)+cov(2,2)+cov(3,1)+cov(4,0)+SFfit.GetParameter(1), cov(0,5)+cov(1,4)+cov(2,3)+cov(3,2)+cov(4,1)+cov(5,0)+SFfit.GetParameter(2), cov(1,5)+cov(2,4)+cov(3,3)+cov(4,2)+cov(5,1)+SFfit.GetParameter(3), cov(2,5)+cov(3,4)+cov(4,3)+cov(5,2)+SFfit.GetParameter(4), cov(3,5)+cov(4,4)+cov(5,3)+SFfit.GetParameter(5), cov(4,5)+cov(5,4), cov(5,5))
  SFfitDown = TF1("fitFunctionDown", "[0]/x/x/x/x/x/x + [1]/x/x/x/x/x + [2]/x/x/x/x + [3]/x/x/x + [4]/x/x + [5]/x + [6] + [7]*x + [8]*x*x + [9]*x*x*x + [10]*x*x*x*x", 180., 2000.)
  SFfitDown.SetParameters(-cov(0,0), -cov(0,1)-cov(1,0), -cov(0,2)-cov(1,1)-cov(2,0), -cov(0,3)-cov(1,2)-cov(2,1)-cov(3,0)+SFfit.GetParameter(0), -cov(0,4)-cov(1,3)-cov(2,2)-cov(3,1)-cov(4,0)+SFfit.GetParameter(1), -cov(0,5)-cov(1,4)-cov(2,3)-cov(3,2)-cov(4,1)-cov(5,0)+SFfit.GetParameter(2), -cov(1,5)-cov(2,4)-cov(3,3)-cov(4,2)-cov(5,1)+SFfit.GetParameter(3), -cov(2,5)-cov(3,4)-cov(4,3)-cov(5,2)+SFfit.GetParameter(4), -cov(3,5)-cov(4,4)-cov(5,3)+SFfit.GetParameter(5), -cov(4,5)-cov(5,4), -cov(5,5))
else:
  SFfitUp = TF1("fitFunctionUp", "[0]/x/x + [1]/x + [2] + [3]*x + [4]*x*x + [5]*x*x*x + [6]*x*x*x*x", 210., 2000.)
  SFfitUp.SetParameters(cov(0,0), cov(0,1)+cov(1,0)+SFfit.GetParameter(0), cov(0,2)+cov(1,1)+cov(2,0)+SFfit.GetParameter(1), cov(0,3)+cov(1,2)+cov(2,1)+cov(3,0)+SFfit.GetParameter(2), cov(1,3)+cov(2,2)+SFfit.GetParameter(3)+cov(3,1), cov(2,3)+cov(3,2), cov(3,3))
  SFfitDown = TF1("fitFunctionDown", "[0]/x/x + [1]/x + [2] + [3]*x + [4]*x*x + [5]*x*x*x + [6]*x*x*x*x", 210., 2000.)
  SFfitDown.SetParameters(-cov(0,0), -cov(0,1)-cov(1,0)+SFfit.GetParameter(0), -cov(0,2)-cov(1,1)-cov(2,0)+SFfit.GetParameter(1), -cov(0,3)-cov(1,2)-cov(2,1)-cov(3,0)+SFfit.GetParameter(2), -cov(1,3)-cov(2,2)+SFfit.GetParameter(3)-cov(3,1), -cov(2,3)-cov(3,2), -cov(3,3))



canvSF1b = CMS.cmsCanvas("STSFfit_"+binS+"_"+str(year)+"_1b", STstart, STend, SFfitDown.GetMinimum(rangelow,rangehigh)*0.8, SFfit.GetMaximum(rangelow,rangehigh)*1.2, "S_{T} [GeV/c]", "(data - MC(w/o t#bar{t}))/t#bar{t}", square=CMS.kSquare, extraSpace = 0.01, iPos = 0)
canvSF1b.cd(1)
CMS.cmsDraw(SF, "P", mcolor=1)
SFfitUp.SetLineColor(2)
SFfitUp.SetLineStyle(2)
SFfitUp.Draw("L,same")
SFfitDown.SetLineColor(2)
SFfitDown.SetLineStyle(2)
SFfitDown.Draw("L,same")
stat1bfit = SF.GetListOfFunctions().FindObject("stats")
stat1bfit.SetX1NDC(0.5)
stat1bfit.SetX2NDC(0.9)
stat1bfit.SetY1NDC(0.6)
stat1bfit.SetY2NDC(0.9)
canvSF1b.Update()

CMS.SaveCanvas(canvSF1b, "STfit_Wprime"+binS+"1_"+str(year)+".pdf")
