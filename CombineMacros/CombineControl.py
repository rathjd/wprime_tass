#macro to write cards for the tW' to tb analysis within B2G, CADI number yet to be assigned, by Denis Rathjens
import os, sys
import ROOT
import math

#define the input names
signalNames = ["M$MASS"] #$MASS gets replaced by runing combine with -m option, supports only 1 signal mass at a time
bgrNames = ["ttbar", "wjets", "single_top", "diboson", "qcd"]
binNumber = 1153 #this needs to be set, valid entries are 1150, 1160, 2150, 2160
yearName = "2018"

try:
    if int(sys.argv[1]) > 0:
        binNumber = int(sys.argv[1])
        print("bin number set to "+str(binNumber))
except:
    print("bin number defaults to "+str(binNumber))

yearOptions = ["2016_APV", "2016", "2017", "2018"]

try:
    if sys.argv[2] in yearOptions:
        yearName = sys.argv[2]
        print("year number set to "+ yearName)
except:
    print("year number defautls to "+ yearName)



binString = str(binNumber)

eospath = "/eos/cms/store/group/phys_b2g/wprime/temp"

#First, generate the shape variation histograms
print("Starting processing of intermediate files")
if os.path.isdir(eospath):
  #os.system("rm /eos/cms/store/group/phys_b2g/wprime/temp/*.root") #reset
  print("already exists")
else:
  os.system("mkdir "+eospath) #make directory, if it doesn't exist

#run dump from intermediate tree including ST correction cycle, generates 3- and 4-tag regions both
os.system("root -l -b -q 'runCombineHistogramDumpster.C+(" + str(binNumber) + ', "' + yearName + '"' + ")'")

bins = [binString[0:3]+"1", binString[0:3]+"2", binString[0:3]+"3", binString[0:3]+"4"]

#assign region name
if binString[0:3] == "115":
    regName = "mu5j1b"
elif binString[0:3] == "215":
    regName = "e5j1b"
if binString[0:3] == "116":
    regName = "mu6j1b"
elif binString[0:3] == "216":
    regName = "e6j1b"

#deal with 2016apv uncertainties being 2016 in type very often
simpYearName = yearName

#change year name string for 2016_APV to the Sifu-scheme-complian 2016apv for further processing
if yearName == "2016_APV":
    yearName = "2016apv"
    simpYearName = "2016"

print(bins)

for binN in bins:
  if binN == "1154" or binN == "2154":
      continue
  fileName = binN + "_" + yearName
  binName = "Wprime" + binN + "_" + yearName
  print(binName," ",fileName)

  if os.path.isdir(eospath + "/" + fileName):
      print(fileName," directory already exists, removing it")
      os.system("rm -rf " + eospath + "/" + fileName)
  os.system("mkdir " + eospath + "/" + fileName)

  #assemble results
  os.system("hadd -f " + eospath + "/" + fileName + "/SimpleShapes_" + binName + ".root "+eospath+"/SimpleShapes_Bin" + fileName + "_*.root") #hadd all histograms to a convenient combined file
  os.system("hadd -f " + eospath + "/" + fileName + "/HT_SimpleShapes_" + binName + ".root "+eospath+"/HT_SimpleShapes_Bin" + fileName + "_*.root") #hadd all histograms to a convenient combined file
  os.system("hadd -f " + eospath + "/" + fileName + "/TwoD_SimpleShapes_" + binName + ".root "+eospath+"/TwoD_SimpleShapes_Bin" + fileName + "_*.root") #hadd all histograms to a convenient combined file 

  #transfer SF files where appropriate
  if int(binN) % 10 == 2:
      os.system("cp " + eospath + "/SF_Bin"+binString[0:3]+"1_" + yearName + ".root " + eospath + "/" + fileName + "/.")
      print("cp " + eospath + "/SF_Bin"+binString[0:3]+"1_" + yearName + ".root " + eospath + "/" + fileName + "/.")
  #elif int(binN) % 10 > 2:
  #    os.system("cp " + eospath + "/SF_Bin"+binString[0:3]+"2_" + yearName + ".root " + eospath + "/" + fileName + "/.")
  #    print("cp " + eospath + "/SF_Bin"+binString[0:3]+"2_" + yearName + ".root " + eospath + "/" + fileName + "/.") 

  #skip making cards for 1 tag region
  if int(binN) % 10 < 2:
      continue

  #define correlated entities for usage in card
  #https://twiki.cern.ch/twiki/bin/view/CMS/LumiRecommendationsRun2#Combination_and_correlations is now a shape uncertainty built into histograms in the file

  #define nested lnN uncertainty dictionary
  lnNdict = {"lumi_13TeV_1516_l":     {"2016": "1.0118","2016apv": "1.0118","2017": "-",     "2018": "-"},
             "lumi_13TeV_151617_l":   {"2016": "1.0004","2016apv": "1.0004","2017": "1.0055","2018": "-"},
             "lumi_13TeV_15161718_l": {"2016": "1.0035","2016apv": "1.0035","2017": "1.0061","2018": "1.0084"},
             "CMS_eff_e_HLTzvtx_17":  {"2016": "-",     "2016apv": "-",     "2017": "1.001", "2018": "-"}
            }

  B2Gn = "25008"
  systMaster = [#lumi and generic normalization uncertainties
                ["lumi_13TeV_1516_l",                                   "lnN", "-"],
                ["lumi_13TeV_151617_l",                                 "lnN", "-"],
                ["lumi_13TeV_15161718_l",                               "lnN", "-"],
                ["CMS_eff_e_HLTzvtx_17",                                "lnN", "-"],

                #event weight variation uncertainties
                ["CMS_eff_e_trigger",                                   "shape", "1"],
                ["CMS_eff_e_reco",                                      "shape", "1"],
                ["CMS_eff_e",                                           "shape", "1"],
                ["CMS_eff_m_trigger_",                                  "shape", "1"],
                ["CMS_eff_m_id_",                                       "shape", "1"],
                ["CMS_eff_m_iso_",                                      "shape", "1"],
                ["CMS_btag_light",                                      "shape", "1"],
                ["CMS_btag_heavy",                                      "shape", "1"],
                ["CMS_btag_light_"+yearName,                            "shape", "1"],
                ["CMS_btag_heavy_"+yearName,                            "shape", "1"],
                ["CMS_eff_j_PUJET_id_"+yearName,                        "shape", "1"],
                ["CMS_l1_ecal_prefiring_"+yearName,                     "shape", "1"],
                ["CMS_pileup",                                          "shape", "1"],
                ["pdf_B2G"+B2Gn+"_envelope_ttbar",                      "shape", "-"],
                ["pdf_B2G"+B2Gn+"_envelope_wjets",                      "shape", "-"],
                ["pdf_B2G"+B2Gn+"_envelope_single_top",                 "shape", "-"],
                ["pdf_B2G"+B2Gn+"_envelope_diboson",                    "shape", "-"],
                #["pdf_B2G"+B2Gn+"_envelope_signal",                     "shape", "-"],
                ["QCDscale_ren_ttbar",                                  "shape", "-"],
                ["QCDscale_ren_wjets",                                  "shape", "-"],
                ["QCDscale_ren_single_top",                             "shape", "-"],
                ["QCDscale_ren_diboson",                                "shape", "-"],
                ["QCDscale_fac_ttbar",                                  "shape", "-"],
                ["QCDscale_fac_wjets",                                  "shape", "-"],
                ["QCDscale_fac_single_top",                             "shape", "-"],
                ["QCDscale_fac_diboson",                                "shape", "-"],
                ["ps_isr",                                              "shape", "1"],
                ["ps_fsr",                                              "shape", "1"],
                #["QCDscale_signal",                                     "shape", "-"],
                
                #object pT variation uncertainties
                ["CMS_scale_e",                                         "lnN",   "1"],
                ["CMS_res_e",                                           "shape", "1"],
                ["CMS_res_j_"+yearName,                                 "shape", "1"],
                ["CMS_scale_j_AbsoluteStat_"+yearName,                  "shape", "1"],
                ["CMS_scale_j_AbsoluteScale",                           "shape", "1"],
                ["CMS_scale_j_AbsoluteMPFBias",                         "shape", "1"],
                ["CMS_scale_j_FlavorQCD",                               "shape", "1"],
                ["CMS_scale_j_Fragmentation",                           "shape", "1"],
                ["CMS_scale_j_PileUpDataMC",                            "shape", "1"],
                ["CMS_scale_j_PileUpPtBB",                              "shape", "1"],
                ["CMS_scale_j_PileUpPtEC1",                             "shape", "1"],
                ["CMS_scale_j_PileUpPtEC2",                             "shape", "1"],
                ["CMS_scale_j_PileUpPtHF",                              "shape", "1"],
                ["CMS_scale_j_PileUpPtRef",                             "shape", "1"],
                ["CMS_scale_j_RelativeFSR",                             "shape", "1"],
                ["CMS_scale_j_RelativeJEREC1_"+yearName,                "shape", "1"],
                ["CMS_scale_j_RelativeJEREC2_"+yearName,                "shape", "1"],
                ["CMS_scale_j_RelativeJERHF",                           "shape", "1"],
                ["CMS_scale_j_RelativePtBB",                            "shape", "1"],
                ["CMS_scale_j_RelativePtEC1_"+yearName,                 "shape", "1"],
                ["CMS_scale_j_RelativePtEC2_"+yearName,                 "shape", "1"],
                ["CMS_scale_j_RelativePtHF",                            "shape", "1"],
                ["CMS_scale_j_RelativeBal",                             "shape", "1"],
                ["CMS_scale_j_RelativeSample_"+yearName,                "shape", "1"],
                ["CMS_scale_j_RelativeStatEC_"+yearName,                "shape", "1"],
                ["CMS_scale_j_RelativeStatFSR_"+yearName,               "shape", "1"],
                ["CMS_scale_j_RelativeStatHF_"+yearName,                "shape", "1"],
                ["CMS_scale_j_PionECAL",                                "shape", "1"],
                ["CMS_scale_j_PionHCAL",                                "shape", "1"],
                ["CMS_scale_j_TimePtEta_"+yearName,                     "shape", "1"],
                ["CMS_scale_met_unclustered_energy_"+yearName,          "shape", "1"],

                #control-region uncertainties
                ["CMS_B2G"+B2Gn+"_STfit_"+yearName+"_"+regName,         "shape", "-"],
                ["CMS_B2G"+B2Gn+"_STfitFunc_"+yearName+"_"+regName,     "shape", "-"],
                ["CMS_B2G"+B2Gn+"_STfitQCD",                            "shape", "-"]]


  #write the actual combine cards
  CardNames = [["FitMass", ""], ["HT", "HT_"]] #by default makes both the fit mass and HT extraction cards, separately
  for massBin in range(3,12):
    signalNames = ["M" + str(massBin*100)]
    for CardName in CardNames:
      print("Creating Combine card file",fileName + "/" + CardName[0] + "_" + binName + "_M" + str(massBin*100) + ".txt")
      f = open(eospath + "/" + fileName + "/" + CardName[0] + "_" + binName + "_M" + str(massBin*100) + ".txt","w")
      f.write("imax " + str(1) + "\n") #number of channels
      f.write("jmax " + str(len(bgrNames)) + "\n") #number of backgrounds
      f.write("kmax " + str(len(systMaster)) + "\n") #number of nuisance parameters
      f.write("----------\n")
      f.write("shapes * * " + CardName[1] + "SimpleShapes_" + binName + ".root "+ CardName[1] + "$PROCESS_$CHANNEL_M" + str(massBin*100) + "_ " + CardName[1] + "$PROCESS_$CHANNEL_M" + str(massBin*100) + "_$SYSTEMATIC\n")
      f.write("----------\n")
      f.write("bin         " + binName + "\n")

      #load ROOT file, find observation number
      print("Reading observed events numbers")
      r = ROOT.TFile.Open(eospath + "/" + fileName + "/" + CardName[1] + "SimpleShapes_" + binName + ".root", "read")
      print(eospath + "/" + fileName + "/" + CardName[1] + "SimpleShapes_" + binName + ".root")
      h = r.Get(CardName[1]+"data_obs_" + binName + "_M" + str(massBin*100) + "_")
      print("data_obs_" + binName + "_M" + str(massBin*100) + "_")
      print(type(h))
      observed = h.Integral()
      f.write("observation " + str(observed) + "\n")
      f.write("----------\n")

      ##assemble strings for lines
      print("Assembling lines for Combine card ",CardName[0])
      #systematic lines first, as this is an entire column to be processed, later, and they are longest
      systLines = []
      maxLength = 0
      for i in range(0, len(systMaster)): #assemble systematic names
        systLines.append(systMaster[i][0])
        maxLength = max(maxLength, len(systLines[i]))

      maxLength2 = 0
      for i in range(0, len(systLines)): #align systematic names, then assemble systematic types
        while len(systLines[i]) < (maxLength + 3):
          systLines[i] += " "
        systLines[i] += systMaster[i][1]
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

      #estimate electron scale uncertainty
      ESF    = ROOT.TFile.Open(eospath + "/" + binName[6:9] + "2" + binName[10:] + "/SimpleShapes_Wprime" + binName[6:9] + "2" + binName[10:] + ".root", "read")
      print("open file", eospath + "/" + binName[6:9] + "2" + binName[10:] + "/SimpleShapes_Wprime" + binName[6:9] + "2" + binName[10:] + ".root", ESF)
      ESFHu  = ESF.Get("data_obs_Wprime" + binName[6:9] + "2" + binName[10:] + "_M" + str(massBin*100) + "_CMS_scale_e" + "Up")
      ESFHd  = ESF.Get("data_obs_Wprime" + binName[6:9] + "2" + binName[10:] + "_M" + str(massBin*100) + "_CMS_scale_e" + "Down")
      ESFHn  = ESF.Get("data_obs_Wprime" + binName[6:9] + "2" + binName[10:] + "_M" + str(massBin*100) + "_")
      print("estimate e scale uncertainty with","data_obs_Wprime" + binName[6:9] + "2" + binName[10:] + "_M" + str(massBin*100) + "_CMS_scale_e" + "Up",ESFHu)
      ESFvar = str(max(math.fabs(ESFHu.Integral()/ESFHn.Integral()-1.), math.fabs(ESFHd.Integral()/ESFHn.Integral()-1.))+1.)
      #find the electron scale uncertainty and replace the value with the corresponding estimate
      for i in range(0, len(systMaster)):
          if systMaster[i][0].find("CMS_scale_e") > -1:
              systMaster[i][2] = ESFvar[0:4]
              break
      ESF.Close()

      for i in range(0, len(allNames)): #assemble bin, process, rate, and systematic line entries, then align
        binLine += binName
        processLine1 += allNames[i]
        processLine2 += str(allNumbers[i])
        if not allNames[i] == "qcd":
            rateLine += "-1" #this option makes Combine read the rate from the histogram integrals
        else:
            rateLine += " 0"

        currentLength = max(len(binLine), len(processLine1), len(processLine2), len(rateLine))

        #print(systLines)
        for j in range(0, len(systLines)): #assemble systematic values
          #print(j, systMaster[j])
          if allNames[i] == "ttbar" and systLines[j].find("STfit") > -1:
            systLines[j] += systMaster[j][2].replace("-","1") #activate ST fit uncertainty for ttbar only in the card
          elif systMaster[j][0].find(allNames[i]) > -1: #activate ISR/FSR and PDF uncertainties only specific background samples
            systLines[j] += systMaster[j][2].replace("-","1")
          elif systMaster[j][0] in lnNdict:
            systLines[j] += systMaster[j][2].replace("-",lnNdict[systMaster[j][0]][yearName])
          else:
            systLines[j] += systMaster[j][2]
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
      f.write(processLine1 + "\n")
      f.write(processLine2 + "\n")
      f.write(rateLine + "\n")
      f.write("----------\n")

      for i in range(0, len(systLines)):
        f.write(systLines[i] + "\n")

      #add MC statistics evaluation
      f.write("\n")
      f.write("* autoMCStats 10")

      f.close()

