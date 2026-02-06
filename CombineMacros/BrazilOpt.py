import os,sys
from ROOT import TGraph, TFile, TTree, TCanvas
from array import array

savefile = TFile("Combination/OptimizedLimit.root","RECREATE");

b2gpath = "/eos/cms/store/group/phys_b2g/wprime/June25_FixesApplied_newOpt_Combine/" 

masses = array( 'd' )

crossSections = [683.8+708.3, 321.7+336.1, 161.1+165.3, 85.92+85.82, 48.84+47.47, 29.81+27.73, 18.33+16.49, 11.73+10.25, 7.683+6.546]

limitNumbers = []

years = ["2016apv", "2016", "2017", "2018"]
binS  = [["1152", "2152", "1153", "2153"],
         ["1162", "2162", "1163", "2163", "1164", "2164"]]

optimization = []

compromise = True
folder = "test"
skipSlicing = True
skipLimits = False

#accept shell inputs

try:
    if sys.argv[1] == "False":
        skipSlicing = False
        print("skipping slicing set to",skipSlicing)
except:
    print("skipping slicing defaults to",skipSlicing)

try:
    if sys.argv[2] == "False":
        compromise = False
        print("compromise optimization option set to",compromise)
except:
    print("compromise optimization option defaults to",compromise)

try:
    if sys.argv[3] == "True":
        skipLimits = True
        print("skipping limits set to",skipLimits)
except:
    print("skipping limits defaults to",skipLimits)

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
    #optimization = [[[5, 19],  [1, 9],  [4, 13]],
    #                [[6, 26],  [9, 13], [6, 13]],
    #                [[12, 27], [5, 16], [4, 13]],
    #                [[5, 30],  [6, 21], [6, 14]],
    #                [[5, 29],  [7, 21], [15, 21]],
    #                [[14, 47], [3, 14], [13, 22]],
    #                [[6, 33],  [7, 17], [13, 22]],
    #                [[4, 28],  [10, 22], [7, 22]],
    #                [[4, 28],  [1, 17], [3, 22]]]
    #all year all mass-dependent optimization result
    #optimization = [[[ 5, 15], [12, 14], [12, 22]],
    #                [[ 9, 23], [ 4, 13], [12, 22]],
    #                [[ 8, 22], [ 3, 13], [12, 22]],
    #                [[10, 26], [ 4, 13], [11, 22]],
    #                [[ 3, 23], [ 4, 14], [11, 22]],
    #                [[ 9, 27], [ 4, 15], [10, 22]],
    #                [[16, 27], [ 3, 14], [12, 22]],
    #                [[14, 28], [ 7, 14], [10, 22]],
    #                [[17, 27], [ 7, 14], [10, 22]]]
    #all year mass-dependent optimization result post single top negative weight fix
    #optimization = [[[ 5, 21], [2, 13], [12, 22]],
    #                [[ 9, 23], [4, 13], [10, 22]],
    #                [[ 8, 22], [3, 13], [ 7, 22]],
    #                [[10, 26], [4, 13], [11, 22]],
    #                [[ 9, 23], [3, 14], [10, 22]],
    #                [[ 9, 27], [4, 15], [11, 22]],
    #                [[11, 28], [2, 14], [13, 22]],
    #                [[14, 25], [1, 14], [10, 22]],
    #                [[17, 27], [7, 14], [ 8, 22]]]
    #reran 6j4b channels, excluding slices with bins having signal, but not background predictions, categorically
    #optimization = [[[ 5, 21], [2, 13], [12, 22]],
    #                [[ 9, 23], [4, 13], [10, 22]],
    #                [[ 8, 22], [3, 13], [13, 22]],#change
    #                [[10, 26], [4, 13], [11, 22]],
    #                [[ 9, 23], [3, 14], [10, 22]],
    #                [[ 9, 27], [4, 15], [11, 22]],
    #                [[11, 28], [2, 14], [13, 22]],
    #                [[14, 25], [1, 14], [10, 22]],
    #                [[17, 27], [7, 14], [ 8, 22]]]
    #optimization = [[[1, 50], [4, 22], [7, 22]], 
    #                [[2, 51], [5, 22], [7, 22]], 
    #                [[3, 48], [3, 22], [5, 22]], 
    #                [[2, 49], [3, 22], [5, 22]], 
    #                [[2, 50], [4, 20], [5, 22]], 
    #                [[2, 48], [3, 22], [5, 22]], 
    #                [[3, 49], [4, 22], [5, 22]], 
    #                [[3, 49], [3, 22], [5, 22]], 
    #                [[3, 51], [3, 22], [5, 21]]]
    #new ttbb limits, ps_fsrDown is bust
    #optimization =   [[[13, 61], [11, 24]],
    #                  [[ 7, 59], [ 9, 22]],
    #                  [[ 8, 65], [ 7, 24]],
    #                  [[10, 76], [ 7, 24]],
    #                  [[11, 75], [ 7, 24]],
    #                  [[12, 67], [ 7, 24]],
    #                  [[12, 67], [ 7, 24]],
    #                  [[12, 67], [ 7, 24]],
    #                  [[12, 74], [ 7, 24]]]
    #rebinning November 11 long scan, too many NLL bins, many inputs missing
    #optimization =  [[[21, 90], [2, 10], [1, 10]],
    #                 [[18, 99], [1, 10], [1, 10]],
    #                 [[25, 105], [1, 10], [1, 10]],
    #                 [[15, 106], [2, 10], [1, 10]],
    #                 [[15, 106], [4, 10], [1, 10]],
    #                 [[21, 106], [4, 9], [1, 10]],
    #                 [[22, 106], [1, 5], [1, 10]],
    #                 [[22, 106], [2, 7], [1, 10]],
    #                 [[19, 107], [1, 6], [1, 10]]]
    #optimization = [[[6, 20], [5, 20], [2,  9]],
    #                [[6, 20], [7, 20], [2,  9]],
    #                [[6, 20], [6, 20], [2,  9]],
    #                [[8, 20], [5, 20], [2, 11]],
    #                [[8, 20], [4,  9], [2,  9]],
    #                [[8, 19], [2,  9], [2, 10]],
    #                [[8, 19], [3, 20], [2, 20]],
    #                [[8, 20], [3, 20], [2, 20]],
    #                [[6, 20], [4, 20], [2, 20]]]
    #1-jet QCD included, same ST fit function pol4l for 4j and 6j, 6j4b only HT, Feb 2026
    optimization = [[[5, 20], [7, 20]],
                    [[6, 20], [4, 18]],
                    [[6, 20], [6, 20]],
                    [[6, 20], [5, 20]],
                    [[7, 20], [5, 20]],
                    [[6, 20], [6, 20]],
                    [[8, 20], [6, 20]],
                    [[8, 20], [6, 19]],
                    [[8, 20], [6, 20]]]
else:
#compromise version
    optimization = [[[7, 20], [4, 16]],
                    [[7, 20], [4, 16]],
                    [[7, 20], [4, 16]],
                    [[7, 20], [4, 16]],
                    [[7, 20], [4, 16]],
                    [[7, 20], [4, 16]],
                    [[7, 20], [4, 16]],
                    [[7, 20], [4, 16]],
                    [[7, 20], [4, 16]]]
    folder = "allYears_CompromiseSlices"

print(compromise)
print(optimization)
print(folder)

#macro to extract limits from combined cards, once set up optimized slices

os.chdir("Combination")

for mass in range(0,9):
    masses.append(float((3+mass)*100))
    massString = str((3+mass)*100)
    
    #set up combination cards
    if not skipSlicing:
        counterJet = 0
        for binN in binS:
            for year in years:
                for binX in binN:
                    #print(year,binX,mass,counterJet)
                    #print(str(optimization[mass][counterJet][0]))
                    #print(str(optimization[mass][counterJet][1]))
                    #print(massString)
                    if binX.find("15") > -1: #5 jets 2-3 tags case
                        counterJet=0
                    elif binX.find("164") > -1: #6 jets 4 tags exception (coarser NLL binning)
                        counterJet=2
                    else: #leftover 6 jets 2-3 tags cases
                        counterJet=1 
                    if not counterJet==2: #except 6j4b which isn't sliced
                        os.system("python3 ../SliceHTvsFitMass.py "+year+" "+binX+" "+str(optimization[mass][counterJet][0])+" "+str(optimization[mass][counterJet][1])+" "+massString)#+" direct")

    #combination cards across years for different bins
    AllCards = []

    for i in range(0,2):
        for bins in binS[i]:
            baseStringComb     = "combineCards.py "
            baseStringFitSlice = "combineCards.py "
            baseStringHTslice  = "combineCards.py "

            #assemble naming string
            name = ""
            #if bins[0]=="1":
            #    name+="mu"
            #elif bins[0]=="2":
            #    name+="e"
            #name+="-"+bins[2]+"j-"+bins[3]+"b-"
            #if bins[3]=="2":
            #    name+="cr"
            #else:
            #    name+="sr"
            #name+="="
            
            if not bins.find("164") > -1:
                for year in years:
                    baseStringComb     += name+"CombinationSlices_Wprime"+bins+"_"+year+"_M"+massString+".txt "
                    baseStringFitSlice += name+"FitSlice_Wprime"+bins+"_"+year+"_M"+massString+".txt "
                    baseStringHTslice  += name+"HTslice_Wprime"+bins+"_"+year+"_M"+massString+".txt "
                baseStringComb     += "> CombinationSlices_Wprime"+bins+"_all_M"+massString+".txt"
                baseStringFitSlice += "> FitSlice_Wprime"+bins+"_all_M"+massString+".txt"
                baseStringHTslice  += "> HTslice_Wprime"+bins+"_all_M"+massString+".txt"
                AllCards.append("CombinationSlices_Wprime"+bins+"_all_M"+massString+".txt ")
                AllCards.append("FitSlice_Wprime"+bins+"_all_M"+massString+".txt ")
                AllCards.append("HTslice_Wprime"+bins+"_all_M"+massString+".txt ")
                if not skipSlicing:
                    print(baseStringComb)
                    os.system(baseStringComb)
                    print(baseStringFitSlice)
                    os.system(baseStringFitSlice)
                    print(baseStringHTslice)
                    os.system(baseStringHTslice)
            else: #exception to process only the HT 1D version for the 6j4b case
                for year in years:
                    baseStringComb    += name+"HT_Wprime"+bins+"_"+year+"_M"+massString+".txt "
                    baseStringHTslice += name+"HT_Wprime"+bins+"_"+year+"_M"+massString+".txt "
                baseStringComb     += "> CombinationSlices_Wprime"+bins+"_all_M"+massString+".txt"
                baseStringHTslice  += "> HTslice_Wprime"+bins+"_all_M"+massString+".txt"
                AllCards.append("CombinationSlices_Wprime"+bins+"_all_M"+massString+".txt ")
                AllCards.append("HTslice_Wprime"+bins+"_all_M"+massString+".txt ")
                if not skipSlicing:
                    print(baseStringComb)
                    os.system(baseStringComb)
                    print(baseStringHTslice)
                    os.system(baseStringHTslice)
            

    #combination cards across bins for different years
    for year in years:
        baseStringComb     = "combineCards.py "
        baseStringFitSlice = "combineCards.py "
        baseStringHTslice  = "combineCards.py "
        for i in range(0,2):
            for bins in binS[i]:
                #assemble naming string
                name = ""
                #if bins[0]=="1":
                #    name+="mu"
                #elif bins[0]=="2":
                #    name+="e"
                #name+="-"+bins[2]+"j-"+bins[3]+"b-"
                #if bins[3]=="2":
                #    name+="cr"
                #else:
                #    name+="sr"
                #name+="="
                if not bins.find("164") > -1:
                    baseStringComb     += name+"CombinationSlices_Wprime"+bins+"_"+year+"_M"+massString+".txt "
                    baseStringFitSlice += name+"FitSlice_Wprime"+bins+"_"+year+"_M"+massString+".txt "
                    baseStringHTslice  += name+"HTslice_Wprime"+bins+"_"+year+"_M"+massString+".txt "
                else:
                    baseStringComb    += name+"HT_Wprime"+bins+"_"+year+"_M"+massString+".txt "
                    baseStringHTslice += name+"HT_Wprime"+bins+"_"+year+"_M"+massString+".txt "
        baseStringComb     += "> CombinationSlices_WprimeAll_"+year+"_M"+massString+".txt"
        baseStringFitSlice += "> FitSlice_WprimeAll_"+year+"_M"+massString+".txt"
        baseStringHTslice  += "> HTslice_WprimeAll_"+year+"_M"+massString+".txt"
        AllCards.append("CombinationSlices_WprimeAll_"+year+"_M"+massString+".txt ")
        AllCards.append("FitSlice_WprimeAll_"+year+"_M"+massString+".txt ")
        AllCards.append("HTslice_WprimeAll_"+year+"_M"+massString+".txt ")
        if not skipSlicing:
            print(baseStringComb)
            os.system(baseStringComb)
            print(baseStringFitSlice)
            os.system(baseStringFitSlice)
            print(baseStringHTslice)
            os.system(baseStringHTslice)

    AllCards.append("Optimized_WprimeAll_all_M"+massString+".txt ")


    #os.system("combineCards.py CombinationSlices_Wprime1153_2016_M"+massString+".txt CombinationSlices_Wprime1153_2017_M"+massString+".txt CombinationSlices_Wprime1153_2018_M"+massString+".txt CombinationSlices_Wprime2153_2016_M"+massString+".txt CombinationSlices_Wprime2153_2017_M"+massString+".txt CombinationSlices_Wprime2153_2018_M"+massString+".txt > CombinationSlices_Wprimex153_all_M"+massString+".txt")
    #os.system("combineCards.py CombinationSlices_Wprime1163_2016_M"+massString+".txt CombinationSlices_Wprime1163_2017_M"+massString+".txt CombinationSlices_Wprime1163_2018_M"+massString+".txt CombinationSlices_Wprime2163_2016_M"+massString+".txt CombinationSlices_Wprime2163_2017_M"+massString+".txt CombinationSlices_Wprime2163_2018_M"+massString+".txt > CombinationSlices_Wprimex163_all_M"+massString+".txt")
    #os.system("combineCards.py CombinationSlices_Wprime1164_2016_M"+massString+".txt CombinationSlices_Wprime1164_2017_M"+massString+".txt CombinationSlices_Wprime1164_2018_M"+massString+".txt CombinationSlices_Wprime2164_2016_M"+massString+".txt CombinationSlices_Wprime2164_2017_M"+massString+".txt CombinationSlices_Wprime2164_2018_M"+massString+".txt > CombinationSlices_Wprimex164_all_M"+massString+".txt")
    #combine all cards, second into the optimized subsets across bins
    
    #full combination
    if not skipSlicing:
        os.system("combineCards.py CombinationSlices_WprimeAll_2016_M"+massString+".txt CombinationSlices_WprimeAll_2016apv_M"+massString+".txt CombinationSlices_WprimeAll_2017_M"+massString+".txt CombinationSlices_WprimeAll_2018_M"+massString+".txt > Optimized_WprimeAll_all_M"+massString+".txt")

        #make a folder to copy this masspoints interpretation files to in totality
        #if not os.path.isdir(b2gpath+folder+"/"+massString):
        #    os.system("mkdir "+b2gpath+folder+"/"+massString)
        #else:
        #    os.system("rm -rf "+b2gpath+folder+"/"+massString)
        #    os.system("mkdir "+b2gpath+folder+"/"+massString)

        #print("card ",os.path.exists("Optimized_WprimeAll_all_M"+massString+".txt"))
        #card = open("Optimized_WprimeAll_all_M"+massString+".txt","r")
        #lines = card.readlines()
        #rootfiles = []
        #for line in lines:
        #    lineSplit = line.split()
        #    for lineS in lineSplit:
        #        if lineS.find(".root") > -1:
        #            rootfiles.append(lineS)

        #copy all root files in final card
        #for rootfile in rootfiles:
        #    print(rootfile,os.path.exists(rootfile))
        #    print("cp "+rootfile+" "+b2gpath+folder+"/"+massString+"/.")
        #    os.system("cp "+rootfile+" "+b2gpath+folder+"/"+massString+"/.")
        #    print(os.path.exists(b2gpath+folder+"/"+massString+"/"+rootfile))

        #copy current cards and final card
        #copyString = "cp "
        #for card in AllCards:
        #    copyString += card
        #print(copyString +b2gpath+folder+"/"+massString+"/.")
        #os.system(copyString +b2gpath+folder+"/"+massString+"/.")
    #print("cp CombinationSlices_Wprime1153_2016_M"+massString+".txt CombinationSlices_Wprime1153_2017_M"+massString+".txt CombinationSlices_Wprime1153_2018_M"+massString+".txt CombinationSlices_Wprime2153_2016_M"+massString+".txt CombinationSlices_Wprime2153_2017_M"+massString+".txt CombinationSlices_Wprime2153_2018_M"+massString+".txt CombinationSlices_Wprimex153_all_M"+massString+".txt CombinationSlices_Wprime1163_2016_M"+massString+".txt CombinationSlices_Wprime1163_2017_M"+massString+".txt CombinationSlices_Wprime1163_2018_M"+massString+".txt CombinationSlices_Wprime2163_2016_M"+massString+".txt CombinationSlices_Wprime2163_2017_M"+massString+".txt CombinationSlices_Wprime2163_2018_M"+massString+".txt CombinationSlices_Wprimex163_all_M"+massString+".txt CombinationSlices_Wprime1164_2016_M"+massString+".txt CombinationSlices_Wprime1164_2017_M"+massString+".txt CombinationSlices_Wprime1164_2018_M"+massString+".txt CombinationSlices_Wprime2164_2016_M"+massString+".txt CombinationSlices_Wprime2164_2017_M"+massString+".txt CombinationSlices_Wprime2164_2018_M"+massString+".txt CombinationSlices_Wprimex164_all_M"+massString+".txt Optimized_WprimeAll_all_M"+massString+".txt "+b2gpath+folder+"/"+massString+"/.")
    #os.system("cp CombinationSlices_Wprime1153_2016_M"+massString+".txt CombinationSlices_Wprime1153_2017_M"+massString+".txt CombinationSlices_Wprime1153_2018_M"+massString+".txt CombinationSlices_Wprime2153_2016_M"+massString+".txt CombinationSlices_Wprime2153_2017_M"+massString+".txt CombinationSlices_Wprime2153_2018_M"+massString+".txt CombinationSlices_Wprimex153_all_M"+massString+".txt CombinationSlices_Wprime1163_2016_M"+massString+".txt CombinationSlices_Wprime1163_2017_M"+massString+".txt CombinationSlices_Wprime1163_2018_M"+massString+".txt CombinationSlices_Wprime2163_2016_M"+massString+".txt CombinationSlices_Wprime2163_2017_M"+massString+".txt CombinationSlices_Wprime2163_2018_M"+massString+".txt CombinationSlices_Wprimex163_all_M"+massString+".txt CombinationSlices_Wprime1164_2016_M"+massString+".txt CombinationSlices_Wprime1164_2017_M"+massString+".txt CombinationSlices_Wprime1164_2018_M"+massString+".txt CombinationSlices_Wprime2164_2016_M"+massString+".txt CombinationSlices_Wprime2164_2017_M"+massString+".txt CombinationSlices_Wprime2164_2018_M"+massString+".txt CombinationSlices_Wprimex164_all_M"+massString+".txt Optimized_WprimeAll_all_M"+massString+".txt "+b2gpath+folder+"/"+massString+"/.")

    #run Combine for this set of cards
    #os.system("combine -M AsymptoticLimits -m "+massString+" Optimized_WprimeAll_all_M"+massString+".txt")

    #infile = TFile("higgsCombineTest.AsymptoticLimits.mH"+massString+".root","READ")

    #print("cp higgsCombineTest.AsymptoticLimits.mH"+massString+".root "+b2gpath+folder+"/"+massString+"/.")
    #os.system("cp higgsCombineTest.AsymptoticLimits.mH"+massString+".root "+b2gpath+folder+"/"+massString+"/.")
    #print(os.path.exists(b2gpath+folder+"/"+massString+"/higgsCombineTest.AsymptoticLimits.mH"+massString+".root"))

    #limitNumbers.append([])

    #for event in infile.limit:
    #    limitNumbers[mass].append(event.limit*crossSections[mass])

    #infile.Close()

#make all the Brazil plots for all years, all channels, and the final combinations
os.chdir("..")

yearsToProduce = years
yearsToProduce.append("all")

binsToProduce = []
for entry in binS:
    for ent in entry:
        binsToProduce.append(ent)
binsToProduce.append("All")

CardNamesToProduce = ["CombinationSlices", "FitSlice", "HTslice"]

if not skipLimits:
    for binN in binsToProduce:
        for yearN in yearsToProduce:
            for cardN in CardNamesToProduce:
                if not (cardN.find("fit") > -1 and binN.find("164") > -1): #except fit slices for 64 channel only runs
                    print("python3 Brazil.py "+binN+" "+yearN+" "+cardN)
                    os.system("python3 Brazil.py "+binN+" "+yearN+" "+cardN)
    
    #produce final limit plot
    print("python3 Brazil.py All all Optimized")
    os.system("python3 Brazil.py All all Optimized")

#order this for TGraph
#TwoSigmaBand = array( 'd' )
#OneSigmaBand = array( 'd' )
#central = array( 'd' )
#massBand = array( 'd' )
#for x in range(0,9):
#    central.append(limitNumbers[x][2])
#    OneSigmaBand.append(limitNumbers[x][1])
#    TwoSigmaBand.append(limitNumbers[x][0])
#    massBand.append(float((3+x)*100))
#
#for x in range(0,9):
#    OneSigmaBand.append(limitNumbers[8-x][3])
#    TwoSigmaBand.append(limitNumbers[8-x][4])
#    massBand.append(float((11-x)*100))
#        
#print(central)
#print(OneSigmaBand)
#print(TwoSigmaBand)
#print(massBand)
#
#CentralGraph = TGraph(9, masses, central)
#OneSigmaGraph = TGraph(18, massBand, OneSigmaBand)
#TwoSigmaGraph = TGraph(18, massBand, TwoSigmaBand)
#
#canvas = TCanvas("CombinedLimit","",1000,1000)
#CentralGraph.SetLineColor(1)
#CentralGraph.SetMarkerStyle(47)
#CentralGraph.SetMarkerColor(1)
#
#OneSigmaGraph.SetFillColor(8)
#OneSigmaGraph.SetMarkerStyle(1)
#
#TwoSigmaGraph.SetFillColor(5)
#TwoSigmaGraph.SetMarkerStyle(1)
#TwoSigmaGraph.GetXaxis().SetTitle("W' mass [GeV/c^{2}]")
#TwoSigmaGraph.GetYaxis().SetTitle("#sigma [fb]")
#
#TwoSigmaGraph.Draw("A,F")
#OneSigmaGraph.Draw("F,same")
#CentralGraph.Draw("P,L,same")
#
#savefile.cd()
#canvas.Write()
#canvas.SaveAs("FullOptimizationFullCombination.pdf")
#canvas.SaveAs("FullOptimizationFullCombination.root")
#os.system("cp FullOptimizationFullCombination.pdf "+b2gpath+folder+"/.")
#os.system("cp FullOptimizationFullCombination.root "+b2gpath+folder+"/.")
#savefile.Close()
