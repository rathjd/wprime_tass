#!/bin/sh

# source /cvmfs/sft.cern.ch/lcg/views/LCG_105/x86_64-el9-gcc11-opt/setup.sh
cd /afs/cern.ch/work/r/rathjd/private/TakeOverWprime/wprime
root -b -q "Validation.cc+($2,$3,$1,$4)"
