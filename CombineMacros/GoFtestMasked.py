import os,sys

#macro to produce Goodness of Fit, split into one job per masspoint

mass = 300

try:
    if int(sys.argv[1]) > -1:
        mass = (int(sys.argv[1])+3)*100
        print("mass set to", mass)
except:
    print("mass defaults to", mass)

#generate channel masks
maskListOn = []
maskListOff = []
FirstOrder  = ["1","2","3","4"] #all 4 years
SecondOrder = ["1","2","5","6"] #1152, 2152, 1162, 2162
ThirdOrder  = ["1","2"] #mFit, HT
for first in FirstOrder:
    for second in SecondOrder:
        for third in ThirdOrder:
            maskListOn.append( "mask_ch"+first+"_ch"+second+"_ch"+third+"=1")
            maskListOff.append("mask_ch"+first+"_ch"+second+"_ch"+third+"=0")
maskStringOn = ""
for i, maskOn in enumerate(maskListOn):
    if i < len(maskListOn)-1:
        maskStringOn += maskOn + ","
    else:
        maskStringOn += maskOn
maskStringOff = ""
for i, maskOff in enumerate(maskListOff):
    if i < len(maskListOff)-1:
        maskStringOff += maskOff + ","
    else:
        maskStringOff += maskOff
maskStringOnPlus = maskStringOn + ",r=0"

#build RooStat Workspace with masks
print("text2workspace.py Combination/Optimized_WprimeAll_all_M"+str(mass)+".txt -o workspaceGoFmasked_WprimeAll_all_M"+str(mass)+".root --channel-masks")
os.system("text2workspace.py Combination/Optimized_WprimeAll_all_M"+str(mass)+".txt -o workspaceGoFmasked_WprimeAll_all_M"+str(mass)+".root --channel-masks")

#throw frequentist toys S+B model
print("combine -M GoodnessOfFit -d workspaceGoFmasked_WprimeAll_all_M"+str(mass)+".root --algo=saturated -n _result_toy_sb --toysFrequentist -t 500")
os.system("combine -M GoodnessOfFit -d workspaceGoFmasked_WprimeAll_all_M"+str(mass)+".root --algo=saturated -n _result_toy_sb --toysFrequentist -t 500")

#throw frequentist toys using CR constraints in B only model
print("combine -M GoodnessOfFit -d workspaceGoFmasked_WprimeAll_all_M"+str(mass)+".root --algo=saturated -n _result_bonly_CRonly --setParametersForFit "+maskStringOn+" --setParametersForEval "+maskStringOff+" --freezeParameters r --setParameters "+maskStringOnPlus+" --toysFrequentist -t 500")
os.system("combine -M GoodnessOfFit -d workspaceGoFmasked_WprimeAll_all_M"+str(mass)+".root --algo=saturated -n _result_bonly_CRonly --setParametersForFit "+maskStringOn+" --setParametersForEval "+maskStringOff+" --freezeParameters r --setParameters "+maskStringOnPlus+"  --toysFrequentist -t 500")


