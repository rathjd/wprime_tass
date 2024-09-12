import os,sys

from ROOT import TH1F, TFile, THStack, TRatioPlot, TCanvas, TLegend, TLine, TF1, TMatrixD, TFitResultPtr, TGraph, TColor, TPaveStats
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
inSF2    = TFile(binS+"3_"+str(year)+"/SF_Bin"+binS+"2_"+str(year)+".root","READ")
inResult = TFile(binS+"2_"+str(year)+"/SimpleShapes_Wprime"+binS+"2_"+str(year)+".root","READ")

#extract data histograms
Data1b = inOrigin.Get("ST_data_obs_Wprime"+binS+"1_"+str(year)+"_")
Data1b.Scale(1.,"width")
Data2b = inResult.Get("ST_data_obs_Wprime"+binS+"2_"+str(year)+"_")
Data2b.Scale(1.,"width")
Fit1b = inSF.Get("SF_")

#make empty stacks
Stack1b    = THStack("Stack_"+binS+"1_"+str(year),"")
Stack2b    = THStack("Stack_"+binS+"2_"+str(year),"")
Stack2braw = THStack("Stackraw_"+binS+"2_"+str(year),"")
StackNLLres= THStack("StackNLL_"+binS+"2_"+str(year),"")

#configure legends
leg1b    = CMS.cmsLeg(0.51,0.89-0.05*6, 0.8, 0.89, textSize=0.05)
leg2b    = CMS.cmsLeg(0.51,0.89-0.05*6, 0.8, 0.89, textSize=0.05)
leg2braw = CMS.cmsLeg(0.51,0.89-0.05*6, 0.8, 0.89, textSize=0.05)
leg2bNLL = CMS.cmsLeg(0.46,0.59-0.05*9, 0.8, 0.59, textSize=0.05)

if LeptonFlav == 1:
    if JetMult == 5:
        CMS.cmsHeader(leg1b, "#mu + 5 jets and 1 b-tag", textSize=0.05)
        CMS.cmsHeader(leg2b, "#mu + 5 jets and 2 b-tags rew.", textSize=0.05)
        CMS.cmsHeader(leg2braw, "#mu + 5 jets and 2 b-tags", textSize=0.05)
        CMS.cmsHeader(leg2bNLL, "#mu + 5 jets and 2 b-tags", textSize=0.05)
    elif JetMult == 6:
        CMS.cmsHeader(leg1b, "#mu + 6 jets and 1 b-tag", textSize=0.05)
        CMS.cmsHeader(leg2b, "#mu + 6 jets and 2 b-tags rew.", textSize=0.05)
        CMS.cmsHeader(leg2braw, "#mu + 6 jets and 2 b-tags", textSize=0.05)
        CMS.cmsHeader(leg2bNLL, "#mu + 6 jets and 2 b-tags", textSize=0.05)
elif LeptonFlav == 2:
    if JetMult == 5:
        CMS.cmsHeader(leg1b, "e + 5 jets and 1 b-tag", textSize=0.05)
        CMS.cmsHeader(leg2b, "e + 5 jets and 2 b-tags rew.", textSize=0.05)
        CMS.cmsHeader(leg2braw, "e + 5 jets and 2 b-tags", textSize=0.05)
        CMS.cmsHeader(leg2bNLL, "e + 5 jets and 2 b-tags", textSize=0.05)
    elif JetMult == 6:
        CMS.cmsHeader(leg1b, "e + 6 jets and 1 b-tag", textSize=0.05)
        CMS.cmsHeader(leg2b, "e + 6 jets and 2 b-tags rew.", textSize=0.05)
        CMS.cmsHeader(leg2braw, "e + 6 jets and 2 b-tags", textSize=0.05)
        CMS.cmsHeader(leg2bNLL, "e + 6 jets and 2 b-tags", textSize=0.05)


#determine number of bins
BinOrigin = inOrigin.Get("ST_ttbar_Wprime"+binS+"1_"+str(year)+"_")

#fill background dictionaries
Bgr1b = {}
Bgr1bSystUp = [0] * BinOrigin.GetNbinsX()
Bgr1bSystDown = [0] * BinOrigin.GetNbinsX()
Bgr2b = {}
Bgr2bSystUp = [0] * BinOrigin.GetNbinsX()
Bgr2bSystDown = [0] * BinOrigin.GetNbinsX()
Bgr2braw = {}
Bgr2brawSystUp = [0] * BinOrigin.GetNbinsX()
Bgr2brawSystDown = [0] * BinOrigin.GetNbinsX()
NLLresList = []

for background in backgrounds:
    BgrPart1b = inOrigin.Get("ST_"+background[0]+"_Wprime"+binS+"1_"+str(year)+"_")
    BgrPart1b.Scale(1.,"width")
    Bgr1b[background[0]] = BgrPart1b

    BgrPart2braw = inResult.Get("ST_"+background[0]+"_Wprime"+binS+"2_"+str(year)+"_")
    BgrPart2braw.Scale(1.,"width")
    Bgr2braw[background[0]] = BgrPart2braw

    
    if background[1]==2:
        BgrPart2b = inResult.Get("STrew_"+background[0]+"_Wprime"+binS+"2_"+str(year)+"_")
        BgrPart2b.Scale(1.,"width")
        BgrTotal1b    = BgrPart1b.Clone("BgrTotal1b")
        BgrTotal2b    = BgrPart2b.Clone("BgrTotal2b")
        BgrTotal2braw = BgrPart2braw.Clone("BgrTotal2braw")
    else:
        BgrPart2b = inResult.Get("ST_"+background[0]+"_Wprime"+binS+"2_"+str(year)+"_")
        BgrPart2b.Scale(1.,"width")
        BgrTotal1b.Add(BgrPart1b)
        BgrTotal2b.Add(BgrPart2b)
        BgrTotal2braw.Add(BgrPart2braw)

    Bgr2b[background[0]] = BgrPart2b

    #statistical uncertainty of a background per bin
    for bin in range(0,BgrPart1b.GetNbinsX()):
        Bgr1bSystUp[bin] += pow(BgrPart1b.GetBinError(bin+1),2)
        Bgr1bSystDown[bin] += pow(BgrPart1b.GetBinError(bin+1),2)
        Bgr2bSystUp[bin] += pow(BgrPart2b.GetBinError(bin+1),2)
        Bgr2bSystDown[bin] += pow(BgrPart2b.GetBinError(bin+1),2)
        Bgr2brawSystUp[bin] += pow(BgrPart2b.GetBinError(bin+1),2)
        Bgr2brawSystDown[bin] += pow(BgrPart2b.GetBinError(bin+1),2)

    #determine the uncertainties per bin
    for syst in baseSystematics:
        STstr = "ST_"
        if background[1]==2:
            STstr = "STrew_"
        Up1b      = inOrigin.Get("ST_"+background[0]+"_Wprime"+binS+"1_"+str(year)+"_"+syst+"Up")
        Down1b    = inOrigin.Get("ST_"+background[0]+"_Wprime"+binS+"1_"+str(year)+"_"+syst+"Down")
        Up2b      = inResult.Get(STstr+background[0]+"_Wprime"+binS+"2_"+str(year)+"_"+syst+"Up")
        Down2b    = inResult.Get(STstr+background[0]+"_Wprime"+binS+"2_"+str(year)+"_"+syst+"Down")
        Up2braw   = inResult.Get("ST_"+background[0]+"_Wprime"+binS+"2_"+str(year)+"_"+syst+"Up")
        Down2braw = inResult.Get("ST_"+background[0]+"_Wprime"+binS+"2_"+str(year)+"_"+syst+"Down")
        Up1b.Scale(1.,"width")
        Down1b.Scale(1.,"width")
        Up2b.Scale(1.,"width")
        Down2b.Scale(1.,"width")
        Up2braw.Scale(1.,"width")
        Down2braw.Scale(1.,"width")

        for bin in range(0,BgrPart1b.GetNbinsX()):
            Bgr1bSystUp[bin] += pow(max(Up1b.GetBinContent(bin+1) - BgrPart1b.GetBinContent(bin+1),
                                    Down1b.GetBinContent(bin+1) - BgrPart1b.GetBinContent(bin+1),
                                    0.), 2)
            Bgr1bSystDown[bin] += pow(min(Up1b.GetBinContent(bin+1) - BgrPart1b.GetBinContent(bin+1),
                                    Down1b.GetBinContent(bin+1) - BgrPart1b.GetBinContent(bin+1),
                                    0.), 2)
            Bgr2bSystUp[bin] += pow(max(Up2b.GetBinContent(bin+1) - BgrPart2b.GetBinContent(bin+1),
                                    Down2b.GetBinContent(bin+1) - BgrPart2b.GetBinContent(bin+1),
                                    0.), 2)
            Bgr2bSystDown[bin] += pow(min(Up2b.GetBinContent(bin+1) - BgrPart2b.GetBinContent(bin+1),
                                    Down2b.GetBinContent(bin+1) - BgrPart2b.GetBinContent(bin+1),
                                    0.), 2)
            Bgr2brawSystUp[bin] += pow(max(Up2braw.GetBinContent(bin+1) - BgrPart2braw.GetBinContent(bin+1),
                                    Down2braw.GetBinContent(bin+1) - BgrPart2braw.GetBinContent(bin+1),
                                    0.), 2)
            Bgr2brawSystDown[bin] += pow(min(Up2braw.GetBinContent(bin+1) - BgrPart2braw.GetBinContent(bin+1),
                                    Down2braw.GetBinContent(bin+1) - BgrPart2braw.GetBinContent(bin+1),
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

#fill NLL variations by mass
for mass in range(3,12):
    NLLresPart = inSF2.Get("NLLresidual_"+binS+"2_"+str(year)+"_M"+str(mass*100))
    NLLresPart.Scale(1.+(mass-3)*0.1) #add a constant offset
    NLLresPart.SetLineColor(mass-2)
    NLLresPart.SetMarkerColor(mass-2)
    NLLresPart.SetMarkerStyle(20)
    leg2bNLL.AddEntry(NLLresPart,"M"+str(mass*100)+" * "+str("%.1f" % (1.+(mass-3)*0.1)),"lp")
    NLLresList.append(NLLresPart)

#get all the uncertainty values
for bin in range(0, len(Bgr1bSystUp)):
    Bgr1bSystUp[bin] = math.sqrt(Bgr1bSystUp[bin])
    Bgr1bSystDown[bin] = math.sqrt(Bgr1bSystDown[bin])
    Bgr2bSystUp[bin] = math.sqrt(Bgr2bSystUp[bin])
    Bgr2bSystDown[bin] = math.sqrt(Bgr2bSystDown[bin])
    Bgr2brawSystUp[bin] = math.sqrt(Bgr2brawSystUp[bin])
    Bgr2brawSystDown[bin] = math.sqrt(Bgr2brawSystDown[bin])

#fill error bands
Err1bX, Err1bY, Err2bX, Err2bY, Err2brawX, Err2brawY, Err1bR, Err2bR, Err2brawR = array('d'), array('d'), array('d'), array('d'), array('d'), array('d'), array('d'), array('d'), array('d')
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

    Err2brawX.append(BgrTotal2braw.GetBinLowEdge(bin+1))
    Err2brawY.append(Bgr2brawSystUp[bin]+BgrTotal2braw.GetBinContent(bin+1))
    Err2brawR.append(BgrTotal2braw.GetBinContent(bin+1) and Bgr2brawSystUp[bin]/BgrTotal2braw.GetBinContent(bin+1)+1. or 0.)
    Err2brawX.append(BgrTotal2braw.GetBinLowEdge(bin+2))
    Err2brawY.append(Bgr2brawSystUp[bin]+BgrTotal2braw.GetBinContent(bin+1))
    Err2brawR.append(BgrTotal2braw.GetBinContent(bin+1) and Bgr2brawSystUp[bin]/BgrTotal2braw.GetBinContent(bin+1)+1. or 0.)
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

    Err2brawX.append(BgrTotal2braw.GetBinLowEdge(bin+2))
    Err2brawY.append(-Bgr2brawSystDown[bin]+BgrTotal2braw.GetBinContent(bin+1))
    Err2brawR.append(BgrTotal2braw.GetBinContent(bin+1) and -Bgr2brawSystDown[bin]/BgrTotal2braw.GetBinContent(bin+1)+1. or 0.)
    Err2brawX.append(BgrTotal2braw.GetBinLowEdge(bin+1))
    Err2brawY.append(-Bgr2brawSystDown[bin]+BgrTotal2braw.GetBinContent(bin+1))
    Err2brawR.append(BgrTotal2braw.GetBinContent(bin+1) and -Bgr2brawSystDown[bin]/BgrTotal2braw.GetBinContent(bin+1)+1. or 0.)

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

#make reweighted 2 b-tag region plot
STmax2b = max(Data2b.GetMaximum(), BgrTotal2b.GetMaximum())*1.1
canv2b = CMS.cmsDiCanvas("STSFrew_"+binS+"_"+str(year)+"_2b",STstart,STend,0,STmax2b,0.5,1.5,"S_{T} [GeV/c]", "Events/bin width", "Data/Pred.", square=CMS.kSquare, extraSpace=0.1, iPos=0)
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

CMS.SaveCanvas(canv2b, "STrew_Wprime"+binS+"2_"+str(year)+".pdf")

#make raw 2 b-tag region plot
STmax2braw = max(Data2b.GetMaximum(), BgrTotal2braw.GetMaximum())*1.1
canv2braw = CMS.cmsDiCanvas("STSF_"+binS+"_"+str(year)+"_2b",STstart,STend,0,STmax2braw,0.5,1.5,"S_{T} [GeV/c]", "Events/bin width", "Data/Pred.", square=CMS.kSquare, extraSpace=0.1, iPos=0)
canv2braw.cd(1)

leg2braw.AddEntry(Data2b, "Data", "lp")
CMS.cmsDrawStack(Stack2braw, leg2braw, Bgr2braw)
grErr2braw = TGraph(len(Err2brawX), Err2brawX, Err2brawY)
grErr2braw.SetFillColor(17)
grErr2braw.SetFillStyle(3008)
grErr2braw.Draw("F, same")
CMS.cmsDraw(Data2b, "P", mcolor=1)
leg2braw.Draw()

CMS.fixOverlay()

canv2braw.cd(2)

Ratio2braw = Data2b.Clone("Ratio2braw")
Ratio2braw.Divide(BgrTotal2braw)

grErr2bRraw = TGraph(len(Err2brawX), Err2brawX, Err2brawR)
grErr2bRraw.SetFillColor(17)
grErr2bRraw.SetFillStyle(3008)
grErr2bRraw.Draw("F")

CMS.cmsDraw(Ratio2braw, "P", mcolor=1)

ref_lineraw = TLine(STstart, 1, STend, 1)
CMS.cmsDrawLine(ref_lineraw, lcolor=1, lstyle=3)

CMS.SaveCanvas(canv2braw, "ST_Wprime"+binS+"2_"+str(year)+".pdf")

#make 1b-tag ST correction fit plot
SF = inSF.Get("SF_")
if JetMult == 5:
  SFfit = TF1("fitFunction","[0]/x/x/x + [1]/x/x + [2]/x + [3] + [4]*x + [5]*x*x", 180., 2000.)
  SFfit.SetParNames("p0*x^{-3}","p1*x^{-2}","p2*x^{-1}","p3*x^{0}","p4*x^{1}","p5*x^{2}")
  rangelow = 180.
  rangehigh = 2000.
else:
    SFfit = TF1("fitFunction","[0]/x + [1] + [2]*x + [3]*x*x", 210., 2000.)
    SFfit.SetParNames("p0*x^{-1}","p1*x^{0}","p2*x^{1}","p3*x^{2}")
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
stat1bfit.SetY1NDC(0.2)
stat1bfit.SetY2NDC(0.5)
ps1b = canvSF1b.GetPrimitive("stats")
ps1b.SetFillColor(0)
ps1b.SetFitFormat("0.2f")
ps1b.SetStatFormat("0.2f")
canvSF1b.Update()

CMS.SaveCanvas(canvSF1b, "STfit_Wprime"+binS+"1_"+str(year)+".pdf")

#make 2b-tag ST correction fit plot
SF2 = inSF2.Get("SF_")
if JetMult == 5:
  SFfit2 = TF1("fitFunction2","[0]/x/x/x + [1]/x/x + [2]/x + [3] + [4]*x + [5]*x*x", 180., 2000.)
  SFfit2.SetParNames("p0*x^{-3}","p1*x^{-2}","p2*x^{-1}","p3*x^{0}","p4*x^{1}","p5*x^{2}")
  rangelow2 = 180.
  rangehigh2 = 2000.
else:
    SFfit2 = TF1("fitFunction","[0]/x + [1] + [2]*x + [3]*x*x", 210., 2000.)
    SFfit2.SetParNames("p0*x^{-1}","p1*x^{0}","p2*x^{1}","p3*x^{2}")
    rangelow2 = 180.
    rangehigh2 = 2000.
fr2 = SF2.Fit(SFfit2,"SRF")
cov2 = fr2.GetCovarianceMatrix()
if JetMult == 5:
  SFfitUp2 = TF1("fitFunctionUp2", "[0]/x/x/x/x/x/x + [1]/x/x/x/x/x + [2]/x/x/x/x + [3]/x/x/x + [4]/x/x + [5]/x + [6] + [7]*x + [8]*x*x + [9]*x*x*x + [10]*x*x*x*x", 180., 2000.)
  SFfitUp2.SetParameters(cov2(0,0), cov2(0,1)+cov2(1,0), cov2(0,2)+cov2(1,1)+cov2(2,0), cov2(0,3)+cov2(1,2)+cov2(2,1)+cov2(3,0)+SFfit2.GetParameter(0), cov2(0,4)+cov2(1,3)+cov2(2,2)+cov2(3,1)+cov2(4,0)+SFfit2.GetParameter(1), cov2(0,5)+cov2(1,4)+cov2(2,3)+cov2(3,2)+cov2(4,1)+cov2(5,0)+SFfit2.GetParameter(2), cov2(1,5)+cov2(2,4)+cov2(3,3)+cov2(4,2)+cov2(5,1)+SFfit2.GetParameter(3), cov2(2,5)+cov2(3,4)+cov2(4,3)+cov2(5,2)+SFfit2.GetParameter(4), cov2(3,5)+cov2(4,4)+cov2(5,3)+SFfit2.GetParameter(5), cov2(4,5)+cov2(5,4), cov2(5,5))
  SFfitDown2 = TF1("fitFunctionDown2", "[0]/x/x/x/x/x/x + [1]/x/x/x/x/x + [2]/x/x/x/x + [3]/x/x/x + [4]/x/x + [5]/x + [6] + [7]*x + [8]*x*x + [9]*x*x*x + [10]*x*x*x*x", 180., 2000.)
  SFfitDown2.SetParameters(-cov2(0,0), -cov2(0,1)-cov2(1,0), -cov2(0,2)-cov2(1,1)-cov2(2,0), -cov2(0,3)-cov2(1,2)-cov2(2,1)-cov2(3,0)+SFfit2.GetParameter(0), -cov2(0,4)-cov2(1,3)-cov2(2,2)-cov2(3,1)-cov2(4,0)+SFfit2.GetParameter(1), -cov2(0,5)-cov2(1,4)-cov2(2,3)-cov2(3,2)-cov2(4,1)-cov2(5,0)+SFfit2.GetParameter(2), -cov2(1,5)-cov2(2,4)-cov2(3,3)-cov2(4,2)-cov2(5,1)+SFfit2.GetParameter(3), -cov2(2,5)-cov2(3,4)-cov2(4,3)-cov2(5,2)+SFfit2.GetParameter(4), -cov2(3,5)-cov2(4,4)-cov2(5,3)+SFfit2.GetParameter(5), -cov2(4,5)-cov2(5,4), -cov2(5,5))
else:
  SFfitUp2 = TF1("fitFunctionUp2", "[0]/x/x + [1]/x + [2] + [3]*x + [4]*x*x + [5]*x*x*x + [6]*x*x*x*x", 210., 2000.)
  SFfitUp2.SetParameters(cov2(0,0), cov2(0,1)+cov2(1,0)+SFfit2.GetParameter(0), cov2(0,2)+cov2(1,1)+cov2(2,0)+SFfit2.GetParameter(1), cov2(0,3)+cov2(1,2)+cov2(2,1)+cov2(3,0)+SFfit2.GetParameter(2), cov2(1,3)+cov2(2,2)+SFfit2.GetParameter(3)+cov2(3,1), cov2(2,3)+cov2(3,2), cov2(3,3))
  SFfitDown2 = TF1("fitFunctionDown2", "[0]/x/x + [1]/x + [2] + [3]*x + [4]*x*x + [5]*x*x*x + [6]*x*x*x*x", 210., 2000.)
  SFfitDown2.SetParameters(-cov2(0,0), -cov2(0,1)-cov2(1,0)+SFfit2.GetParameter(0), -cov2(0,2)-cov2(1,1)-cov2(2,0)+SFfit2.GetParameter(1), -cov2(0,3)-cov2(1,2)-cov2(2,1)-cov2(3,0)+SFfit2.GetParameter(2), -cov2(1,3)-cov2(2,2)+SFfit2.GetParameter(3)-cov2(3,1), -cov2(2,3)-cov2(3,2), -cov2(3,3))



canvSF2b = CMS.cmsCanvas("STSFfit_"+binS+"_"+str(year)+"_2b", STstart, STend, SFfitDown2.GetMinimum(rangelow2,rangehigh2)*0.8, SFfit2.GetMaximum(rangelow2,rangehigh2)*1.2, "S_{T} [GeV/c]", "(data - MC(w/o t#bar{t}))/t#bar{t}", square=CMS.kSquare, extraSpace = 0.01, iPos = 0)
canvSF2b.cd(1)
CMS.cmsDraw(SF2, "P", mcolor=1)
SFfitUp2.SetLineColor(2)
SFfitUp2.SetLineStyle(2)
SFfitUp2.Draw("L,same")
SFfitDown2.SetLineColor(2)
SFfitDown2.SetLineStyle(2)
SFfitDown2.Draw("L,same")
stat2bfit = SF2.GetListOfFunctions().FindObject("stats")
stat2bfit.SetX1NDC(0.5)
stat2bfit.SetX2NDC(0.9)
stat2bfit.SetY1NDC(0.6)
stat2bfit.SetY2NDC(0.9)
ps2b = canvSF2b.GetPrimitive("stats")
ps2b.SetFillColor(0)
ps2b.SetFitFormat("0.2f")
ps2b.SetStatFormat("0.2f")
canvSF2b.Update()

CMS.SaveCanvas(canvSF2b, "STfit_Wprime"+binS+"2_"+str(year)+".pdf")

#make NLL residual plot
canvNLL = CMS.cmsCanvas("NLL_"+binS+"_"+str(year)+"_2b",0,30,-1.5,2.5,"-log(L)", "residual after S_{T} reweight", square=CMS.kSquare, extraSpace=0.1, iPos=0)
canvNLL.cd(1)

lines = []
for mass in range(3,12):
    lines.append(TLine(0., 1.+(mass-3.)*0.1, 30., 1.+(mass-3.)*0.1))
    CMS.cmsDrawLine(lines[mass-3], lcolor=mass-2, lstyle=2)
    NLLresList[mass-3].Draw("E1, same")

leg2bNLL.Draw()

CMS.fixOverlay()

CMS.SaveCanvas(canvNLL, "NLLresiduals_Wprime"+binS+"2_"+str(year)+".pdf")

