import os,sys
from ROOT import TFile, TH1F, TChain, TCanvas
import numpy

chain = TChain("t")
chain.Add("/eos/cms/store/group/phys_b2g/wprime/SifuFW_Fitted/2017_SingleElectron/2017_SingleElectron_*.root")

hist5j = TH1F("SThist5j","e 5j 2b;S_{T} [GeV];Events",2000,180.,2000.)
hist6j = TH1F("SThist6j","e 6j 2b;S_{T} [GeV];Events",2000,210.,2000.)

#Fill ST histograms from e data in 2017 as a reasonably de-populated example
for event in range(0,chain.GetEntries()):
    chain.GetEntry(event)
    if chain.RegionIdentifier[0]==2152:
        hist5j.Fill(chain.LeptonPt+chain.METPt+chain.JetPt[0]+chain.JetPt[1]+chain.JetPt[2]+chain.JetPt[3]+chain.JetPt[4])
    elif chain.RegionIdentifier[0]==2162:
        hist6j.Fill(chain.LeptonPt+chain.METPt+chain.JetPt[0]+chain.JetPt[1]+chain.JetPt[2]+chain.JetPt[3]+chain.JetPt[4]+chain.JetPt[5])

#Test 5j histogram binning
nq5j = int(round(hist5j.GetEntries()/1000))
print(hist5j.GetEntries())
print(nq5j)
x5j = numpy.linspace(0,1,num=nq5j+1)
y5j = numpy.zeros(nq5j+1)
hist5j.GetQuantiles(nq5j+1,y5j,x5j)

print("5j: xVals=",x5j,"; yVals=",y5j)

nq6j = int(round(hist6j.GetEntries()/1000))
x6j = numpy.linspace(0,1,num=nq6j+1)
y6j = numpy.zeros(nq6j+1)
hist6j.GetQuantiles(nq6j+1,y6j,x6j)

print("6j: xVals=",x6j,"; yVals=",y6j)

