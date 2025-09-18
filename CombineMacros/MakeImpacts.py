import os,sys

#macro to produce impact plots, split into one job per masspoint

mass = 300

try:
    if int(sys.argv[1]) > -1:
        mass = (int(sys.argv[1])+3)*100
        print("mass set to", mass)
except:
    print("mass defaults to", mass)

#build ROOStat Workspace
print("text2workspace.py Combination/Optimized_WprimeAll_all_M"+str(mass)+".txt -o workspace_WprimeAll_all_M"+str(mass)+".root")
os.system("text2workspace.py Combination/Optimized_WprimeAll_all_M"+str(mass)+".txt -o workspace_WprimeAll_all_M"+str(mass)+".root")

#run initial fit, blinded by using Asimov datasets (-t -1)
print("combineTool.py -M Impacts -d workspace_WprimeAll_all_M"+str(mass)+".root -t -1 -m "+str(mass)+" --rMin -10 --rMax 10 --doInitialFit -n 500 --robustFit 1 --expectSignal 1 --parallel 8 --cminDefaultMinimizerStrategy 0")
os.system("combineTool.py -M Impacts -d workspace_WprimeAll_all_M"+str(mass)+".root -t -1 -m "+str(mass)+" --rMin -10 --rMax 10 --doInitialFit -n 500 --robustFit 1 --expectSignal 1 --parallel 8 --cminDefaultMinimizerStrategy 0")

#run full fits with 500 toys, blinded by using Asimov datasets (-t -1)
print("combineTool.py -M Impacts -d workspace_WprimeAll_all_M"+str(mass)+".root -t -1 -m "+str(mass)+" --rMin -10 --rMax 10 --doFits -n 500 --robustFit 1 --expectSignal 1 --parallel 8 --cminDefaultMinimizerStrategy 0")
os.system("combineTool.py -M Impacts -d workspace_WprimeAll_all_M"+str(mass)+".root -t -1 -m "+str(mass)+" --rMin -10 --rMax 10 --doFits -n 500 --robustFit 1 --expectSignal 1 --parallel 8 --cminDefaultMinimizerStrategy 0")

#produce impact outputs
print("combineTool.py -M Impacts -d workspace_WprimeAll_all_M"+str(mass)+".root -m "+str(mass)+" --output impacts_WprimeAll_all_M"+str(mass)+".json -n 500")
os.system("combineTool.py -M Impacts -d workspace_WprimeAll_all_M"+str(mass)+".root -m "+str(mass)+" --output impacts_WprimeAll_all_M"+str(mass)+".json -n 500")

#produce actual plots
print("plotImpacts.py -i impacts_WprimeAll_all_M"+str(mass)+".json -o impacts_WprimeAll_all_M"+str(mass))
os.system("plotImpacts.py -i impacts_WprimeAll_all_M"+str(mass)+".json -o impacts_WprimeAll_all_M"+str(mass))
