#!/bin/sh

p=/eos/user/s/siluo/WPrimeAnalysis/ValidationFitted/Hists/
pre=Validation
j=2017

for i in LeptonPt LeptonEta LeptonPhi Jet0Pt Jet0Eta Jet0Phi Jet1Pt Jet1Eta Jet1Phi Jet2Pt Jet2Eta Jet2Phi Jet3Pt Jet3Eta Jet3Phi Jet4Pt Jet4Eta Jet4Phi METPt METPhi dPhiMetLep mT HT ST WPrimeMassSimpleFL WPrimeMassSimpleLL WPrimeMass WPrimeMassFL WPrimeMassLL Likelihood LikelihoodCorrect LikelihoodEffCorrect LikelihoodInCorrect
do
  for k in ttbar_RW ttbar_RW2On2 ttbar_NRW ttbar SingleMuon wjets_HT_100_200 wjets_HT_200_400 wjets_HT_400_600 wjets_HT_600_800 wjets_HT_800_1200 wjets_HT_1200_2500 wjets_HT_2500_inf single_top_schan single_top_tchan single_antitop_tchan single_top_tw single_antitop_tw WW ZZ WZTo1L1Nu2Q WZTo1L3Nu WZTo2Q2L WZTo3LNu FL300 FL400 FL500 FL600 FL700 FL800 FL900 FL1000 FL1100 LL300 LL400 LL500 LL600 LL700 LL800 LL900 LL1000 LL1100
  do
    if [ "$k" = "ttbar_RW" ] || [ "$k" = "ttbar_RW2On2" ] || [ "$k" = "ttbar_NRW" ]; then
      t="ttbar"
    else
      t="$k"
    fi
    rm "${p}${j}_${pre}_${i}_${k}.root"
    hadd -f "${p}${j}_${pre}_${i}_${k}.root" "${p}${j}_${t}/${j}_${pre}_${i}_${k}_*"
    echo "hadd -f ${p}${j}_${pre}_${i}_${t}.root ${p}${j}_${k}/${j}_${pre}_${i}_${t}_*"
  done
done
