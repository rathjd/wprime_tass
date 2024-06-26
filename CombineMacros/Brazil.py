import os,sys
from ROOT import TGraph, TFile, TTree, TCanvas
from array import array

binS = "All"

year = "2017"

cardName = "Combination"

cardNameOptions = ["FitMass", "HT", "Combination"]

#accept shell inputs
try:
    if sys.argv[1]:
        binS = sys.argv[1]
        print("bin set to "+binS)
except:
    print("bin defaults to "+binS)

try:
    if sys.argv[2]:
        year = sys.argv[2]
        print("year set to "+year)
except:
    print("year defaults to "+year)

try:
    if sys.argv[3] in cardNameOptions:
        cardName = sys.argv[3]
        print("card type set to "+cardName)
except:
    print("card type defaults to "+cardName)




savefile = TFile("CombinationAll/CombinedLimit_"+cardName+"_"+binS+"_"+year+".root","RECREATE");

masses = array( 'd' )

crossSections = [683.8+708.3, 321.7+336.1, 161.1+165.3, 85.92+85.82, 48.84+47.47, 29.81+27.73, 18.33+16.49, 11.73+10.25, 7.683+6.546]

limitNumbers = []

#macro to extract limits from combined card

for mass in range(0,9):
    masses.append(float((3+mass)*100))
    massString = str((3+mass)*100)
    print("combine -M AsymptoticLimits -m "+massString+" ""CombinationAll/"+cardName+"_Wprime"+binS+"_"+year+".txt")
    os.system("combine -M AsymptoticLimits -m "+massString+" ""CombinationAll/"+cardName+"_Wprime"+binS+"_"+year+".txt")

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
for x in range(0,9):
    central.append(limitNumbers[x][2])
    OneSigmaBand.append(limitNumbers[x][1])
    TwoSigmaBand.append(limitNumbers[x][0])
    massBand.append(float((3+x)*100))

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

canvas = TCanvas("CombinedLimit","",1000,1000)
CentralGraph.SetLineColor(1)
CentralGraph.SetMarkerStyle(47)
CentralGraph.SetMarkerColor(1)

OneSigmaGraph.SetFillColor(8)
OneSigmaGraph.SetMarkerStyle(1)

TwoSigmaGraph.SetFillColor(5)
TwoSigmaGraph.SetMarkerStyle(1)
TwoSigmaGraph.GetXaxis().SetTitle("W' mass [GeV/c^{2}]")
TwoSigmaGraph.GetYaxis().SetTitle("#sigma [fb]")

TwoSigmaGraph.Draw("A,F")
OneSigmaGraph.Draw("F,same")
CentralGraph.Draw("P,L,same")

savefile.cd()
canvas.Write()
canvas.SaveAs(cardName+"_"+binS+"_"+year+".pdf")
savefile.Close()
