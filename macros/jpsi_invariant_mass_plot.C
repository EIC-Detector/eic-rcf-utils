#include "jpsi_invariant_mass_analysis.C"

int jpsi_invariant_mass_plot()
{
  
    gROOT->LoadMacro("/sphenix/user/gregtom3/SBU/research/macros/macros/sPHENIXStyle/sPhenixStyle.C");
  SetsPhenixStyle();
  
  std::vector<TString> file_names;
  file_names.push_back("JP_C_1.root");
  file_names.push_back("JP_C_2.root");
  file_names.push_back("JP_C_3.root");
  file_names.push_back("JP_C_5.root");
  file_names.push_back("JP_C_8.root");
  file_names.push_back("JP_C_10.root");
  file_names.push_back("JP_C_15.root");
  file_names.push_back("JP_C_20.root");
  file_names.push_back("JP_F_1.root");
  file_names.push_back("JP_F_2.root");
  file_names.push_back("JP_F_3.root");
  file_names.push_back("JP_F_5.root");
  file_names.push_back("JP_F_8.root");
  file_names.push_back("JP_F_10.root");
  file_names.push_back("JP_F_15.root");
  file_names.push_back("JP_F_20.root");

  
  
  std::vector<TString> titles; // Legend Titles
  titles.push_back("1GeV CEMC ");
  titles.push_back("2GeV CEMC ");
  titles.push_back("3GeV CEMC ");
  titles.push_back("5GeV CEMC ");
  titles.push_back("8GeV CEMC ");
  titles.push_back("10GeV CEMC ");
  titles.push_back("15GeV CEMC ");
  titles.push_back("20GeV CEMC ");
  titles.push_back("1GeV FEMC ");
  titles.push_back("2GeV FEMC ");
  titles.push_back("3GeV FEMC ");
  titles.push_back("5GeV FEMC ");
  titles.push_back("8GeV FEMC ");
  titles.push_back("10GeV FEMC ");
  titles.push_back("15GeV FEMC ");
  titles.push_back("20GeV FEMC ");


  std::vector<TString> save_names;
  save_names.push_back("recoMass_JPsi_1GeV_pt_CEMC.png");
  save_names.push_back("recoMass_JPsi_2GeV_pt_CEMC.png");
  save_names.push_back("recoMass_JPsi_3GeV_pt_CEMC.png");
  save_names.push_back("recoMass_JPsi_5GeV_pt_CEMC.png");
  save_names.push_back("recoMass_JPsi_8GeV_pt_CEMC.png");
  save_names.push_back("recoMass_JPsi_10GeV_pt_CEMC.png");
  save_names.push_back("recoMass_JPsi_15GeV_pt_CEMC.png");
  save_names.push_back("recoMass_JPsi_20GeV_pt_CEMC.png");
  save_names.push_back("recoMass_JPsi_1GeV_pt_FEMC.png");
  save_names.push_back("recoMass_JPsi_2GeV_pt_FEMC.png");
  save_names.push_back("recoMass_JPsi_3GeV_pt_FEMC.png");
  save_names.push_back("recoMass_JPsi_5GeV_pt_FEMC.png");
  save_names.push_back("recoMass_JPsi_8GeV_pt_FEMC.png");
  save_names.push_back("recoMass_JPsi_10GeV_pt_FEMC.png");
  save_names.push_back("recoMass_JPsi_15GeV_pt_FEMC.png");
  save_names.push_back("recoMass_JPsi_20GeV_pt_FEMC.png");
  if(!((save_names.size()==titles.size())&&(save_names.size()==file_names.size())))
    {
      cout << "Vector arrays are not same size. \n Each file must have a titled legend and PNG save name" << endl;
      return -1;
    }
  for(unsigned file_idx=0 ; file_idx<file_names.size() ; file_idx++ )
    {
      file_name=file_names.at(file_idx);
      jpsi_invariant_mass_analysis(); // creates vector 'invariant_mass'
      TH1F *h1 = new TH1F("h1","h1",120,0,5);
      TH1F *h2 = new TH1F("h2","h2",120,0,5);
      h1->SetLineColor(kRed);
      h2->SetLineColor(kBlue);
      h2->GetXaxis()->SetTitle("Invariant Mass (GeV)");
      h2->GetYaxis()->SetTitle("Counts");
      cout << file_idx << " " << invariant_mass_cluster.size() << endl;
      for(int i = 0; i<invariant_mass_cluster.size(); i++)
	{
	  h1->Fill(invariant_mass_cluster.at(i));
	}
      for(int i = 0; i<invariant_mass_track.size(); i++)
	{
	  h2->Fill(invariant_mass_track.at(i));
	}
      histToPNG(h1,h2,titles.at(file_idx),save_names.at(file_idx));
      delete h1; delete h2; h1=NULL; h2=NULL;
      invariant_mass_cluster.clear();
      invariant_mass_track.clear();
    }
      return 0;
}

void histToPNG(TH1F* h_cluster, TH1F* h_track, TString title, TString saveFileName)
{
  TCanvas *cPNG = new TCanvas("cPNG",title,600,400);
  TImage *img = TImage::Create();
  cPNG->SetLogy();
  h_track->Draw();
  h_cluster->Draw("SAME");
  
  auto legend = new TLegend(0.2,0.7,0.5,0.9,title);
  legend->AddEntry(h_cluster,"Cluster","l");
  legend->AddEntry(h_track,"Track","l");
  legend->SetTextSize(0.06);
  legend->Draw();
  gPad->RedrawAxis();
  
  img->FromPad(cPNG);
  img->WriteImage(saveFileName);
  
  delete img;
  delete cPNG;

}
