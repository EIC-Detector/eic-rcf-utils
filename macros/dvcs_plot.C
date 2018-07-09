int dvcs_plot()
{

  // -----------------------------------------------------------------------//
  // Momentum vs. Eta Plot
  // -----------------------------------------------------------------------//

  // Use a tree for momentum vs. eta plot
  TFile *f_mom_eta = new TFile("/sphenix/user/gregtom3/data/Summer2018/DVCS_sample_studies/Test7_DVCS.root","READ");
  TTree *t_mom_eta = (TTree*)f_mom_eta->Get("event_truth");
  
  // Creating Histogram //
  TCanvas *c = new TCanvas("c","c",600,400);
  TH2F *h2_mom_eta = new TH2F("h2_mom_eta","h2_mom_eta",100,-4,4,100,0,35);
  
  // Filling Histogram //
  gen_hist(h2_mom_eta,t_mom_eta,"mom_eta");

  // Drawing Histogram //
  c->SetLogz();
  h2_mom_eta->GetXaxis()->SetTitle("Photon Pseudorapidity #eta");
  h2_mom_eta->GetYaxis()->SetTitle("Photon Momentum [GeV]");
  h2_mom_eta->Draw("colz");
  
  // -----------------------------------------------------------------------//
  // x-Q2 Plot
  // -----------------------------------------------------------------------//

   // These three trees used for x Q2 plot
  TFile *f_EEMC = new TFile("/sphenix/user/gregtom3/data/Summer2018/DVCS_sample_studies/100K_Q2>1.0_EEMC_DVCS.root","READ");
  TTree *t_EEMC = (TTree*)f_EEMC->Get("event_truth");
  
  TFile *f_CEMC = new TFile("/sphenix/user/gregtom3/data/Summer2018/DVCS_sample_studies/100K_Q2>1.0_CEMC_DVCS.root","READ");
  TTree *t_CEMC = (TTree*)f_CEMC->Get("event_truth");

  TFile *f_FEMC = new TFile("/sphenix/user/gregtom3/data/Summer2018/DVCS_sample_studies/100K_Q2>1.0_FEMC_DVCS.root","READ");
  TTree *t_FEMC = (TTree*)f_FEMC->Get("event_truth");

  // Creating Histograms //
  int nbins = 40;
  TH2F *h2_x_Q2_EEMC = new TH2F("h2_x_Q2_EEMC","h2_x_Q2_EEMC",nbins,-4,0,nbins,0,2);
  TH2F *h2_x_Q2_CEMC = new TH2F("h2_x_Q2_CEMC","h2_x_Q2_CEMC",nbins,-4,0,nbins,0,2);
  TH2F *h2_x_Q2_FEMC = new TH2F("h2_x_Q2_FEMC","h2_x_Q2_FEMC",nbins,-4,0,nbins,0,2);
  BinLog(h2_x_Q2_EEMC);
  BinLog(h2_x_Q2_CEMC);
  BinLog(h2_x_Q2_FEMC);
  
  // Filling Histograms //
  gen_hist(h2_x_Q2_EEMC,t_EEMC,"x_Q2");
  gen_hist(h2_x_Q2_CEMC,t_CEMC,"x_Q2");
  gen_hist(h2_x_Q2_FEMC,t_FEMC,"x_Q2");

  // Drawing Histograms //
  TCanvas *c2 = new TCanvas("c2","c2");
  c2->SetWindowSize(gStyle->GetCanvasDefW()*3,gStyle->GetCanvasDefH());
  c2->Divide(3,1,0,0);
  c2->SetTopMargin(0.05);
  c2->SetLeftMargin(0.5);
  c2_1->SetTopMargin(0.05);
  c2_2->SetTopMargin(0.05);
  c2_3->SetTopMargin(0.05);

  c2_1->SetRightMargin(0.1);
  c2_2->SetLeftMargin(0.1);
  c2_2->SetRightMargin(0.1);
  c2_3->SetLeftMargin(0.1);
  c2_3->SetRightMargin(0.1);
  c2->cd(1);
  gPad->SetLogx(); gPad->SetLogy(); 
  

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  h2_x_Q2_EEMC->GetYaxis()->SetTitle("Q2");
  h2_x_Q2_EEMC->GetZaxis()->SetRangeUser(0,2500);
  h2_x_Q2_EEMC->Draw("col");

  c2->cd(2);
  gPad->SetLogx(); gPad->SetLogy(); 
  h2_x_Q2_CEMC->GetZaxis()->SetRangeUser(0,2500);
  h2_x_Q2_CEMC->Draw("col");

  c2->cd(3);
  gPad->SetLogx(); gPad->SetLogy(); 
  h2_x_Q2_FEMC->GetXaxis()->SetTitle("x");
  h2_x_Q2_FEMC->GetZaxis()->SetRangeUser(0,2500);
  h2_x_Q2_FEMC->Draw("colz");


  return 0;
}


void BinLog(TH2F *h) 
{

   TAxis *axis = h->GetXaxis(); 
   int bins = axis->GetNbins();

   Axis_t from = axis->GetXmin();
   Axis_t to = axis->GetXmax();
   Axis_t width = (to - from) / bins;
   Axis_t *new_bins = new Axis_t[bins + 1];

   for (int i = 0; i <= bins; i++) {
     new_bins[i] = TMath::Power(10, from + i * width);
   } 
   axis->Set(bins, new_bins); 

   TAxis *axis2 = h->GetYaxis(); 
   int bins2 = axis2->GetNbins();

   Axis_t from2 = axis2->GetXmin();
   Axis_t to2 = axis2->GetXmax();
   Axis_t width2 = (to2 - from2) / bins2;
   Axis_t *new_bins2 = new Axis_t[bins2 + 1];

   for (int i = 0; i <= bins2; i++) {
     new_bins2[i] = TMath::Power(10, from2 + i * width2);
   } 
   axis2->Set(bins2, new_bins2); 

   delete new_bins;
   delete new_bins2;
}

void gen_hist(TH2F* hist, TTree *t, char *c)
{
  Float_t Q2;
  Float_t x;
  Float_t y;
  std::vector<float> energy;
  std::vector<float> eta;
  std::vector<float> pid;

  std::vector<float>* energy_pointer=&energy;
  std::vector<float>* eta_pointer=&eta;
  std::vector<float>* pid_pointer=&pid;

  t->SetBranchAddress("evtgen_Q2",&Q2);
  t->SetBranchAddress("evtgen_x",&x);
  t->SetBranchAddress("evtgen_y",&y);
  t->SetBranchAddress("em_evtgen_ptotal",&energy_pointer);
  t->SetBranchAddress("em_evtgen_eta",&eta_pointer);
  t->SetBranchAddress("em_evtgen_pid",&pid_pointer);
  
  Int_t nentries = Int_t(t->GetEntries());
  for(Int_t entryInChain=0; entryInChain<nentries; entryInChain++)
    {
      Int_t entryInTree = t->LoadTree(entryInChain);
      if (entryInTree < 0) break;
      t->GetEntry(entryInChain);

      // Momentum and Eta of Scattered Photon
      float the_eta;
      float the_mom;

      // Flip through all the particles in the event (only 3)
      for(unsigned i = 0; i<energy.size(); i++)
	{
	  if(pid.at(i)==22) // Scattered photon
	    {
	      the_eta = eta.at(i);
	      the_mom = energy.at(i);
	    }
	}
      if(Q2>1.0&&y>0.01) // Plotting cut
      	{
	  if(c=="mom_eta")
	    hist->Fill(the_eta,the_mom);
	  else if(c=="x_Q2")
	    hist->Fill(x,Q2);
	}
    }
}
