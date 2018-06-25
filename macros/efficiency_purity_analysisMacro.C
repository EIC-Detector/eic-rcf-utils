#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>

// Macro to flexibly print out the True Positive, False Positive, and Purity for selected cuts to E/p and shower probability variables

//To utilize, edit the 'EDIT CUTS HERE' and 'SELECT OUTPUT HERE' blocks

//Run with 'root -l efficiency_purity_analysisMacro.C'

TString getCut(float);

void printInfoCEMC(float, float , float, TString, TString );

void printInfoEEMC(float, float , float, TString);

void printTitle(TString, int);

void getInfo(int, TString,float[],float[]);

double get_true_positive(TString, TString, int);

double get_false_positive(TString, TString, int);

double get_purity(TString, TString, int);

int efficiency_purity_analysisMacro()
{
   //Main Print Statement//
  std::cout<<"******************************************************************"<<std::endl;
  std::cout<<"T.P = True Positive, Percentage of correctly identified electrons"<<std::endl;
  std::cout<<"F.P = False Positive, Percentage of total pions identified as electrons"<<std::endl;
  std::cout<<"Purity, Percentage of total particles identified as electrons which are actually electrons"<<std::endl;
  std::cout<<"******************************************************************"<<std::endl;

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
  std::cout<<"******************************************************************"<<std::endl;
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
  //std::cout<<"E/p > "<<ep_cut<<" : Prob > "<<prob_cut<<" | T.P = "<<truePositive<<" | F.P = "<<falsePositive<<" | Purity = "<<purity<<std::endl;
  printf("E/p > %1.4s : Prob > %1.4s\t | T.P = %2.4f | F.P = %2.4f | Purity = %2.4f\n",ep_cut.Data(),prob_cut.Data(),truePositive,falsePositive,purity);
} 

void printInfoEEMC(float truePositive, float falsePositive, float purity, TString ep_cut)
{
  printf("E/True E > %1.4s \t | T.P = %2.4f | F.P = %2.4f | Purity = %2.4f\n",ep_cut.Data(),truePositive,falsePositive,purity);
} 

void printTitle(TString detector, int GeV)
{
  if(detector=="C")
    std::cout<<"************* CEMC "<<GeV<<"GeV *************"<<std::endl;
  else
    std::cout<<"************* EEMC "<<GeV<<"GeV *************"<<std::endl;
  
}
void getInfo(int GeV, TString detector,float ep[],float prob[],int sizeOf_ep,int sizeOf_prob)
{
  printTitle(detector, GeV);
  if(detector=="C")
    {
      for(int i = 0; i<sizeOf_ep; i++)
	{
	  for(int j = 0; j<sizeOf_prob; j++)
	    {
	      TString totalCut = "em_cluster_e/em_track_ptotal>"+getCut(ep[i])+"&&em_cluster_prob>="+getCut(prob[j]);
	      float truePositive=(float)get_true_positive("C",totalCut,GeV);
	      float falsePositive=(float)get_false_positive("C",totalCut,GeV);
	      float purity=(float)get_purity("C",totalCut,GeV);
	      printInfoCEMC(truePositive,falsePositive,purity,getCut(ep[i]),getCut(prob[j]));
	    }
	}
    }
  else
    {
      for(int i = 0; i<sizeOf_ep; i++)
	{
	  TString totalCut="em_cluster_e/"+getCut((float)GeV)+">"+getCut(ep[i]);
	  float truePositive = get_true_positive(detector,totalCut,GeV);
	  float falsePositive = get_false_positive(detector,totalCut,GeV);
	  float purity = get_purity(detector,totalCut,GeV);
	  printInfoEEMC(truePositive,falsePositive,purity,getCut(ep[i]));
	}
    }
  
}

double get_true_positive(TString detector, TString cut, int GeV)
{
  TString FILE_NAME_E="Electrons/Electrons"+getCut(GeV)+detector+".root";
  TFile *f1 = new TFile("/sphenix/user/gregtom3/data/Summer2018/ECAL_probability_studies/" + FILE_NAME_E, "READ");
  TTree *t_electron =(TTree*)f1->Get("event_cluster");

  double truePositive = t_electron->GetEntries(cut)/10000.0;
  //f1->Close(); delete t_electron; t_electron=NULL;
  delete t_electron; t_electron=NULL; f1->Close();
  return truePositive;
}

double get_false_positive(TString detector, TString cut, int GeV)
{
  TString FILE_NAME_P="Pions/Pions"+getCut(GeV)+detector+".root";
  TFile *f2 = new TFile("/sphenix/user/gregtom3/data/Summer2018/ECAL_probability_studies/" + FILE_NAME_P, "READ");
  TTree *t_pion =(TTree*)f2->Get("event_cluster");

  double falsePositive = t_pion->GetEntries(cut)/10000.0;
  delete t_pion; t_pion=NULL; f2->Close();
  return falsePositive;
}

double get_purity(TString detector, TString cut, int GeV)
{
  TString FILE_NAME_E="Electrons/Electrons"+getCut(GeV)+detector+".root";
  TString FILE_NAME_P="Pions/Pions"+getCut(GeV)+detector+".root";
  TFile *f1 = new TFile("/sphenix/user/gregtom3/data/Summer2018/ECAL_probability_studies/" + FILE_NAME_E, "READ");
  TFile *f2 = new TFile("/sphenix/user/gregtom3/data/Summer2018/ECAL_probability_studies/" + FILE_NAME_P, "READ");
  TTree *t_electron =(TTree*)f1->Get("event_cluster");
  TTree *t_pion =(TTree*)f2->Get("event_cluster");
  
  double purity = (float)t_electron->GetEntries(cut)/(t_electron->GetEntries(cut)+t_pion->GetEntries(cut));
  delete t_electron; delete t_pion; t_electron=NULL; t_pion=NULL; f1->Close(); f2->Close();
  return purity;
}


