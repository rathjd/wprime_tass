#include "CombineHistogramDumpster.C"
#include "ScaleFactorTTbarCalc.C"

//macro to run the Combine histogram generation process
//The first and third bin digits for the 4-digit bin number are relevant. Options are:
//first: 1 muon, 2 electron
//third: jet multiplicity, either 5 or 6
//year options are: 2016, 2017, 2018
void runCombineHistogramDumpster(int bin = 1153, int year = 2018){

  //determine lepton flavour
  int binMajor = 0;
  binMajor += (bin / 1000) * 1000 + 100 + ((bin % 100) / 10) * 10; //set muon or electron and primary selection region, as well as jet multiplicity

  std::cout<<"binMajor = "<<binMajor<<" in year "<<year<<std::endl;

  //determine years to be run
  vector<TString> years;
  if(year==2016){
    years.push_back("2016");
    years.push_back("2016apv");
  }
  else years.push_back(TString::Format("%d",year));

  //run 1 b-tag control region without signal samples
  for(unsigned y = 0; y < years.size(); ++ y) for(unsigned i = 0; i < 22; ++i){
    if(bin/1000 == 1 && i == 0) continue; //skip electron data sample for muon regions
    if(bin/1000 == 2 && i == 1) continue; //skip muon data sample for electron regions
    CombineHistogramDumpster D(0, i, binMajor+1, years[y], 0);
    D.Loop();
  }
  //fit SFs and variations for 1 b-tag region
  ScaleFactorTTbarCalc(binMajor+1, year);
  
  //run 2 b-tag control region without signal samples, add reweighted ST distributions and NLL histograms with 1 b-tag corrections, as well as unreweighted ST distributions
  for(unsigned y = 0; y < years.size(); ++ y) for(unsigned i = 0; i < 22; ++i){
    if(bin/1000 == 1 && i == 0) continue; //skip electron data sample for muon regions
    if(bin/1000 == 2 && i == 1) continue; //skip muon data sample for electron regions
    CombineHistogramDumpster D(0, i, binMajor+2, years[y], binMajor+1);
    D.Loop();
  }

  //fit SFs and variations for 2 b-tag region
  ScaleFactorTTbarCalc(binMajor+2, year);

  //run actual variations and SF'd ttbar for 3- and 4-btag regions
  for(unsigned y = 0; y < years.size(); ++ y) for(unsigned i = 0; i < 40; ++i){
    if(bin/1000 == 1 && i == 0) continue;
    else if(bin/2000 == 1 && i == 1) continue;
      CombineHistogramDumpster D(0, i, binMajor+3, years[y], binMajor+2);
      D.Loop();
      CombineHistogramDumpster E(0, i, binMajor+4, years[y], binMajor+2);
      E.Loop();
  }
}
