import os,sys

#macro to produce Goodness of Fit for control regions only, split into one job per masspoint

mass = 300

try:
    if int(sys.argv[1]) > -1:
        mass = (int(sys.argv[1])+3)*100
        print("mass set to", mass)
except:
    print("mass defaults to", mass)

cardname = "WprimeAll2tag_all_M"+str(mass)

try:
    if sys.argv[2].find("Wprime") > -1:
        cardname = sys.argv[2]+"_M"+str(mass)
        print("card name set to", cardname)
except:
    print("card name defaults to", cardname)

ntoys = 100

try:
    if int(sys.argv[3]) > -1:
        ntoys = int(sys.argv[3])
        print("n toys set to", ntoys)
except:
    print("n toys defaults to", ntoys)

try:
    if sys.argv[4].find("True") > -1:
        cardname += "_smooth"
        print("smooth version active")
except:
    print("not smooth version active")

#build RooStat Workspace
print("text2workspace.py Combination/CRslices_"+cardname+".txt -o workspaceGoFCRsOnly_"+cardname+".root")
os.system("text2workspace.py Combination/CRslices_"+cardname+".txt -o workspaceGoFCRsOnly_"+cardname+".root")

#run data GoF test
print("combine -M GoodnessOfFit -d workspaceGoFCRsOnly_"+cardname+".root --algo saturated -n _data --freezeParameters r --setParameters r=0")
os.system("combine -M GoodnessOfFit -d workspaceGoFCRsOnly_"+cardname+".root --algo saturated -n _data --freezeParameters r --setParameters r=0")

#run GoF test toys
print("combine -M GoodnessOfFit -d workspaceGoFCRsOnly_"+cardname+".root --algo saturated -n _toys --toysFrequentist -t "+str(ntoys)+" --freezeParameters r --setParameters r=0")
os.system("combine -M GoodnessOfFit -d workspaceGoFCRsOnly_"+cardname+".root --algo saturated -n _toys --toysFrequentist -t "+str(ntoys)+" --freezeParameters r --setParameters r=0")

#make GoF plots #TBD
print("combineTool.py -M CollectGoodnessOfFit --input higgsCombine_data.GoodnessOfFit.mH120.root higgsCombine_toys.GoodnessOfFit.mH120.123456.root -m 120 -o GoF_CRsOnly_"+cardname+".json")
os.system("combineTool.py -M CollectGoodnessOfFit --input higgsCombine_data.GoodnessOfFit.mH120.root higgsCombine_toys.GoodnessOfFit.mH120.123456.root -m 120 -o GoF_CRsOnly_"+cardname+".json")

title = ""

if cardname.find("All") > -1:
    if cardname.find("all") > -1:
        title = "#mu/e 5/6j 2b Run2"
    elif cardname.find("2016apv") > -1:
        title = "#mu/e 5/6j 2b 2016apv"
    elif cardname.find("2016") > -1:
        title = "#mu/e 5/6j 2b 2016"
    elif cardname.find("2017") > -1:
        title = "#mu/e 5/6j 2b 2017"
    elif cardname.find("2018") > -1:
        title = "#mu/e 5/6j 2b 2018"
    else:
        print("WARNING, couldn't figure out right title")
elif cardname.find("Electron") > -1:
    if cardname.find("all") > -1:
        title = "e 5/6j 2b Run2"
    elif cardname.find("2016apv") > -1:
        title = "e 5/6j 2b 2016apv"
    elif cardname.find("2016") > -1:
        title = "e 5/6j 2b 2016"
    elif cardname.find("2017") > -1:
        title = "e 5/6j 2b 2017"
    elif cardname.find("2018") > -1:
        title = "e 5/6j 2b 2018"
    else:
        print("WARNING, couldn't figure out right title")
elif cardname.find("Muon") > -1:
    if cardname.find("all") > -1:
        title = "#mu 5/6j 2b Run2"
    elif cardname.find("2016apv") > -1:
        title = "#mu 5/6j 2b 2016apv"
    elif cardname.find("2016") > -1:
        title = "#mu 5/6j 2b 2016"
    elif cardname.find("2017") > -1:
        title = "#mu 5/6j 2b 2017"
    elif cardname.find("2018") > -1:
        title = "#mu 5/6j 2b 2018"
    else:
        print("WARNING, couldn't figure out right title")

print("plotGof.py GoF_CRsOnly_"+cardname+".json --statistic saturated --mass 120.0 -o GoF_CRsOnly_"+cardname+"_plot --title-right='"+title+"'")
os.system("plotGof.py GoF_CRsOnly_"+cardname+".json --statistic saturated --mass 120.0 -o GoF_CRsOnly_"+cardname+"_plot --title-right='"+title+"'")
