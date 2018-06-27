#include "jpsi_invariant_mass_analysis.C"

int jpsi_invariant_mass_plot()
{
  jpsi_invariant_mass_analysis(); // creates vector 'invariant_mass'
  
  TCanvas *c = new TCanvas("c","c",600,400);
  TH1F *h1 = new TH1F("h1","h1",120,0,5);
  TH1F *h2 = new TH1F("h2","h2",120,0,5);
  h1->SetLineColor(kRed);
  h2->SetLineColor(kBlue);
  for(int i = 0; i<invariant_mass_cluster.size(); i++)
    {
      h1->Fill(invariant_mass_cluster.at(i));
    }
  for(int i = 0; i<invariant_mass_track.size(); i++)
    {
      h2->Fill(invariant_mass_track.at(i));
    }
  h2->Draw();
  h1->Draw("Same");
  
  return 0;
}
