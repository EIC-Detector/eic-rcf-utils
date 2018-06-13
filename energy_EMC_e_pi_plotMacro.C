#include <iostream>
#include <cstdlib>

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
TTree* loadTree(TString FILE_NAME, TString TREE_NAME)
{
    TFile *f = new TFile(FILE_NAME, "READ");
    TTree *t = (TTree*)f->Get(TREE_NAME);
    return t;
}

TH1F* fillHist(TH1F *THE_HIST, TTree *THE_TREE, Float_t THE_WEIGHT, bool DO_NORMALIZE)
/* *
 * This function, when fed a histogram, tree,
 * a floating point weight variable, and a boolean, will fill each entry
 * inside the specificed branch into the histogram, so long as each entry
 * is a floating point number GREATER than the weight. If DO_NORMALIZE is
 * set to 'true', the histogram will be normalize based on its number of
 * entries. The axes titles are furthermore assumed to be generic and have
 * been already set.
 */
{
  Float_t measured_energy;
  Float_t true_energy;
  THE_TREE->SetBranchAddress("e",&measured_energy);
  THE_TREE->SetBranchAddress("ge",&true_energy);
  Int_t nentries = Int_t(THE_TREE->GetEntries());
  for(Int_t entryInChain=0; entryInChain<nentries; entryInChain++)
    {
      Int_t entryInTree = THE_TREE->LoadTree(entryInChain);
      if (entryInTree < 0) break;
      THE_TREE->GetEntry(entryInChain);
      if (measured_energy > THE_WEIGHT && true_energy > 0.1){ THE_HIST->Fill(measured_energy/true_energy);}
    }
  if (DO_NORMALIZE)
    { 
      THE_HIST->Scale(1/THE_HIST->GetEntries());
    }
  THE_HIST->SetXTitle("GeV");
  THE_HIST->SetYTitle("entries/#sum entries      ");
  
  return THE_HIST;
}

/* *
 * Main Code
 */

void energy_EMC_e_pi_plotMacro()
{

  /* *
   * sPHENIX Style
   */
  
  gROOT->LoadMacro("/sphenix/user/gregtom3/SBU/research/macros/macros/sPHENIXStyle/sPhenixStyle.C");
  SetsPhenixStyle();

  /* *
   * Base Histogram (Recreated from Matching Plots)
   */

  TH1F *h_base = new TH1F("h_base","",25,0.0,1.2);
  
  /* *
   * Loading Histograms and Trees for
   *   Electrons Pions 1, 2, 5, 10, 20GeV at CEMC
   */

  TH1F *h_P_1GeV_CEMC = (TH1F*)h_base->Clone();
  TH1F *h_P_2GeV_CEMC = (TH1F*)h_base->Clone();
  TH1F *h_P_5GeV_CEMC = (TH1F*)h_base->Clone();
  TH1F *h_P_10GeV_CEMC = (TH1F*)h_base->Clone();
  TH1F *h_P_20GeV_CEMC = (TH1F*)h_base->Clone();

  TH1F *h_E_1GeV_CEMC = (TH1F*)h_base->Clone();
  TH1F *h_E_2GeV_CEMC = (TH1F*)h_base->Clone();
  TH1F *h_E_5GeV_CEMC = (TH1F*)h_base->Clone();
  TH1F *h_E_10GeV_CEMC = (TH1F*)h_base->Clone();
  TH1F *h_E_20GeV_CEMC = (TH1F*)h_base->Clone();

  h_P_1GeV_CEMC->SetName("h_P_1GeV_CEMC");
  h_P_2GeV_CEMC->SetName("h_P_2GeV_CEMC");
  h_P_5GeV_CEMC->SetName("h_P_5GeV_CEMC");
  h_P_10GeV_CEMC->SetName("h_P_10GeV_CEMC");
  h_P_20GeV_CEMC->SetName("h_P_20GeV_CEMC");

  h_E_1GeV_CEMC->SetName("h_E_1GeV_CEMC");
  h_E_2GeV_CEMC->SetName("h_E_2GeV_CEMC");
  h_E_5GeV_CEMC->SetName("h_E_5GeV_CEMC");
  h_E_10GeV_CEMC->SetName("h_E_10GeV_CEMC");
  h_E_20GeV_CEMC->SetName("h_E_20GeV_CEMC");

  TTree *t_P_1GeV_CEMC = loadTree("/sphenix/user/gregtom3/SBU/research/gunEvents/Pions/Pions1C.root","ntp_cluster");
  TTree *t_P_2GeV_CEMC = loadTree("/sphenix/user/gregtom3/SBU/research/gunEvents/Pions/Pions2C.root","ntp_cluster");
  TTree *t_P_5GeV_CEMC = loadTree("/sphenix/user/gregtom3/SBU/research/gunEvents/Pions/Pions5C.root","ntp_cluster");
  TTree *t_P_10GeV_CEMC = loadTree("/sphenix/user/gregtom3/SBU/research/gunEvents/Pions/Pions10C.root","ntp_cluster");
  TTree *t_P_20GeV_CEMC = loadTree("/sphenix/user/gregtom3/SBU/research/gunEvents/Pions/Pions20C.root","ntp_cluster");
  
  TTree *t_E_1GeV_CEMC = loadTree("/sphenix/user/gregtom3/SBU/research/gunEvents/Electrons/Electrons1C.root","ntp_cluster");
  TTree *t_E_2GeV_CEMC = loadTree("/sphenix/user/gregtom3/SBU/research/gunEvents/Electrons/Electrons2C.root","ntp_cluster");
  TTree *t_E_5GeV_CEMC = loadTree("/sphenix/user/gregtom3/SBU/research/gunEvents/Electrons/Electrons5C.root","ntp_cluster");
  TTree *t_E_10GeV_CEMC = loadTree("/sphenix/user/gregtom3/SBU/research/gunEvents/Electrons/Electrons10C.root","ntp_cluster");
  TTree *t_E_20GeV_CEMC = loadTree("/sphenix/user/gregtom3/SBU/research/gunEvents/Electrons/Electrons20C.root","ntp_cluster");
  
  /* *
   * Plotting Electrons and Pions at 1, 2, 5, 10, 20GeV at CEMC
   */
  
  TCanvas *c1_CEMC = new TCanvas("c1_CEMC","1GeV CEMC",600,400);

  c1_CEMC->SetLogy();
  
  h_P_1GeV_CEMC=fillHist(h_P_1GeV_CEMC,t_P_1GeV_CEMC,0.3,true);
  h_E_1GeV_CEMC=fillHist(h_E_1GeV_CEMC,t_E_1GeV_CEMC,0.3,true);

  h_P_1GeV_CEMC->SetLineColor(kRed);
  h_E_1GeV_CEMC->SetLineColor(kBlue);
  
  h_P_1GeV_CEMC->Draw();
  h_E_1GeV_CEMC->Draw("SAME");
  h_P_1GeV_CEMC->SetTitle("e^{-} (Blue)\n #pi^{-} (Red)\n 1 GeV at CEMC");

  TCanvas *c2_CEMC = new TCanvas("c2_CEMC","2GeV CEMC",600,400);

  c2_CEMC->SetLogy();
  
  h_P_2GeV_CEMC=fillHist(h_P_2GeV_CEMC,t_P_2GeV_CEMC,0.3,true);
  h_E_2GeV_CEMC=fillHist(h_E_2GeV_CEMC,t_E_2GeV_CEMC,0.3,true);

  h_P_2GeV_CEMC->SetLineColor(kRed);
  h_E_2GeV_CEMC->SetLineColor(kBlue);
  
  h_P_2GeV_CEMC->Draw();
  h_E_2GeV_CEMC->Draw("SAME");
  h_P_2GeV_CEMC->SetTitle("e^{-} (Blue)\n #pi^{-} (Red)\n 2 GeV at CEMC");
  
  TCanvas *c5_CEMC = new TCanvas("c5_CEMC","5GeV CEMC",600,400);

  c5_CEMC->SetLogy();
  
  h_P_5GeV_CEMC=fillHist(h_P_5GeV_CEMC,t_P_5GeV_CEMC,0.3,true);
  h_E_5GeV_CEMC=fillHist(h_E_5GeV_CEMC,t_E_5GeV_CEMC,0.3,true);

  h_P_5GeV_CEMC->SetLineColor(kRed);
  h_E_5GeV_CEMC->SetLineColor(kBlue);
  
  h_P_5GeV_CEMC->Draw();
  h_E_5GeV_CEMC->Draw("SAME");
  h_P_5GeV_CEMC->SetTitle("e^{-} (Blue)\n #pi^{-} (Red)\n 5 GeV at CEMC");
  
  TCanvas *c10_CEMC = new TCanvas("c10_CEMC","10GeV CEMC",600,400);

  c10_CEMC->SetLogy();
  
  h_P_10GeV_CEMC=fillHist(h_P_10GeV_CEMC,t_P_10GeV_CEMC,0.3,true);
  h_E_10GeV_CEMC=fillHist(h_E_10GeV_CEMC,t_E_10GeV_CEMC,0.3,true);

  h_P_10GeV_CEMC->SetLineColor(kRed);
  h_E_10GeV_CEMC->SetLineColor(kBlue);
  
  h_P_10GeV_CEMC->Draw();
  h_E_10GeV_CEMC->Draw("SAME");
  h_P_10GeV_CEMC->SetTitle("e^{-} (Blue)\n #pi^{-} (Red)\n 10 GeV at CEMC");

  TCanvas *c20_CEMC = new TCanvas("c20_CEMC","20GeV CEMC",600,400);

  c20_CEMC->SetLogy();
  
  h_P_20GeV_CEMC=fillHist(h_P_20GeV_CEMC,t_P_20GeV_CEMC,0.3,true);
  h_E_20GeV_CEMC=fillHist(h_E_20GeV_CEMC,t_E_20GeV_CEMC,0.3,true);

  h_P_20GeV_CEMC->SetLineColor(kRed);
  h_E_20GeV_CEMC->SetLineColor(kBlue);
  
  h_P_20GeV_CEMC->Draw();
  h_E_20GeV_CEMC->Draw("SAME");
  h_P_20GeV_CEMC->SetTitle("e^{-} (Blue)\n #pi^{-} (Red)\n 20 GeV at CEMC");
  
  /* *
   * Loading histograms and trees for 1, 2, 5, 10, 20 GeV for EEMC
   */

  
  TH1F *h_P_1GeV_EEMC = (TH1F*)h_base->Clone();
  TH1F *h_P_2GeV_EEMC = (TH1F*)h_base->Clone();
  TH1F *h_P_5GeV_EEMC = (TH1F*)h_base->Clone();
  TH1F *h_P_10GeV_EEMC = (TH1F*)h_base->Clone();
  TH1F *h_P_20GeV_EEMC = (TH1F*)h_base->Clone();

  TH1F *h_E_1GeV_EEMC = (TH1F*)h_base->Clone();
  TH1F *h_E_2GeV_EEMC = (TH1F*)h_base->Clone();
  TH1F *h_E_5GeV_EEMC = (TH1F*)h_base->Clone();
  TH1F *h_E_10GeV_EEMC = (TH1F*)h_base->Clone();
  TH1F *h_E_20GeV_EEMC = (TH1F*)h_base->Clone();

  h_P_1GeV_EEMC->SetName("h_P_1GeV_EEMC");
  h_P_2GeV_EEMC->SetName("h_P_2GeV_EEMC");
  h_P_5GeV_EEMC->SetName("h_P_5GeV_EEMC");
  h_P_10GeV_EEMC->SetName("h_P_10GeV_EEMC");
  h_P_20GeV_EEMC->SetName("h_P_20GeV_EEMC");

  h_E_1GeV_EEMC->SetName("h_E_1GeV_EEMC");
  h_E_2GeV_EEMC->SetName("h_E_2GeV_EEMC");
  h_E_5GeV_EEMC->SetName("h_E_5GeV_EEMC");
  h_E_10GeV_EEMC->SetName("h_E_10GeV_EEMC");
  h_E_20GeV_EEMC->SetName("h_E_20GeV_EEMC");

  TTree *t_P_1GeV_EEMC = loadTree("/sphenix/user/gregtom3/SBU/research/gunEvents/Pions/Pions1E.root","ntp_cluster");
  TTree *t_P_2GeV_EEMC = loadTree("/sphenix/user/gregtom3/SBU/research/gunEvents/Pions/Pions2E.root","ntp_cluster");
  TTree *t_P_5GeV_EEMC = loadTree("/sphenix/user/gregtom3/SBU/research/gunEvents/Pions/Pions5E.root","ntp_cluster");
  TTree *t_P_10GeV_EEMC = loadTree("/sphenix/user/gregtom3/SBU/research/gunEvents/Pions/Pions10E.root","ntp_cluster");
  TTree *t_P_20GeV_EEMC = loadTree("/sphenix/user/gregtom3/SBU/research/gunEvents/Pions/Pions20E.root","ntp_cluster");
  
  TTree *t_E_1GeV_EEMC = loadTree("/sphenix/user/gregtom3/SBU/research/gunEvents/Electrons/Electrons1E.root","ntp_cluster");
  TTree *t_E_2GeV_EEMC = loadTree("/sphenix/user/gregtom3/SBU/research/gunEvents/Electrons/Electrons2E.root","ntp_cluster");
  TTree *t_E_5GeV_EEMC = loadTree("/sphenix/user/gregtom3/SBU/research/gunEvents/Electrons/Electrons5E.root","ntp_cluster");
  TTree *t_E_10GeV_EEMC = loadTree("/sphenix/user/gregtom3/SBU/research/gunEvents/Electrons/Electrons10E.root","ntp_cluster");
  TTree *t_E_20GeV_EEMC = loadTree("/sphenix/user/gregtom3/SBU/research/gunEvents/Electrons/Electrons20E.root","ntp_cluster");
  
  /* *
   * Plotting Electrons and Pions at 1, 2, 5, 10, 20GeV at EEMC
   */
  
  TCanvas *c1_EEMC = new TCanvas("c1_EEMC","1GeV EEMC",600,400);

  c1_EEMC->SetLogy();
  
  h_P_1GeV_EEMC=fillHist(h_P_1GeV_EEMC,t_P_1GeV_EEMC,0.3,true);
  h_E_1GeV_EEMC=fillHist(h_E_1GeV_EEMC,t_E_1GeV_EEMC,0.3,true);

  h_P_1GeV_EEMC->SetLineColor(kRed);
  h_E_1GeV_EEMC->SetLineColor(kBlue);
  
  h_P_1GeV_EEMC->Draw();
  h_E_1GeV_EEMC->Draw("SAME");
  h_P_1GeV_EEMC->SetTitle("e^{-} (Blue)\n #pi^{-} (Red)\n 1 GeV at EEMC");

  TCanvas *c2_EEMC = new TCanvas("c2_EEMC","2GeV EEMC",600,400);

  c2_EEMC->SetLogy();
  
  h_P_2GeV_EEMC=fillHist(h_P_2GeV_EEMC,t_P_2GeV_EEMC,0.3,true);
  h_E_2GeV_EEMC=fillHist(h_E_2GeV_EEMC,t_E_2GeV_EEMC,0.3,true);

  h_P_2GeV_EEMC->SetLineColor(kRed);
  h_E_2GeV_EEMC->SetLineColor(kBlue);
  
  h_P_2GeV_EEMC->Draw();
  h_E_2GeV_EEMC->Draw("SAME");
  h_P_2GeV_EEMC->SetTitle("e^{-} (Blue)\n #pi^{-} (Red)\n 2 GeV at EEMC");
  
  TCanvas *c5_EEMC = new TCanvas("c5_EEMC","5GeV EEMC",600,400);

  c5_EEMC->SetLogy();
  
  h_P_5GeV_EEMC=fillHist(h_P_5GeV_EEMC,t_P_5GeV_EEMC,0.3,true);
  h_E_5GeV_EEMC=fillHist(h_E_5GeV_EEMC,t_E_5GeV_EEMC,0.3,true);

  h_P_5GeV_EEMC->SetLineColor(kRed);
  h_E_5GeV_EEMC->SetLineColor(kBlue);
  
  h_P_5GeV_EEMC->Draw();
  h_E_5GeV_EEMC->Draw("SAME");
  h_P_5GeV_EEMC->SetTitle("e^{-} (Blue)\n #pi^{-} (Red)\n 5 GeV at EEMC");
  
  TCanvas *c10_EEMC = new TCanvas("c10_EEMC","10GeV EEMC",600,400);

  c10_EEMC->SetLogy();
  
  h_P_10GeV_EEMC=fillHist(h_P_10GeV_EEMC,t_P_10GeV_EEMC,0.3,true);
  h_E_10GeV_EEMC=fillHist(h_E_10GeV_EEMC,t_E_10GeV_EEMC,0.3,true);

  h_P_10GeV_EEMC->SetLineColor(kRed);
  h_E_10GeV_EEMC->SetLineColor(kBlue);
  
  h_P_10GeV_EEMC->Draw();
  h_E_10GeV_EEMC->Draw("SAME");
  h_P_10GeV_EEMC->SetTitle("e^{-} (Blue)\n #pi^{-} (Red)\n 10 GeV at EEMC");

  TCanvas *c20_EEMC = new TCanvas("c20_EEMC","20GeV EEMC",600,400);

  c20_EEMC->SetLogy();
  
  h_P_20GeV_EEMC=fillHist(h_P_20GeV_EEMC,t_P_20GeV_EEMC,0.3,true);
  h_E_20GeV_EEMC=fillHist(h_E_20GeV_EEMC,t_E_20GeV_EEMC,0.3,true);

  h_P_20GeV_EEMC->SetLineColor(kRed);
  h_E_20GeV_EEMC->SetLineColor(kBlue);
  
  h_P_20GeV_EEMC->Draw();
  h_E_20GeV_EEMC->Draw("SAME");
  h_P_20GeV_EEMC->SetTitle("e^{-} (Blue)\n #pi^{-} (Red)\n 20 GeV at EEMC");
 

}


