import os, sys

#determine folder path to be split and output path to put into, as well as target file size in events
#inputPath = "/eos/cms/store/group/phys_b2g/wprime/skimmed_samples"
#outputPath = "/eos/cms/store/group/phys_b2g/wprime/ReportionedSkimmedSamples"
#TargetSize = "25000"

inputPath = "/afs/cern.ch/user/r/rathjd/work/private/TakeOverWprime/wprime/Submits/Testdir"
outputPath = "/afs/cern.ch/user/r/rathjd/work/private/TakeOverWprime/wprime/Submits/OutTest"
TargetSize = "2500"

print("Attempting to reportion files in all subdirectories of " + inputPath + "to a size of " + TargetSize)

if not os.path.isdir(inputPath):
    print("Input directory does not exist -> aborting")
    exit(1)

print("Target directory is " + outputPath)

if os.path.isdir(outputPath):
    print("Output directory exists, commencing")
else:
    os.mkdir(outputPath)
    if not os.path.isdir(outputPath):
        print("Tried making directory, failed -> aborting")
        exit(1)
    print("Output directory successfully made")

#loop over subdirectories
Samples = os.listdir(inputPath)

for sample in Samples:
    subPath = inputPath + "/" + sample
    if os.path.isfile(subPath): #skip stray files
        continue
    Years = os.listdir(subPath)
    subOutputPath = outputPath + "/" + sample
    if not os.path.isdir(subOutputPath):
        os.mkdir(subOutputPath)
        if not os.path.isdir(subOutputPath):
            print("Tried making subdirectory, failed -> aborting")
            exit(1)

    #check that the output directory exists
    for year in Years:
        finalPath = subPath + "/" + year
        if os.path.isfile(finalPath): #skip stray files
            continue
        finalOutputPath = subOutputPath + "/" + year
        if not os.path.isdir(finalOutputPath):
            os.mkdir(finalOutputPath)
            if not os.path.isdir(finalOutputPath):
                print("Tried making final subdirectory, failed -> aborting")
        
        #list and run over individual files
        files = os.listdir(finalPath)
        for file in files:
            if os.path.isdir(finalPath + "/" + file): #skip stray directories
                continue

            #skip files not from root and shorten the result of the ending for easier processing
            ending = file.find(".root")
            if ending < 0:
                continue
            file = file[0:ending]

            rootCommand = "root -l -b -q 'FileProportioner.C+g(" + '"' + finalPath + '", "' + finalOutputPath + '", "' + file + '", ' + TargetSize + ")'"
            #print(rootCommand) #just redundant for testing
            os.system(rootCommand)
