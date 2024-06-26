import os,sys

for year in range(2016,2019):
    for lept in range(1,3):
        for jet in range(5,7):
            for btag in range(3,5):
                if jet == 5 and btag == 4:
                    continue
                print("condor_submit SubmitSplit"+str(lept)+"1"+str(jet)+str(btag)+"_"+str(year)+".sub")
                os.system("condor_submit SubmitSplit"+str(lept)+"1"+str(jet)+str(btag)+"_"+str(year)+".sub")


