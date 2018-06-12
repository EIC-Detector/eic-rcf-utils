#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>

void tree_Chaining(
		   const TString DIR_NAME = "particle-gun-simulation-2018-06-11-12:43:46Pions1GeVEEMC/",
		   const TString PATH_TO_DIR_NAME = "/direct/phenix+u/gregtom3/SBU/research/eic-utils/",
		   const TString DETECTOR_NAME = "cemc",
		   const TString TREE_NAME = "ntp_cluster",
		   const int NUMBER_EVENT_BLOCKS = 100
)
{
  TFile *f = new TFile("myTest.root","RECREATE");
  TChain *t1=new TChain(TREE_NAME);
  
  for( int i = 0 ; i < NUMBER_EVENT_BLOCKS ; i++ )
    {
      stringstream tmp;
      tmp << i;
      TString s = tmp.str();
      
      TString fullPath = PATH_TO_DIR_NAME + DIR_NAME + "G4EICDetector-" + s +
	                ".root_g4" + DETECTOR_NAME + "_eval.root";
      t1->Add(fullPath);
    }
  t1->Draw("e/ge");
}
