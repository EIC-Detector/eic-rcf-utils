#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

void tree_Chaining(
		   const TString TREE_NAME = "ntp_cluster",
		   const TString DETECTOR_NAME = "cemc",
		   const int NUMBER_EVENT_BLOCKS = 100,
		   const TString DIR_NAME = "Pions1GeVCEMC",
		   const TString OUTPUT_FILE = "temporary.root",
		   const TString PATH_TO_DIR_NAME = "/sphenix/user/gregtom3/SBU/research/eic-utils/"	   
)
{
  TFile *f = new TFile(OUTPUT_FILE,"RECREATE");
  TChain *tC = new TChain("");
  tC->SetName(TREE_NAME);
  for( int i = 0 ; i < NUMBER_EVENT_BLOCKS ; i++ )
    {
      stringstream tmp;
      tmp << i;
      TString s = tmp.str();
      TString fullPath = PATH_TO_DIR_NAME + DIR_NAME + "/G4EICDetector-" +                                 s + ".root_g4" + DETECTOR_NAME + "_eval.root";
  
      tC->Add(fullPath);

    
    }  
  tC->CloneTree(-1,"fast");
  f->Write();
}
