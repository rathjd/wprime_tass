import os,sys

#This is a macro to call all viable CheckPlots.py macro combinations for ease of use

LeptonFlavs = ["1","2"]
JetMults = ["5","6"]
years = ["2016apv","2016","2017","2018"]

for lf in LeptonFlavs:
    for jm in JetMults:
        for y in years:
            print("python3 CheckPlots.py "+lf+" "+jm+" "+y)
            os.system("python3 CheckPlots.py "+lf+" "+jm+" "+y)
