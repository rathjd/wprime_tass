import os,sys

from ROOT import TH1F, TH2F, TFile, gDirectory

#macro to remove listed uncertainty histogram normalisations and copy into a new file, splitting off new lnN uncertainties

#subfunction to determine normalisation in input file, build new root file without normalisation, and return the normalisation values for each nuisance and channel
def NoNormFile(fileName, regionName, histList):
    inFile = TFile(fileName,"READ")

    #generate reference ttbar histogram name from filename
    refName = fileName
    refName = refName.replace("Combination/","")
    refName = refName.replace("_splitPSbySample","")
    refName = refName.replace("SimpleShapes_","")
    if refName.find("Fit") > -1:
        refName = refName.replace("Slices","_ttbar")
        refName = refName.replace("Wprime","WprimeFit")
    elif refName.find("HT") > -1:
        refName = refName.replace("slices","_ttbar")
        if refName.find("64") < 0:
            refName = refName.replace("Wprime","WprimeHT")
        else:
            refName = refName.replace("HT_", "HT_ttbar_")
    refName = refName.replace(".root","_")

    #setup of input file contents and new copied file
    InFileContent = [key.GetName() for key in gDirectory.GetListOfKeys()]

    outFile = TFile(fileName[0:len(fileName)-5]+"_noNormTtbar.root","RECREATE")
    outFile.cd()

    #find normal ttbar for the region as reference value to scale to
    refVal = -1.
    for content in InFileContent:
        if content == refName:
            hist = inFile.Get(content)
            refVal = hist.Integral(0,-1)
            break

    #catch cases where this goes wrong
    if refVal < 0.:
        print("Warning! Reference value is invalid!!!")
        print(refName)
        print(refVal)

    #define variation dict
    varDict = {}
    for name in histList:
        varDict[name] = [1., 1.] #up/down

    #scan through file content for variations
    for content in InFileContent:
        isNormed = False
        #if content is in histList, remove normalisation, then save altered version to outfile
        #if content.find("data") == -1:
        for target in histList:
            if content.find(target) > -1:
                hist = inFile.Get(content)
                scale = refVal/hist.Integral(0,-1)
                if content.find("Up") > -1:
                    varDict[target][0] = round(scale,2)
                elif content.find("Down") > -1:
                    varDict[target][1] = round(scale,2)
                hist.Scale(scale)
                outFile.cd()
                hist.Write()
                isNormed = True
                #break
        #else, save normal version to outfile
        if not isNormed:
        #else:
            hist = inFile.Get(content)
            outFile.cd()
            hist.Write()

    inFile.Close()
    outFile.Close()
    return varDict

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
mass = mass.replace("_splitPSbySample","")

#list of systematics to be smoothened:
noNormList = ["QCDscale_fac_ttbar", "QCDscale_ren_ttbar",
              "pdf_B2G25008_envelope_ttbar",
              "pdf_B2G25008_alphaS_ttbar",
              "ps_isr_ttbar", "ps_fsr_ttbar"]

#load input card with root file names
inCard =  open("Combination/"+cardname,"r")
inLines = inCard.readlines()
#output card to be written
outCard = open("Combination/"+cardname[0:len(cardname)-4]+"_noNormTtbar.txt","w")

#define dictionary of card changes
cardAddDict = {}
binOrder = []
processOrder = []

#loop over lines in inCard and find root file lines
for line in inLines:

    #increase number of nuisance parameters, if necessary
    if line.find("kmax") > -1:
        elements = line.split()
        count = int(elements[1]) + len(noNormList)
        line = line.replace(elements[1],str(count))
        #outCard.write(line)

    #identify relevant lines for channels
    #pos = line.find("../Combination")
    pos = line.find("shapes *")
    if pos > -1:
        splits = line.split()
        filename = splits[3]
        regionName = splits[2]
       # end = line.find(".root")
       # filename = line[pos+3:end+5]
       # pos2 = line.find(" ch")
       # regionName = line[pos2+1:pos2+12]
        #smoothened version root file gets generated
        print(filename)
        cardAddDict[regionName] = NoNormFile(filename, regionName, noNormList)
        #write line with new file ending
        line = line.replace(".root","_noNormTtbar.root")
        line = line.replace("../Combination/","")
        outCard.write(line)
    else: #otherwise just write line into outCard
        outCard.write(line)

    #split lines to identify elements
    elements = line.split()

    #store bins
    if elements[0] == "bin" and elements[1] == elements[2]:
        binOrder = elements[1:]

    #store processes
    if elements[0] == "process" and elements[1] == mass:
        processOrder = elements[1:]

#assemble new lines
for addition in noNormList:
    newLine = addition.replace("ttbar","norm_ttbar")
    newLine += " lnN "
    for index, channel in enumerate(binOrder):
        if processOrder[index] == "ttbar":
            scales = cardAddDict[channel][addition]
            scale = "0."
            #take the maximum scale, but use it in up-direction either way for consistent correlations
            if scales[0] > 1./scales[1]:
                scale = str(scales[0])
            else:
                scale = str(round(1./scales[1],2))
            newLine += "        "+scale
        else:
            newLine += "            -"
    outCard.write(newLine + "\n")

#ensure that the card is written
outCard.close()
