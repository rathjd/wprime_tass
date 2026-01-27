import os,sys

from ROOT import TH1F, TFile, THStack, TRatioPlot, TCanvas, TLegend, TLine, TF1, TMatrixD, TFitResultPtr, TGraph, TColor, TPaveStats
import cmsstyle as CMS
from array import array
import math

#settings for what to plot
LeptonFlav = 1
JetMult = 5
year = "2017"

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
    if (sys.argv[3]).find("20") > -1:
        year = sys.argv[3]
        print("year set to "+year)
except:
    print("year defaults to "+year)

#CMS style setup
if year == "2016":
    CMS.SetLumi("16.8")
elif year == "2016apv":
    CMS.SetLumi("19.52")
elif year == "2017":
    CMS.SetLumi("41.5")
elif year == "2018":
    CMS.SetLumi("59.8")
CMS.SetEnergy("13")

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

backgroundsExt = backgrounds
backgroundsExt.append(["qcd", 6])

signals = [["M300", 6],
           ["M600", 7],
           ["M900", 8]]

B2Gn = "25008"
baseSystematics = [
                   #uncertainties on object variations other than electron scale
                   "CMS_res_e"             ,
                   "CMS_scale_j_"           +year,
                   "CMS_res_j_"             +year,

                   #event weight variations
                   "CMS_eff_e_trigger"     ,
                   "CMS_eff_e_reco"        ,
                   "CMS_eff_e"             ,
                   "CMS_eff_m_trigger_"     +year,
                   "CMS_eff_m_id_"          +year,
                   "CMS_eff_m_iso_"         +year,
                   "CMS_btag_light"              ,
                   "CMS_btag_heavy"              ,
                   "CMS_btag_light_"        +year,
                   "CMS_btag_heavy_"        +year,
                   "CMS_eff_j_PUJET_id_"    +year,
                   "CMS_l1_ecal_prefiring_" +year,
                   "CMS_pileup"                  ,
                   "ps_isr"                      ,
                   "ps_fsr"                      ,

                   #uncertainties on normalization
                   #"lumi_13TeV_correlated"       ,
                   #"lumi_13TeV_1718"             ,
                   #"lumi_"                  +year,
                   #"CMS_eff_e_HLTzvtx_17"
                   ]

#make list of sample-dependent systematics
extendSystematics = []
for bgr in backgrounds:
    #if not bgr[0].find("single_top") > -1:
    extendSystematics.append("pdf_B2G"+B2Gn+"_envelope_"+bgr[0])
    extendSystematics.append("QCDscale_ren_"+bgr[0])
    extendSystematics.append("QCDscale_fac_"+bgr[0])
baseSystematics.extend(extendSystematics)

SRsystematics = [  "CMS_scale_e"            ,
                   "CMS_B2G"+B2Gn+"_STfit_"+year+"_"+binString[0:3]+"1",
                   "CMS_B2G"+B2Gn+"_STfitFunc_"+year+"_"+binString[0:3]+"1",
                   "CMS_B2G"+B2Gn+"_STfitQCD"]

#input files
eospath = "/eos/cms/store/group/phys_b2g/wprime/temp/" 
binS = str(LeptonFlav)+"1"+str(JetMult)
print("assembling results for",binS,"in year",year)
inOrigin = TFile(eospath + binS + "1_" + year + "/SimpleShapes_Wprime" + binS + "1_" + year + ".root","READ")
print("origin of correction",binS+"1_"+year+"/SimpleShapes_Wprime"+binS+"1_"+year+".root")
inSF     = TFile(eospath + binS + "2_" + year + "/SF_Bin" + binS + "1_" + year + ".root","READ")
print("SF file of origin",binS+"2_"+year+"/SF_Bin"+binS+"1_"+year+".root")
inResult = TFile(eospath + binS + "2_" + year + "/SimpleShapes_Wprime" + binS + "2_" + year + ".root","READ")
print("result file of validation region",binS+"2_"+year+"/SimpleShapes_Wprime"+binS+"2_"+year+".root")

#extract data histograms
Data1b = inOrigin.Get("ST_data_obs_Wprime"+binS+"1_"+year+"_")
print("1b data","ST_data_obs_Wprime"+binS+"1_"+year+"_")
Data1b.Scale(1.,"width")
Data2b = inResult.Get("ST_data_obs_Wprime"+binS+"2_"+year+"_")
print("2b data","ST_data_obs_Wprime"+binS+"2_"+year+"_")
Data2b.Scale(1.,"width")
Fit1b = inSF.Get("SF_")

#make empty stacks
Stack1b    = THStack("Stack_"+binS+"1_"+year,"")
Stack2b    = THStack("Stack_"+binS+"2_"+year,"")
Stack2braw = THStack("Stackraw_"+binS+"2_"+year,"")

#configure legends
leg1b    = CMS.cmsLeg(0.51,0.89-0.05*7, 0.8, 0.89, textSize=0.05)
leg2b    = CMS.cmsLeg(0.51,0.89-0.05*6, 0.8, 0.89, textSize=0.05)
leg2braw = CMS.cmsLeg(0.51,0.89-0.05*6, 0.8, 0.89, textSize=0.05)

if LeptonFlav == 1:
    if JetMult == 5:
        CMS.cmsHeader(leg1b, "#mu + 5 jets and 1 b tag", textSize=0.05)
        CMS.cmsHeader(leg2b, "#mu + 5 jets and 2 b tags rew.", textSize=0.05)
        CMS.cmsHeader(leg2braw, "#mu + 5 jets and 2 b tags", textSize=0.05)
    elif JetMult == 6:
        CMS.cmsHeader(leg1b, "#mu + 6 jets and 1 b tag", textSize=0.05)
        CMS.cmsHeader(leg2b, "#mu + 6 jets and 2 b tags rew.", textSize=0.05)
        CMS.cmsHeader(leg2braw, "#mu + 6 jets and 2 b tags", textSize=0.05)
elif LeptonFlav == 2:
    if JetMult == 5:
        CMS.cmsHeader(leg1b, "e + 5 jets and 1 b tag", textSize=0.05)
        CMS.cmsHeader(leg2b, "e + 5 jets and 2 b tags rew.", textSize=0.05)
        CMS.cmsHeader(leg2braw, "e + 5 jets and 2 b tags", textSize=0.05)
    elif JetMult == 6:
        CMS.cmsHeader(leg1b, "e + 6 jets and 1 b tag", textSize=0.05)
        CMS.cmsHeader(leg2b, "e + 6 jets and 2 b tags rew.", textSize=0.05)
        CMS.cmsHeader(leg2braw, "e + 6 jets and 2 b tags", textSize=0.05)


#determine number of bins
BinOrigin = inOrigin.Get("ST_ttbar_Wprime"+binS+"1_"+year+"_")
print("bin origin","ST_ttbar_Wprime"+binS+"1_"+year+"_")

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

testHistIn = inResult.Get("ST_ttbar_Wprime"+binS+"2_"+year+"_")
testHist = testHistIn.Clone("testHist")
testHist.Scale(0.)

for background in backgroundsExt:
    BgrPart1b = inOrigin.Get("ST_"+background[0]+"_Wprime"+binS+"1_"+year+"_")
    print("Background part for 1b","ST_"+background[0]+"_Wprime"+binS+"1_"+year+"_")

    if not background[1]==6:
        BgrPart2braw = inResult.Get("ST_"+background[0]+"_Wprime"+binS+"2_"+year+"_")
        print("Background part for 2b raw","ST_"+background[0]+"_Wprime"+binS+"2_"+year+"_")

    if background[1]==2:
        BgrPart2b = inResult.Get("STrew_"+background[0]+"_Wprime"+binS+"2_"+year+"_")
        print("Background part for 2b reweighted","STrew_"+background[0]+"_Wprime"+binS+"2_"+year+"_")
        BgrTotal1b    = BgrPart1b.Clone("BgrTotal1b")
        BgrTotal2b    = BgrPart2b.Clone("BgrTotal2b")
        BgrTotal2braw = BgrPart2braw.Clone("BgrTotal2braw")
    else:
        if not background[1]==6:
            BgrPart2b = inResult.Get("ST_"+background[0]+"_Wprime"+binS+"2_"+year+"_")
            print("Background part for 2b","ST_"+background[0]+"_Wprime"+binS+"2_"+year+"_")
            BgrTotal2b.Add(BgrPart2b)
            BgrTotal2braw.Add(BgrPart2braw)
        BgrTotal1b.Add(BgrPart1b)

    BgrPart1b.Scale(1.,"width")

    if not background[1]==6:
        BgrPart2b.Scale(1.,"width")
        if not BgrPart2b.GetName() == BgrPart2braw.GetName():
            BgrPart2braw.Scale(1.,"width")

    Bgr1b[background[0]] = BgrPart1b

    if not background[1]==6:
        Bgr2b[background[0]] = BgrPart2b
        Bgr2braw[background[0]] = BgrPart2braw

    #statistical uncertainty of a background per bin
    for bin in range(0,BgrPart1b.GetNbinsX()):
        Bgr1bSystUp[bin] += pow(BgrPart1b.GetBinError(bin+1),2)
        Bgr1bSystDown[bin] += pow(BgrPart1b.GetBinError(bin+1),2)

        if not background[1]==6:
            Bgr2bSystUp[bin] += pow(BgrPart2b.GetBinError(bin+1),2)
            Bgr2bSystDown[bin] += pow(BgrPart2b.GetBinError(bin+1),2)
            Bgr2brawSystUp[bin] += pow(BgrPart2braw.GetBinError(bin+1),2)
            Bgr2brawSystDown[bin] += pow(BgrPart2braw.GetBinError(bin+1),2)

    #determine the uncertainties per bin
    for syst in baseSystematics:

        #special dispensation to remove mismatched extended systematics from consideration
        if syst.find("pdf") > -1 or syst.find("QCDscale") > -1:
            if syst.find(background[0]) < 0:
                continue


        STstr = "ST_"
        if background[1]==2:
            STstr = "STrew_"
        Up1b      = inOrigin.Get("ST_"+background[0]+"_Wprime"+binS+"1_"+year+"_"+syst+"Up")
        Down1b    = inOrigin.Get("ST_"+background[0]+"_Wprime"+binS+"1_"+year+"_"+syst+"Down")

        if not background[1]==6:
            Up2b      = inResult.Get(STstr+background[0]+"_Wprime"+binS+"2_"+year+"_"+syst+"Up")
            Down2b    = inResult.Get(STstr+background[0]+"_Wprime"+binS+"2_"+year+"_"+syst+"Down")
            Up2braw   = inResult.Get("ST_"+background[0]+"_Wprime"+binS+"2_"+year+"_"+syst+"Up")
            Down2braw = inResult.Get("ST_"+background[0]+"_Wprime"+binS+"2_"+year+"_"+syst+"Down")
        print("ST_"+background[0]+"_Wprime"+binS+"1_"+year+"_"+syst+"Up")
        Up1b.Scale(1.,"width")
        Down1b.Scale(1.,"width")

        if not background[1]==6:
            Up2b.Scale(1.,"width")
            Down2b.Scale(1.,"width")
            if not Up2b.GetName() == Up2braw.GetName():
                Up2braw.Scale(1.,"width")
                Down2braw.Scale(1.,"width")

        for bin in range(0,BgrPart1b.GetNbinsX()):
            Bgr1bSystUp[bin] += pow(max(Up1b.GetBinContent(bin+1) - BgrPart1b.GetBinContent(bin+1),
                                    Down1b.GetBinContent(bin+1) - BgrPart1b.GetBinContent(bin+1),
                                    0.), 2)
            Bgr1bSystDown[bin] += pow(min(Up1b.GetBinContent(bin+1) - BgrPart1b.GetBinContent(bin+1),
                                    Down1b.GetBinContent(bin+1) - BgrPart1b.GetBinContent(bin+1),
                                    0.), 2)
            if not background[1]==6:
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
#scale all totals
BgrTotal1b.Scale(1.,"width")
BgrTotal2b.Scale(1.,"width")
BgrTotal2braw.Scale(1.,"width")

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
for bin in range(len(Bgr1bSystUp)-1,-1,-1): #lower band
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
canv1b = CMS.cmsDiCanvas("STSF_"+binS+"_"+year+"_1b",STstart,STend,0,STmax1b,0.5,1.5,"S_{T} [GeV/c]", "Events/bin width", "Data/Pred.", square=CMS.kSquare, extraSpace=0.1, iPos=0)
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

CMS.SaveCanvas(canv1b, "ST_Wprime"+binS+"1_"+year+".pdf")

#make reweighted 2 b-tag region plot
Ratio2b = Data2b.Clone("Ratio2b")
Ratio2b.Divide(BgrTotal2b)

STmax2b = max(Data2b.GetMaximum(), BgrTotal2b.GetMaximum())*1.1
canv2b = CMS.cmsDiCanvas("STSFrew_"+binS+"_"+year+"_2b",STstart,STend,0,STmax2b,0.5,1.5,"S_{T} [GeV/c]", "Events/bin width", "Data/Pred.", square=CMS.kSquare, extraSpace=0.1, iPos=0)
canv2b.cd(1)

leg2b.AddEntry(Data2b, "Data", "lp")
CMS.cmsDrawStack(Stack2b, leg2b, Bgr2b)
grErr2b = TGraph(len(Err2bX), Err2bX, Err2bY)
grErr2b.SetFillColor(17)
grErr2b.SetFillStyle(3008)
grErr2b.Draw("F, same")
CMS.cmsDraw(Data2b, "P", mcolor=1)
leg2b.Draw()

print("further checks for reweighted 2b region")
print("max Stack =",Stack2b.GetMaximum())
print("max Total =",BgrTotal2b.GetMaximum())

CMS.fixOverlay()

canv2b.cd(2)

grErr2bR = TGraph(len(Err2bX), Err2bX, Err2bR)
grErr2bR.SetFillColor(17)
grErr2bR.SetFillStyle(3008)
grErr2bR.Draw("F")

CMS.cmsDraw(Ratio2b, "P", mcolor=1)

ref_line = TLine(STstart, 1, STend, 1)
CMS.cmsDrawLine(ref_line, lcolor=1, lstyle=3)

CMS.SaveCanvas(canv2b, "STrew_Wprime"+binS+"2_"+year+".pdf")

#make raw 2 b-tag region plot
STmax2braw = max(Data2b.GetMaximum(), BgrTotal2braw.GetMaximum())*1.1
canv2braw = CMS.cmsDiCanvas("STSF_"+binS+"_"+year+"_2b",STstart,STend,0,STmax2braw,0.5,1.5,"S_{T} [GeV/c]", "Events/bin width", "Data/Pred.", square=CMS.kSquare, extraSpace=0.1, iPos=0)
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

CMS.SaveCanvas(canv2braw, "ST_Wprime"+binS+"2_"+year+".pdf")

#make 1b-tag ST correction fit plot
SF = inSF.Get("SF_")
if JetMult == 5:
  SFfit = TF1("SFfit","[0]/x/x+[1]/x+[2]+[3]*x+[4]*x*x", STstart, STend)
  SFfit.SetParNames("p0*x^{-2}","p1*x^{-1}","p2*x^{0}","p3*x^{1}","p4*x^{2}")
else:
    SFfit = TF1("SFfit","[0]/x/x+[1]/x+[2]+[3]*x", STstart, STend)
    SFfit.SetParNames("p0*x^{-2}","p1*x^{-1}","p2*x^{0}","p3*x^{1}")
fr = SF.Fit(SFfit,"SRF")
cov = fr.GetCovarianceMatrix()
if JetMult == 5:
  SFfitUp = TF1("SFfitUp", "TMath::Sqrt([0]*pow(x,-4) + [1]*pow(x,-3) + [2]*pow(x,-2) + [3]*pow(x,-1) + [4] + [5]*pow(x,1) + [6]*pow(x,2) + [7]*pow(x,3) + [8]*pow(x,4))+[9]/x/x+[10]/x+[11]+[12]*x+[13]*x*x", STstart, STend)
  SFfitUpPars = array('d',[cov(0,0), #0: x^-4
                           cov(0,1)+cov(1,0), #1: x^-3
                           cov(0,2)+cov(1,1)+cov(2,0), #2: x^-2
                           cov(0,3)+cov(1,2)+cov(2,1)+cov(3,0), #3: x^-1
                           cov(0,4)+cov(1,3)+cov(2,2)+cov(3,1)+cov(4,0), #4: x^0
                           cov(1,4)+cov(2,3)+cov(3,2)+cov(4,1), #5: x^1
                           cov(2,4)+cov(3,3)+cov(4,2), #6: x^2
                           cov(3,4)+cov(4,3), #7: x^3
                           cov(4,4), #8: x^4
                           SFfit.GetParameter(0),
                           SFfit.GetParameter(1),
                           SFfit.GetParameter(2),
                           SFfit.GetParameter(3),
                           SFfit.GetParameter(4)])
  SFfitUp.SetParameters(SFfitUpPars)
  SFfitDown = TF1("SFfitDown", "-TMath::Sqrt([0]*pow(x,-4) + [1]*pow(x,-3) + [2]*pow(x,-2) + [3]*pow(x,-1) + [4] + [5]*pow(x,1) + [6]*pow(x,2) + [7]*pow(x,3) + [8]*pow(x,4))+[9]/x/x+[10]/x+[11]+[12]*x+[13]*x*x", STstart, STend)
  SFfitDownPars = array('d', [cov(0,0), #0: x^-4
                              cov(0,1)+cov(1,0), #1: x^-3
                              cov(0,2)+cov(1,1)+cov(2,0), #2: x^-2
                              cov(0,3)+cov(1,2)+cov(2,1)+cov(3,0), #3: x^-1
                              cov(0,4)+cov(1,3)+cov(2,2)+cov(3,1)+cov(4,0), #4: x^0
                              cov(1,4)+cov(2,3)+cov(3,2)+cov(4,1), #5: x^1
                              cov(2,4)+cov(3,3)+cov(4,2), #6: x^2
                              cov(3,4)+cov(4,3), #7: x^3
                              cov(4,4), #8: x^4
                              SFfit.GetParameter(0),
                              SFfit.GetParameter(1),
                              SFfit.GetParameter(2),
                              SFfit.GetParameter(3),
                              SFfit.GetParameter(4)])
  SFfitDown.SetParameters(SFfitDownPars)
else:
  SFfitUp = TF1("SFfitUp", "TMath::Sqrt([0]*pow(x,-4) + [1]*pow(x,-3) + [2]*pow(x,-2) + [3]*pow(x,-1) + [4] + [5]*pow(x,1) + [6]*pow(x,2)) + [7]/x/x+[8]/x+[9]+[10]*x", STstart, STend)
  SFfitUpPars = array('d', [cov(0,0), #0: x^-4
                            cov(0,1)+cov(1,0), #1: x^-3
                            cov(0,2)+cov(1,1)+cov(2,0), #2: x^-2
                            cov(0,3)+cov(1,2)+cov(2,1)+cov(3,0), #3: x^-1
                            cov(1,3)+cov(2,2)+cov(3,1), #4: x^0
                            cov(2,3)+cov(3,2), #5: x^1
                            cov(3,3), #6: x^2
                            SFfit.GetParameter(0),
                            SFfit.GetParameter(1),
                            SFfit.GetParameter(2),
                            SFfit.GetParameter(3)])
  SFfitUp.SetParameters(SFfitUpPars)
  SFfitDown = TF1("SFfitDown", "-TMath::Sqrt([0]*pow(x,-4) + [1]*pow(x,-3) + [2]*pow(x,-2) + [3]*pow(x,-1) + [4] + [5]*pow(x,1) + [6]*pow(x,2)) + [7]/x/x+[8]/x+[9]+[10]*x", STstart, STend)
  SFfitDownPars = array('d', [cov(0,0), #0: x^-4
                              cov(0,1)+cov(1,0), #1: x^-3
                              cov(0,2)+cov(1,1)+cov(2,0), #2: x^-2
                              cov(0,3)+cov(1,2)+cov(2,1)+cov(3,0), #3: x^-1
                              cov(1,3)+cov(2,2)+cov(3,1), #4: x^0
                              cov(2,3)+cov(3,2), #5: x^1
                              cov(3,3), #6: x^2
                              SFfit.GetParameter(0),
                              SFfit.GetParameter(1),
                              SFfit.GetParameter(2),
                              SFfit.GetParameter(3)])
  SFfitDown.SetParameters(SFfitDownPars)


canvSF1b = CMS.cmsCanvas("STSFfit_"+binS+"_"+year+"_1b", STstart, STend, SFfitDown.GetMinimum(STstart,STend)*0.8, SFfit.GetMaximum(STstart,STend)*1.2, "S_{T} [GeV/c]", "(data - MC(w/o t#bar{t}))/t#bar{t}", square=CMS.kSquare, extraSpace = 0.01, iPos = 0)
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

CMS.SaveCanvas(canvSF1b, "STfit_Wprime"+binS+"1_"+year+".pdf")
