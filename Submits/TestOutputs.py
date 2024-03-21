import os,sys
from ROOT import TTree, TProfile, TFile

#configuration
SubmitterName = "rathjd"

#resembles Sifu's C++ configuration in Utilities/Dataset.cc

years = [["2016", "1"],
         ["2016apv", "0"],
         ["2017", "2"],
         ["2018", "3"]]

#dataset name, dataset position for Validation.cc, queue
Datasets=[["SingleElectron", 0, "tomorrow"],
          ["SingleMuon", 1, "tomorrow"],
          ["ttbar", 2, "nextweek"],
          ["wjets_HT_70_100", 3, "workday"],
          ["wjets_HT_100_200", 4, "workday"],
          ["wjets_HT_200_400", 5, "workday"],
          ["wjets_HT_400_600", 6, "workday"],
          ["wjets_HT_600_800", 7, "workday"],
          ["wjets_HT_800_1200", 8, "workday"],
          ["wjets_HT_1200_2500", 9, "workday"],
          ["wjets_HT_2500_inf", 10, "workday"],
          ["single_top_schan", 11, "tomorrow"],
          ["single_top_tchan", 12, "tomorrow"],
          ["single_antitop_tchan", 13, "tomorrow"],
          ["single_top_tw", 14, "tomorrow"],
          ["single_antitop_tw", 15, "tomorrow"],
          ["WW", 16, "workday"],
          ["ZZ", 17, "workday"],
          ["WZTo1L1Nu2Q", 18, "workday"],
          ["WZTo1L3Nu", 19, "workday"],
          ["WZTo2Q2L", 20, "workday"],
          ["WZTo3LNu", 21, "workday"],
          ["FL300", 22, "nextweek"],
          ["FL400", 23, "nextweek"],
          ["FL500", 24, "nextweek"],
          ["FL600", 25, "nextweek"],
          ["FL700", 26, "nextweek"],
          ["FL800", 27, "nextweek"],
          ["FL900", 28, "nextweek"],
          ["FL1000", 29, "nextweek"],
          ["FL1100", 30, "nextweek"],
          ["LL300", 31, "nextweek"],
          ["LL400", 32, "nextweek"],
          ["LL500", 33, "nextweek"],
          ["LL600", 34, "nextweek"],
          ["LL700", 35, "nextweek"],
          ["LL800", 36, "nextweek"],
          ["LL900", 37, "nextweek"],
          ["LL1000", 38, "nextweek"],
          ["LL1100", 39, "nextweek"]]

#configure input files and output files locations
inputPath = "/eos/cms/store/group/phys_b2g/wprime/skimmed_samples/"
outputPath = "/eos/cms/store/group/phys_b2g/wprime/SifuFW_Fitted/"

#read current queue status
if os.path.isfile("queue.txt"):
    os.system("rm queue.txt")
os.system("condor_q > queue.txt")
queue = open("queue.txt", "r")
queueLines = queue.readlines()

CurrentlyRunning = []
for line in queueLines:
    if line.find(SubmitterName) < 0:
        continue
    if line.find("Total") > -1:
        continue
    #find year + dataset name for job
    start = line.find(" ") + 1
    end = line[start:-1].find(" ") + start
    TypeRunning = line[start:end]

    #test it is not a single job
    Singlet = TypeRunning.find("%")
    if Singlet < 0:

        #find job ID
        IDstart = line.rfind(" ") + 1
        IDend = line.rfind(".")

        #find nobatch instances running for that job ID
        if os.path.isfile("nobatch.txt"):
            os.system("rm nobatch.txt")
        os.system("condor_q --nobatch " + line[IDstart:IDend] + " > nobatch.txt")
        nobatch = open("nobatch.txt", "r")
        nobatchLines = nobatch.readlines()
        IDsRunning = []
        for nobatchLine in nobatchLines:
            if nobatchLine.find(SubmitterName) < 0:
                continue
            SubIdxStart = nobatchLine.find(".") + 1
            SubIdxEnd = nobatchLine.find(" ")
            IDsRunning.append(int(nobatchLine[SubIdxStart:SubIdxEnd]))

    else:
        #single job handling is simple
        IDsRunning = []
        IDsRunning.append(int(TypeRunning[Singlet+1:len(TypeRunning)]))
        TypeRunning = TypeRunning[0:Singlet]

    #this list contains the type of file running and all IDs currently running for that filetype
    found = False
    for CR in CurrentlyRunning:
        if CR[0] == TypeRunning:
            for job in IDsRunning:
                CR[1].append(job)
            found = True
            break
    if not found:
        CurrentlyRunning.append([TypeRunning, IDsRunning])

print(CurrentlyRunning)

#loop over years and datasets to find missing and incomplete files
for year in years:
    if year[0] != "2016apv": #FIXME: For a single year for now
        continue
    for dataset in Datasets:
        print("__________Now testing " + year[0] + " " + dataset[0] + "__________")
        filesPath = outputPath + year[0] + "_" + dataset[0]
        files = os.listdir(filesPath)
        with open("../filenames/" + dataset[0] + "_" + year[0] + ".txt", "r") as filelist:
            filenames = filelist.read().splitlines()
        currentFileIndex = 0
        completeIndices = []
        skipIndices = []

        #test for already running job indices so they can be ignored
        jobTitle = year[0] + "_" + dataset[0]
        for running in CurrentlyRunning:
            if running[0] == jobTitle:
                skipIndices = running[1]
                break

        #look for files in folder already existing and whether they are complete
        for file in files:
            filePath = filesPath + "/" + file
            if not os.path.isfile(filePath):
                continue
            indexStart = file.rfind("_") + 1
            indexEnd = file.find(".")
            #extract exact filenumber in filelist
            fileIdx = int(file[indexStart:indexEnd])

            #make sure to avoid testing running job files
            if fileIdx in skipIndices:
                continue

            #open input file, get entries
            currentFile = filenames[fileIdx]

            print(filePath," vs ",filenames[fileIdx])

            try:
                TFile(currentFile,"READ")
            except:
                print("FAILED ! inFile not readable")
                continue

            inFile = TFile(currentFile,"READ")

            try:
                inFile.Get("Events")
            except:
                print("FAILED ! inFile has no tree called Events")
                continue

            inTree = inFile.Get("Events")

            #open output file, get TProfile from it, find entries run over
            try:
                TFile(filePath, "READ")
            except:
                print("FAILED ! outFile not readable")
                continue

            outFile = TFile(filePath, "READ")

            try:
                outFile.Get("CutflowSkim")
            except:
                print("FAILED ! outFile has no TProfile called CutflowSkim")
                continue

            outCutflow = outFile.Get("CutflowSkim")

            try:
                outCutflow.GetBinEntries(1)
            except:
                print("FAILED ! Cutflow inaccessible object")
                continue

            try:
                outFile.Get("t")
            except:
                print("FAILED ! outFile has no tree")
                continue


            if int(outCutflow.GetBinEntries(1)) == inTree.GetEntries():
                print("PASSED")
                completeIndices.append(fileIdx)
            else:
                print("FAILED")

            inFile.Close()
            outFile.Close()

        #final result for the dataset, checkup
        print("-----SKIPPED currently running-----")
        print(skipIndices)
        print("-_-_-COMPLETED successfully-_-_-")
        print(completeIndices)

        #make list of missing indices for this dataset
        if len(skipIndices) + len(completeIndices) != len(filenames):
            print("indices missing from " + year[0] + " " + dataset[0] + ":")
            missingIndices = []
            for i in range(len(filenames)):
                if i in skipIndices:
                    continue
                if i in completeIndices:
                    continue
                missingIndices.append(i)
            
            print("_____FAILED to run for any reason_____")
            print(missingIndices)
            
            #make sure the submission file is locally present, just in case, as well as that the folder exists
            if not os.path.isdir("SingleSubmissions"):
                os.mkdir("SingleSubmissions")

            if not os.path.isfile("SingleSubmissions/Submit.sh"):
                os.system("cp Submit.sh SingleSubmissions/.")

            #make new submission files
            for miss in missingIndices:
                lines = []
                runname = year[0] + "_" + dataset[0] + "%" + str(miss)
                setname = year[0] + "_" + dataset[0]
                logname = setname + "_" + str(miss)
                lines.append("arguments    = " + str(miss) + " " + year[1] + " " + str(dataset[1]) + " 0\n")
                lines.append("executable   = Submit.sh\n")
                lines.append("max_retries  = 10\n")
                lines.append("batch_name   = " + runname +"\n")
                lines.append("output       = ../logs/"+setname+"/"+logname+".out\n")
                lines.append("error        = ../logs/"+setname+"/"+logname+".err\n")
                lines.append("log          = ../logs/"+setname+"/"+logname+".log\n")
                lines.append("universe     = vanilla\n")
                lines.append('Requirements = (OpSysAndVer =?= "AlmaLinux9")\n')
                lines.append('+JobFlavour  = "' + dataset[2]  + '"\n')
                lines.append("RequestCpus  = 2\n")
                lines.append("stream_error = True\n")
                lines.append("periodic_release =  (NumJobStarts < 10) && ((CurrentTime - EnteredCurrentStatus) > (5*60))\n")
                lines.append("queue 1\n")
                
                subFile = open("SingleSubmissions/" + year[0] + "_" + dataset[0] + "_" + str(miss) + ".sub", "w")
                subFile.writelines(lines)


