import os,sys

from ROOT import TH1F, TFile, THStack, TRatioPlot, TCanvas, TLegend

#settings for what to plot
LeptonFlav = 1
JetMult = 5
year = 2018

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

#all backgrounds considered in stacks and ratios
backgrounds = [["ttbar", 2], 
               ["wjets", 3],
               ["single_top", 4],
               ["diboson", 5]]

#input files
binS = str(LeptonFlav)+"1"+str(JetMult)

inOrigin = TFile(binS+"1_"+str(year)+"/SimpleShapes_Wprime"+binS+"1_"+str(year)+".root","READ")
inSF     = TFile(binS+"2_"+str(year)+"/SF_Bin"+binS+"1_"+str(year)+".root","READ")
inResult = TFile(binS+"2_"+str(year)+"/SimpleShapes_Wprime"+binS+"2_"+str(year)+".root","READ")

#output canvas
canv = TCanvas("STSF_"+binS+"_"+str(year), "", 1500, 500)
canv.Divide(3,1)

#extract data histograms
Data1b = inOrigin.Get("ST_data_obs_Wprime"+binS+"1_"+str(year)+"_")
Data1b.SetMarkerStyle(8)
Data1b.SetLineColor(1)
Data1b.Scale(1.,"width")
Data2b = inResult.Get("ST_data_obs_Wprime"+binS+"2_"+str(year)+"_")
Data2b.SetMarkerStyle(8)
Data2b.SetLineColor(1)
Data2b.Scale(1.,"width")

#make stacks
Stack1b = THStack("Stack_"+binS+"1_"+str(year),"")
Stack2b = THStack("Stack_"+binS+"2_"+str(year),"")

#configure legends
LepS = ""
if LeptonFlav == 1:
    LepS = "#mu"
else:
    LepS = "e"

leg1b = TLegend(0.6,0.6,0.9,0.9,"1"+LepS+", "+str(JetMult)+" jets, 1 b-tag")
leg1b.SetLineColor(0)
leg1b.AddEntry(Data1b,"data","lp")
leg2b = TLegend(0.6,0.6,0.9,0.9,"1"+LepS+", "+str(JetMult)+" jets, 2 b-tags")
leg2b.SetLineColor(0)
leg2b.AddEntry(Data2b,"data","lp")

#fill background histograms and stacks
for background in backgrounds:
    BgrPart1b = inOrigin.Get("ST_"+background[0]+"_Wprime"+binS+"1_"+str(year)+"_")
    BgrPart1b.SetFillColor(background[1])
    BgrPart1b.Scale(1.,"width")

    if background[0] == "ttbar":
      BgrPart2b = inResult.Get("STrew_"+background[0]+"_Wprime"+binS+"2_"+str(year)+"_")
      BgrPart2b.SetFillColor(background[1])
    else:
      BgrPart2b = inResult.Get("ST_"+background[0]+"_Wprime"+binS+"2_"+str(year)+"_")
      BgrPart2b.SetFillColor(background[1])
    BgrPart2b.Scale(1.,"width")

    Stack1b.Add(BgrPart1b)
    Stack2b.Add(BgrPart2b)

    leg1b.AddEntry(BgrPart1b,background[0],"f")
    leg2b.AddEntry(BgrPart1b,background[0],"f")

#make 1 b-tag region plot
canv.cd(1)
canv.SetLogy(1)

Stack1b.SetMaximum(Data1b.GetMaximum()*1.1)

Ratio1b = TRatioPlot(Stack1b, Data1b)
Ratio1b.Draw();
Ratio1b.GetUpperRefXaxis().SetTitle("S_{T} [GeV/c]")
Ratio1b.GetUpperRefYaxis().SetTitle("Events/bin width")
Ratio1b.GetUpperRefYaxis().SetTitleOffset(1.25)
Ratio1b.GetLowerRefYaxis().SetTitle("Data/MC")
Ratio1b.GetLowerRefYaxis().SetTitleOffset(1.25)
Ratio1b.GetUpperPad()
leg1b.Draw("same");
Ratio1b.GetLowerRefGraph().SetMinimum(0.5)
Ratio1b.GetLowerRefGraph().SetMaximum(1.5)

#make 1 b-tag SF with fit
canv.cd(2)
SFfit = inSF.Get("SF_")
SFfit.SetStats(0)
SFfit.SetTitle("")
SFfit.Draw();
SFfit.GetXaxis().SetTitle("S_{T} [GeV/c]")
SFfit.GetYaxis().SetTitle("(data-!ttbar)/ttbar")

#make 2 b-tag region plot
canv.cd(3)
canv.SetLogy(1)

Ratio2b = TRatioPlot(Stack2b, Data2b)
Ratio2b.Draw();
Ratio2b.GetUpperRefXaxis().SetTitle("S_{T} [GeV/c]")
Ratio2b.GetUpperRefYaxis().SetTitle("Events/bin width")
Ratio2b.GetUpperRefYaxis().SetTitleOffset(1.25)
Ratio2b.GetLowerRefYaxis().SetTitle("Data/MC")
Ratio2b.GetLowerRefYaxis().SetTitleOffset(1.25)
Ratio2b.GetUpperPad()
leg2b.Draw("same");
Ratio2b.GetLowerRefGraph().SetMinimum(0.5)
Ratio2b.GetLowerRefGraph().SetMaximum(1.5)

canv.SaveAs("STcheck_"+binS+"_"+str(year)+".pdf")
