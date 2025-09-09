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

#build ROOStat Workspace
print("text2workspace.py Combination/CRslices_WprimeAll2tag_all_M"+str(mass)+".txt -o workspace_CRsOnly_WprimeAll2tag_all_M"+str(mass)+".root")
os.system("text2workspace.py Combination/CRslices_WprimeAll2tag_all_M"+str(mass)+".txt -o workspace_CRsOnly_WprimeAll2tag_all_M"+str(mass)+".root")

#run initial fit, blinded by using Asimov datasets (-t -1)
print("combineTool.py -M Impacts -d workspace_CRsOnly_WprimeAll2tag_all_M"+str(mass)+".root -t -1 -m "+str(mass)+" --doInitialFit --expectSignal 0 --robustFit 1 --parallel 8")
os.system("combineTool.py -M Impacts -d workspace_CRsOnly_WprimeAll2tag_all_M"+str(mass)+".root -t -1 -m "+str(mass)+" --doInitialFit --expectSignal 0 --robustFit 1 --parallel 8")

#run full fits with 500 toys, blinded by using Asimov datasets (-t -1)
print("combineTool.py -M Impacts -d workspace_CRsOnly_WprimeAll2tag_all_M"+str(mass)+".root -t -1 -m "+str(mass)+" --doFits --robustFit 1 --expectSignal 0 --parallel 8")
os.system("combineTool.py -M Impacts -d workspace_CRsOnly_WprimeAll2tag_all_M"+str(mass)+".root -t -1 -m "+str(mass)+" --doFits --robustFit 1 --expectSignal 0 --parallel 8")

#produce impact outputs
print("combineTool.py -M Impacts -d workspace_CRsOnly_WprimeAll2tag_all_M"+str(mass)+".root -m "+str(mass)+" --output impacts_CRsOnly_WprimeAll2tag_all_M"+str(mass)+".json")
os.system("combineTool.py -M Impacts -d workspace_CRsOnly_WprimeAll2tag_all_M"+str(mass)+".root -m "+str(mass)+" --output impacts_CRsOnly_WprimeAll2tag_all_M"+str(mass)+".json")

#produce actual plots
print("plotImpacts.py -i impacts_CRsOnly_WprimeAll2tag_all_M"+str(mass)+".json -o impacts_CRsOnly_WprimeAll2tag_all_M"+str(mass))
os.system("plotImpacts.py -i impacts_CRsOnly_WprimeAll2tag_all_M"+str(mass)+".json -o impacts_CRsOnly_WprimeAll2tag_all_M"+str(mass))
