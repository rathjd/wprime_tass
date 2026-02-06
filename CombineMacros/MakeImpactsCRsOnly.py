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

era = "all"

#prepare empty impacts folder for all the files
path = "Impacts_"+str(mass)

if os.path.isdir(path):
  print(path," directory already exists, removing it")
  os.system("rm -rf " + path)
os.system("mkdir " + path)


#build ROOStat Workspace
print("text2workspace.py Combination/CRslices_WprimeAll2tag_"+era+"_M"+str(mass)+".txt -o "+path+"/workspace_CRsOnly_WprimeAll2tag_"+era+"_M"+str(mass)+".root")
os.system("text2workspace.py Combination/CRslices_WprimeAll2tag_"+era+"_M"+str(mass)+".txt -o "+path+"/workspace_CRsOnly_WprimeAll2tag_"+era+"_M"+str(mass)+".root")

#change into newly-created folder
os.chdir(path)
print("now in directory",os.getcwd())

#run initial fit
print("combineTool.py -M Impacts -d workspace_CRsOnly_WprimeAll2tag_"+era+"_M"+str(mass)+".root -m "+str(mass)+" --doInitialFit --rMin -10 --rMax 10 --robustFit 1 --parallel 8")
os.system("combineTool.py -M Impacts -d workspace_CRsOnly_WprimeAll2tag_"+era+"_M"+str(mass)+".root -m "+str(mass)+" --doInitialFit --rMin -10 --rMax 10 --robustFit 1 --parallel 8")

#run full fits with 500 toys
print("combineTool.py -M Impacts -d workspace_CRsOnly_WprimeAll2tag_"+era+"_M"+str(mass)+".root -m "+str(mass)+" --doFits --rMin -10 --rMax 10 --robustFit 1 --parallel 8")
os.system("combineTool.py -M Impacts -d workspace_CRsOnly_WprimeAll2tag_"+era+"_M"+str(mass)+".root -m "+str(mass)+" --doFits --rMin -10 --rMax 10 --robustFit 1 --parallel 8")

#produce impact outputs
print("combineTool.py -M Impacts -d workspace_CRsOnly_WprimeAll2tag_"+era+"_M"+str(mass)+".root -m "+str(mass)+" --rMin -10 --rMax 10 --output impacts_CRsOnly_WprimeAll2tag_"+era+"_M"+str(mass)+".json")
os.system("combineTool.py -M Impacts -d workspace_CRsOnly_WprimeAll2tag_"+era+"_M"+str(mass)+".root -m "+str(mass)+" --rMin -10 --rMax 10 --output impacts_CRsOnly_WprimeAll2tag_"+era+"_M"+str(mass)+".json")

#produce actual plots
print("plotImpacts.py -i impacts_CRsOnly_WprimeAll2tag_"+era+"_M"+str(mass)+".json -o impacts_CRsOnly_WprimeAll2tag_"+era+"_M"+str(mass))
os.system("plotImpacts.py -i impacts_CRsOnly_WprimeAll2tag_"+era+"_M"+str(mass)+".json -o impacts_CRsOnly_WprimeAll2tag_"+era+"_M"+str(mass))
