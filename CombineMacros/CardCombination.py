import os,sys

os.chdir("CombinationAll")

for m in range(3,12):
    mS = str(m*100)
    print("combineCards.py CombinationSlices_Wprime1153_2017_M"+mS+".txt CombinationSlices_Wprime2153_2017_M"+mS+".txt CombinationSlices_Wprime1163_2017_M"+mS+".txt CombinationSlices_Wprime2163_2017_M"+mS+".txt CombinationSlices_Wprime1164_2017_M"+mS+".txt CombinationSlices_Wprime2164_2017_M"+mS+".txt > Combination_WprimeAll_2017_M"+mS+".txt")
    os.system("combineCards.py CombinationSlices_Wprime1153_2017_M"+mS+".txt CombinationSlices_Wprime2153_2017_M"+mS+".txt CombinationSlices_Wprime1163_2017_M"+mS+".txt CombinationSlices_Wprime2163_2017_M"+mS+".txt CombinationSlices_Wprime1164_2017_M"+mS+".txt CombinationSlices_Wprime2164_2017_M"+mS+".txt > Combination_WprimeAll_2017_M"+mS+".txt")
