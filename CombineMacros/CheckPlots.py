import os,sys

from ROOT import TH1F, TFile, THStack, TRatioPlot, TCanvas, TLegend, TLine, TF1, TMatrixD, TFitResultPtr, TGraph
import cmsstyle as CMS
from array import array

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

#fill background dictionaries
Bgr1b = {}
Bgr2b = {}
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

#make 1 b-tag region plot
STmax1b = max(Data1b.GetMaximum(), BgrTotal1b.GetMaximum())*1.1
canv1b = CMS.cmsDiCanvas("STSF_"+binS+"_"+str(year)+"_1b",STstart,STend,0,STmax1b,0.5,1.5,"S_{T} [GeV/c]", "Events/bin width", "Data/Pred.", square=CMS.kSquare, extraSpace=0.1, iPos=0)
canv1b.cd(1)

leg1b.AddEntry(Data1b, "Data", "lp")
CMS.cmsDrawStack(Stack1b, leg1b, Bgr1b)
CMS.cmsDraw(Data1b, "P", mcolor=1)
leg1b.Draw()

CMS.fixOverlay()

canv1b.cd(2)

Ratio1b = Data1b.Clone("Ratio1b")
Ratio1b.Divide(BgrTotal1b)
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
CMS.cmsDraw(Data2b, "P", mcolor=1)
leg2b.Draw()

CMS.fixOverlay()

canv2b.cd(2)

Ratio2b = Data2b.Clone("Ratio2b")
Ratio2b.Divide(BgrTotal2b)
CMS.cmsDraw(Ratio2b, "P", mcolor=1)

ref_line = TLine(STstart, 1, STend, 1)
CMS.cmsDrawLine(ref_line, lcolor=1, lstyle=3)

CMS.SaveCanvas(canv2b, "ST_Wprime"+binS+"2_"+str(year)+".pdf")

#make 1b-tag ST correction fit plot
SF = inSF.Get("SF_")
if JetMult == 5:
  SFfit = TF1("fitFunction","[0]/x/x/x + [1]/x/x + [2]/x + [3] + [4]*x + [5]*x*x", 180., 2000.)
else:
    SFfit = TF1("fitFunction","[0]/x + [1] + [2]*x + [3]*x*x", 210., 2000.)
fr = SF.Fit(SFfit,"SRF")
cov = fr.GetCovarianceMatrix()
if JetMult == 5:
  SFfitUp = TF1("fitFunctionUp", "[0]/x/x/x/x/x/x + [1]/x/x/x/x/x + [2]/x/x/x/x + [3]/x/x/x + [4]/x/x + [5]/x + [6] + [7]*x + [8]*x*x + [9]*x*x*x + [10]*x*x*x*x", 180., 2000.)
  SFfitUp.SetParameters(cov(0,0), cov(0,1)+cov(1,0), cov(0,2)+cov(1,1)+cov(2,0), cov(0,3)+cov(1,2)+cov(2,1)+cov(3,0)+SFfit.GetParameter(0), cov(0,4)+cov(1,3)+cov(2,2)+cov(3,1)+cov(4,0)+SFfit.GetParameter(1), cov(0,5)+cov(1,4)+cov(2,3)+cov(3,2)+cov(4,1)+cov(5,0)+SFfit.GetParameter(2), cov(1,5)+cov(2,4)+cov(3,3)+cov(4,2)+cov(5,1)+SFfit.GetParameter(3), cov(2,5)+cov(3,4)+cov(4,3)+cov(5,2)+SFfit.GetParameter(4), cov(3,5)+cov(4,4)+cov(5,3)+SFfit.GetParameter(5), cov(4,5)+cov(5,4), cov(5,5))
  SFfitDown = TF1("fitFunctionDown", "[0]/x/x/x/x/x/x + [1]/x/x/x/x/x + [2]/x/x/x/x + [3]/x/x/x + [4]/x/x + [5]/x + [6] + [7]*x + [8]*x*x + [9]*x*x*x + [10]*x*x*x*x", 180., 2000.)
  SFfitDown.SetParameters(-cov(0,0), -cov(0,1)-cov(1,0), -cov(0,2)-cov(1,1)-cov(2,0), -cov(0,3)-cov(1,2)-cov(2,1)-cov(3,0)+SFfit.GetParameter(0), -cov(0,4)-cov(1,3)-cov(2,2)-cov(3,1)-cov(4,0)+SFfit.GetParameter(1), -cov(0,5)-cov(1,4)-cov(2,3)-cov(3,2)-cov(4,1)-cov(5,0)+SFfit.GetParameter(2), -cov(1,5)-cov(2,4)-cov(3,3)-cov(4,2)-cov(5,1)+SFfit.GetParameter(3), -cov(2,5)-cov(3,4)-cov(4,3)-cov(5,2)+SFfit.GetParameter(4), -cov(3,5)-cov(4,4)-cov(5,3)+SFfit.GetParameter(5), -cov(4,5)-cov(5,4), -cov(5,5))
else:
  SFfitDiff = TF1("fitFunctionDiff", "[0]/x/x + [1]/x + [2] + [3]*x + [4]*x*x + [5]*x*x*x + [6]*x*x*x*x", 210., 2000.)
  SFfitDiff.SetParameters(cov(0,0), cov(0,1)+cov(1,0)+SFfit.GetParamter(0), cov(0,2)+cov(1,1)+cov(2,0)+SFfit.GetParameter(1), cov(0,3)+cov(1,2)+cov(2,1)+cov(3,0)+SFfit.GetParameter(2), cov(1,3)+cov(2,2)+SFfit.GetParameter(3)+cov(3,1), cov(2,3)+cov(3,2), cov(3,3))

canvSF1b = CMS.cmsCanvas("STSFfit_"+binS+"_"+str(year)+"_1b", STstart, STend, 0.9, 1.6, "S_{T} [GeV/c]", "(data - MC(w/o t#bar{t}))/t#bar{t}", square=CMS.kSquare, extraSpace = 0.01, iPos = 0)
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
