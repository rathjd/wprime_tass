import os, sys
import ROOT
import math

#define the input names
signalNames = ["M$MASS"] #$MASS gets replaced by runing combine with -m option, supports only 1 signal mass at a time
bgrNames = ["ttbar", "wjets", "single_top", "diboson"]
binNumber = 1153 #this needs to be set, valid entries are 1150, 1160, 2150, 2160
yearNumber = 2018

try:
    if int(sys.argv[1]) > 0:
        binNumber = int(sys.argv[1])
        print("bin number set to "+str(binNumber))
except:
    print("bin number defaults to "+str(binNumber))

try:
    if int(sys.argv[2]) > 0:
        yearNumber = int(sys.argv[2])
        print("year number set to "+str(yearNumber))
except:
    print("year number defautls to "+str(yearNumber))



binString = str(binNumber)
yearName = str(yearNumber)

#First, generate the shape variation histograms
print("Starting processing of intermediate files")
if os.path.isdir("TestHistograms"):
  #os.system("rm TestHistograms/*.root") #reset
  print("already exists")
else:
  os.system("mkdir TestHistograms") #make directory, if it doesn't exist

#run dump from intermediate tree including ST correction cycle, generates 3- and 4-tag regions both
os.system("root -l -b -q 'runCombineHistogramDumpster.C+(" + str(binNumber) + ", " + yearName + ")'")

bins = [binString[0:3]+"1", binString[0:3]+"2", binString[0:3]+"3", binString[0:3]+"4"]

print(bins)

for binN in bins:
  fileName = binN + "_" + yearName
  binName = "Wprime" + binN + "_" + yearName
  print(binName," ",fileName)

  if os.path.isdir(fileName):
      print(fileName," directory already exists, removing it")
      os.system("rm -rf " + fileName)
  os.system("mkdir " + fileName)

  #assemble results
  if yearNumber != 2016:
    os.system("hadd -f " + fileName + "/SimpleShapes_" + binName + ".root TestHistograms/SimpleShapes_Bin" + fileName + "_*.root") #hadd all histograms to a convenient combined file
    os.system("hadd -f " + fileName + "/HT_SimpleShapes_" + binName + ".root TestHistograms/HT_SimpleShapes_Bin" + fileName + "_*.root") #hadd all histograms to a convenient combined file
    os.system("hadd -f " + fileName + "/TwoD_SimpleShapes_" + binName + ".root TestHistograms/TwoD_SimpleShapes_Bin" + fileName + "_*.root") #hadd all histograms to a convenient combined file 
  else: #2016 has to be merged with 2016apv, which is handled by this exception
    fileNameAlt = fileName + "apv"
    os.system("hadd -f " + fileName + "/SimpleShapes_" + binName + ".root TestHistograms/SimpleShapes_Bin" + fileName + "_*.root TestHistograms/SimpleShapes_Bin" + fileNameAlt + "_*.root") #hadd all histograms to a convenient combined file
    os.system("hadd -f " + fileName + "/HT_SimpleShapes_" + binName + ".root TestHistograms/HT_SimpleShapes_Bin" + fileName + "_*.root TestHistograms/HT_SimpleShapes_Bin" + fileNameAlt + "_*.root") #hadd all histograms to a convenient combined file
    os.system("hadd -f " + fileName + "/TwoD_SimpleShapes_" + binName + ".root TestHistograms/TwoD_SimpleShapes_Bin" + fileName + "_*.root TestHistograms/TwoD_SimpleShapes_Bin" + fileNameAlt + "_*.root") #hadd all histograms to a convenient combined file

  #transfer SF files where appropriate
  if int(binN) % 10 == 2:
      os.system("cp TestHistograms/SF_Bin"+binString[0:3]+"1_" + yearName + ".root " + fileName + "/.")
      print("cp TestHistograms/SF_Bin"+binString[0:3]+"1_" + yearName + ".root " + fileName + "/.")
  elif int(binN) % 10 > 2:
      os.system("cp TestHistograms/SF_Bin"+binString[0:3]+"2_" + yearName + ".root " + fileName + "/.")
      print("cp TestHistograms/SF_Bin"+binString[0:3]+"2_" + yearName + ".root " + fileName + "/.")

  #skip making cards for 1- and 2-tag regions
  if int(binN) % 10 < 3:
      continue

  #define correlated entities for usage in card
  #https://twiki.cern.ch/twiki/bin/view/CMS/LumiRecommendationsRun2#Combination_and_correlations is now a shape uncertainty built into histograms in the file

  #define all the systematic names, types, and values
  systNames = ["LumiCorr", "LumiStat"+yearName, "electron"+yearName, "muonTrigger"+yearName, "muonId"+yearName, "muonIso"+yearName, "BjetTagCorr", "BjetTagUncorr"+yearName, "PUID"+yearName, "L1PreFiring"+yearName, "PUreweight"+yearName, "PDF",   "LHEScale", "electronScale"+yearName, "electronRes"+yearName, "JES"+yearName, "JER"+yearName, "STfit_"+yearName+"_"+binString[0:3]+"2_STfit", "NLLnonClosure"+yearName+"_"+binString[0:3]+"2"]
  systTypes = ["shape",    "shape",             "shape",             "shape",                "shape",           "shape",            "shape",       "shape",                  "shape",         "shape",                "shape",               "shape", "shape",    "lnN",                    "shape",                "shape",        "shape",        "shape",                                  "lnN"] 
  systVals  = ["1",        "1",                 "1",                 "1",                    "1",               "1",                "1",           "1",                      "1",             "1",                    "1",                   "1",     "1",        "1",                      "1",                    "1",            "1",            "0",                                      "0"]

  #write the actual combine cards
  CardNames = [["FitMass", ""], ["HT", "HT_"]] #by default makes both the fit mass and HT extraction cards, separately
  for massBin in range(3,12):
    signalNames = ["M" + str(massBin*100)]
    for CardName in CardNames:
      print("Creating Combine card file",fileName + "/" + CardName[0] + "_" + binName + "_M" + str(massBin*100) + ".txt")
      f = open(fileName + "/" + CardName[0] + "_" + binName + "_M" + str(massBin*100) + ".txt","w")
      f.write("imax " + str(1) + "\n") #number of channels
      print("imax " + str(1) + "\n")
      f.write("jmax " + str(len(bgrNames)) + "\n") #number of backgrounds
      print("jmax " + str(len(bgrNames)) + "\n")
      f.write("kmax " + str(len(systNames)) + "\n") #number of nuisance parameters
      print("kmax " + str(len(systNames)) + "\n")
      f.write("----------\n")
      print("----------\n")
      f.write("shapes * * " + CardName[1] + "SimpleShapes_" + binName + ".root "+ CardName[1] + "$PROCESS_$CHANNEL_M" + str(massBin*100) + "_ " + CardName[1] + "$PROCESS_$CHANNEL_M" + str(massBin*100) + "_$SYSTEMATIC\n")
      print("shapes * * " + CardName[1] + "SimpleShapes_" + binName + ".root "+ CardName[1] + "$PROCESS_$CHANNEL_M" + str(massBin*100) + "_ " + CardName[1] + "$PROCESS_$CHANNEL_M" + str(massBin*100) + "_$SYSTEMATIC\n")
      f.write("----------\n")
      print("----------\n")
      f.write("bin         " + binName + "\n")
      print("bin         " + binName + "\n")

      #load ROOT file, find observation number
      print("Reading observed events numbers")
      r = ROOT.TFile.Open(fileName + "/" + CardName[1] + "SimpleShapes_" + binName + ".root", "read")
      print(fileName + "/" + CardName[1] + "SimpleShapes_" + binName + ".root")
      h = r.Get(CardName[1]+"data_obs_" + binName + "_M" + str(massBin*100) + "_")
      print("data_obs_" + binName + "_M" + str(massBin*100) + "_")
      print(type(h))
      observed = h.Integral()
      f.write("observation " + str(observed) + "\n")
      print("observation " + str(observed) + "\n")
      f.write("----------\n")
      print("----------\n")

      ##assemble strings for lines
      print("Assembling lines for Combine card ",CardName[0])
      #systematic lines first, as this is an entire column to be processed, later, and they are longest
      systLines = []
      maxLength = 0
      for i in range(0, len(systNames)): #assemble systematic names
        systLines.append(systNames[i])
        maxLength = max(maxLength, len(systLines[i]))

      maxLength2 = 0
      for i in range(0, len(systLines)): #align systematic names, then assemble systematic types
        while len(systLines[i]) < (maxLength + 3):
          systLines[i] += " "
        systLines[i] += systTypes[i]
        maxLength2 = max(maxLength2, len(systLines[i]))

      for i in range(0, len(systLines)): #align syst types
        while len(systLines[i]) < (maxLength2 + 5):
          systLines[i] += " "

      maxLength2 = len(systLines[0])

      #assemble bin and process block
      allNames = signalNames + bgrNames
      allNumbers = []
      for i in range(-len(signalNames)+1, 1): #negative and zero numbers for signals
        allNumbers.append(i)
      for i in range (1, len(bgrNames)+1): #positive nonzero numbers for backgrounds
        allNumbers.append(i)
      binLine      = "bin     "
      processLine1 = "process "
      processLine2 = "process "
      rateLine     = "rate    "

      #align bin, process, and rate lines with systematic line length
      while len(binLine) < maxLength2:
        binLine += " "
      while len(processLine1) < maxLength2:
        processLine1 += " "
      while len(processLine2) < maxLength2:
        processLine2 += " "
      while len(rateLine) < maxLength2:
        rateLine += " "

      for i in range(0, len(allNames)): #assemble bin, process, rate, and systematic line entries, then align
        binLine += binName
        processLine1 += allNames[i]
        processLine2 += str(allNumbers[i])
        rateLine += "-1" #this option makes Combine read the rate from the histogram integrals

        currentLength = max(len(binLine), len(processLine1), len(processLine2), len(rateLine))

        #estimate electron scale uncertainty
        ESF    = ROOT.TFile.Open(binName[6:9] + "2" + binName[10:] + "/SimpleShapes_Wprime" + binName[6:9] + "2" + binName[10:] + ".root", "read")
        ESFHu  = ESF.Get("data_obs_Wprime" + binName[6:9] + "2" + binName[10:] + "_M" + str(massBin*100) + "_electronScale2017Up")
        ESFHd  = ESF.Get("data_obs_Wprime" + binName[6:9] + "2" + binName[10:] + "_M" + str(massBin*100) + "_electronScale2017Down")
        ESFHn  = ESF.Get("data_obs_Wprime" + binName[6:9] + "2" + binName[10:] + "_M" + str(massBin*100) + "_")
        ESFvar = str(max(math.fabs(ESFHu.Integral()/ESFHn.Integral()-1.), math.fabs(ESFHd.Integral()/ESFHn.Integral()-1.))+1.)
        systVals[13] = ESFvar[0:4]
        ESF.Close()
  
        for j in range(0, len(systLines)): #assemble systematic values
          if allNames[i] == "ttbar" and systLines[j].find("STfit") > -1:
            systLines[j] += systVals[j].replace("0","1") #activate ST fit uncertainty for ttbar only in the card
          elif allNames[i] != signalNames[0] and systLines[j].find("NLLnonClosure") > -1: #NLL non-closure systematic for all backgrounds
            NLLresF = ROOT.TFile.Open(fileName + "/SF_Bin" + binName[6:9] + "2" + binName[10:] + ".root", "read")
            NLLresH = NLLresF.Get("NLLresidual_" + binName[6:9] + "2" + binName[10:] + "_M" + str(massBin*100))
            NLLH    = r.Get("NegLogLnoB_" + allNames[i] + "_" + binName + "_M" + str(massBin*100) + "_")
            NLLresH.Multiply(NLLH)
            if NLLH.Integral() == 0:
              systLines[j] += systVals[j]
              continue
            ratio = str(NLLresH.Integral(0,-1)/NLLH.Integral(0,-1))
            dot = ratio.find(".")
            if dot >= 0:
              systLines[j] += systVals[j].replace("0",ratio[0:dot+3]) #limit precision to keep cards readable
            else:
              systLines[j] += systVals[j].replace("0",ratio)
          else:
            systLines[j] += systVals[j]
          currentLength = max(currentLength, len(systLines[j]))

        #align all lines with extra space
        currentLength += 5

        while len(binLine) < currentLength:
          binLine += " "

        while len(processLine1) < currentLength:
          processLine1 += " "

        while len(processLine2) < currentLength:
          processLine2 += " "

        while len(rateLine) < currentLength:
          rateLine += " "

        for j in range(0, len(systLines)):
          while len(systLines[j]) < currentLength:
            systLines[j] += " "

      print("Writing Combine Card values")
      f.write(binLine + "\n")
      print(binLine + "\n")
      f.write(processLine1 + "\n")
      print(processLine1 + "\n")
      f.write(processLine2 + "\n")
      print(processLine2 + "\n")
      f.write(rateLine + "\n")
      print(rateLine + "\n")
      f.write("----------\n")
      print("----------\n")

      for i in range(0, len(systLines)):
        f.write(systLines[i] + "\n")
        print(systLines[i] + "\n")

