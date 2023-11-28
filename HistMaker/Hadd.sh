#!/bin/sh

p=/eos/user/s/siluo/WPrimeAnalysis/ValidationFitted/Hists/
pre=Validation
for i in LeptonPt LeptonEta LeptonPhi Jet0Pt Jet0Eta Jet0Phi Jet1Pt Jet1Eta Jet1Phi Jet2Pt Jet2Eta Jet2Phi Jet3Pt Jet3Eta Jet3Phi Jet4Pt Jet4Eta Jet4Phi METPt METPhi dPhiMetLep mT HT ST WPrimeMassSimpleFL WPrimeMassSimpleLL WPrimeMass WPrimeMassFL WPrimeMassLL Likelihood
# for i in Jet0Eta Jet0Phi Jet1Pt Jet1Eta Jet1Phi Jet2Pt Jet2Eta Jet2Phi Jet3Pt Jet3Eta Jet3Phi Jet4Pt Jet4Eta Jet4Phi METPt METPhi dPhiMetLep mT HT ST WPrimeMassSimpleFL WPrimeMassSimpleLL
do
for j in 2018
do
if [ $1 -eq 0 ]; then
  rm ${p}${j}_${pre}_NRW_${i}_ttbar.root
  hadd -f ${p}${j}_${pre}_NRW_${i}_ttbar.root ${p}${j}_ttbar/${j}_${pre}_NRW_${i}_ttbar_*
fi
if [ $1 -eq 1 ]; then
  rm ${p}${j}_${pre}_${i}_SingleElectron.root
  hadd -f ${p}${j}_${pre}_${i}_SingleElectron.root ${p}${j}_SingleElectron/${j}_${pre}_${i}_SingleElectron_* 
fi
if [ $1 -eq 2 ]; then
  rm ${p}${j}_${pre}_${i}_SingleMuon.root
  hadd -f ${p}${j}_${pre}_${i}_SingleMuon.root ${p}${j}_SingleMuon/${j}_${pre}_${i}_SingleMuon_* 
fi
if [ $1 -eq 3 ]; then
  rm ${p}${j}_${pre}_RW_${i}_ttbar.root
  hadd -f ${p}${j}_${pre}_RW_${i}_ttbar.root ${p}${j}_ttbar/${j}_${pre}_RW_${i}_ttbar_*
fi

# rm outputs/${pre}${i}.root
# hadd -f outputs/${pre}${i}.root ${p}${pre}${i}* ${p}${pre}NRW_${i}*
# rm outputs/${pre}RW_${i}.root
# hadd -f outputs/${pre}RW_${i}.root ${p}${pre}${i}* ${p}${pre}RW_${i}*
done
done
# cp ${p}${pre}*ReweightSF.root outputs/