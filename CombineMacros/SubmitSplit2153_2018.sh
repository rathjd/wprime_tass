#!/bin/sh

# source /cvmfs/sft.cern.ch/lcg/views/LCG_105/x86_64-el9-gcc11-opt/setup.sh

cd /afs/cern.ch/user/r/rathjd/work/private/NewCombine/CMSSW_14_1_0_pre4/src/
cmsenv

cd /afs/cern.ch/user/r/rathjd/work/private/TakeOverWprime/wprime/CombineMacros/
python3 SplitStudies.py 2018 2153
