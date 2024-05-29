import os,sys

from ROOT import TH1F, TFile, THStack, TRatioPlot, TCanvas, TLegend, TLine
import cmsstyle as CMS

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


#CMS.cmsDrawStack(Stack1b, leg, {"Background": 

#Ratio1b = TRatioPlot(Stack1b, Data1b)
#Ratio1b.Draw();
#Ratio1b.GetUpperRefXaxis().SetTitle("S_{T} [GeV/c]")
#Ratio1b.GetUpperRefYaxis().SetTitle("Events/bin width")
#Ratio1b.GetUpperRefYaxis().SetTitleOffset(1.25)
#Ratio1b.GetLowerRefYaxis().SetTitle("Data/MC")
#Ratio1b.GetLowerRefYaxis().SetTitleOffset(1.25)
#Ratio1b.GetUpperPad()
#leg1b.Draw("same");
#Ratio1b.GetLowerRefGraph().SetMinimum(0.5)
#Ratio1b.GetLowerRefGraph().SetMaximum(1.5)

#make 1 b-tag SF with fit
#canv.cd(2)
#SFfit = inSF.Get("SF_")
#SFfit.SetStats(0)
#SFfit.SetTitle("")
#SFfit.Draw();
#SFfit.GetXaxis().SetTitle("S_{T} [GeV/c]")
#SFfit.GetYaxis().SetTitle("(data-!ttbar)/ttbar")

#make 2 b-tag region plot
#canv.cd(3)
#canv.SetLogy(1)

#Ratio2b = TRatioPlot(Stack2b, Data2b)
#Ratio2b.Draw();
#Ratio2b.GetUpperRefXaxis().SetTitle("S_{T} [GeV/c]")
#Ratio2b.GetUpperRefYaxis().SetTitle("Events/bin width")
#Ratio2b.GetUpperRefYaxis().SetTitleOffset(1.25)
#Ratio2b.GetLowerRefYaxis().SetTitle("Data/MC")
#Ratio2b.GetLowerRefYaxis().SetTitleOffset(1.25)
#Ratio2b.GetUpperPad()
#leg2b.Draw("same");
#Ratio2b.GetLowerRefGraph().SetMinimum(0.5)
#Ratio2b.GetLowerRefGraph().SetMaximum(1.5)

#canv.SaveAs("STcheck_"+binS+"_"+str(year)+".pdf")
