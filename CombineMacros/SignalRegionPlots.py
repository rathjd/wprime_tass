import os,sys
from ROOT import TGraph, TFile, TTree, TCanvas, TColor, TLegend, TLatex, kBlack, THStack, TLine
from array import array
import cmsstyle as CMS
import math

#ResultsPath = '/eos/cms/store/group/phys_b2g/wprime/PatrickCombine/allYears_OptimizedSlices/'
ResultsPath = 'CombinationAll/'

#lists of bins, years, systematics, backgrounds
Years = ['2016','2017','2018']
Bins  = ['1153','2153','1163','2163','1164','2164']
backgrounds = [["ttbar", 2],
               ["wjets", 3],
               ["single_top", 4],
               ["diboson", 5]]

#loop over years
for yearName in Years:

    #CMS style setup
    if yearName == '2016':
        CMS.SetLumi("36.3")
    elif yearName == '2017':
        CMS.SetLumi("41.5")
    elif yearName == '2018':
        CMS.SetLumi("59.8")
    CMS.SetEnergy("13")

    #general systematics to be considered
    baseSystematics = ["LumiCorr",
                       "LumiStat"+yearName,
                       "electron"+yearName,
                       "muonTrigger"+yearName,
                       "muonId"+yearName,
                       "muonIso"+yearName,
                       "BjetTagCorr",
                       "BjetTagUncorr"+yearName,
                       "PUID"+yearName,
                       "L1PreFiring"+yearName,
                       "PUreweight"+yearName,
                       "PDF",
                       "LHEScale",
                       "electronRes"+yearName,
                       "JES"+yearName,
                       "JER"+yearName]

    #loop over bins
    for binString in Bins:
        #card-based systematics to be considered
        CardSystematics = ["electronScale"+yearName,
                           "NLLnonClosure"+yearName+"_"+binString[0:3]+"2"]

        #title configuration
        if binString[0] == 1:
            if binString[2] == 5:
                CMS.cmsHeader(leg1b, "#mu + 5 jets and 1 b-tag", textSize=0.05)
                CMS.cmsHeader(leg2b, "#mu + 5 jets and 2 b-tags rew.", textSize=0.05)
                CMS.cmsHeader(leg2braw, "#mu + 5 jets and 2 b-tags", textSize=0.05)
                CMS.cmsHeader(leg2bNLL, "#mu + 5 jets and 2 b-tags", textSize=0.05)
            elif binString[2] == 6:
                CMS.cmsHeader(leg1b, "#mu + 6 jets and 1 b-tag", textSize=0.05)
                CMS.cmsHeader(leg2b, "#mu + 6 jets and 2 b-tags rew.", textSize=0.05)
                CMS.cmsHeader(leg2braw, "#mu + 6 jets and 2 b-tags", textSize=0.05)
                CMS.cmsHeader(leg2bNLL, "#mu + 6 jets and 2 b-tags", textSize=0.05)
        elif binString[0] == 2:
            if binString[2] == 5:
                CMS.cmsHeader(leg1b, "e + 5 jets and 1 b-tag", textSize=0.05)
                CMS.cmsHeader(leg2b, "e + 5 jets and 2 b-tags rew.", textSize=0.05)
                CMS.cmsHeader(leg2braw, "e + 5 jets and 2 b-tags", textSize=0.05)
                CMS.cmsHeader(leg2bNLL, "e + 5 jets and 2 b-tags", textSize=0.05)
            elif binString[2] == 6:
                CMS.cmsHeader(leg1b, "e + 6 jets and 1 b-tag", textSize=0.05)
                CMS.cmsHeader(leg2b, "e + 6 jets and 2 b-tags rew.", textSize=0.05)
                CMS.cmsHeader(leg2braw, "e + 6 jets and 2 b-tags", textSize=0.05)
                CMS.cmsHeader(leg2bNLL, "e + 6 jets and 2 b-tags", textSize=0.05)

        #loop over masspoints
        for mass in range(3,12):
            mStr = str(mass*100)
            identifier = binString + "_" + yearName + "_M" + mStr

            #generate output histogram strings
            fitHistName = 'FitMass_' + identifier
            HThistName = 'HT_' + identifier

            #define input files
            inFitROOT = TFile(ResultsPath + "FitSlices_Wprime" + identifier + ".root", "READ")
            inFitCard = open(ResultsPath + "FitSlice_Wprime" + identifier + ".txt", "r")
            inFitLines = inFitCard.readlines()

            inHTROOT  = TFile(ResultsPath + "HTslices_Wprime" + identifier + ".root", "READ")
            inHTcard  = open(ResultsPath + "HTslice_Wprime" + identifier + ".txt", "r")
            inHTlines = inHTcard.readlines()

            #make empty stacks
            StackFit = THStack("StackFit_" + identifier, "")
            StackHT  = THStack("StackHT_" + identifier, "")

            #configure legends
            legFit = CMS.cmsLeg(0.51,0.89-0.05*8, 0.8, 0.89, textSize=0.05)
            legHT  = CMS.cmsLeg(0.51,0.89-0.05*8, 0.8, 0.89, textSize=0.05)

            #determine number of bins and axis limits
            BinOriginFit = inFitROOT.Get("Fit_data_obs_WprimeFit" + identifier + "_")
            FitStart = BinOriginFit.GetBinLowEdge(1)
            FitBins  = BinOriginFit.GetNbinsX()
            FitEnd   = BinOriginFit.GetBinLowEdge(FitBins+1)
            BinOriginFit.SetMarkerStyle(20)
            BinOriginFit.SetMarkerColor(1)
            BinOriginFit.SetLineColor(1)

            BinOriginHT = inHTROOT.Get("HT_data_obs_WprimeHT" + identifier + "_")
            HTstart = BinOriginHT.GetBinLowEdge(1)
            HTbins  = BinOriginHT.GetNbinsX()
            HTend   = BinOriginHT.GetBinLowEdge(HTbins+1)
            BinOriginHT.SetMarkerStyle(20)
            BinOriginHT.SetMarkerColor(1)
            BinOriginHT.SetLineColor(1)

            #fill background dictionaries
            BgrFit = {}
            BgrFitSystUp = [0]*FitBins
            BgrFitSystDown = [0]*FitBins

            BgrHT = {}
            BgrHTsystUp = [0]*HTbins
            BgrHTsystDown = [0]*HTbins

            #add signal uncertainties
            SigFitSystUp = [0]*FitBins
            SigFitSystDown = [0]*FitBins

            SigHTsystUp = [0]*HTbins
            SigHTsystDown = [0]*HTbins

            #assemble the main histograms
            for background in backgrounds:

                BgrPartFit = inFitROOT.Get("Fit_" + background[0] + "_WprimeFit" + identifier + "_")

                BgrPartHT = inHTROOT.Get("HT_" + background[0] + "_WprimeHT" + identifier + "_")

                if background[1]==2:
                    BgrTotalFit = BgrPartFit.Clone("BgrTotalFit")

                    BgrTotalHT = BgrPartHT.Clone("BgrTotalHT")
                else:
                    BgrTotalFit.Add(BgrPartFit)

                    BgrTotalHT.Add(BgrPartHT)

                BgrPartFit.Scale(1., "width")
                BgrFit[background[0]] = BgrPartFit

                BgrPartHT.Scale(1., "width")
                BgrHT[background[0]] = BgrPartHT

                #statistical uncertainty of a background per bin
                for binN in range(0, FitBins):
                    BgrFitSystUp[binN]   += pow(BgrPartFit.GetBinError(binN+1), 2)
                    BgrFitSystDown[binN] += pow(BgrPartFit.GetBinError(binN+1), 2)

                for binN in range(0, HTbins):
                    BgrHTsystUp[binN]   += pow(BgrPartHT.GetBinError(binN+1), 2)
                    BgrHTsystDown[binN] += pow(BgrPartHT.GetBinError(binN+1), 2)

                #determine the uncertainties per bin
                for syst in baseSystematics:
                    UpFit   = inFitROOT.Get("Fit_" + background[0] + "_WprimeFit" + identifier + "_" + syst + "Up")
                    DownFit = inFitROOT.Get("Fit_" + background[0] + "_WprimeFit" + identifier + "_" + syst + "Down")
                    UpFit.Scale(1., "width")
                    DownFit.Scale(1., "width")

                    UpHT   = inHTROOT.Get("HT_" + background[0] + "_WprimeHT" + identifier + "_" + syst + "Up")
                    DownHT = inHTROOT.Get("HT_" + background[0] + "_WprimeHT" + identifier + "_" + syst + "Down")
                    UpHT.Scale(1., "width")
                    DownHT.Scale(1., "width")

                    for binN in range(0, FitBins):
                        BgrFitSystUp[binN]   += pow(max(UpFit.GetBinContent(binN+1)   - BgrPartFit.GetBinContent(binN+1),
                                                        DownFit.GetBinContent(binN+1) - BgrPartFit.GetBinContent(binN+1),
                                                        0.), 2)
                        BgrFitSystDown[binN] += pow(min(UpFit.GetBinContent(binN+1)   - BgrPartFit.GetBinContent(binN+1),
                                                        DownFit.GetBinContent(binN+1) - BgrPartFit.GetBinContent(binN+1),
                                                        0.), 2)

                    for binN in range(0, HTbins):
                        BgrHTsystUp[binN]   += pow(max(UpHT.GetBinContent(binN+1)   - BgrPartHT.GetBinContent(binN+1),
                                                       DownHT.GetBinContent(binN+1) - BgrPartHT.GetBinContent(binN+1),
                                                       0.), 2)
                        BgrHTsystDown[binN] += pow(min(UpHT.GetBinContent(binN+1)   - BgrPartHT.GetBinContent(binN+1),
                                                       DownHT.GetBinContent(binN+1) - BgrPartHT.GetBinContent(binN+1),
                                                       0.), 2)
                
                #add card-based systematics
                for line in inFitLines:
                    for syst in CardSystematics:
                        if line.find(syst) > -1:
                            entries = line.split()
                            if entries[background[1]+1] == "-":
                                continue
                            if float(entries[background[1]+1]) == 0.:
                                continue
                            for binN in range(0, FitBins):
                                BgrFitSystUp[binN]   += pow((float(entries[background[1]+1])-1.) * BgrPartFit.GetBinContent(binN+1), 2)
                                BgrFitSystDown[binN] += pow((float(entries[background[1]+1])-1.) * BgrPartFit.GetBinContent(binN+1), 2)

                for line in inHTlines:
                    for syst in CardSystematics:
                        if line.find(syst) > -1:
                            entries = line.split()
                            if entries[background[1]+1] == "-":
                                continue
                            if float(entries[background[1]+1]) == 0.:
                                continue
                            for binN in range(0, HTbins):
                                BgrHTsystUp[binN]   += pow((float(entries[background[1]+1])-1.) * BgrPartHT.GetBinContent(binN+1), 2)
                                BgrHTsystDown[binN] += pow((float(entries[background[1]+1])-1.) * BgrPartHT.GetBinContent(binN+1), 2)

            #get signal
            SigFit = inFitROOT.Get("Fit_M" + mStr + "_WprimeFit" + identifier + "_")
            SigFit.Scale(1., "width")

            SigHT = inHTROOT.Get("HT_M" + mStr + "_WprimeHT" + identifier + "_")
            SigHT.Scale(1., "width")

            #statistical uncertainty of the signal per bin
            for binN in range(0, FitBins):
                SigFitSystUp[binN]   += pow(SigFit.GetBinError(binN+1), 2)
                SigFitSystDown[binN] += pow(SigFit.GetBinError(binN+1), 2)

            for binN in range(0, HTbins):
                SigHTsystUp[binN]   += pow(SigHT.GetBinError(binN+1), 2)
                SigHTsystDown[binN] += pow(SigHT.GetBinError(binN+1), 2)

            #determine the uncertainties per bin
            for syst in baseSystematics:
                UpFit   = inFitROOT.Get("Fit_M" + mStr + "_WprimeFit" + identifier + "_" + syst + "Up")
                DownFit = inFitROOT.Get("Fit_M" + mStr + "_WprimeFit" + identifier + "_" + syst + "Down")
                UpFit.Scale(1., "width")
                DownFit.Scale(1., "width")

                UpHT   = inHTROOT.Get("HT_M" + mStr + "_WprimeHT" + identifier + "_" + syst + "Up")
                DownHT = inHTROOT.Get("HT_M" + mStr + "_WprimeHT" + identifier + "_" + syst + "Down")
                UpHT.Scale(1., "width")
                DownHT.Scale(1., "width")

                for binN in range(0, FitBins):
                    SigFitSystUp[binN]   += pow(max(UpFit.GetBinContent(binN+1)   - SigFit.GetBinContent(binN+1),
                                                    DownFit.GetBinContent(binN+1) - SigFit.GetBinContent(binN+1),
                                                    0.), 2)
                    SigFitSystDown[binN] += pow(min(UpFit.GetBinContent(binN+1)   - SigFit.GetBinContent(binN+1),
                                                    DownFit.GetBinContent(binN+1) - SigFit.GetBinContent(binN+1),
                                                    0.), 2)

                for binN in range(0, HTbins):
                    SigHTsystUp[binN]   += pow(max(UpHT.GetBinContent(binN+1)   - SigHT.GetBinContent(binN+1),
                                                   DownHT.GetBinContent(binN+1) - SigHT.GetBinContent(binN+1),
                                                   0.), 2)
                    SigHTsystDown[binN] += pow(min(UpHT.GetBinContent(binN+1)   - SigHT.GetBinContent(binN+1),
                                                   DownHT.GetBinContent(binN+1) - SigHT.GetBinContent(binN+1),
                                                   0.), 2)
            #add card-based systematics for signal
            for line in inFitLines:
                for syst in CardSystematics:
                    if line.find(syst) > -1:
                        entries = line.split()
                        if entries[2] == "-":
                                continue
                        if float(entries[2]) == 0.:
                                continue
                        for binN in range(0, FitBins):
                            SigFitSystUp[binN]   += pow((float(entries[2])-1.) * SigFit.GetBinContent(binN+1), 2)
                            SigFitSystDown[binN] += pow((float(entries[2])-1.) * SigFit.GetBinContent(binN+1), 2)
            for line in inHTlines:
                for syst in CardSystematics:
                    if line.find(syst) > -1:
                        entries = line.split()
                        if entries[2] == "-":
                                continue
                        if float(entries[2]) == 0.:
                                continue
                        for binN in range(0, HTbins):
                            SigHTsystUp[binN]   += pow((float(entries[2])-1.) * SigHT.GetBinContent(binN+1), 2)
                            SigHTsystDown[binN] += pow((float(entries[2])-1.) * SigHT.GetBinContent(binN+1), 2)

            #scale all totals
            BgrTotalFit.Scale(1., "width")
            BgrTotalHT.Scale(1., "width")

            #get all uncertainty values
            for binN in range(0, FitBins):
                BgrFitSystUp[binN]   = math.sqrt(BgrFitSystUp[binN])
                BgrFitSystDown[binN] = math.sqrt(BgrFitSystDown[binN])
                SigFitSystUp[binN]   = math.sqrt(SigFitSystUp[binN])
                SigFitSystDown[binN] = math.sqrt(SigFitSystDown[binN])

            for binN in range(0, HTbins):
                BgrHTsystUp[binN]   = math.sqrt(BgrHTsystUp[binN])
                BgrHTsystDown[binN] = math.sqrt(BgrHTsystDown[binN])
                SigHTsystUp[binN]   = math.sqrt(SigHTsystUp[binN])
                SigHTsystDown[binN] = math.sqrt(SigHTsystDown[binN])

            #fill error bands
            ErrFitX, ErrFitY, ErrFitR, ErrHTX, ErrHTY, ErrHTR = array('d'), array('d'), array('d'), array('d'), array('d'), array('d')
            ErrSigFitY, ErrSigHTY = array('d'), array('d')

            for binN in range(0, FitBins): #upper band
                ErrFitX.append(BgrTotalFit.GetBinLowEdge(binN+1))
                ErrFitY.append(BgrFitSystUp[binN] + BgrTotalFit.GetBinContent(binN+1))
                ErrFitR.append(BgrTotalFit.GetBinContent(binN+1) and BgrFitSystUp[binN] / BgrTotalFit.GetBinContent(binN+1)+1. or 0.)
                ErrSigFitY.append(SigFitSystUp[binN] + SigFit.GetBinContent(binN+1))

                ErrFitX.append(BgrTotalFit.GetBinLowEdge(binN+2))
                ErrFitY.append(BgrFitSystUp[binN] + BgrTotalFit.GetBinContent(binN+1))
                ErrFitR.append(BgrTotalFit.GetBinContent(binN+1) and BgrFitSystUp[binN] / BgrTotalFit.GetBinContent(binN+1)+1. or 0.)
                ErrSigFitY.append(SigFitSystUp[binN] + SigFit.GetBinContent(binN+1))

                #test for empty bins in signal region
                if BgrTotalFit.GetBinContent(binN+1)<=0 and SigFit.GetBinContent(binN+1) > 0:
                    print("Warning!!! Empty bin", binN, "found for mFit in",identifier)

            for binN in range(FitBins-1, -2, -1): #lower band
                ErrFitX.append(BgrTotalFit.GetBinLowEdge(binN+2))
                ErrFitY.append(-BgrFitSystDown[binN] + BgrTotalFit.GetBinContent(binN+1))
                ErrFitR.append(BgrTotalFit.GetBinContent(binN+1) and -BgrFitSystDown[binN] / BgrTotalFit.GetBinContent(binN+1)+1. or 0.)
                ErrSigFitY.append(-SigFitSystDown[binN] + SigFit.GetBinContent(binN+1))

                ErrFitX.append(BgrTotalFit.GetBinLowEdge(binN+1))
                ErrFitY.append(-BgrFitSystDown[binN] + BgrTotalFit.GetBinContent(binN+1))
                ErrFitR.append(BgrTotalFit.GetBinContent(binN+1) and -BgrFitSystDown[binN] / BgrTotalFit.GetBinContent(binN+1)+1. or 0.)
                ErrSigFitY.append(-SigFitSystDown[binN] + SigFit.GetBinContent(binN+1))

            for binN in range(0, HTbins): #upper band
                ErrHTX.append(BgrTotalHT.GetBinLowEdge(binN+1))
                ErrHTY.append(BgrHTsystUp[binN] + BgrTotalHT.GetBinContent(binN+1))
                ErrHTR.append(BgrTotalHT.GetBinContent(binN+1) and BgrHTsystUp[binN] / BgrTotalHT.GetBinContent(binN+1)+1. or 0.)
                ErrSigHTY.append(SigHTsystUp[binN] + SigHT.GetBinContent(binN+1))

                ErrHTX.append(BgrTotalHT.GetBinLowEdge(binN+2))
                ErrHTY.append(BgrHTsystUp[binN] + BgrTotalHT.GetBinContent(binN+1))
                ErrHTR.append(BgrTotalHT.GetBinContent(binN+1) and BgrHTsystUp[binN] / BgrTotalHT.GetBinContent(binN+1)+1. or 0.)
                ErrSigHTY.append(SigHTsystUp[binN] + SigHT.GetBinContent(binN+1))

                #test for empty bins in signal region
                if BgrTotalHT.GetBinContent(binN+1)<=0 and SigHT.GetBinContent(binN+1) > 0:
                    print("Warning!!! Empty bin", binN, "found for HT in",identifier)

            for binN in range(HTbins-1, -2, -1): #lower band
                ErrHTX.append(BgrTotalHT.GetBinLowEdge(binN+2))
                ErrHTY.append(-BgrHTsystDown[binN] + BgrTotalHT.GetBinContent(binN+1))
                ErrHTR.append(BgrTotalHT.GetBinContent(binN+1) and -BgrHTsystDown[binN] / BgrTotalHT.GetBinContent(binN+1)+1. or 0.)
                ErrSigHTY.append(-SigHTsystDown[binN] + SigHT.GetBinContent(binN+1))

                ErrHTX.append(BgrTotalHT.GetBinLowEdge(binN+1))
                ErrHTY.append(-BgrHTsystDown[binN] + BgrTotalHT.GetBinContent(binN+1))
                ErrHTR.append(BgrTotalHT.GetBinContent(binN+1) and -BgrHTsystDown[binN] / BgrTotalHT.GetBinContent(binN+1)+1. or 0.)
                ErrSigHTY.append(-SigHTsystDown[binN] + SigHT.GetBinContent(binN+1))

            #make fit mass plot
            maxFit = max(BgrTotalFit.GetMaximum(), SigFit.GetMaximum())*1.1

            canvFit = CMS.cmsDiCanvas("Fit_" + identifier, FitStart, FitEnd, 0, maxFit, 0.5, 1.5, "m_{fit} [GeV/c^{2}]", "Events/bin width", "Data/Pred.", square = CMS.kSquare, extraSpace = 0.1, iPos = 0)
            canvFit.cd(1)

            legFit.AddEntry(BinOriginFit, "Data", "lp")
            CMS.cmsDrawStack(StackFit, legFit, BgrFit)

            grErrFit = TGraph(len(ErrFitX), ErrFitX, ErrFitY)
            grErrFit.SetFillColor(17)
            grErrFit.SetFillStyle(3008)

            grErrFit.Draw("F, same")

            SigFit.SetLineColor(8)
            SigFit.SetLineWidth(3)
            SigFit.SetLineStyle(2)
            SigFit.SetMarkerStyle(21)
            SigFit.SetMarkerColor(2)

            grErrSigFit = TGraph(len(ErrFitX), ErrFitX, ErrSigFitY)
            grErrSigFit.SetFillColor(8)
            grErrSigFit.SetFillStyle(3008)

            grErrSigFit.Draw("F, same")
            SigFit.Draw("lp, same")

            legFit.AddEntry(SigFit, "m_{W'}="+mStr+" GeV", "lp")
            legFit.AddEntry(grErrFit, "stat. + syst. unc.", "f")

            legFit.Draw()

            CMS.fixOverlay()

            canvFit.cd(2)

            grErrFitR = TGraph(len(ErrFitX), ErrFitX, ErrFitR)
            grErrFitR.SetFillColor(17)
            grErrFitR.SetFillStyle(3008)

            grErrFitR.Draw("F")

            ref_lineFit = TLine(FitStart, 1, FitEnd, 1)
            CMS.cmsDrawLine(ref_lineFit, lcolor=1, lstyle=3)

            print("SRplots/Fit_" + identifier +".pdf")

            CMS.SaveCanvas(canvFit, "SRplots/Fit_" + identifier +".pdf")

            #make HT plot
            maxHT = max(BgrTotalHT.GetMaximum(), SigHT.GetMaximum())*1.1

            canvHT = CMS.cmsDiCanvas("HT_" + identifier, HTstart, HTend, 0, maxHT, 0.5, 1.5, "H_{T} [GeV/c]", "Events/bin width", "Data/Pred.", square = CMS.kSquare, extraSpace = 0.1, iPos = 0)
            canvHT.cd(1)

            legHT.AddEntry(BinOriginHT, "Data", "lp")
            CMS.cmsDrawStack(StackHT, legHT, BgrHT)

            grErrHT = TGraph(len(ErrHTX), ErrHTX, ErrHTY)
            grErrHT.SetFillColor(17)
            grErrHT.SetFillStyle(3008)

            grErrHT.Draw("F, same")

            SigHT.SetLineColor(8)
            SigHT.SetLineWidth(3)
            SigHT.SetLineStyle(2)
            SigHT.SetMarkerStyle(21)
            SigHT.SetMarkerColor(2)

            grErrSigHT = TGraph(len(ErrHTX), ErrHTX, ErrSigHTY)
            grErrSigHT.SetFillColor(8)
            grErrSigHT.SetFillStyle(3008)

            grErrSigHT.Draw("F, same")
            SigHT.Draw("lp, same")

            legHT.AddEntry(SigHT, "m_{W'}="+mStr+" GeV", "lp")
            legHT.AddEntry(grErrHT, "stat. + syst. unc.", "f")

            legHT.Draw()

            CMS.fixOverlay()

            canvHT.cd(2)

            grErrHTR = TGraph(len(ErrHTX), ErrHTX, ErrHTR)
            grErrHTR.SetFillColor(17)
            grErrHTR.SetFillStyle(3008)

            grErrHTR.Draw("F")

            ref_lineHT = TLine(HTstart, 1, HTend, 1)
            CMS.cmsDrawLine(ref_lineHT, lcolor=1, lstyle=3)

            CMS.SaveCanvas(canvHT, "SRplots/HT_" + identifier +".pdf")
