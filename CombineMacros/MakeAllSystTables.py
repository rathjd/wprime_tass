import os,sys

anaBins = ['FitSlice', 'HTslice']
years = ["2016apv", "2016", "2017", "2018"]

for year in years:
    for lept in range(1,3):
        for jet in range(5,7):
            for btag in range(3,5):
                if jet == 5 and btag == 4:
                    continue
                for var in anaBins:
                    for mass in range(3, 12):
                        print("python3 MakeSystTables.py "+str(lept)+"1"+str(jet)+str(btag)+" "+year+" "+var+" "+str(mass*100))
                        os.system("python3 MakeSystTables.py "+str(lept)+"1"+str(jet)+str(btag)+" "+year+" "+var+" "+str(mass*100))


