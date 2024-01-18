#PyROOT macro to test proportioned files for full validity after copying

import os,sys
from ROOT import TChain

inputPath = "/eos/cms/store/group/phys_b2g/wprime/skimmed_samples"
outputPath = "/eos/cms/store/group/phys_b2g/wprime/ReportionedSkimmedSamples"

print("Testing files reproportioned out of " + inputPath + " into " + outputPath)

if not os.path.isdir(inputPath):
    print("Input directory does not exist -> aborting")
    exit(1)

if not os.path.isdir(outputPath):
    print("Output directory does not exist -> aborting")
    exit(1)

#loop over subdirectories
Samples = os.listdir(inputPath)

for sample in Samples:
    subPath = inputPath + "/" + sample
    if os.path.isfile(subPath): #skip stray files
        continue
    Years = os.listdir(subPath)
    subOutputPath = outputPath + "/" + sample
    if not os.path.isdir(subOutputPath):
        print("Could not find subdirectory, failed -> aborting")
        exit(1)

    #check that the output directory exists
    for year in Years:
        finalPath = subPath + "/" + year
        if os.path.isfile(finalPath): #skip stray files
            continue
        if year != "2017": #FIXME: ONLY RUN ON 2017
            continue
        finalOutputPath = subOutputPath + "/" + year
        if not os.path.isdir(finalOutputPath):
            print("Could not find final subdirectory, failed -> aborting")
            exit(1)

        #list and run over individual files
        files = os.listdir(finalPath)
        for file in files:
            if os.path.isdir(finalPath + "/" + file): #skip stray directories
                continue

            #Make input TChain
            inChain = TChain("Events")
            inChain.Add(finalPath + "/" + file)

            #skip files not from root and shorten the result of the ending for easier processing
            ending = file.find(".root")
            if ending < 0:
                continue

            #prepare output TChain
            outChain = TChain("Events")

            #check for already existing files and add them to the comparison TChain
            targetFiles = os.listdir(finalOutputPath)
            for targetFile in targetFiles:
                endingTarget = targetFile.find("_")
                if endingTarget < 0:
                    continue
                if targetFile[0:endingTarget] == file[0:ending]:
                    outChain.Add(finalOutputPath + "/" + targetFile)

            #test same event size
            if not inChain.GetEntries() == outChain.GetEntries():
                print(file + " does not contain the same event number as ", targetFiles)
                print(inChain.GetEntries())
                print(outChain.GetEntries())

        #confirm all files okay
        print(finalOutputPath + " is correct and complete")
