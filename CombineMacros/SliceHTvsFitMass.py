import os,sys

from ROOT import TH2F, TFile, gDirectory

#macro to splice a bin between fitted mass and HT along -log(likelihood) values

#configure bin
binNr = "1153"
binSplit = 8

#load file with 2D distributions and get filenames
inFile = TFile("TwoD_SimpleShapes_Wprime"+binNr+".root","READ")

FileContent = [key.GetName() for key in gDirectory.GetListOfKeys()]

#define output files
outFileHT = TFile("HTsplices_SimpleShapes_Wprime"+binNr+".root","RECREATE")
outFileFit = TFile("FitSplices_SimpleShapes_Wprime"+binNr+".root","RECREATE")

#run over keys, sort and Project them appropriately
for content in FileContent:
    if content.find("HT2D_") > -1:
        HT = inFile.Get(content)
        keyname = content.replace("HT2D_", "HT_")
        ProjHT = HT.ProjectionX(keyname, binSplit+1, -1)
        outFileHT.cd()
        ProjHT.Write()
    elif content.find("FitMass2D_") > -1:
        Fit = inFile.Get(content)
        keyname = content.replace("FitMass2D_", "Fit_")
        ProjFit = Fit.ProjectionX(keyname, 0, binSplit)
        outFileFit.cd()
        ProjFit.Write()
    elif content.find("HT_data_obs_") > -1:
        HT = inFile.Get(content)
        keyname = content.replace("HT_", "HT_")
        ProjHT = HT.ProjectionX(keyname, binSplit+1, -1)
        outFileHT.cd()
        ProjHT.Write()
    elif content.find("FitMass_data_obs_") > -1:
        Fit = inFile.Get(content)
        keyname = content.replace("FitMass_", "Fit_")
        ProjFit = Fit.ProjectionX(keyname, 0, binSplit)
        outFileFit.cd()
        ProjFit.Write()

outFileHT.Close()
outFileFit.Close()
inFile.Close()
    
