import os,sys
from ROOT import TGraph, TFile, TTree, TCanvas, TColor, TLegend, TLatex, kBlack, TF1
from array import array
import cmsstyle as CMS
import math
import numpy as np

#generate dictionaries for theoretical preferences from RD and RD* values with uncertainties
#from HFLAV CKM2025 version

RD_SM    = 0.296
RDsig_SM = 0.004
RDs_SM   = 0.254
RDsSig_SM= 0.005

RD_NP    = 0.358
RDsig_NP = 0.024
RDs_NP   = 0.281
RDsSig_NP= 0.011

RDconfig = {}
RDsConfig = {}

for m in range(3,12):
    couplingProductRD    = (math.sqrt(RD_NP/RD_SM) - 1.) / 761969 * (m*100)**2
    couplingProductRDsig = (m*100)**2 * math.sqrt(RDsig_NP**2 / (2 * 761969 * math.sqrt(RD_SM * RD_NP))**2 + RDsig_SM**2 * (math.sqrt(RD_NP / RD_SM**3) / (2 * 761969))**2)
    RDconfig[m*100]      = [couplingProductRD, couplingProductRDsig]

    couplingProductRDs    = (math.sqrt(RDs_NP/RDs_SM) - 1.) / 761969 * (m*100)**2
    couplingProductRDsSig = (m*100)**2 * math.sqrt(RDsSig_NP**2 / (2 * 761969 * math.sqrt(RDs_SM * RDs_NP))**2 + RDsSig_SM**2 * (math.sqrt(RDs_NP / RDs_SM**3) / (2 * 761969))**2)
    RDsConfig[m*100]      = [couplingProductRDs, couplingProductRDsSig]

#dictionary holding the theoretical preferences for RD
#           mWp: [RD central, RD unc.]
#RDconfig = {300: [0.0098,   0.0047],
#            400: [0.017,    0.008],
#            500: [0.027,    0.013],
#            600: [0.039,    0.053],
#            700: [0.053,    0.026],
#            800: [0.069,    0.033],
#            900: [0.088,    0.042],
#            1000:[0.11,     0.05],
#            1100:[0.13,     0.06]}

#dictionary holding the theoretical preferences for RD*
#           mWp: [RD* central, RD* unc.]
#RDsConfig = {300: [0.0077,   0.0029],
#             400: [0.014,    0.005],
#             500: [0.021,    0.008],
#             600: [0.031,    0.012],
#             700: [0.042,    0.016],
#             800: [0.054,    0.021],
#             900: [0.069,    0.026],
#             1000:[0.085,    0.032],
#             1100:[0.103,    0.039]}

#dictionary of expected limit r values in order of 2.5%, 16%, 50%, 84%, 97.5%, and observed (-1 if not available)
#Excl    = {300: 99.92,
#           400: 90.25,
#           500: 69.81,
#           600: 54.34,
#           700: 44.02,
#           800: 36.07,
#           900: 31.42,
#           1000:26.36,
#           1100:23.79}
Excl    = { 300: [0.0390,    0.0518,    0.0718,   0.1004,   0.1340,   -1], 
            400: [0.0744991, 0.0989944, 0.137207, 0.191351, 0.254879, -1],
            500: [0.115288,  0.153795,  0.213867, 0.296558, 0.394677, -1],
            600: [0.170563,  0.227533,  0.316406, 0.441266, 0.589798, -1],
            700: [0.246368,  0.328659,  0.457031, 0.637384, 0.85193,  -1],
            800: [0.337967,  0.450852,  0.626953, 0.87436,  1.16867,  -1],
            900: [0.48642,   0.64889,   0.902344, 1.26202,  1.69038,  -1],
           1000: [0.646454,  0.862378,  1.19922,  1.67723,  2.25421,  -1],
           1100: [0.9012,    1.2023,    1.6719,   2.3450,   3.1474,   -1]}

#MC production parameters and outcomes for our signal samples
#           mWp: gq, gtau, cs prediction [fb]
MCPars  = {300: [1.28, 0.0084, 683.8+708.3],
           400: [1.2 , 0.0159, 321.7+336.1],
           500: [1.17, 0.0256, 161.1+165.3],
           600: [1.15, 0.0374, 85.92+85.82],
           700: [1.14, 0.0513, 48.84+47.47],
           800: [1.14, 0.0674, 29.81+27.73],
           900: [1.13, 0.0856, 18.33+16.49],
           1000:[1.13, 0.106 , 11.73+10.25],
           1100:[1.13, 0.129 , 7.683+6.546]}

#total decay width calculation
def Gamma_bt(mWp, gq):
    #return 3. * gq**2 * (172.5**2 / mWp**2 + 2.)
    return gq**2 * mWp / 16 / math.pi * (1 - (172.5/mWp)**2)**2 * (1 + (172.5/mWp)**2 / 2)

def Gamma_bc(mWp, gq):
    return gq**2 * mWp / 8 / math.pi

def Gamma_tau(mWp, gtau):
    return gtau**2 * mWp / 24 / math.pi

def TotalDecayWidth(mWp, gq, gtau):
    return Gamma_bt(mWp, gq) + Gamma_bc(mWp, gq) + Gamma_tau(mWp, gtau)

def TheoryCurve(mWp, gq, isStar):
    #list of central, upper, and lower bound values
    gtaus = [0., 0., 0.]
    if isStar:
        gtaus[0] =  RDsConfig[mWp][0] / gq
        gtaus[1] = (RDsConfig[mWp][0] + RDsConfig[mWp][1]) / gq
        gtaus[2] = (RDsConfig[mWp][0] - RDsConfig[mWp][1]) / gq
    else:
        gtaus[0] =  RDconfig[mWp][0] / gq
        gtaus[1] = (RDconfig[mWp][0] + RDconfig[mWp][1]) / gq
        gtaus[2] = (RDconfig[mWp][0] - RDconfig[mWp][1]) / gq
    return gtaus

#calculates the cross section scaling constant, depends on sample generation only
def CSscaling(mWp):
    pars = MCPars[mWp]
    return pars[2] / pars[0]**2 / Gamma_bt(mWp, pars[0]) * TotalDecayWidth(mWp, pars[0], pars[1])

def WidthVal(mWp, gq):
    gtau = math.sqrt((0.1 - (Gamma_bc(mWp, gq) + Gamma_bt(mWp, gq)) / mWp ) * 24 * math.pi )
    return gtau

#calculates the exclusion limit translation to the coupling space from the simplified parametrized formula according to the mass
def ExpExcl(mWp, gq, excl, scaling):
    gtau = -1.
    #print("calculating exclusion for",mWp, gq, excl, scaling)
    try:
        #gtau = math.sqrt(24. * math.pi * gq**2 / mWp * ( 3. * (172.5**2 / mWp**2 + 2.) * (gq**2 * scaling / excl - 1.) + mWp / (8 * math.pi)))
        gtau = math.sqrt(24 * math.pi / mWp * (Gamma_bt(mWp, gq) * (scaling * gq**2 / excl - 1.) - Gamma_bc(mWp, gq)))
    except:
        #print("no cross section exceeding exclusion possible")
        #print("invalid complex gtau for gq", gq, ", excl vs predicted =", excl, "vs", scaling * gq**2 * Gamma_bt(mWp, gq) / TotalDecayWidth(mWp, gq, gtau))
        return [gtau, -1]
    if TotalDecayWidth(mWp, gq, gtau)/mWp <= 0.1:
        #print("valid gtau =",gtau)
        return [gtau, 0]
    else:
        #calculate the 10% width border, instead
        try:
            gtau = WidthVal(mWp, gq)
            return [gtau, 1]
        except:
            #print("hadronic decay width alone exceeds 10%")
            return [gtau, -1]
        #print("Total decay width exceeds 10%", TotalDecayWidth(mWp, gq, gtau)/mWp, mWp, gq, gtau)
        #return -1.

#in the approximation of gtau equaling 0, what gq does the exclusion cross section correspond to?
def GtauZero(mWp, excl, scaling):
    return math.sqrt(excl/scaling * (1 + 1./(0.5 * (1-(172.5/mWp)**2)**2 * (1 +(172.5/mWp)**2/2))))

#in the limit of gtau = 0, when does the hadronic relative width exceed 10%?
def HadWidthTooLarge(mWp):
    return 4 * math.sqrt(math.pi/5) * mWp**3 / math.sqrt(172.5**6 - 3 * 172.5**2 * mWp**4 + 6 * mWp**6)

#fill values from a list of [gq,gtau] values in clockwise plotting order
def FillArrayFromBand(band, mode, Xarray, Yarray, WidBand):
    #initial filling, has to be the +sigma variation, logic is the same for observed and expected (no band)
    if mode >= 0:
        for val in band:
            Xarray.append(val[0])
            Yarray.append(val[1])
    #reverse order filling, also needs to bridgge the 10% width function edgeg
    else:
        #insert width band values from the end of Xarray as already filled with the width band values until hitting the limit of the last entry of the new width band for the -sigma variation
        if len(band) > 0:
            minGq = Xarray[len(Xarray)-1]
            maxGq = band[len(band)-1][0] 
            for val in WidBand:
                if val[0] > minGq and val[0] < maxGq:
                    Xarray.append(val[0])
                    Yarray.append(val[1])
            #insert -sigma band in reverse order
            for val in reversed(band):
                Xarray.append(val[0])
                Yarray.append(val[1])
        else: #case where the band is just beyond the max 10% width
            minGq = Xarray[len(Xarray)-1]
            for val in WidBand:
                if val[0] > minGq:
                    Xarray.append(val[0])
                    Yarray.append(val[1])

            
    return [Xarray, Yarray]

#function to make a plot for a given mWp and exclusion limit
def MakeCouplingsPlot(excl, mWp):
    
    #define TGraph arrays
    RDcentralX = array('d')
    RDsCentralX= array('d')
    RDbandX    = array('d')
    RDsBandX   = array('d')
    OneSigmaX  = array('d')
    TwoSigmaX  = array('d')
    ExpX       = array('d')
    ObsX       = array('d')
    WidX       = array('d')

    RDcentralY = array('d')
    RDsCentralY= array('d')
    RDbandY    = array('d')
    RDsBandY   = array('d')
    OneSigmaY  = array('d')
    TwoSigmaY  = array('d')
    ExpY       = array('d')
    ObsY       = array('d')
    WidY       = array('d')

    
    #define lists of gq-ordered values
    gtausRD     = []
    gtausRDs    = []
    gtaus10pWid = []

    #define the gQ maximum where 10% is exceeded by the hadronic width
    maxGq = HadWidthTooLarge(mWp)

    #define theory curve
    for gqMult in range(1,1000):
        gq = gqMult * 0.01
        gtausRD.append( [gq, TheoryCurve(mWp, gq, False)])
        gtausRDs.append([gq, TheoryCurve(mWp, gq, True)])
        if gq <= maxGq:
            gtaus10pWid.append([gq, WidthVal(mWp, gq)])

    #add endpoint for 10% width curve
    gtaus10pWid.append([maxGq, 0.01])

    #calculate scaling for the current masspoint
    scale = CSscaling(mWp)
    print(mWp, "scale is", scale)

    #get the exclusion band members
    tempBands = []
    for exc in excl:
        #deactivate observed or invalid values
        if exc < 0:
            continue

        excVal = exc * MCPars[mWp][2]

        #temporary list of gq-ordered exclusion values
        gtausExcl   = []

        #define the gtau = 0 starting point as an approximation
        minGq = GtauZero(mWp, excVal, scale)
        if minGq < maxGq:
            gtausExcl.append([minGq, 0.01])

            #define a gtau scan between minimum and maximum values for exp limits
            for i in range(1, 100):
                gq = minGq + (maxGq-minGq)/100. * float(i)
                #print(gq, maxGq, minGq)
                ExpExclRes = ExpExcl(mWp, gq, excVal, scale)
                if ExpExclRes[1] == 0: #normal cross section exclusion values
                    gtausExcl.append([gq, ExpExclRes[0]])
                elif ExpExclRes[1] == -1: #catch invalid gtau results
                    continue
                elif ExpExclRes[1] == 1: #first value of 10% width line, break once hit
                    gtausExcl.append([gq, ExpExclRes[0]])
                    break

        tempBands.append(gtausExcl)
    
    #dump tempBands into appropriate band values from tempBand[+2, +1, exp, -1, -2, obs]
    #2 sigma band
    TwoSigmas = FillArrayFromBand(tempBands[0],  1, TwoSigmaX, TwoSigmaY, gtaus10pWid)
    TwoSigmaX = TwoSigmas[0]
    TwoSigmaY = TwoSigmas[1]
    TwoSigmas = FillArrayFromBand(tempBands[4], -1, TwoSigmaX, TwoSigmaY, gtaus10pWid)
    TwoSigmaX = TwoSigmas[0]
    TwoSigmaY = TwoSigmas[1]
    #1 sigma band
    OneSigmas = FillArrayFromBand(tempBands[1],  1, OneSigmaX, OneSigmaY, gtaus10pWid)
    OneSigmaX = OneSigmas[0]
    OneSigmaY = OneSigmas[1]
    OneSigmas = FillArrayFromBand(tempBands[3], -1, OneSigmaX, OneSigmaY, gtaus10pWid)
    OneSigmaX = OneSigmas[0]
    OneSigmaY = OneSigmas[1]
    #exp limit
    ExpLim = FillArrayFromBand(tempBands[2],  1, ExpX, ExpY, gtaus10pWid)
    ExpX   = ExpLim[0]
    ExpY   = ExpLim[1]

    #build theory TGraphs
    for gtau in gtausRD:
        RDcentralX.append(gtau[0])
        RDcentralY.append(gtau[1][0])
        RDbandX.append(gtau[0])
        RDbandY.append(gtau[1][1])
    for gtau in reversed(gtausRD):
        RDbandX.append(gtau[0])
        RDbandY.append(gtau[1][2])

    RDcentralGraph = TGraph(len(RDcentralX), RDcentralX, RDcentralY)
    RDbandGraph    = TGraph(len(RDbandX),    RDbandX,    RDbandY)

    for gtau in gtausRDs:
        RDsCentralX.append(gtau[0])
        RDsCentralY.append(gtau[1][0])
        RDsBandX.append(gtau[0])
        RDsBandY.append(gtau[1][1])
    for gtau in reversed(gtausRDs):
        RDsBandX.append(gtau[0])
        RDsBandY.append(gtau[1][2])

    RDsCentralGraph = TGraph(len(RDsCentralX), RDsCentralX, RDsCentralY)
    RDsBandGraph    = TGraph(len(RDsBandX),    RDsBandX,    RDsBandY)

    for gtau in gtaus10pWid:
        WidX.append(gtau[0])
        WidY.append(gtau[1])

    WidGraph = TGraph(len(WidX), WidX, WidY)

    #build experimental TGraphs
    if len(ExpX) > 0:
        ExclusionGraph  = TGraph(len(ExpX), ExpX, ExpY)

    if len(OneSigmaX) > 0:
        OneSigmaGraph = TGraph(len(OneSigmaX), OneSigmaX, OneSigmaY)
    if len(TwoSigmaX) > 0:
        TwoSigmaGraph = TGraph(len(TwoSigmaX), TwoSigmaX, TwoSigmaY)

    #style configurations
    RDcentralGraph.SetLineColor(2)
    RDcentralGraph.SetLineStyle(1)
    RDcentralGraph.SetLineWidth(2)

    RDbandGraph.SetLineColor(2)
    RDbandGraph.SetLineStyle(2)
    RDbandGraph.SetLineWidth(2)

    RDsCentralGraph.SetLineColor(4)
    RDsCentralGraph.SetLineStyle(1)
    RDsCentralGraph.SetLineWidth(2)

    RDsBandGraph.SetLineColor(4)
    RDsBandGraph.SetLineStyle(2)
    RDsBandGraph.SetLineWidth(2)

    WidGraph.SetLineColor(6)
    WidGraph.SetLineStyle(3)
    WidGraph.SetLineWidth(2)

    if len(ExpX) > 0:
        ExclusionGraph.SetLineColor(1)
        ExclusionGraph.SetLineWidth(2)
        ExclusionGraph.SetLineStyle(2)

    #official colors for Brazil band
    if len(OneSigmaX) > 0:
        OneSigmaGraph.SetFillColor(TColor.GetColor("#228b22"))
    if len(TwoSigmaX) > 0:
        TwoSigmaGraph.SetFillColor(TColor.GetColor("#ffcc00"))

    #build CMS plot
    CMS.SetEnergy("13")
    CMS.SetLumi("138")
    CMS.SetExtraText("Simulation Preliminary")
    canvas = CMS.cmsCanvas("CouplingSpace_MWpr"+str(mWp), 0.01, 10., 0.01, 10., "g_{q}", "g_{#tau}", square = CMS.kSquare, extraSpace = 0.01, iPos = 0)
    canvas.cd(1)

    if len(TwoSigmaX) > 0:
        TwoSigmaGraph.Draw("F")
        OneSigmaGraph.Draw("F,same")
        RDbandGraph.Draw("L,same")
    elif len(OneSiggmaX) > 0:
        OneSigmaGraph.Draw("F")
        RDbandGraph.Draw("L,same")
    else:
        RDbandGraph.Draw("L")
    RDsBandGraph.Draw("L,same")
    RDcentralGraph.Draw("L,same")
    RDsCentralGraph.Draw("L,same")
    if len(ExpX) > 0:
        ExclusionGraph.Draw("L,same")
    WidGraph.Draw("L,same")

    legend = CMS.cmsLeg(0.20, 0.89-0.04*6, 0.89, 0.89, textSize=0.04)
    legend.SetHeader("m_{W'} = "+str(mWp)+" GeV")
    legend.AddEntry(RDcentralGraph, "R(D) prediction", "l")
    #legend.AddEntry(RDbandGraph, "R(D) #pm 1 #sigma", "l")
    legend.AddEntry(RDsCentralGraph, "R(D*) prediction", "l")
    legend.AddEntry(WidGraph, "10% relative decay width", "l")
    #legend.AddEntry(RDsBandGraph, "R(D*) #pm 1 #sigma", "l")
    if len(ExpX) > 0:
        legend.AddEntry(ExclusionGraph, "median exp. upper limit", "l")
    if len(OneSigmaX) > 0:
        legend.AddEntry(OneSigmaGraph, "68% exp. upper limit", "f")
    if len(TwoSigmaX) > 0:
        legend.AddEntry(TwoSigmaGraph, "95% exp. upper limit", "f")

    canvas.SetLogx(True)
    canvas.SetLogy(True)

    CMS.SaveCanvas(canvas, "CouplingSpace_MWpr"+str(mWp)+".pdf")

#actually run this for all masses
for mass in range(0,9):
    mWp = (mass+3)*100
    MakeCouplingsPlot(Excl[mWp], mWp)
