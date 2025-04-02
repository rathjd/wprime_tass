import os,sys

Bins  = ["1153", "2153", "1163", "1164", "2163", "2164"]
Years = ["2016apv", "2016", "2017", "2018"]

eospathIn = "/eos/cms/store/group/phys_b2g/wprime/temp/"
pathOut = ""

if os.path.isdir(pathOut + "Combination"):
    print("Combination directory exists, removing directory and regenerating")
    os.system("rm -rf " + pathOut + "Combination")
os.system("mkdir " + pathOut + "Combination")

for Bin in Bins:
    for Year in Years:
        directory = Bin+"_"+Year
        if not os.path.isdir(eospathIn + directory):
            print("missing",eospathIn + directory)
        else:
            print("copying", eospathIn + directory)
            os.system("cp " + eospathIn + directory + "/* " + pathOut + "Combination/.")
