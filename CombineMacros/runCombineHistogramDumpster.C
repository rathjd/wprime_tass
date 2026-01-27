#include "CombineHistogramDumpster.C"
#include "CombineHistogramDumpsterRvec.C"
#include "ScaleFactorTTbarCalc.C"

//macro to run the Combine histogram generation process
//The first and third bin digits for the 4-digit bin number are relevant. Options are:
//first: 1 muon, 2 electron
//third: jet multiplicity, either 5 or 6
//year options are: 2016, 2017, 2018
void runCombineHistogramDumpster(int bin = 1153, TString year = "2016_APV"){

  //determine lepton flavour
  int binMajor = 0;
  binMajor += (bin / 1000) * 1000 + 100 + ((bin % 100) / 10) * 10; //set muon or electron and primary selection region, as well as jet multiplicity

  int jetMultiplicity = 0;
  jetMultiplicity += (bin / 10) % 10;

  std::cout<<"binMajor = "<<binMajor<<" in year "<<year<<" with jet multiplicity "<<jetMultiplicity<<std::endl;

  //run 1 b-tag control region with signal samples
  for(unsigned i = 0; i < 51; ++i){
    if(bin/1000 == 1 && i == 0) continue; //skip electron data sample for muon regions
    if(bin/1000 == 2 && i == 1) continue; //skip muon data sample for electron regions
    if(i > 1 || (year == "2016" || year == "2017")){
	CombineHistogramDumpster D(0, i, binMajor+1, year, 0);
	D.Loop();
    }
    else{
	CombineHistogramDumpsterRvec D(0, i, binMajor+1, year, 0);
	D.Loop();
    }
  }
  //fit SFs and variations for 1 b-tag region
  ScaleFactorTTbarCalc(binMajor+1, year);
  
  //run actual variations and SF'd ttbar for 2-, 3-, and 4-btag regions
  for(unsigned i = 0; i < 51; ++i){
    if(bin/1000 == 1 && i == 0) continue;
    else if(bin/2000 == 1 && i == 1) continue;
      if(i > 1 || (year == "2016" || year == "2017")){
	CombineHistogramDumpster C(0, i, binMajor+2, year, binMajor+1);
        C.Loop();
        CombineHistogramDumpster D(0, i, binMajor+3, year, binMajor+1);
        D.Loop();
      }
      else{
	CombineHistogramDumpsterRvec C(0, i, binMajor+2, year, binMajor+1);
        C.Loop();
        CombineHistogramDumpsterRvec D(0, i, binMajor+3, year, binMajor+1);
        D.Loop();
      }
      if(jetMultiplicity != 5){
	if(i > 1 || (year == "2016" || year == "2017")){
          CombineHistogramDumpster E(0, i, binMajor+4, year, binMajor+1);
          E.Loop();
        }
        else{
          CombineHistogramDumpsterRvec E(0, i, binMajor+4, year, binMajor+1);
          E.Loop();
        }
      }
  }
}
