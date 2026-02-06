import os,sys
from ROOT import TFile, TH3F, TH2F
import math

#macro to optimize slicing working points

crossSections = [683.8+708.3, 321.7+336.1, 161.1+165.3, 85.92+85.82, 48.84+47.47, 29.81+27.73, 18.33+16.49, 11.73+10.25, 7.683+6.546]

OptimiBins = [[[1153, 2153], ["2016", "2016apv", "2017", "2018"]],
              [[1163, 2163], ["2016", "2016apv", "2017", "2018"]]]
              #[[1164, 2164], ["2016", "2016apv", "2017", "2018"]]]

saveFile = TFile("LegoOptimization.root","RECREATE")

FinalList = []

#maximum number of NLL bins depending on jet multiplicity, directly read from binning input file
maxNLL5 = 0
maxNLL6 = 0
binsFile = open("BinTables.C","r")
binsLines = binsFile.readlines()
for line in binsLines:
    splitLine = line.split()
    if len(splitLine) < 2:
        continue
    if splitLine[1].find("nNLLlimits53_500") > -1:
        splitLine[3]=splitLine[3].rstrip(";")
        maxNLL5 = int(splitLine[3])
    if splitLine[1].find("nNLLlimits64_500") > -1:
        splitLine[3]=splitLine[3].rstrip(";")
        maxNLL6 = int(splitLine[3])

print("number of NLL bins 5:",maxNLL5,"; 6:",maxNLL6)

#loop over all masses for optimization
for mass in range(0,9):
    massP = (3+mass)*100
    FinalList.append([])
    for OptimiBin in OptimiBins:
        print(OptimiBin)

        #set up matrices with file contents
        OptiMatrix = [[]]
        if OptimiBin[0][0] % 100 < 60: #5-jet bin
            OptiMatrix = [[0 for i in range(maxNLL5)] for j in range(maxNLL5)]
        else: #6-jet bin
            OptiMatrix = [[0 for i in range(maxNLL6)] for j in range(maxNLL6)]

        #fill the matrix with 1/expected limits^2
        for binN in OptimiBin[0]:
            for year in OptimiBin[1]:
                try:
                    inFile = TFile(str(year)+"_"+str(binN)+"_"+str(massP)+"_combineTmp/LimitSplits_M"+str(massP)+"_"+str(binN)+"_"+year+".root","READ")
                    FullHist = inFile.Get("CombHist_"+str(massP))
                    FullHist.GetZaxis().SetRange(3,3)
                    Expected = FullHist.Project3D("yx")
                    #FullHist.GetZaxis().SetRange(2,2)
                    #Min1Sig = FullHist.Project3D("yx")
                    #FullHist.GetZaxis().SetRange(4,4)
                    #Plu1Sig = FullHist.Project3D("yx")
                    for xc in range(0,Expected.GetNbinsX()):
                        for yc in range(0,Expected.GetNbinsY()):
                            if Expected.GetBinContent(xc+1,yc+1) > 0 and OptiMatrix[xc][yc] >= 0.:
                                OptiMatrix[xc][yc] += pow(Expected.GetBinContent(xc+1,yc+1),-2)
                            else: #this exception invalidates points which for one channel or year have insufficient background/signal coverage
                                OptiMatrix[xc][yc] = -1.
                    inFile.Close()
                            
                except:
                    print(str(year)+"_"+str(binN)+"_"+str(massP)+"_combineTmp/LimitSplits_M"+str(massP)+"_"+str(binN)+"_"+str(year)+".root not found, skipping")
        
        #Combine expected limits, find lowest value
        BestLimit = [0, 0, 100.] #binSplit, binEnd, r
        Lego5 = TH2F("Lego5j_Wp"+str(massP)+"_"+str(OptimiBin[0][0])+"_"+str(OptimiBin[0][1]),"optimization mass "+str(massP)+" in bins "+str(OptimiBin[0][0])+" and "+str(OptimiBin[0][1])+";binEnd;binSplit;95% CL [fb]", maxNLL5, -0.5, -0.5 + maxNLL5, maxNLL5, -0.5, -0.5 + maxNLL5)
        Lego6 = TH2F("Lego6j_Wp"+str(massP)+"_"+str(OptimiBin[0][0])+"_"+str(OptimiBin[0][1]),"optimization mass "+str(massP)+" in bins "+str(OptimiBin[0][0])+" and "+str(OptimiBin[0][1])+";binEnd;binSplit;95% CL [fb]", maxNLL6, -0.5, -0.5 + maxNLL6, maxNLL6, -0.5, -0.5 + maxNLL6)

        for xc in range(0, len(OptiMatrix)):
            for yc in range(0, len(OptiMatrix[xc])):
                if OptiMatrix[xc][yc] > 0:
                    val = 1./math.sqrt(OptiMatrix[xc][yc])
                    if OptimiBin[0][0] % 100 < 60:
                        Lego5.SetBinContent(yc+1,xc+1,val*crossSections[mass])
                    else:
                        Lego6.SetBinContent(yc+1,xc+1,val*crossSections[mass])
                    if BestLimit[2] > val:
                        BestLimit = [xc, yc, val]

        print("Best value ",BestLimit," for m(Wp)=",massP," with limit ",crossSections[mass]*BestLimit[2])
        FinalList[len(FinalList)-1].append([BestLimit[0], BestLimit[1]])
        if OptimiBin[0][0] % 100 < 60:
            saveFile.cd()
            Lego5.GetZaxis().SetRangeUser(crossSections[mass]*BestLimit[2]*0.9, Lego5.GetMaximum()*1.1)
            Lego5.Write()
        else:
            saveFile.cd()
            Lego6.GetZaxis().SetRangeUser(crossSections[mass]*BestLimit[2]*0.9, Lego6.GetMaximum()*1.1)
            Lego6.Write()

saveFile.Close()
            
print(FinalList)

