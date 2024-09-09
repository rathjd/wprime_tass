import os,sys
from ROOT import TGraph, TFile, TTree, TCanvas
from array import array

savefile = TFile("CombinationAll/OptimizedLimit.root","RECREATE");

masses = array( 'd' )

crossSections = [683.8+708.3, 321.7+336.1, 161.1+165.3, 85.92+85.82, 48.84+47.47, 29.81+27.73, 18.33+16.49, 11.73+10.25, 7.683+6.546]

limitNumbers = []

years = ["2016", "2017", "2018"]
binS  = [["1153", "2153"], ["1163", "2163"], ["1164", "2164"]]

optimization = []

compromise = True
folder = "test"

#set up the optimized working points for each masspoint
#order is 1153, 2153; 1163, 2163; 1164, 2164
#optimization = [[[18,25], [ 1,13], [6,22]], #300 GeV
#                [[ 1,28], [ 1,13], [7,22]], #400
#                [[ 6,28], [ 1,13], [3,22]], #500
#                [[17,25], [ 1,13], [1,13]], #600
#                [[ 8,30], [ 1,12], [3,22]], #700
#                [[ 9,30], [ 7,15], [6,22]], #800
#                [[ 7,30], [ 6,13], [6,22]], #900
#                [[ 7,30], [ 5,15], [6,22]], #1000
#                [[ 7,30], [ 5,15], [1,22]]] #1100

#best optimization so far
if not compromise:
    #mass-independent all-year optimization result
    #optimization = [[[18, 25],[1, 13], [6, 22]],
    #                [[1, 28], [1, 13], [7, 22]],
    #                [[6, 28], [1, 13], [3, 22]],
    #                [[17, 25],[1, 13], [3, 22]],
    #                [[8, 30], [1, 12], [3, 22]],
    #                [[9, 30], [7, 15], [6, 22]],
    #                [[7, 30], [6, 13], [6, 22]],
    #                [[7, 30], [5, 15], [6, 22]],
    #                [[7, 30], [5, 15], [1, 22]]]
    folder = "allYears_OptimizedSlices"
    #mass-dependent 2017 only optimization result
    optimization = [[[5, 19],  [1, 9],  [4, 13]],
                    [[6, 26],  [9, 13], [6, 13]],
                    [[12, 27], [5, 16], [4, 13]],
                    [[5, 30],  [6, 21], [6, 14]],
                    [[5, 29],  [7, 21], [15, 21]],
                    [[14, 47], [3, 14], [13, 22]],
                    [[6, 33],  [7, 17], [13, 22]],
                    [[4, 28],  [10, 22], [7, 22]],
                    [[4, 28],  [1, 17], [3, 22]]]
else:
#compromise version
    optimization = [[[7, 28], [5, 13], [6, 22]],
                    [[7, 28], [5, 13], [6, 22]],
                    [[7, 28], [5, 13], [6, 22]],
                    [[7, 28], [5, 13], [6, 22]],
                    [[7, 28], [5, 13], [6, 22]],
                    [[7, 28], [5, 13], [6, 22]],
                    [[7, 28], [5, 13], [6, 22]],
                    [[7, 28], [5, 13], [6, 22]],
                    [[7, 28], [5, 13], [6, 22]]]
    folder = "allYears_CompromiseSlices"

print(compromise)
print(optimization)
print(folder)

#macro to extract limits from combined cards, once set up optimized slices

os.chdir("CombinationAll")

for mass in range(0,8):
    masses.append(float((3+mass)*100))
    massString = str((3+mass)*100)
    
    #set up combination cards
    counterJet = 0
    for binN in binS:
        for year in years:
            for binX in binN:
                os.system("python3 ../SliceHTvsFitMass.py "+year+" "+binX+" "+str(optimization[mass][counterJet][0])+" "+str(optimization[mass][counterJet][1]))#+" direct")
        counterJet+=1

    #combine all cards, first into the optimized subsets
    #os.system("combineCards.py HT_Wprime1154_2016.txt HT_Wprime1154_2017.txt HT_Wprime1154_2018.txt HT_Wprime2154_2016.txt HT_Wprime2154_2017.txt HT_Wprime2154_2018.txt > HT_Wprimex154_all.txt")
    os.system("combineCards.py CombinationSlices_Wprime1153_2016_M"+massString+".txt CombinationSlices_Wprime1153_2017_M"+massString+".txt CombinationSlices_Wprime1153_2018_M"+massString+".txt CombinationSlices_Wprime2153_2016_M"+massString+".txt CombinationSlices_Wprime2153_2017_M"+massString+".txt CombinationSlices_Wprime2153_2018_M"+massString+".txt > CombinationSlices_Wprimex153_all_M"+massString+".txt")
    os.system("combineCards.py CombinationSlices_Wprime1163_2016_M"+massString+".txt CombinationSlices_Wprime1163_2017_M"+massString+".txt CombinationSlices_Wprime1163_2018_M"+massString+".txt CombinationSlices_Wprime2163_2016_M"+massString+".txt CombinationSlices_Wprime2163_2017_M"+massString+".txt CombinationSlices_Wprime2163_2018_M"+massString+".txt > CombinationSlices_Wprimex163_all_M"+massString+".txt")
    os.system("combineCards.py CombinationSlices_Wprime1164_2016_M"+massString+".txt CombinationSlices_Wprime1164_2017_M"+massString+".txt CombinationSlices_Wprime1164_2018_M"+massString+".txt CombinationSlices_Wprime2164_2016_M"+massString+".txt CombinationSlices_Wprime2164_2017_M"+massString+".txt CombinationSlices_Wprime2164_2018_M"+massString+".txt > CombinationSlices_Wprimex164_all_M"+massString+".txt")
    #full combination
    os.system("combineCards.py CombinationSlices_Wprimex153_all_M"+massString+".txt CombinationSlices_Wprimex163_all_M"+massString+".txt CombinationSlices_Wprimex164_all_M"+massString+".txt > Optimized_WprimeAll_all_M"+massString+".txt")

    #make a folder to copy this masspoints interpretation files to in totality
    if not os.path.isdir("/eos/cms/store/group/phys_b2g/wprime/PatrickCombine/"+folder+"/"+massString):
        os.system("mkdir /eos/cms/store/group/phys_b2g/wprime/PatrickCombine/"+folder+"/"+massString)
    else:
        os.system("rm -rf /eos/cms/store/group/phys_b2g/wprime/PatrickCombine/"+folder+"/"+massString)
        os.system("mkdir /eos/cms/store/group/phys_b2g/wprime/PatrickCombine/"+folder+"/"+massString)

    print("card ",os.path.exists("Optimized_WprimeAll_all._M"+massString+"txt"))
    card = open("Optimized_WprimeAll_all._M"+massString+"txt","r")
    lines = card.readlines()
    rootfiles = []
    for line in lines:
        lineSplit = line.split()
        for lineS in lineSplit:
            if lineS.find(".root") > -1:
                rootfiles.append(lineS)

    for rootfile in rootfiles:
        print(rootfile,os.path.exists(rootfile))
        print("cp "+rootfile+" /eos/cms/store/group/phys_b2g/wprime/PatrickCombine/"+folder+"/"+massString+"/.")
        os.system("cp "+rootfile+" /eos/cms/store/group/phys_b2g/wprime/PatrickCombine/"+folder+"/"+massString+"/.")
        print(os.path.exists("/eos/cms/store/group/phys_b2g/wprime/PatrickCombine/"+folder+"/"+massString+"/"+rootfile))

    #copy current cards and all root files in final card
    print("cp CombinationSlices_Wprime1153_2016_M"+massString+".txt CombinationSlices_Wprime1153_2017_M"+massString+".txt CombinationSlices_Wprime1153_2018_M"+massString+".txt CombinationSlices_Wprime2153_2016_M"+massString+".txt CombinationSlices_Wprime2153_2017_M"+massString+".txt CombinationSlices_Wprime2153_2018_M"+massString+".txt CombinationSlices_Wprimex153_all_M"+massString+".txt CombinationSlices_Wprime1163_2016_M"+massString+".txt CombinationSlices_Wprime1163_2017_M"+massString+".txt CombinationSlices_Wprime1163_2018_M"+massString+".txt CombinationSlices_Wprime2163_2016_M"+massString+".txt CombinationSlices_Wprime2163_2017_M"+massString+".txt CombinationSlices_Wprime2163_2018_M"+massString+".txt CombinationSlices_Wprimex163_all_M"+massString+".txt CombinationSlices_Wprime1164_2016_M"+massString+".txt CombinationSlices_Wprime1164_2017_M"+massString+".txt CombinationSlices_Wprime1164_2018_M"+massString+".txt CombinationSlices_Wprime2164_2016_M"+massString+".txt CombinationSlices_Wprime2164_2017_M"+massString+".txt CombinationSlices_Wprime2164_2018_M"+massString+".txt CombinationSlices_Wprimex164_all_M"+massString+".txt Optimized_WprimeAll_all_M"+massString+".txt /eos/cms/store/group/phys_b2g/wprime/PatrickCombine/"+folder+"/"+massString+"/.")
    os.system("cp CombinationSlices_Wprime1153_2016_M"+massString+".txt CombinationSlices_Wprime1153_2017_M"+massString+".txt CombinationSlices_Wprime1153_2018_M"+massString+".txt CombinationSlices_Wprime2153_2016_M"+massString+".txt CombinationSlices_Wprime2153_2017_M"+massString+".txt CombinationSlices_Wprime2153_2018_M"+massString+".txt CombinationSlices_Wprimex153_all_M"+massString+".txt CombinationSlices_Wprime1163_2016_M"+massString+".txt CombinationSlices_Wprime1163_2017_M"+massString+".txt CombinationSlices_Wprime1163_2018_M"+massString+".txt CombinationSlices_Wprime2163_2016_M"+massString+".txt CombinationSlices_Wprime2163_2017_M"+massString+".txt CombinationSlices_Wprime2163_2018_M"+massString+".txt CombinationSlices_Wprimex163_all_M"+massString+".txt CombinationSlices_Wprime1164_2016_M"+massString+".txt CombinationSlices_Wprime1164_2017_M"+massString+".txt CombinationSlices_Wprime1164_2018_M"+massString+".txt CombinationSlices_Wprime2164_2016_M"+massString+".txt CombinationSlices_Wprime2164_2017_M"+massString+".txt CombinationSlices_Wprime2164_2018_M"+massString+".txt CombinationSlices_Wprimex164_all_M"+massString+".txt Optimized_WprimeAll_all_M"+massString+".txt /eos/cms/store/group/phys_b2g/wprime/PatrickCombine/"+folder+"/"+massString+"/.")

    #run Combine for this set of cards
    os.system("combine -M AsymptoticLimits Optimized_WprimeAll_all_M"+massString+".txt")

    infile = TFile("higgsCombineTest.AsymptoticLimits.mH"+massString+".root","READ")

    print("cp higgsCombineTest.AsymptoticLimits.mH"+massString+".root /eos/cms/store/group/phys_b2g/wprime/SifuCombine/"+folder+"/"+massString+"/.")
    os.system("cp higgsCombineTest.AsymptoticLimits.mH"+massString+".root /eos/cms/store/group/phys_b2g/wprime/SifuCombine/"+folder+"/"+massString+"/.")
    print(os.path.exists("/eos/cms/store/group/phys_b2g/wprime/SifuCombine/"+folder+"/"+massString+"/higgsCombineTest.AsymptoticLimits.mH"+massString+".root"))

    limitNumbers.append([])

    for event in infile.limit:
        limitNumbers[mass].append(event.limit*crossSections[mass])

    infile.Close()

os.chdir("..")

#order this for TGraph
TwoSigmaBand = array( 'd' )
OneSigmaBand = array( 'd' )
central = array( 'd' )
massBand = array( 'd' )
for x in range(0,8):
    central.append(limitNumbers[x][2])
    OneSigmaBand.append(limitNumbers[x][1])
    TwoSigmaBand.append(limitNumbers[x][0])
    massBand.append(float((3+x)*100))

for x in range(0,8):
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
canvas.SaveAs("FullOptimizationFullCombination.pdf")
canvas.SaveAs("FullOptimizationFullCombination.root")
os.system("cp FullOptimizationFullCombination.pdf /eos/cms/store/group/phys_b2g/wprime/SifuCombine/"+folder+"/.")
os.system("cp FullOptimizationFullCombination.root /eos/cms/store/group/phys_b2g/wprime/SifuCombine/"+folder+"/.")
savefile.Close()
