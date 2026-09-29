import os,sys

#macro to produce impact plots, split into one job per masspoint
#This macro is used for CRs only, including data constraints

mass = 300

try:
    if int(sys.argv[1]) > -1:
        mass = (int(sys.argv[1])+3)*100
        print("mass set to", mass)
except:
    print("mass defaults to", mass)

Rmin = -10
try:
    if int(sys.argv[2]) < 0:
        Rmin = int(sys.argv[2])
        print("--rMin set to",Rmin)
except:
    print("--rMin defaults to",Rmin)

Rmax = 10
try:
    if int(sys.argv[3]) > 0:
        Rmax = int(sys.argv[3])
        print("--rMax set to",Rmax)
except:
    print("--rMax defaults to",Rmax)
        

era = "all"

#prepare empty impacts folder for all the files
path = "Impacts_"+str(mass)

if os.path.isdir(path):
  print(path," directory already exists, removing it")
  os.system("rm -rf " + path)
os.system("mkdir " + path)

#number of parallel processors
parallel = "8"

#dictionary of R values
RvalDict = {300: 4,
            400: 10,
            500: 10,
            600: 10,
            700: 10,
            800: 10,
            900: 10,
           1000: 10,
           1100: 10}

Rmin = -RvalDict[mass]
Rmax =  RvalDict[mass]

noCminOptList = []

cMinOpt = "--cminDefaultMinimizerStrategy=0"
if mass in noCminOptList:
    cMinOpt = ""

noRobFitOptList = [300]

robFitOpt = "--robustFit=1"
if mass in noRobFitOptList:
    robFitOpt = ""



#build ROOStat Workspace
print("text2workspace.py Combination/CRslices_WprimeAll2tag_"+era+"_M"+str(mass)+"_splitPSbySample_noNormTtbar.txt -o "+path+"/workspace_CRsOnly_WprimeAll2tag_"+era+"_M"+str(mass)+"_splitPSbySample_noNormTtbar.root")
os.system("text2workspace.py Combination/CRslices_WprimeAll2tag_"+era+"_M"+str(mass)+"_splitPSbySample_noNormTtbar.txt -o "+path+"/workspace_CRsOnly_WprimeAll2tag_"+era+"_M"+str(mass)+"_splitPSbySample_noNormTtbar.root")

#change into newly-created folder
os.chdir(path)
print("now in directory",os.getcwd())

#run initial fit
print("combineTool.py -M Impacts -d workspace_CRsOnly_WprimeAll2tag_"+era+"_M"+str(mass)+"_splitPSbySample_noNormTtbar.root -m "+str(mass)+" --doInitialFit --rMin="+str(Rmin)+" --rMax="+str(Rmax)+" "+robFitOpt+" "+cMinOpt)
os.system("combineTool.py -M Impacts -d workspace_CRsOnly_WprimeAll2tag_"+era+"_M"+str(mass)+"_splitPSbySample_noNormTtbar.root -m "+str(mass)+" --doInitialFit --rMin="+str(Rmin)+" --rMax="+str(Rmax)+" "+robFitOpt+" "+cMinOpt)

#run full fits with 500 toys
print("combineTool.py -M Impacts -d workspace_CRsOnly_WprimeAll2tag_"+era+"_M"+str(mass)+"_splitPSbySample_noNormTtbar.root -m "+str(mass)+" --doFits --rMin="+str(Rmin)+" --rMax="+str(Rmax)+" "+robFitOpt+" "+cMinOpt+" --parallel="+parallel)
os.system("combineTool.py -M Impacts -d workspace_CRsOnly_WprimeAll2tag_"+era+"_M"+str(mass)+"_splitPSbySample_noNormTtbar.root -m "+str(mass)+" --doFits --rMin="+str(Rmin)+" --rMax="+str(Rmax)+" "+robFitOpt+" "+cMinOpt+" --parallel="+parallel)

#produce impact outputs
print("combineTool.py -M Impacts -d workspace_CRsOnly_WprimeAll2tag_"+era+"_M"+str(mass)+"_splitPSbySample_noNormTtbar.root -m "+str(mass)+" --rMin="+str(Rmin)+" --rMax="+str(Rmax)+" --output impacts_CRsOnly_WprimeAll2tag_"+era+"_M"+str(mass)+"_splitPSbySample_noNormTtbar.json")
os.system("combineTool.py -M Impacts -d workspace_CRsOnly_WprimeAll2tag_"+era+"_M"+str(mass)+"_splitPSbySample_noNormTtbar.root -m "+str(mass)+" --rMin="+str(Rmin)+" --rMax="+str(Rmax)+" --output impacts_CRsOnly_WprimeAll2tag_"+era+"_M"+str(mass)+"_splitPSbySample_noNormTtbar.json")

#produce actual plots
print("plotImpacts.py -i impacts_CRsOnly_WprimeAll2tag_"+era+"_M"+str(mass)+"_splitPSbySample_noNormTtbar.json -o impacts_CRsOnly_WprimeAll2tag_"+era+"_M"+str(mass)+"_splitPSbySample_noNormTtbar")
os.system("plotImpacts.py -i impacts_CRsOnly_WprimeAll2tag_"+era+"_M"+str(mass)+"_splitPSbySample_noNormTtbar.json -o impacts_CRsOnly_WprimeAll2tag_"+era+"_M"+str(mass)+"_splitPSbySample_noNormTtbar")
