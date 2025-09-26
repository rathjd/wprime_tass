import os,sys
from ROOT import TGraph, TFile, TTree, TCanvas, TColor, TLegend, TLatex, kBlack, TF1
from array import array
import cmsstyle as CMS
import math
import numpy as np

#dictionary holding the theoretical preferences for RD
#           mWp: [RD central, RD unc.]
RDconfig = {300: [0.0098,   0.0047],
            400: [0.017,    0.008],
            500: [0.027,    0.013],
            600: [0.039,    0.053],
            700: [0.053,    0.026],
            800: [0.069,    0.033],
            900: [0.088,    0.042],
            1000:[0.11,     0.05],
            1100:[0.13,     0.06]}

#dictionary holding the theoretical preferences for RD*
#           mWp: [RD* central, RD* unc.]
RDsConfig = {300: [0.0077,   0.0029],
             400: [0.014,    0.005],
             500: [0.021,    0.008],
             600: [0.031,    0.012],
             700: [0.042,    0.016],
             800: [0.054,    0.021],
             900: [0.069,    0.026],
             1000:[0.085,    0.032],
             1100:[0.103,    0.039]}

#dictionary of for now 95% CL expected median exclusions
Excl    = {300: 97.20,
           400: 82.55,
           500: 65.34,
           600: 53.00,
           700: 43.83,
           800: 36.86,
           900: 31.96,
           1000:28.33,
           1100:25.68}

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
        return gtau
    if TotalDecayWidth(mWp, gq, gtau)/mWp <= 0.1:
        #print("valid gtau =",gtau)
        return gtau
    else:
        #calculate the 10% width border, instead
        try:
            gtau = math.sqrt((0.1 - (Gamma_bc(mWp, gq) + Gamma_bt(mWp, gq)) / mWp ) * 24 * math.pi )
            return gtau
        except:
            #print("hadronic decay width alone exceeds 10%")
            return -1.
        #print("Total decay width exceeds 10%", TotalDecayWidth(mWp, gq, gtau)/mWp, mWp, gq, gtau)
        #return -1.

#in the approximation of gtau equaling 0, what gq does the exclusion cross section correspond to?
def GtauZero(mWp, excl, scaling):
    return math.sqrt(excl/scaling * (1 + 1./(0.5 * (1-(172.5/mWp)**2)**2 * (1 +(172.5/mWp)**2/2))))

#in the limit of gtau = 0, when does the hadronic relative width exceed 10%?
def HadWidthTooLarge(mWp):
    return 4 * math.sqrt(math.pi/5) * mWp**3 / math.sqrt(172.5**6 - 3 * 172.5**2 * mWp**4 + 6 * mWp**6)

#function to make a plot for a given mWp and exclusion limit
def MakeCouplingsPlot(excl, mWp):
    #define TGraph arrays
    RDcentralX = array('d')
    RDsCentralX= array('d')
    RDbandX    = array('d')
    RDsBandX   = array('d')
    ExclusionX = array('d')
    RDcentralY = array('d')
    RDsCentralY= array('d')
    RDbandY    = array('d')
    RDsBandY   = array('d')
    ExclusionY = array('d')

    #define lists of gq-ordered values
    gtausRD     = []
    gtausRDs    = []
    gtausExcl   = []

    #calculate scaling for the current masspoint
    scale = CSscaling(mWp)
    print(mWp, "scale is", scale)

    #define the gtau = 0 starting point as an approximation
    minGq = GtauZero(mWp, excl, scale)
    gtausExcl.append([minGq, 0.01])

    #define the gQ maximum where 10% is exceeded by the hadronic width
    maxGq = HadWidthTooLarge(mWp)

    #gqlist
    gqlist = [0.01, 0.015, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 2., 3., 4., 5, 6., 7., 8., 9., 10.]

    #define theory curve
    for gq in gqlist:
        gtausRD.append( [gq, TheoryCurve(mWp, gq, False)])
        gtausRDs.append([gq, TheoryCurve(mWp, gq, True)])

    #define a gtau scan between minimum and maximum values for exp limits
    for i in range(1, 100):
        gq = minGq + (maxGq-minGq)/100. * float(i)
        #print(gq, maxGq, minGq)
        ExpExclRes = ExpExcl(mWp, gq, excl, scale)
        #catch invalid gtau results
        if ExpExclRes > 0.:
            gtausExcl.append([gq, ExpExclRes])

    #add endpoint for exclusion
    gtausExcl.append([maxGq, 0.01])

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

    #build experimental TGraph
    #print(gtausExcl)
    for gtau in gtausExcl:
        ExclusionX.append(gtau[0])
        ExclusionY.append(gtau[1])

    ExclusionGraph  = TGraph(len(ExclusionX), ExclusionX, ExclusionY)

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

    ExclusionGraph.SetLineColor(1)
    #ExclusionGraph.SetLineWidth(-9902)
    ExclusionGraph.SetFillStyle(3004);
    ExclusionGraph.SetFillColor(9);

    #build CMS plot
    CMS.SetEnergy("13")
    CMS.SetLumi("138")
    CMS.SetExtraText("Simulation Preliminary")
    canvas = CMS.cmsCanvas("CouplingSpace_MWpr"+str(mWp), 0.01, 10., 0.01, 10., "g_{q}", "g_{#tau}", square = CMS.kSquare, extraSpace = 0.01, iPos = 0)
    canvas.cd(1)

    RDbandGraph.Draw("L")
    RDsBandGraph.Draw("L,same")
    RDcentralGraph.Draw("L,same")
    RDsCentralGraph.Draw("L,same")
    ExclusionGraph.Draw("LF,same")

    legend = CMS.cmsLeg(0.20, 0.89-0.04*6, 0.89, 0.89, textSize=0.04)
    legend.SetHeader("m_{W'} = "+str(mWp)+" GeV")
    legend.AddEntry(RDcentralGraph, "R(D) central prediction", "l")
    legend.AddEntry(RDbandGraph, "R(D) #pm 1 #sigma", "l")
    legend.AddEntry(RDsCentralGraph, "R(D*) central prediction", "l")
    legend.AddEntry(RDsBandGraph, "R(D*) #pm 1 #sigma", "l")
    legend.AddEntry(ExclusionGraph, "95% CL median exp. upper limit", "lf")

    canvas.SetLogx(True)
    canvas.SetLogy(True)

    CMS.SaveCanvas(canvas, "CouplingSpace_MWpr"+str(mWp)+".pdf")

#actually run this for all masses
for mass in range(0,9):
    mWp = (mass+3)*100
    MakeCouplingsPlot(Excl[mWp], mWp)
