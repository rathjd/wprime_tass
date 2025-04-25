import os,sys
from ROOT import TFile, TH1F, TF1

#macro to test a variety of polynomials and print necessary output for F tests to determine the best function to be used

def dumpFitInformation(fit):
    print("nickname, Chi^2, ndf")
    print(fit.GetChisquare(),",",fit.GetNumberFreeParameters(),",",fit.GetName())

#get 2018 muon region ST scale factor histograms for 5j and 6j separately
eosprefix = "/eos/cms/store/group/phys_b2g/wprime/temp/"
year = "2018"
inFile5j = TFile(eosprefix+"1153_"+year+"/SF_Bin1152_"+year+".root","READ")
inFile6j = TFile(eosprefix+"1163_"+year+"/SF_Bin1162_"+year+".root","READ")

SFhist5j = inFile5j.Get("SF_")
SFhist6j = inFile6j.Get("SF_")

#define list of functions to be tested
fitFuncs = []
fitFuncs.append(TF1("pol2l","[0]/x+[1]"))
fitFuncs.append(TF1("pol2r","[0]+[1]*x"))
fitFuncs.append(TF1("pol3","[0]/x+[1]+[2]*x"))
fitFuncs.append(TF1("pol4l","[0]/x/x+[1]/x+[2]+[3]*x"))
fitFuncs.append(TF1("pol4r","[0]/x+[1]+[2]*x+[3]*x*x"))
fitFuncs.append(TF1("pol5","[0]/x/x+[1]/x+[2]+[3]*x+[4]*x*x"))
fitFuncs.append(TF1("pol6l","[0]/x/x/x+[1]/x/x+[2]/x+[3]+[4]*x+[5]*x*x"))
fitFuncs.append(TF1("pol6r","[0]/x/x+[1]/x+[2]+[3]*x+[4]*x*x+[5]*x*x*x"))
fitFuncs.append(TF1("pol7","[0]/x/x/x+[1]/x/x+[2]/x+[3]+[4]*x+[5]*x*x+[6]*x*x*x"))

#test all functions on 5j and print outputs
print("testing 5j region with",SFhist5j.GetNbinsX(),"data points")
for func in fitFuncs:
    SFhist5j.Fit(func)
for func in fitFuncs:
    dumpFitInformation(func)

#test all functions on 5j and print outputs
print("testing 6j region with",SFhist6j.GetNbinsX(),"data points")
for func in fitFuncs:
    SFhist6j.Fit(func)
for func in fitFuncs:
    dumpFitInformation(func)

#cleanup
inFile5j.Close()
inFile6j.Close()
