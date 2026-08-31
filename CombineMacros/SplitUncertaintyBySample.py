import os,sys

from ROOT import TH1F, TH2F, TFile, gDirectory

#macro to rename uncertainty histograms to split nuisance by sample into a new file and write a new combine textfile alongside

#subfunction to split nuisances by sample
def cardSplitBySampleFile(fileName, SplitList, appendName, SampList):
    inFile = TFile(fileName,"READ")

    #generate reference histogram names from filename per sample and nuisance, then add sample name to new nuisance name
    replaceDict = {}
    for sample in SampList:
        refName = fileName
        refName = refName.replace("Combination/","")
        refName = refName.replace("../","")
        if refName.find("Fit") > -1:
            refName = refName.replace("Slices","_"+sample)
            refName = refName.replace("Wprime","WprimeFit")
        elif refName.find("HT") > -1:
            refName = refName.replace("slices","_"+sample)
            refName = refName.replace("Wprime","WprimeHT")
        refName = refName.replace(".root","_")

        for split in SplitList:
            oldName = refName
            oldName += split
            replaceDict[oldName] = oldName + "_" + sample

    #setup of input file contents and new copied file
    InFileContent = [key.GetName() for key in gDirectory.GetListOfKeys()]

    outFile = TFile(fileName[0:len(fileName)-5]+"_"+appendName+".root","RECREATE")
    outFile.cd()

    #scan through file content for variations
    for content in InFileContent:
        toRename = False
        appendix = ""
        testName = ""

        #make testable format, retain Up or Down info
        if content.find("Up") > -1:
            testName = content.replace("Up","")
            appendix = "Up"
        elif content.find("Down") > -1:
            testName = content.replace("Down","")
            appendix = "Down"
        
        #find out, whether the string is in dict
        #print(testName)
        if testName in replaceDict:
            toRename = True
            testName = replaceDict[testName] + appendix

        #load histogram, replace name when appropriate
        hist = inFile.Get(content)
        outFile.cd()
        if not toRename:
            hist.Write()
        else:
            hist.SetName(testName)
            hist.Write(testName)

    inFile.Close()
    outFile.Close()
    return True


#configure input card for smoothened version
cardname = "CRslices_WprimeAll2tag_all_M500.txt"

try:
    if sys.argv[1].find(".txt") > -1:
        cardname = sys.argv[1]
        print("input card configured as",cardname)
except:
    print("input card defaults to",cardname)

#find masspoint
mpos = cardname.find("_M")
mend = cardname.find(".txt")
mass = cardname[mpos+1:mend]

#list of samples
SignalsList = ["M300", "M400", "M500", "M600", "M700", "M800", "M900", "M1000", "M1100"]
SampList = [mass]
SampList += ["ttbar", "wjets", "single_top", "diboson", "qcd"]
Sequence = ["1  -   -   -   -   -   ",
            "-  1   -   -   -   -   ",
            "-  -   1   -   -   -   ",
            "-  -   -   1   -   -   ",
            "-  -   -   -   1   -   ",
            "-  -   -   -   -   1   "]

#list of systematics to split:
SplitList = ["ps_isr", "ps_fsr"]

#load input card with root file names
inCard =  open("Combination/"+cardname,"r")
inLines = inCard.readlines()
regionCount = 0

#output card to be written
appendName = "splitPSbySample"
outCard = open("Combination/"+cardname[0:len(cardname)-4]+"_"+appendName+".txt","w")

#loop over lines in inCard and find root file lines
for line in inLines:

    elements = line.split()

    #increase number of nuisance parameters, if necessary
    if line.find("kmax") > -1:
        count = int(elements[1]) + (len(SampList) - 1) * len(SplitList)
        line = line.replace(elements[1],str(count))

    if elements[0] == "imax":
        regionCount = int(elements[1])

    #identify relevant lines for channels
    pos = line.find("shapes *")
    if pos > -1:
        splits = line.split()
        filename = splits[3]
        regionName = splits[2]
        #split nuisances version root file gets generated
        cardSplitBySampleFile(filename, SplitList, appendName, SampList)
        #write line with new file ending
        line = line.replace(".root","_"+appendName+".root")
        line = line.replace("../Combination/","")
        outCard.write(line)
    elif elements[0] not in SplitList: #otherwise just write line into outCard
        outCard.write(line)
    else:
        #write new lines instead
        for split in SplitList:
            if elements[0] == split:
                for it, sample in enumerate(SampList):
                    newLine = split + "_" + sample + "  shape  "
                    for j in range(0, regionCount):
                        newLine += Sequence[it]

                    outCard.write(newLine + "\n")
                

#ensure that the card is written
outCard.close()

