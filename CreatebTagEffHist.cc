#include <TChain.h>
#include <TString.h>
#include <vector>
#include <TLorentzVector.h>
#include <iostream>
#include <cmath>
#include <array>

#include <TApplication.h>

#include "Utilities/NanoAODReader.cc"

void CreatebTagEffHist(int sampleyear = 3, int sampletype = 2, int ifile = -1, string infile = "All") {
  dlib.AppendAndrewDatasets();
  if (sampletype < 0) {
    dlib.ListDatasets();
    cout << "Please enter the index for a dataset:" <<endl;
    cin >> sampletype;
  }
  Configs* conf = new Configs(sampleyear, sampletype, ifile);
  conf->DASInput = false;
  conf->bTagEffHistCreation = true;
  // conf->ProgressInterval = 1;
  conf->InputFile = infile;
  // conf->EntryMax = 2000000;

  NanoAODReader* r = new NanoAODReader(conf);
  bTagEff* bTE = new bTagEff(conf);
  r->SetbTag(bTE);
  
  Long64_t nentries = r->GetEntries();
  Progress* progress = new Progress(nentries,conf->ProgressInterval);
  for (unsigned ievt = 0; ievt < nentries; ++ievt) {
    progress->Print(ievt);
    if (r->ReadEvent(ievt) < 0) {
      continue;
    }
    r->BranchSizeCheck();
    for (unsigned ij = 0; ij < r->Jets.size(); ++ij) {
      bTE->FillJet(r->Jets[ij]);
    }
  }
  bTE->PostProcess();
  bTE->Clear();
  
  progress->JobEnd();

  gApplication->Terminate();
}
