#include <iostream>
#include <string>
#include <sstream>

// Macro to flexibly print out the True Positive, False Positive, and Purity for selected cuts to E/p and shower probability variables

//To utilize, edit the 'EDIT CUTS HERE' and 'SELECT OUTPUT HERE' blocks

//Run with 'root -l efficiency_purity_analysisMacro.C'

int efficiency_purity_analysisMacro()
{
  TString getCut(float);
  void printInfoCEMC(float, float , float, TString, TString );
  void printInfoEEMC(float, float , float, TString);
  void getInfo(int, TString,float[],float[]);

  //************EDIT CUTS HERE**************//
  //*
  float ep[]={0.8,0.9};
  float prob[]={0.0,0.2};
  //*
  //****************************************//

  //Calculate size of array//
  int size_ep=sizeof(ep)/sizeof(ep[0]);
  int size_prob=sizeof(prob)/sizeof(prob[0]);

  //**************SELECT OUTPUT HERE*************************//
  //*
  //* getInfo(True GeV,"C" or "E" depending on detector,array,array)//
  //*
  getInfo(1,"C",ep,prob,size_ep,size_prob);
  getInfo(5,"E",ep,prob,size_ep,size_prob);
  //*
  //********************************************************//

  return 0;
}
TString getCut(float value)
{
  stringstream ss;
  ss<<value;
  string temp = ss.str();
  TString cut = temp;
  return cut;
}

void printInfoCEMC(float truePositive, float falsePositive, float purity, TString ep_cut, TString prob_cut)
{
  std::cout<<"E/p > "<<ep_cut<<" : Prob > "<<prob_cut<<" | T.P = "<<truePositive<<" | F.P = "<<falsePositive<<" | Purity = "<<purity<<std::endl;
} 

void printInfoEEMC(float truePositive, float falsePositive, float purity, TString ep_cut)
{
  std::cout<<"E/p > "<<ep_cut<<" | T.P = "<<truePositive<<" | F.P = "<<falsePositive<<" | Purity = "<<purity<<std::endl;
} 

void getInfo(int GeV, TString detector,float ep[],float prob[],int sizeOf_ep,int sizeOf_prob)
{
  TString FILE_NAME_E="Electrons/Electrons"+getCut(GeV)+detector+".root";
  TString FILE_NAME_P="Pions/Pions"+getCut(GeV)+detector+".root";
  TFile *f1 = new TFile("/sphenix/user/gregtom3/data/Summer2018/ECAL_probability_studies/" + FILE_NAME_E, "READ");
  TFile *f2 = new TFile("/sphenix/user/gregtom3/data/Summer2018/ECAL_probability_studies/" + FILE_NAME_P, "READ");
  TTree *t_electron =(TTree*)f1->Get("event_cluster");
  TTree *t_pion =(TTree*)f2->Get("event_cluster");

  TString ep_cut="";
  TString prob_cut="";

  bool CEMC=false;
  TString title=" " + getCut((float)GeV)+"GeV at ";
  if(detector=="C")
    {
      CEMC=true;
      title+="CEMC ";
    }
  else
    title+="EEMC ";

  std::cout<<"******************"<<title<<"*************************"<<std::endl;
  if(CEMC)
    {
      for(int i=0;i<sizeOf_ep;i++){
	for(int j=0;j<sizeOf_prob;j++){
	  //get cuts//
	  ep_cut = getCut(ep[i]);
	  prob_cut = getCut(prob[j]);
	  //create total cut//
	  TCut totalCut = "em_cluster_e/em_track_ptotal>"+ep_cut+"&&em_cluster_prob>="+prob_cut;
	  //calculate statistics//
	  float truePositive = t_electron->GetEntries(totalCut)/10000.0;
	  float falsePositive = t_pion->GetEntries(totalCut)/10000.0;
	  float purity = (float)t_electron->GetEntries(totalCut)/(t_electron->GetEntries(totalCut)+t_pion->GetEntries(totalCut));
	  printInfoCEMC(truePositive,falsePositive,purity,ep_cut,prob_cut);
	}//end for
      }//end for
    }//end if  
  else
    {
      for(int i=0;i<sizeOf_ep;i++){
	  //get cut//
	  ep_cut = getCut(ep[i]);
	  //create total cut//
	  TString totalCutString="em_cluster_e/"+getCut((float)GeV)+">"+ep_cut;
	  TCut totalCut = totalCutString;
	  //calculate statistics//
	  float truePositive = t_electron->GetEntries(totalCut)/10000.0; //are electrons said electrons
	  float falsePositive = t_pion->GetEntries(totalCut)/10000.0; //are pions said electrons
	  float purity = (float)t_electron->GetEntries(totalCut)/(t_electron->GetEntries(totalCut)+t_pion->GetEntries(totalCut));
	  printInfoEEMC(truePositive,falsePositive,purity,ep_cut);
      }//end for
    }//end else
  delete t_electron;
  delete t_pion;
  t_electron=NULL;
  t_pion=NULL;
  f1->Close();
  f2->Close();
}

