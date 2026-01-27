import os,sys

eras = ["2016", "2016apv", "2017", "2018"]

masses = ["300", "400", "500", "600", "700", "800", "900", "1000", "1100"]

for mass in masses:
    os.system("combineCards.py Combination/CombinationSlices_Wprime*1*2_all_M"+mass+".txt > Combination/CRslices_WprimeAll2tag_all_M"+mass+".txt")
    os.system("combineCards.py Combination/CombinationSlices_Wprime11*2_all_M"+mass+".txt > Combination/CRslices_WprimeMuon2tag_all_M"+mass+".txt")
    os.system("combineCards.py Combination/CombinationSlices_Wprime21*2_all_M"+mass+".txt > Combination/CRslices_WprimeElectron2tag_all_M"+mass+".txt")
    for era in eras:
        os.system("combineCards.py Combination/CombinationSlices_Wprime*1*2_"+era+"_M"+mass+".txt > Combination/CRslices_WprimeAll2tag_"+era+"_M"+mass+".txt")

