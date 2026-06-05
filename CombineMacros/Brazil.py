import os,sys
from ROOT import TGraph, TFile, TTree, TCanvas, TColor, TLegend, TLatex, kBlack
from array import array
import cmsstyle as CMS

binS = "All"

year = "all"

cardName = "Optimized"

cardNameOptions = ["FitMass", "HT", "Combination", "FitSlice", "HTslice", "Optimized", "CombinationSlices"]

yearOptions = ["2016apv", "2016", "2017", "2018", "all"]

OffMass = False

#accept shell inputs
try:
    if sys.argv[1]:
        binS = sys.argv[1]
        print("bin set to "+binS)
except:
    print("bin defaults to "+binS)

try:
    if sys.argv[2] in yearOptions:
        year = sys.argv[2]
        print("year set to "+year)
except:
    print("year defaults to "+year+", legitimate options are:")
    print(yearOptions)

try:
    if sys.argv[3] in cardNameOptions:
        cardName = sys.argv[3]
        print("card type set to "+cardName)
        print(cardNameOptions)
except:
    print("card type defaults to "+cardName+", legitimate options are:")

try:
    if sys.argv[4] == "True":
        OffMass = True
        print("OffMass mode enabled")
except:
    print("OffMass mode disabled by default")

#CMS style setup
if   year == "2016apv":
    CMS.SetLumi("19.5")
elif year == "2016":
    CMS.SetLumi("16.8") 
elif year == "2017":
    CMS.SetLumi("42.1")
elif year == "2018":
    CMS.SetLumi("59.6")
elif year == "all":
    CMS.SetLumi("138")
CMS.SetEnergy("13")

CMS.SetExtraText("Simulation Preliminary")

#display channel and year on plot

Extract = ""
Lepton = ""
Jets = ""
Bjets = ""
if cardName in ["FitMass", "FitSlice"]:
    Extract = "m_{fit}"
elif cardName in ["HT", "HTslice"]:
    Extract = "H_{T}"
else:
    Extract = "m_{fit} and H_{T}"

if binS.find("x") > -1:
    Lepton = "e/#mu, "
elif binS[0] == "1":
    Lepton = "#mu, "
elif binS[0] == "2":
    Lepton = "e, "
else:
    Lepton = ""

if binS == "All":
    Lepton = "e/#mu, 5-6j, and 2-4b tags"
    YearS = year
else:
    Jets  = binS[2]+"j and "
    Bjets = binS[3]+"b tags"
    YearS = year

if year == "all":
    YearS = "Run 2"

crossSections = [683.8+708.3, 321.7+336.1, 161.1+165.3, 85.92+85.82, 48.84+47.47, 29.81+27.73, 18.33+16.49, 11.73+10.25, 7.683+6.546]
maxCrossSections = [1693., 843., 425., 227., 128., 76., 46.6, 29.3, 19.0]

#macro to extract limits from combined card
if not OffMass:
    masses = array( 'd' )
    limitNumbers = []
    for mass in range(0,9):
        masses.append(float((3+mass)*100))
        massString = str((3+mass)*100)
        print("combine -M AsymptoticLimits --run blind -m "+massString+" ""Combination/"+cardName+"_Wprime"+binS+"_"+year+"_M"+massString+".txt")
        os.system("combine -M AsymptoticLimits --run blind -m "+massString+" ""Combination/"+cardName+"_Wprime"+binS+"_"+year+"_M"+massString+".txt")

        infile = TFile("higgsCombineTest.AsymptoticLimits.mH"+massString+".root","READ")

        limitNumbers.append([])

        for event in infile.limit:
            limitNumbers[mass].append(event.limit*crossSections[mass])

        infile.Close()

    #order this for TGraph
    TwoSigmaBand = array( 'd' )
    OneSigmaBand = array( 'd' )
    central = array( 'd' )
    massBand = array( 'd' )
    theory = array( 'd' )

    minimum = 9999.
    maximum = 0.

    for x in range(0,9):
        central.append(limitNumbers[x][2])
        theory.append(maxCrossSections[x])
        OneSigmaBand.append(limitNumbers[x][1])
        TwoSigmaBand.append(limitNumbers[x][0])
        massBand.append(float((3+x)*100))

        minimum = min(minimum, limitNumbers[x][0])
        minimum = min(minimum, limitNumbers[x][4])
        maximum = max(maximum, limitNumbers[x][0])
        maximum = max(maximum, limitNumbers[x][4])

    for x in range(0,9):
        OneSigmaBand.append(limitNumbers[8-x][3])
        TwoSigmaBand.append(limitNumbers[8-x][4])
        massBand.append(float((11-x)*100))
        
    print(central)
    print(OneSigmaBand)
    print(TwoSigmaBand)
    print(massBand)

    CentralGraph = TGraph(9, masses, central)
    OneSigmaGraph = TGraph(18, massBand, OneSigmaBand)
    TwoSigmaGraph = TGraph(18, massBand, TwoSigmaBand)
    TheoryGraph = TGraph(9, masses, theory)

    #Draw CMS style Brazil plot
    canvas = CMS.cmsCanvas("Limit_"+cardName+"_Wprime"+binS+"_"+year, 300., 1100., minimum*0.8, maximum*1.4, "m_{W'} [GeV]", "#sigma#font[12]{B}(pp #rightarrow tW' #rightarrow ttb) [fb]", square = CMS.kSquare, extraSpace = 0.01, iPos = 0)#TCanvas("CombinedLimit","",1000,1000)
    canvas.cd(1)

    CentralGraph.SetLineColor(1)
    CentralGraph.SetLineStyle(2)
    CentralGraph.SetLineWidth(2)
    #CentralGraph.SetMarkerStyle(47)
    #CentralGraph.SetMarkerColor(1)

    #OneSigmaGraph.SetFillColor(TColor.GetColor("#607641"))
    OneSigmaGraph.SetFillColor(TColor.GetColor("228b22"))
    OneSigmaGraph.SetMarkerStyle(1)

    #TwoSigmaGraph.SetFillColor(TColor.GetColor("#F5BB54"))
    TwoSigmaGraph.SetFillColor(TColor.GetColor("ffcc00"))
    TwoSigmaGraph.SetMarkerStyle(1)
    #TwoSigmaGraph.GetXaxis().SetTitle("W' mass [GeV/c^{2}]")
    #TwoSigmaGraph.GetYaxis().SetTitle("#sigma [fb]")

    TheoryGraph.SetLineColor(4)
    TheoryGraph.SetLineWidth(2)

    TwoSigmaGraph.Draw("F")
    OneSigmaGraph.Draw("F,same")
    CentralGraph.Draw("L,same")
    TheoryGraph.Draw("L,same")

    legend = CMS.cmsLeg(0.60, 0.89-0.04*4, 0.89, 0.89, textSize=0.04)
    legend.SetHeader("95% CL upper limits")
    legend.AddEntry(CentralGraph, "Median expected", "l")
    legend.AddEntry(OneSigmaGraph, "68% expected", "f")
    legend.AddEntry(TwoSigmaGraph, "95% expected", "f")

    legend2 = CMS.cmsLeg(0.6, 0.89-0.04*6, 0.89, 0.89-0.04*5, textSize=0.04)
    legend2.AddEntry(TheoryGraph, "#Gamma_{W'}=10% theory, g_{#tau}=0", "l")

    #draw manually channel description
    latex = TLatex()
    latex.SetNDC()
    latex.SetTextAngle(0)
    latex.SetTextColor(kBlack)
    latex.SetTextFont(42)
    latex.SetTextAlign(11)
    latex.SetTextSize(0.04)#0.75*0.76*0.05*1.2)
    additionalInfo = [Extract, Lepton+Jets+Bjets, YearS]
    for ind, tt in enumerate(additionalInfo):
        print(tt)
        latex.DrawLatex(
            0.15,
            1 - 0.05 - 0.045 * (1 - 0.05 - 0.13) - 1.2 * 0.75 * 0.05 - 0.004 - (1.2 * 0.75 * 0.76 * 0.05 /2 + 0.02) * ind,
            tt
        )


    legend.Draw()
    legend2.Draw()

    canvas.Update()

    #set up actual limit extraction
    savefile = TFile("Combination/CombinedLimit_"+cardName+"_"+binS+"_"+year+".root","RECREATE");

    savefile.cd()
    TwoSigmaGraph.Write("TwoSigmaBand")
    OneSigmaGraph.Write("OneSigmaBand")
    CentralGraph.Write("Expected")

    CMS.SaveCanvas(canvas, "Limit_"+cardName+"_Wprime"+binS+"_"+year+".pdf")
    #canvas.SaveAs(cardName+"_"+binS+"_"+year+".pdf")
    savefile.Close()

#OffMass mode
else:
    masses = array( 'd' )
    limitNumbers = []
    limits = []

    minimum = 9999.
    maximum = 0.

    #mass of the card and therefore the NLL training
    for massCard in range(0,9):
        masses.append(float((3+massCard)*100))
        limitNumbers.append([])

        #mass of the signal tested by the card
        for massSignal in range(0,9):
            print(    "combine -M AsymptoticLimits --run blind -m "+str((3+massSignal)*100)+" ""Combination/"+cardName+"_Wprime"+binS+"_"+year+"_M"+str((3+massCard)*100)+".txt")
            os.system("combine -M AsymptoticLimits --run blind -m "+str((3+massSignal)*100)+" ""Combination/"+cardName+"_Wprime"+binS+"_"+year+"_M"+str((3+massCard)*100)+".txt")

            infile = TFile("higgsCombineTest.AsymptoticLimits.mH"+str((3+massSignal)*100)+".root","READ")

            #only extract the central limit for massSignal in massCard
            for i,event in enumerate(infile.limit):
                if i == 2:
                    limitNumbers[massCard].append(event.limit*crossSections[massSignal])
                    minimum = min(event.limit*crossSections[massSignal], minimum)
                    maximum = max(event.limit*crossSections[massSignal], maximum)

            infile.Close()

    #store the expected limit for this card in a TGraph and append it to a list of limits
    for massCard in range(0,9):
        limit = array( 'd' )
        for massSignal in range(0,9):
            limit.append(limitNumbers[massCard][massSignal])
        limits.append(TGraph(9, masses, limit))
    
    #draw comparison plot
    canvas = CMS.cmsCanvas("CompareLimitsByInterpretation_"+cardName+"_Wprime"+binS+"_"+year, 300., 1100., minimum*0.8, maximum*1.4, "m_{W'} [GeV]", "#sigma#font[12]{B}(pp #rightarrow tW' #rightarrow ttb) [fb]", square = CMS.kSquare, extraSpace = 0.01, iPos = 0)#TCanvas("CombinedLimit","",1000,1000)
    canvas.cd(1)

    legend = CMS.cmsLeg(0.40, 0.89-0.04*10, 0.89, 0.89, textSize=0.04)
    legend.SetHeader("95% CL upper limits median exp.")

    for massCard in range(0,9):
        limits[massCard].SetLineColor(massCard+1)
        limits[massCard].SetLineStyle(2)
        limits[massCard].SetLineWidth(2)
        if massCard == 0:
            limits[massCard].Draw("L")
        else:
            limits[massCard].Draw("L,same")
        legend.AddEntry(limits[massCard], str((3+massCard)*100), "l")

    #draw manually channel description
    latex = TLatex()
    latex.SetNDC()
    latex.SetTextAngle(0)
    latex.SetTextColor(kBlack)
    latex.SetTextFont(42)
    latex.SetTextAlign(11)
    latex.SetTextSize(0.04)#0.75*0.76*0.05*1.2)
    additionalInfo = [Extract, Lepton+Jets+Bjets, YearS]
    for ind, tt in enumerate(additionalInfo):
        print(tt)
        latex.DrawLatex(
            0.15,
            1 - 0.05 - 0.045 * (1 - 0.05 - 0.13) - 1.2 * 0.75 * 0.05 - 0.004 - (1.2 * 0.75 * 0.76 * 0.05 /2 + 0.02) * ind,
            tt
        )


    legend.Draw()

    canvas.Update()

    #set up actual limit extraction
    savefile = TFile("Combination/CombinedLimitComparisonByInterpretation_"+cardName+"_"+binS+"_"+year+".root","RECREATE");

    savefile.cd()

    for massCard in range(0,9):
        limits[massCard].Write("M"+str((massCard+3)*100))

    CMS.SaveCanvas(canvas, "LimitComparisonByInterpretation_"+cardName+"_Wprime"+binS+"_"+year+".pdf")
    #canvas.SaveAs(cardName+"_"+binS+"_"+year+".pdf")
    savefile.Close()
