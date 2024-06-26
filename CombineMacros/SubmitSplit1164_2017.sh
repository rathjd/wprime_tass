#!/bin/sh

# source /cvmfs/sft.cern.ch/lcg/views/LCG_105/x86_64-el9-gcc11-opt/setup.sh

cd /afs/cern.ch/user/r/rathjd/work/private/CombineTryouts/CMSSW_11_3_4/src/
cmsenv

cd /afs/cern.ch/user/r/rathjd/work/private/TakeOverWprime/wprime/CombineMacros/
python3 SplitStudies.py 2017 1164
