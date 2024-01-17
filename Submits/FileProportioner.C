#include <TROOT.h>
#include <TString.h>
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>

//Macro to take in all same tree files in a folder and make a copy of same number of events files in a new output filepath with output filename and file size target events for all files but the very last
void FileProportioner(TString InputFilepath, TString OutputFilepath, TString OutputFilename, unsigned FileSizeTarget){  

  //add files in InputFilepath to chain
  TChain *InputChain = new TChain("Events");//tree name is fixed to "Events" for this purpose
  InputChain->Add(InputFilepath+"/"+OutputFilename+".root");//changed to add a single file only, for compatibility with solution two this remains a Chain, not a tree

  const auto nentries = InputChain->GetEntries(); //count entries in input chain

  //set up a copy of original Tree
  TTree *BaseTree = (TTree*) InputChain->GetTree();;// = (TTree*) InputChain->GetTree();

  //This solution works, but only on a single initial file, because GetTree gets the first tree in the chain only that is copied...
  //calculate number of new files 
  unsigned FilesNumber = (unsigned) std::ceil((float)nentries / (float) FileSizeTarget);
  if((nentries % FileSizeTarget) < (FileSizeTarget / 2)) FilesNumber--; //if the last file would be smaller than half the intended size, just add it to the second to last

  //loop over new files to be produced
  for(unsigned i = 0; i < FilesNumber; ++i){

    //set up each new file
    TFile *NewFile = new TFile(OutputFilepath + "/" + OutputFilename + TString::Format("_%d.root", i), "RECREATE");

    //makes copy of original tree starting at i*FileSizeTarget of size up to FileSizeTarget
    TTree *NewTree;
    if(i < FilesNumber-1) NewTree = BaseTree->CopyTree("","", FileSizeTarget, i * FileSizeTarget);
    else NewTree = BaseTree->CopyTree("","", FileSizeTarget*2, i * FileSizeTarget);//make sure to dump the entire rest, if oversized, into the last file

    //file operations
    NewFile->Write();
    NewTree->Clear();
    NewFile->Close();
  }

  //This solution produces the right size of files and with draw commands gives the exact same output as the original files.
  //When loaded in the same order as the files were written, events match original by bytes read (GetEntry function result on TChain) for every single event
  /*unsigned FileCounter = 0;
  TFile *NewFile;
  TTree *NewTree = (TTree*) BaseTree->CloneTree(0); //initialize it to be safe

  //loop over all events and copy the tree events accordingly
  for(unsigned i = 0; i < nentries; ++i){
    InputChain->GetEntry(i);
    if(i % FileSizeTarget == 0){
      if(i != 0){
	NewFile->Write();
	NewFile->Close();
      }
      BaseTree = (TTree*) InputChain->GetTree();
      NewFile = new TFile(OutputFilepath + "/" + OutputFilename + TString::Format("_%d.root", FileCounter), "RECREATE");
      NewTree = (TTree*) BaseTree->CloneTree(0);
      ++FileCounter;
    }
    NewTree->Fill();
    if(i == nentries - 1){
      NewFile->Write();
      NewFile->Close();
    }
  }*/
}
