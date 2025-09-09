import os,sys

#macro to produce Goodness of Fit for control regions only, split into one job per masspoint

mass = 300

try:
    if int(sys.argv[1]) > -1:
        mass = (int(sys.argv[1])+3)*100
        print("mass set to", mass)
except:
    print("mass defaults to", mass)

#build RooStat Workspace
print("text2workspace.py Combination/CRslices_WprimeAll2tag_all_M"+str(mass)+".txt -o workspaceGoFCRsOnly_WprimeAll2tag_all_M"+str(mass)+".root")
os.system("text2workspace.py Combination/CRslices_WprimeAll2tag_all_M"+str(mass)+".txt -o workspaceGoFCRsOnly_WprimeAll2tag_all_M"+str(mass)+".root")

#run data GoF test
print("combine -M GoodnessOfFit -d workspaceGoFCRsOnly_WprimeAll2tag_all_M"+str(mass)+".root --algo saturated -n _data --freezeParameters r --setParameters r=0")
os.system("combine -M GoodnessOfFit -d workspaceGoFCRsOnly_WprimeAll2tag_all_M"+str(mass)+".root --algo saturated -n _data --freezeParameters r --setParameters r=0")

#run GoF test toys
print("combine -M GoodnessOfFit -d workspaceGoFCRsOnly_WprimeAll2tag_all_M"+str(mass)+".root --algo saturated -n _toys --toysFrequentist -t 500 --freezeParameters r --setParameters r=0")
os.system("combine -M GoodnessOfFit -d workspaceGoFCRsOnly_WprimeAll2tag_all_M"+str(mass)+".root --algo saturated -n _toys --toysFrequentist -t 500 --freezeParameters r --setParameters r=0")

#make GoF plots #TBD
#print("combineTool.py -M CollectGoodnessOfFit --input _data.root _toys.root -m "+str(mass)+" -o GoF_CRsOnly_M"+str(mass)+".json")
#os.system("combineTool.py -M CollectGoodnessOfFit --input _data.root _toys.root -m "+str(mass)+" -o GoF_CRsOnly_M"+str(mass)+".json")

#print(plotGof.py GoF_CRsOnly_M"+str(mass)+".json --statistic saturated --mass "+str(mass)+" -o GoF_CRsOnly_M"+str(mass)+"_plot --title-right='CRs only Run2'")
#os.system(plotGof.py GoF_CRsOnly_M"+str(mass)+".json --statistic saturated --mass "+str(mass)+" -o GoF_CRsOnly_M"+str(mass)+"_plot --title-right='CRs only Run2'")
