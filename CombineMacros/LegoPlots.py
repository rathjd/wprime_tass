import os,sys

from ROOT import TH1F, TFile, THStack, TRatioPlot, TCanvas, TLegend, TLine, TF1, TMatrixD, TFitResultPtr, TGraph, TColor, TPaveStats
import cmsstyle as CMS
import math

jbBins = ["53", "63", "64"]

inFile = TFile("LegoOptimization.root","READ")

CMS.SetLumi("138")
CMS.SetEnergy("13")

for j in range(5,7):
    for mass in range(3,12):
        for jbBin in jbBins:
            #only run, if jet multiplicity fits
            if str(j) == jbBin[0]:
                BinN = 52
                if jbBin[0] == "6":
                    BinN = 23

                hist = inFile.Get("Lego"+str(j)+"j_Wp"+str(mass*100)+"_11"+jbBin+"_21"+jbBin)
                leg = CMS.cmsLeg(0.2,0.89-0.05*1,0.6,0.89,textSize=0.05)
                if jbBin == "53":
                    CMS.cmsHeader(leg, "#mu/e + 5 jets and 3 b tags", textSize=0.05)
                elif jbBin == "63":
                    CMS.cmsHeader(leg, "#mu/e + 6 jets and 3 b tags", textSize=0.05)
                elif jbBin == "64":
                    CMS.cmsHeader(leg, "#mu/e + 6 jets and 4 b tags", textSize=0.05)

                canv = CMS.cmsCanvas("canv_Lego"+str(j)+"j_Wp"+str(mass*100)+"_11"+jbBin+"_21"+jbBin, -0.5, BinN+0.5, -0.5, BinN+0.5, "last bin of medium conf.", "last bin of high conf.", square=CMS.kSquare, extraSpace=0.01, iPos=0, with_z_axis=True, scaleLumi=0.8)
                canv.cd(1)
                hist.GetZaxis().SetTitle("median 95% CL asymptotic exp. limit")
                hist.GetZaxis().SetTitleOffset(1.4)
                hist.Draw("same colz")
                CMS.SetAlternative2DColor(hist, CMS.cmsStyle)
                CMS.UpdatePalettePosition(hist, canv)

                leg.Draw()

                CMS.SaveCanvas(canv, "Lego"+str(j)+"j_Wp"+str(mass*100)+"_11"+jbBin+"_21"+jbBin+".pdf")


