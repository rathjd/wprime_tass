import os,sys

from ROOT import TH2F, TFile, gDirectory

#macro to splice a bin between fitted mass and HT along -log(likelihood) values

#configure bin
year = "2018"
binNr = "1153"
binSplit = 8
binEnd = -1

try:
    if int(sys.argv[1]) > 0:
        year = sys.argv[1]
        print("year set to "+ year)
except:
    print("year defaults to "+ year)

try:
    if int(sys.argv[2]) > 0:
        binNr = sys.argv[2]
        print("bin number set to "+ binNr)
except:
    print("bin number defaults to "+ binNr)

try:
    if int(sys.argv[3]) > 0:
        binSplit = int(sys.argv[3])
        print("bin split set to "+str(binSplit))
except:
    print("bin split defaults to "+str(binSplit))

try:
    if int(sys.argv[4]) > 0:
        binEnd = int(sys.argv[4])
        print("bin end set to "+str(binEnd))
except:
    print("bin end defaults to "+str(binEnd))

if binSplit >= binEnd:
    print(binSplit,">=",binEnd,"! aborting")
    exit

#background name; NLL nonclosure uncertainty for mfit then HT; minimal number of events in any variation for mfit, then HT
NLLvals = [["ttbar",      0., 0., 1., 1.],
           ["wjets",      0., 0., 1., 1.],
           ["single_top", 0., 0., 1., 1.],
           ["diboson",    0., 0., 1., 1.]]

#load file with 2D distributions and get filenames
inFileSF = TFile("CombinationAll/SF_Bin"+binNr[0:3]+"2_"+year+".root","READ")
inFile = TFile("CombinationAll/TwoD_SimpleShapes_"+"Wprime"+binNr+"_"+year+".root","READ")

FileContent = [key.GetName() for key in gDirectory.GetListOfKeys()]

#define output files
outFileHT = TFile("CombinationAll/HTslices_Wprime"+binNr+"_"+year+".root","RECREATE")
outFileFit = TFile("CombinationAll/FitSlices_Wprime"+binNr+"_"+year+".root","RECREATE")

#run over keys, sort and Project them appropriately
for content in FileContent:
    if content.find("HT2D_") > -1:
        HT = inFile.Get(content)
        keyname = content.replace("HT2D_", "HT_")
        keyname = keyname.replace("Wprime"+binNr,"WprimeHT"+binNr)
        ProjHT = HT.ProjectionX(keyname, binSplit+1, binEnd)
        outFileHT.cd()
        ProjHT.Write()
        for bgr in NLLvals: #check for empty variations
            if content.find(bgr[0]) > -1:
                bgr[4] = min(bgr[4], ProjHT.Integral(0,-1))
    elif content.find("FitMass2D_") > -1:
        Fit = inFile.Get(content)
        keyname = content.replace("FitMass2D_", "Fit_")
        keyname = keyname.replace("Wprime"+binNr,"WprimeFit"+binNr)
        ProjFit = Fit.ProjectionX(keyname, 0, binSplit)
        outFileFit.cd()
        ProjFit.Write()
        for bgr in NLLvals: #check for empty variations
            if content.find(bgr[0]) > -1:
                bgr[3] = min(bgr[3], ProjFit.Integral(0,-1))
    elif content.find("HT_data_obs_") > -1:
        HT = inFile.Get(content)
        keyname = content.replace("HT_", "HT_")
        keyname = keyname.replace("Wprime"+binNr,"WprimeHT"+binNr)
        ProjHT = HT.ProjectionX(keyname, binSplit+1, binEnd)
        outFileHT.cd()
        ProjHT.Write()
    elif content.find("FitMass_data_obs_") > -1:
        Fit = inFile.Get(content)
        keyname = content.replace("FitMass_", "Fit_")
        keyname = keyname.replace("Wprime"+binNr,"WprimeFit"+binNr)
        ProjFit = Fit.ProjectionX(keyname, 0, binSplit)
        outFileFit.cd()
        ProjFit.Write()
    elif content.find("NegLogLnoBvsNegLogL_") > -1: #calculate the NLL nonclosure uncertainty by cutting on -log(L), getting the corresponding -log(L)!b values, then multiply with the residuals
        NLL = inFile.Get(content)
        ProjNLLFit = NLL.ProjectionX("NLLFit", 0,          binSplit)
        NLLFitVal = ProjNLLFit.Integral(0,-1)
        ProjNLLHT  = NLL.ProjectionX("NLLHT",  binSplit+1, binEnd)
        NLLHTval = ProjNLLHT.Integral(0,-1)
        for bgr in NLLvals:
            if content.find(bgr[0]) > -1:
                NLLres = inFileSF.Get("NLLresidual_"+binNr[0:3]+"2_"+year)
                ProjNLLFit.Multiply(NLLres)
                if NLLFitVal > 0:
                    bgr[1] = ProjNLLFit.Integral(0,-1)/NLLFitVal
                ProjNLLHT.Multiply(NLLres)
                if NLLHTval > 0:
                    bgr[2] = ProjNLLHT.Integral(0,-1)/NLLHTval


#close files
outFileHT.Close()
outFileFit.Close()
inFile.Close()
inFileSF.Close()
    

#write cards ready to be processed
FitCardIn = open("CombinationAll/FitMass_Wprime"+binNr+"_"+year+".txt","r")
HTcardIn  = open("CombinationAll/HT_Wprime"+binNr+"_"+year+".txt","r")

FitCardOut = open("CombinationAll/FitSlice_Wprime"+binNr+"_"+year+".txt","w")
HTcardOut = open("CombinationAll/HTslice_Wprime"+binNr+"_"+year+".txt","w")

#make a sliced fit card
FitLines = FitCardIn.readlines()
newFitLines = []
for FitLine in FitLines:
    if FitLine.find("SimpleShapes") > -1: #make it find the right input files and histograms
        FitLine = FitLine.replace("SimpleShapes","FitSlices")
        FitLine = FitLine.replace("$PROCESS","Fit_$PROCESS")
    if FitLine.find("bin") > -1: #give it an exclusive bin name
        FitLine = FitLine.replace("Wprime", "WprimeFit")
    if FitLine.find("rate") > -1: #deactivate empty backgrounds
        vals = FitLine.split()
        startPos = 0
        bgrIt = 0
        for val in vals:
            try:
                float(val)
            except:
                continue
            if startPos == 0: #case of signal point, no rate should be accounted for
                startPos = FitLine.find(val)+1
            else: #all backgrounds, in order
                newStartPos = FitLine[startPos:].find(val)+3+startPos
                currentPos = FitLine[startPos:].find(val)+startPos
                if NLLvals[bgrIt][3] <= 0: #deactivate backgrounds where any variation hits a 0 minimum to avoid the card from crashing
                    newString = "0 "
                    FitLine = FitLine[0:currentPos] + FitLine[currentPos:currentPos+2].replace(val,newString) + FitLine[currentPos+2:]
                bgrIt += 1
                startPos = newStartPos
    if FitLine.find("NLLnonClosure") > -1:
        vals = FitLine.split()
        startPos = 0
        bgrIt = 0
        for val in vals:
            try:
                float(val)
            except:
                continue
            if startPos == 0: #case of signal point, no unc., so ignore
                startPos = FitLine.find(val)+1
            else: #all backgrounds, in order
                newStartPos = FitLine[startPos:].find(val)+len(val)+1+startPos
                currentPos = FitLine[startPos:].find(val)+startPos
                newString = str(NLLvals[bgrIt][1])
                if len(newString) > 4:
                    newString = newString[0:4]
                while len(newString) < len(val):
                    newString += " "
                FitLine = FitLine[0:currentPos] + FitLine[currentPos:currentPos+len(newString)].replace(val,newString) + FitLine[currentPos+len(newString):]
                bgrIt += 1
                startPos = newStartPos
    newFitLines += FitLine

FitCardOut.writelines(newFitLines)
FitCardOut.close()
FitCardIn.close()

#make a sliced HT card
HTlines = HTcardIn.readlines()
newHTlines = []
for HTline in HTlines:
    if HTline.find("SimpleShapes") > -1: #make it find the right input files and histograms
        HTline = HTline.replace("HT_SimpleShapes","HTslices")
    if HTline.find("bin") > -1: #give it an exclusive bin name
        HTline = HTline.replace("Wprime", "WprimeHT")
    if HTline.find("rate") > -1: #deactivate empty backgrounds
        vals = HTline.split()
        startPos = 0
        bgrIt = 0
        for val in vals:
            try:
                float(val)
            except:
                continue
            if startPos == 0: #case of signal point, no rate should be accounted for
                startPos = HTline.find(val)+1
            else: #all backgrounds, in order
                newStartPos = HTline[startPos:].find(val)+3+startPos
                currentPos = HTline[startPos:].find(val)+startPos
                if NLLvals[bgrIt][4] <= 0: #deactivate backgrounds where any variation hits a 0 minimum to avoid the card from crashing
                    newString = "0 "
                    HTline = HTline[0:currentPos] + HTline[currentPos:currentPos+2].replace(val,newString) + HTline[currentPos+2:]
                bgrIt += 1
                startPos = newStartPos
    if HTline.find("NLLnonClosure") > -1:
        vals = HTline.split()
        startPos = 0
        bgrIt = 0
        for val in vals:
            try:
                float(val)
            except:
                continue
            if startPos == 0: #case of signal point, no unc., so ignore
                startPos = HTline.find(val)+1
            else: #all backgrounds, in order
                newStartPos = HTline[startPos:].find(val)+len(val)+1+startPos
                currentPos = HTline[startPos:].find(val)+startPos
                newString = str(NLLvals[bgrIt][2])
                if len(newString) > 4:
                    newString = newString[0:4]
                while len(newString) < len(val):
                    newString += " "
                HTline = HTline[0:currentPos] + HTline[currentPos:currentPos+len(newString)].replace(val,newString) + HTline[currentPos+len(newString):]
                bgrIt += 1
                startPos = newStartPos
    newHTlines += HTline

HTcardOut.writelines(newHTlines)
HTcardOut.close()
HTcardIn.close()

#define combined card
os.system("combineCards.py CombinationAll/FitSlice_Wprime"+binNr+"_"+year+".txt CombinationAll/HTslice_Wprime"+binNr+"_"+year+".txt > CombinationAll/CombinationSlices_Wprime"+binNr+"_"+year+".txt")
