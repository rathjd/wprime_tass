import os,sys
from ROOT import TGraph, TFile, TTree, TCanvas

binS = "1153"

year = "2016"

varName = "FitSlice"
varNameOptions = ["FitMass", "HT", "FitSlice", "HTslice"]

mass = "300"

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

try:
    if sys.argv[4] and int(sys.argv[4]):
        mass = sys.argv[4]
        print("mass set to "+mass)
except:
    print("mass defaults to "+mass)

if varName == "FitMass":
    fileprefix = "SimpleShapes"
    rootprefix = ""
    regionadd = ""
elif varName == "HT":
    fileprefix = "HT_SimpleShapes"
    rootprefix = "HT_"
    regionadd = ""
elif varName == "FitSlice":
    fileprefix = "FitSlices"
    rootprefix = "Fit_"
    regionadd = "Fit"
elif varName == "HTslice":
    fileprefix = "HTslices"
    rootprefix = "HT_"
    regionadd = "HT"

if binS == "1153":
    chan = "mu5j2b"
elif binS == "2153":
    chan = "e5j2b"
elif binS == "1163" or binS == "1164":
    chan = "mu6j2b"
else:
    chan = "e6j2b"

#read root file with histograms and combine card for systematics sources
infile = TFile("CombinationAll/"+fileprefix+"_Wprime"+binS+"_"+year+"_M"+mass+".root","read")
incard = open("CombinationAll/"+varName+"_Wprime"+binS+"_"+year+"_M"+mass+".txt","r")

#define categories and systematics
colNames = [["M"+mass, True],
            ["ttbar", True],
            ["wjets", True],
            ["single_top", True],
            ["diboson", True]]
rowNames = [["LumiCorr", "lumi_13TeV_correlated"],
            ["LumiStat"+year, "lumi_"+year],
            ["electron"+year, "CMS_eff_e_" + year],
            ["muonTrigger"+year, "CMS_eff_m_trigger_"+year],
            ["muonId"+year, "CMS_eff_m_id_"+year],
            ["muonIso"+year, "CMS_eff_m_iso_"+year],
            ["BjetTagCorr", "CMS_btag_comb"],
            ["BjetTagUncorr"+year, "CMS_eff_b_"+year],
            ["PUID"+year, "CMS_eff_j_PUJET_id_"+year],
            ["L1PreFiring"+year, "CMS_l1_ecal_prefiring_"+year],
            ["PUreweight"+year, "CMS_pileup_"+year],
            ["PDF", "pdf_B2Gxxyyy_envelope"],
            ["LHEScale", "QCDscale_ttbar"],
            ["electronScale"+year, "CMS_scale_e_"+year],
            ["electronRes"+year, "CMS_res_e_"+year],
            ["JES"+year, "CMS_scale_j_"+year],
            ["JER"+year, "CMS_res_j_"+year],
            ["STfit_"+year+"_"+binS[0:3]+"2_STfit", "CMS_B2Gxxyyy_STfit_"+year+"_"+chan],
            ["NLLnonClosure"+year+"_"+binS[0:3]+"2", "CMS_B2Gxxyyy_NLLnonClosure_"+year+"_"+chan]]

#define output table file and table header
outf = open("SystTable_"+varName+"_"+binS+"_"+year+"_M"+mass+".tex", "w")

#deactivate deactivated backgrounds
cardContent = incard.readlines()
for line in cardContent:
    if line.find("rate") > -1:
        lineSplit = line.split()
        for i in [i for i, split in enumerate(lineSplit) if split == "0"]:
                colNames[i-1][1]=False

columnS = ""
colCounter = 0
for col in colNames:
    if not col[1]:
        continue
    columnS += "c|"
    colCounter+=1

columnS+="c"

outf.write("\\begin{table}\n")
outf.write("  \\scriptsize\n")
outf.write("  \\begin{tabular}{l||" + columnS + "}\n")
outf.write("    \multirow{2}*{systematic} & \multicolumn{" + str(colCounter) + "}{c}{samples}\\\\\cline{2-" + str(colCounter+1) + "}\n")

columnN = "   "
for col in colNames:
    if not col[1]:
        continue
    columnN += " & " + col[0].replace("_","\_")
    
outf.write("    " + columnN + "\\\\\hline\hline\n")

#fill all rows with systematics values but for the last
for row in rowNames:
    rowS = "    " + row[1].replace("_","\_")
    if row[0].find("NLLnonClosure") < 0 and row[0].find("electronScale") < 0:
        for col in colNames: #exception for STfit uncertainty which is only active for ttbar
            if not col[1]:
                continue
            if row[0].find("STfit") > -1 and col[0] != "ttbar":
                rowS += " & 0"
            else:
                stringAssembly = rootprefix + col[0] + "_Wprime" + regionadd + binS + "_" + year + "_M" + mass + "_"
                UpVar   = infile.Get(stringAssembly + row[0] + "Up")
                DownVar = infile.Get(stringAssembly + row[0] + "Down")
                Norm    = infile.Get(stringAssembly)
                if Norm.Integral(0,-1) > 0.:
                    UpS     = str(round(UpVar.Integral(0,-1)/Norm.Integral(0,-1),2))
                    DownS   = str(round(DownVar.Integral(0,-1)/Norm.Integral(0,-1),2))
                    rowS   += " & $^{" + UpS + "}_{" + DownS + "}$"
                else:
                    rowS   += " & 0 "
    else: #need an exception for the case of the NLL nonclosure, which is an lnN uncertainty in the card, not a shape uncertainty like all the others
        for line in cardContent:
            if line.find("NLLnonClosure") > -1 and row[0].find("NLLnonClosure") > -1:
                lineSplit = line.split()
                for i in [i for i, col in enumerate(colNames) if col[1]]:
                    rowS+= " & " + lineSplit[i+2]
            elif line.find("electronScale") > -1 and row[0].find("electronScale") > -1:
                lineSplit = line.split()
                for i in [i for i, col in enumerate(colNames) if col[1]]:
                    rowS+= " & " + lineSplit[i+2]

    rowS += " \\\\\hline\n"
    outf.write(rowS)


#define table end
outf.write("  \end{tabular}\n")
outf.write("  \caption{Systematics size in normalization for " + varName + " in " + chan + " " + year + " at $m_{\\textrm{W'}}=$"+mass+"\,GeV. Super(sub)scripts indicate up(down) variations.}\n")
outf.write("  \label{tab:Syst_" + varName + "_" + binS + "_" + year + "_M" + mass +"}\n")
outf.write("\end{table}")
