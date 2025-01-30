import os,sys

yearList = ["2016apv", "2016", "2017", "2018"]

for year in yearList:
    for lept in range(1,3):
        for jet in range(5,7):
            for btag in range(3,4):
                if jet == 5 and btag == 4:
                    continue
                print("condor_submit Submit"+str(lept)+"1"+str(jet)+str(btag)+"_"+year+".sub")
                os.system("condor_submit Submit"+str(lept)+"1"+str(jet)+str(btag)+"_"+year+".sub")


