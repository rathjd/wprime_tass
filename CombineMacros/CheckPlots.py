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
print("assembling results for",binS,"in year",year)
inOrigin = TFile(binS+"1_"+str(year)+"/SimpleShapes_Wprime"+binS+"1_"+str(year)+".root","READ")
print("origin of correction",binS+"1_"+str(year)+"/SimpleShapes_Wprime"+binS+"1_"+str(year)+".root")
inSF     = TFile(binS+"2_"+str(year)+"/SF_Bin"+binS+"1_"+str(year)+".root","READ")
print("SF file of origin",binS+"2_"+str(year)+"/SF_Bin"+binS+"1_"+str(year)+".root")
inSF2    = TFile(binS+"3_"+str(year)+"/SF_Bin"+binS+"2_"+str(year)+".root","READ")
print("SF file of target region",binS+"3_"+str(year)+"/SF_Bin"+binS+"2_"+str(year)+".root")
inResult = TFile(binS+"2_"+str(year)+"/SimpleShapes_Wprime"+binS+"2_"+str(year)+".root","READ")
print("result file of validation region",binS+"2_"+str(year)+"/SimpleShapes_Wprime"+binS+"2_"+str(year)+".root")
inTwoD3 = TFile(binS+"3_"+str(year)+"/TwoD_SimpleShapes_Wprime"+binS+"3_"+str(year)+".root","READ")
if JetMult == 6:
    inTwoD4 = TFile(binS+"4_"+str(year)+"/TwoD_SimpleShapes_Wprime"+binS+"4_"+str(year)+".root","READ")

#extract data histograms
Data1b = inOrigin.Get("ST_data_obs_Wprime"+binS+"1_"+str(year)+"_")
print("1b data","ST_data_obs_Wprime"+binS+"1_"+str(year)+"_")
Data1b.Scale(1.,"width")
Data2b = inResult.Get("ST_data_obs_Wprime"+binS+"2_"+str(year)+"_")
print("2b data","ST_data_obs_Wprime"+binS+"2_"+str(year)+"_")
Data2b.Scale(1.,"width")
Fit1b = inSF.Get("SF_")

#make empty stacks
Stack1b    = THStack("Stack_"+binS+"1_"+str(year),"")
Stack2b    = THStack("Stack_"+binS+"2_"+str(year),"")
Stack2braw = THStack("Stackraw_"+binS+"2_"+str(year),"")

#configure legends
leg1b    = CMS.cmsLeg(0.51,0.89-0.05*6, 0.8, 0.89, textSize=0.05)
leg2b    = CMS.cmsLeg(0.51,0.89-0.05*6, 0.8, 0.89, textSize=0.05)
leg2braw = CMS.cmsLeg(0.51,0.89-0.05*6, 0.8, 0.89, textSize=0.05)
leg2bNLL = CMS.cmsLeg(0.46,0.59-0.05*9, 0.8, 0.59, textSize=0.05)

if LeptonFlav == 1:
    if JetMult == 5:
        CMS.cmsHeader(leg1b, "#mu + 5 jets and 1 b tag", textSize=0.05)
        CMS.cmsHeader(leg2b, "#mu + 5 jets and 2 b tags rew.", textSize=0.05)
        CMS.cmsHeader(leg2braw, "#mu + 5 jets and 2 b tags", textSize=0.05)
        CMS.cmsHeader(leg2bNLL, "#mu + 5 jets and 2 b tags", textSize=0.05)
    elif JetMult == 6:
        CMS.cmsHeader(leg1b, "#mu + 6 jets and 1 b tag", textSize=0.05)
        CMS.cmsHeader(leg2b, "#mu + 6 jets and 2 b tags rew.", textSize=0.05)
        CMS.cmsHeader(leg2braw, "#mu + 6 jets and 2 b tags", textSize=0.05)
        CMS.cmsHeader(leg2bNLL, "#mu + 6 jets and 2 b tags", textSize=0.05)
elif LeptonFlav == 2:
    if JetMult == 5:
        CMS.cmsHeader(leg1b, "e + 5 jets and 1 b tag", textSize=0.05)
        CMS.cmsHeader(leg2b, "e + 5 jets and 2 b tags rew.", textSize=0.05)
        CMS.cmsHeader(leg2braw, "e + 5 jets and 2 b tags", textSize=0.05)
        CMS.cmsHeader(leg2bNLL, "e + 5 jets and 2 b tags", textSize=0.05)
    elif JetMult == 6:
        CMS.cmsHeader(leg1b, "e + 6 jets and 1 b tag", textSize=0.05)
        CMS.cmsHeader(leg2b, "e + 6 jets and 2 b tags rew.", textSize=0.05)
        CMS.cmsHeader(leg2braw, "e + 6 jets and 2 b tags", textSize=0.05)
        CMS.cmsHeader(leg2bNLL, "e + 6 jets and 2 b tags", textSize=0.05)


#determine number of bins
BinOrigin = inOrigin.Get("ST_ttbar_Wprime"+binS+"1_"+str(year)+"_")
print("bin origin","ST_ttbar_Wprime"+binS+"1_"+str(year)+"_")

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
    print("Background part for 1b","ST_"+background[0]+"_Wprime"+binS+"1_"+str(year)+"_")

    BgrPart2braw = inResult.Get("ST_"+background[0]+"_Wprime"+binS+"2_"+str(year)+"_")
    print("Background part for 2b raw","ST_"+background[0]+"_Wprime"+binS+"2_"+str(year)+"_")
    
    if background[1]==2:
        BgrPart2b = inResult.Get("STrew_"+background[0]+"_Wprime"+binS+"2_"+str(year)+"_")
        print("Background part for 2b reweighted","STrew_"+background[0]+"_Wprime"+binS+"2_"+str(year)+"_")
        BgrTotal1b    = BgrPart1b.Clone("BgrTotal1b")
        BgrTotal2b    = BgrPart2b.Clone("BgrTotal2b")
        BgrTotal2braw = BgrPart2braw.Clone("BgrTotal2braw")
    else:
        BgrPart2b = inResult.Get("ST_"+background[0]+"_Wprime"+binS+"2_"+str(year)+"_")
        print("Background part for 2b reweighted","ST_"+background[0]+"_Wprime"+binS+"2_"+str(year)+"_")
        BgrTotal1b.Add(BgrPart1b)
        BgrTotal2b.Add(BgrPart2b)
        BgrTotal2braw.Add(BgrPart2braw)

    BgrPart1b.Scale(1.,"width")
    BgrPart2b.Scale(1.,"width")
    BgrPart2braw.Scale(1.,"width")

    Bgr1b[background[0]] = BgrPart1b
    Bgr2b[background[0]] = BgrPart2b
    Bgr2braw[background[0]] = BgrPart2braw

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

#scale all totals
BgrTotal1b.Scale(1.,"width")
BgrTotal2b.Scale(1.,"width")
BgrTotal2braw.Scale(1.,"width")

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
Ratio2b = Data2b.Clone("Ratio2b")
Ratio2b.Divide(BgrTotal2b)

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

#print("further checks for reweighted 2b region")
#print("max Stack =",Stack2b.GetMaximum())
#print("max Total =",BgrTotal2b.GetMaximum())

CMS.fixOverlay()

canv2b.cd(2)

#for bins in range(0, BgrTotal2b.GetNbinsX()+1):
#    print("content",bins,Data2b.GetBinContent(bins+1),BgrTotal2b.GetBinContent(bins+1),Ratio2b.GetBinContent(bins+1))
#    print("width",bins,BgrTotal2b.GetBinWidth(bins+1),Data2b.GetBinWidth(bins+1))

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
canvNLL = CMS.cmsCanvas("NLL_"+binS+"_"+str(year)+"_2b",0,30,-1.5,2.5,"-log(L)", "Data/Pred. after S_{T} reweight", square=CMS.kSquare, extraSpace=0.1, iPos=0)
canvNLL.cd(1)

lines = []
for mass in range(3,12):
    lines.append(TLine(0., 1.+(mass-3.)*0.1, 30., 1.+(mass-3.)*0.1))
    CMS.cmsDrawLine(lines[mass-3], lcolor=mass-2, lstyle=2)
    NLLresList[mass-3].Draw("E1, same")

leg2bNLL.Draw()

CMS.fixOverlay()

CMS.SaveCanvas(canvNLL, "NLLresiduals_Wprime"+binS+"2_"+str(year)+".pdf")

#make NLL correlation plots per masspoint and background
for background in backgrounds:
    for mass in range(3,12):
        canvNLLcorr = CMS.cmsCanvas("NLL_NLLnoB_"+background[0]+"_"+binS+"3_"+str(year)+"_M"+str(mass*100), 0, 30, 0, 30, "-log(L)_{!b}", "-log(L)", square=CMS.kSquare, extraSpace=0.01, iPos=0, with_z_axis=True, scaleLumi=0.8)
        canvNLLcorr.cd(1)
        NLLcorr = inTwoD3.Get("NegLogLnoBvsNegLogL_"+background[0]+"_Wprime"+binS+"3_"+str(year)+"_M"+str(mass*100)+"_")
        #suppress negative bins
        for x in range(0, 52):
            for y in range(0, 52):
                if NLLcorr.GetBinContent(x+1, y+1) < 0:
                    NLLcorr.SetBinContent(x+1, y+1, 0.)
        NLLcorr.GetZaxis().SetTitleOffset(1.4)
        NLLcorr.Draw("same colz")
        if NLLcorr.Integral() > 0:
            CMS.SetAlternative2DColor(NLLcorr, CMS.cmsStyle)
            CMS.UpdatePalettePosition(NLLcorr, canvNLLcorr)

            CMS.SaveCanvas(canvNLLcorr, "NLL_NLLnoB_"+background[0]+"_"+binS+"3_"+str(year)+"_M"+str(mass*100)+".pdf")
        else:
            print("NegLogLnoBvsNegLogL_"+background[0]+"_Wprime"+binS+"3_"+str(year)+"_M"+str(mass*100)+"_","is empty")

        if JetMult==6:
            canvNLLcorr4 = CMS.cmsCanvas("NLL_NLLnoB_"+background[0]+"_"+binS+"4_"+str(year)+"_M"+str(mass*100), 0, 30, 0, 30, "-log(L)_{!b}", "-log(L)", square=CMS.kSquare, extraSpace=0.01, iPos=0, with_z_axis=True, scaleLumi=0.8)
            canvNLLcorr4.cd(1)
            NLLcorr4 = inTwoD4.Get("NegLogLnoBvsNegLogL_"+background[0]+"_Wprime"+binS+"4_"+str(year)+"_M"+str(mass*100)+"_")
            #suppress negative bins
            for x in range(0, 23):
                for y in range(0, 23):
                    if NLLcorr4.GetBinContent(x+1, y+1) < 0:
                        NLLcorr4.SetBinContent(x+1, y+1, 0.)
            NLLcorr4.GetZaxis().SetTitleOffset(1.4)
            NLLcorr4.Draw("same colz")
            if NLLcorr4.Integral() > 0:
                CMS.SetAlternative2DColor(NLLcorr4, CMS.cmsStyle)
                CMS.UpdatePalettePosition(NLLcorr4, canvNLLcorr4)

                CMS.SaveCanvas(canvNLLcorr4, "NLL_NLLnoB_"+background[0]+"_"+binS+"4_"+str(year)+"_M"+str(mass*100)+".pdf")
            else:
                print("NegLogLnoBvsNegLogL_"+background[0]+"_Wprime"+binS+"4_"+str(year)+"_M"+str(mass*100)+"_","is empty")

#make NLL stack plots per masspoint
for bmult in range(3,JetMult-1):
    #cetermine NLL bin count
    if bmult == 3:
        nbinsX=52
    else:
        nbinsX=23
    for mass in range(3,12):
        #configure containers
        Stack = THStack("Stack_"+binS+str(bmult)+"_"+str(year)+"_M"+str(mass*100),"")
        leg = CMS.cmsLeg(0.45,0.89-7*0.05, 0.8, 0.89, textSize=0.05)
        
        if LeptonFlav == 1:
            lepStr = "#mu "
        else:
            lepStr = "e "
        CMS.cmsHeader(leg, lepStr + str(JetMult) + " jets and "+str(bmult)+" b tags, m_{W'}="+str(mass*100), textSize=0.05)

        #declare background dictionary
        Bgr = {}
        BgrSystUp = [0] * nbinsX
        BgrSystDown = [0] * nbinsX
        SigSystUp = [0] * nbinsX
        SigSystDown = [0] * nbinsX

        #loop over backgrounds
        for background in backgrounds:
        
            #load the background histograms
            if bmult == 3:
                inBgr2D = inTwoD3.Get("FitMass2D_"+background[0]+"_Wprime"+binS+str(bmult)+"_"+str(year)+"_M"+str(mass*100)+"_")
                inBgr = inBgr2D.ProjectionY("NLL_"+background[0]+"_Wprime"+binS+str(bmult)+"_"+str(year)+"_M"+str(mass*100)+"_")
            else:
                inBgr2D = inTwoD4.Get("FitMass2D_"+background[0]+"_Wprime"+binS+str(bmult)+"_"+str(year)+"_M"+str(mass*100)+"_")
                inBgr = inBgr2D.ProjectionY("NLL_"+background[0]+"_Wprime"+binS+str(bmult)+"_"+str(year)+"_M"+str(mass*100)+"_")

            #remove negative bin values
            for binN in range(0,nbinsX):
                if inBgr.GetBinContent(binN+1) < 0.:
                    inBgr.SetBinContent(binN+1, 0.)

            if background[1] == 2:
                BgrTotal = inBgr.Clone("BgrTotal_"+binS+str(bmult)+"_"+str(year)+"_M"+str(mass*100))
            else:
                BgrTotal.Add(inBgr)

            inBgr.Scale(1.,"width")

            Bgr[background[0]] = inBgr

            #stat uncertainty per bin
            for binN in range(0, nbinsX):
                BgrSystUp[binN] += pow(inBgr.GetBinError(binN+1), 2)
                BgrSystDown[binN] += pow(inBgr.GetBinError(binN+1), 2)

            #syst uncertainties
            for syst in baseSystematics:
                if bmult == 3:
                    Up2D = inTwoD3.Get("FitMass2D_"+background[0]+"_Wprime"+binS+str(bmult)+"_"+str(year)+"_M"+str(mass*100)+"_"+syst+"Up")
                    Down2D = inTwoD3.Get("FitMass2D_"+background[0]+"_Wprime"+binS+str(bmult)+"_"+str(year)+"_M"+str(mass*100)+"_"+syst+"Down")
                else:
                    Up2D = inTwoD4.Get("FitMass2D_"+background[0]+"_Wprime"+binS+str(bmult)+"_"+str(year)+"_M"+str(mass*100)+"_"+syst+"Up")
                    Down2D = inTwoD4.Get("FitMass2D_"+background[0]+"_Wprime"+binS+str(bmult)+"_"+str(year)+"_M"+str(mass*100)+"_"+syst+"Down")

                Up = Up2D.ProjectionY("NLL_"+background[0]+"_Wprime"+binS+str(bmult)+"_"+str(year)+"_M"+str(mass*100)+"_"+syst+"Up")
                Down = Down2D.ProjectionY("NLL_"+background[0]+"_Wprime"+binS+str(bmult)+"_"+str(year)+"_M"+str(mass*100)+"_"+syst+"Down")

                #remove negative bin values
                for binN in range(0,nbinsX):
                    if Up.GetBinContent(binN+1) < 0.:
                        Up.SetBinContent(binN+1, 0.)
                    if Down.GetBinContent(binN+1) < 0.:
                        Down.SetBinContent(binN+1, 0.)

                Up.Scale(1.,"width")
                Down.Scale(1.,"width")

                for binN in range(0, nbinsX):
                    BgrSystUp[binN] += pow(max(Up.GetBinContent(binN+1) - inBgr.GetBinContent(binN+1),
                                         Down.GetBinContent(binN+1) - inBgr.GetBinContent(binN+1),
                                         0.), 2)
                    BgrSystDown[binN] += pow(min(Up.GetBinContent(binN+1) - inBgr.GetBinContent(binN+1),
                                         Down.GetBinContent(binN+1) - inBgr.GetBinContent(binN+1),
                                         0.), 2)

        BgrTotal.Scale(1.,"width")

        #load the appropriate signal histogram
        if bmult == 3:
            inSig2D = inTwoD3.Get("FitMass2D_M"+str(mass*100)+"_Wprime"+binS+str(bmult)+"_"+str(year)+"_M"+str(mass*100)+"_")
            inSig = inSig2D.ProjectionY("NLL_M"+str(mass*100)+"_Wprime"+binS+str(bmult)+"_"+str(year)+"_M"+str(mass*100)+"_")
        else:
            inSig2D = inTwoD4.Get("FitMass2D_M"+str(mass*100)+"_Wprime"+binS+str(bmult)+"_"+str(year)+"_M"+str(mass*100)+"_")
            inSig = inSig2D.ProjectionY("NLL_M"+str(mass*100)+"_Wprime"+binS+str(bmult)+"_"+str(year)+"_M"+str(mass*100)+"_")
        inSig.Scale(1.,"width")

        #stat uncertainty per bin
        for binN in range(0, nbinsX):
            SigSystUp[binN] += pow(inSig.GetBinError(binN+1), 2)
            SigSystDown[binN] += pow(inSig.GetBinError(binN+1), 2)

        #syst uncertainty per bin
        for syst in baseSystematics:
            if bmult == 3:
                    Up2D = inTwoD3.Get("FitMass2D_M"+str(mass*100)+"_Wprime"+binS+str(bmult)+"_"+str(year)+"_M"+str(mass*100)+"_"+syst+"Up")
                    Down2D = inTwoD3.Get("FitMass2D_M"+str(mass*100)+"_Wprime"+binS+str(bmult)+"_"+str(year)+"_M"+str(mass*100)+"_"+syst+"Down")
            else:
                Up2D = inTwoD4.Get("FitMass2D_M"+str(mass*100)+"_Wprime"+binS+str(bmult)+"_"+str(year)+"_M"+str(mass*100)+"_"+syst+"Up")
                Down2D = inTwoD4.Get("FitMass2D_M"+str(mass*100)+"_Wprime"+binS+str(bmult)+"_"+str(year)+"_M"+str(mass*100)+"_"+syst+"Down")

            Up = Up2D.ProjectionY("NLL_M"+str(mass*100)+"_Wprime"+binS+str(bmult)+"_"+str(year)+"_M"+str(mass*100)+"_"+syst+"Up")
            Down = Down2D.ProjectionY("NLL_M"+str(mass*100)+"_Wprime"+binS+str(bmult)+"_"+str(year)+"_M"+str(mass*100)+"_"+syst+"Down")
            Up.Scale(1.,"width")
            Down.Scale(1.,"width")

            for binN in range(0, nbinsX):
                SigSystUp[binN] += pow(max(Up.GetBinContent(binN+1) - inSig.GetBinContent(binN+1),
                                     Down.GetBinContent(binN+1) - inSig.GetBinContent(binN+1),
                                     0.), 2)
                SigSystDown[binN] += pow(min(Up.GetBinContent(binN+1) - inSig.GetBinContent(binN+1),
                                     Down.GetBinContent(binN+1) - inSig.GetBinContent(binN+1),
                                     0.), 2)

        #get all uncertainty values
        for binN in range(0, nbinsX):
            BgrSystUp[binN] = math.sqrt(BgrSystUp[binN])
            BgrSystDown[binN] = math.sqrt(BgrSystDown[binN])
            SigSystUp[binN] = math.sqrt(SigSystUp[binN])
            SigSystDown[binN] = math.sqrt(SigSystDown[binN])

        #fill error bands
        ErrX, ErrBy, ErrSy, ErrR = array('d'), array('d'), array('d'), array('d')
        for binN in range(0, nbinsX): #upper band
            ErrX.append(Up.GetBinLowEdge(binN+1))
            ErrBy.append(BgrSystUp[binN]+BgrTotal.GetBinContent(binN+1))
            ErrSy.append(SigSystUp[binN]+inSig.GetBinContent(binN+1))
            ErrR.append(BgrTotal.GetBinContent(binN+1) and BgrSystUp[binN]/BgrTotal.GetBinContent(binN+1)+1. or 0.)

            ErrX.append(Up.GetBinLowEdge(binN+2))
            ErrBy.append(BgrSystUp[binN]+BgrTotal.GetBinContent(binN+1))
            ErrSy.append(SigSystUp[binN]+inSig.GetBinContent(binN+1))
            ErrR.append(BgrTotal.GetBinContent(binN+1) and BgrSystUp[binN]/BgrTotal.GetBinContent(binN+1)+1. or 0.)
        for binN in range(nbinsX-1,-2,-1): #lower band
            ErrX.append(Up.GetBinLowEdge(binN+2))
            ErrBy.append(-BgrSystDown[binN]+BgrTotal.GetBinContent(binN+1))
            ErrSy.append(-SigSystDown[binN]+inSig.GetBinContent(binN+1))
            ErrR.append(BgrTotal.GetBinContent(binN+1) and -BgrSystDown[binN]/BgrTotal.GetBinContent(binN+1)+1. or 0.)

            ErrX.append(Up.GetBinLowEdge(binN+1))
            ErrBy.append(-BgrSystDown[binN]+BgrTotal.GetBinContent(binN+1))
            ErrSy.append(-SigSystDown[binN]+inSig.GetBinContent(binN+1))
            ErrR.append(BgrTotal.GetBinContent(binN+1) and -BgrSystDown[binN]/BgrTotal.GetBinContent(binN+1)+1. or 0.)

        maxVal = max(BgrTotal.GetMaximum()*1.2, inSig.GetMaximum()*1.2)
        canvNLL = CMS.cmsDiCanvas("NLL_"+binS+str(bmult)+"_"+str(year)+"_M"+str(mass*100),0,30,0,maxVal,0.5,1.5,"-log(L)", "Events/bin width", "Data/Pred.", square=CMS.kSquare, extraSpace=0.1, iPos=0)
        canvNLL.cd(1)

        #leg.AddEntry(Data2b, "Data", "lp")
        CMS.cmsDrawStack(Stack, leg, Bgr)
        grErrBgr = TGraph(len(ErrX), ErrX, ErrBy)
        grErrBgr.SetFillColor(17)
        grErrBgr.SetFillStyle(3008)
        grErrBgr.Draw("F, same")
        #CMS.cmsDraw(Data2b, "P", mcolor=1)
        inSig.SetLineColor(8)
        inSig.SetLineWidth(3)
        inSig.SetLineStyle(2)
        inSig.SetMarkerStyle(21)
        inSig.SetMarkerColor(2)
        inSig.SetFillColor(8)
        inSig.SetFillStyle(3008)
        grErrSig = TGraph(len(ErrX), ErrX, ErrSy)
        grErrSig.SetFillColor(8)
        grErrSig.SetFillStyle(3008)
        grErrSig.Draw("F, same")

        inSig.Draw("p, same")

        leg.AddEntry(inSig,"m_{W'}="+str(mass*100)+" GeV", "pf")
        leg.AddEntry(grErrBgr, "stat.+syst. unc.", "f")

        leg.Draw()

        CMS.fixOverlay()

        canvNLL.cd(2)

        #Ratio2braw = Data2b.Clone("Ratio2braw")
        #Ratio2braw.Divide(BgrTotal2braw)

        grErrBgrR = TGraph(len(ErrX), ErrX, ErrR)
        grErrBgrR.SetFillColor(17)
        grErrBgrR.SetFillStyle(3008)
        grErrBgrR.Draw("F")

        #CMS.cmsDraw(Ratio2braw, "P", mcolor=1)

        ref_lineraw = TLine(0, 1, 30, 1)
        CMS.cmsDrawLine(ref_lineraw, lcolor=1, lstyle=3)

        CMS.SaveCanvas(canvNLL, "NLL_"+binS+str(bmult)+"_"+str(year)+"_M"+str(mass*100)+".pdf")

