import os,sys
from ROOT import TGraph, TFile, TTree, TCanvas

binS = "1153"

year = "2016"

varName = "FitMass"
varNameOptions = ["FitMass", "HT"]

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
    if sys.argv[3] in varNameOptions:
        varName = sys.argv[3]
        print("variable type set to "+varName)
except:
    print("variable type defaults to "+varName)

fileprefix = ""
if varName == "HT":
    fileprefix = "HT_"

#read root file with histograms and combine card for systematics sources
infile = TFile("CombinationAll/"+fileprefix+"SimpleShapes_Wprime"+binS+"_"+year+".root","read")
incard = open("CombinationAll/"+varName+"_Wprime"+binS+"_"+year+".txt","r")

#define categories and systematics
colNames = ["M500", "ttbar", "wjets", "single_top", "diboson"]
rowNames = ["LumiCorr", "LumiStat"+year, "electron"+year, "muonTrigger"+year, "muonId"+year, "muonIso"+year, "BjetTagCorr", "BjetTagUncorr"+year, "PUID"+year, "L1PreFiring"+year, "PUreweight"+year, "PDF", "LHEScale", "electronScale"+year, "electronRes"+year, "JES"+year, "JER"+year, "STfit_"+year+"_"+binS[0:3]+"2_STfit", "NLLnonClosure"+year+"_"+binS[0:3]+"2"]

#define output table file and table header
outf = open("SystTable_"+varName+"_"+binS+"_"+year+".tex", "w")

columnS = ""
for i in range(0, len(colNames)-1):
    columnS += "c|"

columnS+="c"

outf.write("\\begin{table}\n")
outf.write("  \\scriptsize\n")
outf.write("  \\begin{tabular}{l||" + columnS + "}\n")
outf.write("    \multirow{2}*{systematic} & \multicolumn{" + str(len(colNames)) + "}{c}{samples}\\\\\cline{2-" + str(len(colNames)+1) + "}\n")

columnN = "   "
for col in colNames:
    columnN += " & " + col.replace("_","\_") 
    
outf.write("    " + columnN + "\\\\\hline\hline\n")

#fill all rows with systematics values but for the last
for row in rowNames:
    rowS = "    " + row.replace("_","\_")
    if row.find("NLLnonClosure") < 0:
        for col in colNames: #exception for STfit uncertainty which is only active for ttbar
            if row.find("STfit") > -1 and col != "ttbar":
                rowS += " & 0"
            else:
                UpVar   = infile.Get(fileprefix + col + "_Wprime" + binS + "_" + year + "_" + row + "Up")
                DownVar = infile.Get(fileprefix + col + "_Wprime" + binS + "_" + year + "_" + row + "Down")
                Norm    = infile.Get(fileprefix + col + "_Wprime" + binS + "_" + year + "_")
                if Norm.Integral(0,-1) > 0.:
                    UpS     = str(round(UpVar.Integral(0,-1)/Norm.Integral(0,-1),2))
                    DownS   = str(round(DownVar.Integral(0,-1)/Norm.Integral(0,-1),2))
                    rowS   += " & $^{" + UpS + "}_{" + DownS + "}$"
                else:
                    rowS   += " & 0 "
    else: #need an exception for the case of the NLL nonclosure, which is an lnN uncertainty in the card, not a shape uncertainty like all the others
        for line in incard.readlines():
            if line.find("NLLnonClosure") > -1:
                lineSplit = line.split()
                for i in range(0, len(colNames)):
                    rowS+= " & " + lineSplit[i+2]
    rowS += " \\\\\hline\n"
    outf.write(rowS)


#define table end
outf.write("  \end{tabular}\n")
outf.write("  \caption{Systematics size in normalization table for " + varName + " in bin " + binS + " " + year + ". Super(sub)scripts indicate up(down) variations of a systematic.}\n")
outf.write("  \label{tab:Syst_" + varName + "_" + binS + "_" + year + "}\n")
outf.write("\end{table}")
