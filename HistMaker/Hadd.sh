#!/bin/sh

p=/eos/user/s/siluo/WPrimeAnalysis/Validation/Hists/
pre=2018_Validation_
for i in LeptonPt LeptonEta LeptonPhi Jet0Pt Jet0Eta Jet0Phi Jet1Pt Jet1Eta Jet1Phi Jet2Pt Jet2Eta Jet2Phi Jet3Pt Jet3Eta Jet3Phi Jet4Pt Jet4Eta Jet4Phi METPt METPhi dPhiMetLep mT HT ST WPrimeMassSimpleFL WPrimeMassSimpleLL #WPrimeMass WPrimeMassFL WPrimeMassLL Likelihood
# for i in ST #WPrimeMass WPrimeMassFL WPrimeMassLL Likelihood
do
rm ${p}${pre}NRW_${i}_ttbar.root
hadd -f ${p}${pre}NRW_${i}_ttbar.root ${p}batch/${pre}NRW_${i}_ttbar_*
rm ${p}${pre}${i}_SingleElectron.root
hadd -f ${p}${pre}${i}_SingleElectron.root ${p}batch/${pre}${i}_SingleElectron_* 
rm ${p}${pre}${i}_SingleMuon.root
hadd -f ${p}${pre}${i}_SingleMuon.root ${p}batch/${pre}${i}_SingleMuon_* 

# hadd -f outputs/${pre}${i}.root ${p}${pre}${i}* ${p}${pre}NRW_${i}*

# hadd -f ${p}${pre}RW_${i}_batch.root ${p}batch/${pre}${i}* ${p}batch/${pre}RW_${i}*
# hadd -f outputs/${pre}RW_${i}.root ${p}${pre}${i}* ${p}${pre}RW_${i}*
done
# cp ${p}${pre}*ReweightSF.root outputs/