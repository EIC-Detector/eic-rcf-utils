#include <iostream>
#include <cstdlib>
#include <memory>

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TLegend.h"

TTree *load_tree(const char *const file_name, const char *const tree_name)
{
	return (TTree *) (new TFile(file_name, "READ"))->Get(tree_name);
}

/*
 * This function, when fed a histogram, tree,
 * a floating point min_value variable, and a boolean, will fill each entry
 * inside the specificed branch into the histogram, so long as each entry
 * is a floating point number GREATER than the min_value. If normalize is
 * set to 'true', the histogram will be normalize based on its number of
 * entries. The axes titles are furthermore assumed to be generic and have
 * been already set.
 */
void fill_histogram(TH1F * const h, TTree * const t, const Float_t min_value,
		    const bool normalize)
{
	Float_t measured_energy;
	Float_t true_energy;
	t->SetBranchAddress("e", &measured_energy);
	t->SetBranchAddress("ge", &true_energy);
	Int_t nentries = Int_t(t->GetEntries());

	for (Int_t i = 0; i < nentries; ++i) {
		if (t->LoadTree(i) < 0)
			break;

		t->GetEntry(i);
		if (measured_energy > min_value && true_energy > 0.1)
			h->Fill(measured_energy / true_energy);
	}
	if (normalize)
		h->Scale(1 / h->GetEntries());

	h->SetXTitle("em_cluster_e / true_e");
	h->SetYTitle("entries / #scale[0.5]{#sum} entries      ");
}

void histogram_to_png(TH1F * const h_pion_CEMC, TH1F * const h_electron_CEMC,
		      TH1F * const h_pion_EEMC, TH1F * const h_electron_EEMC,
		      const char *const title, const char *const save_file_name,
		      const char *const cemc_label,
		      const char *const eemc_label)
{
	TCanvas cPNG("cPNG", title, 1200, 400);
	TImage *img = TImage::Create();

	cPNG.Divide(2, 1);
	cPNG.cd(1);
	h_pion_CEMC->GetYaxis()->SetRangeUser(0.0001, 1);
	h_electron_CEMC->GetYaxis()->SetRangeUser(0.0001, 1);
	gPad->SetLogy();
	h_pion_CEMC->Draw();
	h_electron_CEMC->Draw("SAME");
	gPad->RedrawAxis();

	auto cemc_legend = new TLegend(0.70, 0.9, 0.95, 0.65, cemc_label);
	cemc_legend->AddEntry(h_pion_CEMC, "Pions", "l");
	cemc_legend->AddEntry(h_electron_CEMC, "Electrons", "l");
	cemc_legend->Draw();

	cPNG.cd(2);
	h_pion_EEMC->GetYaxis()->SetRangeUser(0.0001, 1);
	h_electron_EEMC->GetYaxis()->SetRangeUser(0.0001, 1);
	gPad->SetLogy();
	h_pion_EEMC->Draw();
	h_electron_EEMC->Draw("SAME");
	gPad->RedrawAxis();

	auto eemc_legend = new TLegend(0.65, 0.9, 0.9, 0.65, eemc_label);
	eemc_legend->AddEntry(h_pion_EEMC, "Pions", "l");
	eemc_legend->AddEntry(h_electron_EEMC, "Electrons", "l");
	eemc_legend->Draw();

	img->FromPad(&cPNG);
	img->WriteImage(save_file_name);

	delete img;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                Main Code
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void energy_EMC_e_pi_plotMacro()
{

	/* *
	 * sPHENIX Style
	 */

	gROOT->LoadMacro
	    ("/sphenix/user/gregtom3/SBU/research/macros/macros/sPHENIXStyle/sPhenixStyle.C");
	SetsPhenixStyle();

	gROOT->SetBatch(kTRUE);

	/* *
	 * Base Histogram (Recreated from Matching Plots)
	 */

	TH1F *h_base = new TH1F("h_base", "", 25, 0.0, 2.0);
	TH1F *h_base_e = (TH1F *) h_base->Clone();
	TH1F *h_base_p = (TH1F *) h_base->Clone();
	h_base_e->SetLineColor(kRed);
	h_base_p->SetLineColor(kBlue);

	/* *
	 * Loading histograms and trees for CEMC and EEMC, electrons and pions, [1,2,5,10,20] GeV 
	 */

	/* CEMC */
	TH1F *h_P_1GeV_CEMC = (TH1F *) h_base_p->Clone();
	TH1F *h_P_2GeV_CEMC = (TH1F *) h_base_p->Clone();
	TH1F *h_P_5GeV_CEMC = (TH1F *) h_base_p->Clone();
	TH1F *h_P_10GeV_CEMC = (TH1F *) h_base_p->Clone();
	TH1F *h_P_20GeV_CEMC = (TH1F *) h_base_p->Clone();

	TH1F *h_E_1GeV_CEMC = (TH1F *) h_base_e->Clone();
	TH1F *h_E_2GeV_CEMC = (TH1F *) h_base_e->Clone();
	TH1F *h_E_5GeV_CEMC = (TH1F *) h_base_e->Clone();
	TH1F *h_E_10GeV_CEMC = (TH1F *) h_base_e->Clone();
	TH1F *h_E_20GeV_CEMC = (TH1F *) h_base_e->Clone();

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

	TTree *t_P_1GeV_CEMC =
	    load_tree
	    ("/sphenix/user/gregtom3/data/Summer2018/ECAL_energy_studies/Pions/Pions1C.root",
	     "ntp_cluster");
	TTree *t_P_2GeV_CEMC =
	    load_tree
	    ("/sphenix/user/gregtom3/data/Summer2018/ECAL_energy_studies/Pions/Pions2C.root",
	     "ntp_cluster");
	TTree *t_P_5GeV_CEMC =
	    load_tree
	    ("/sphenix/user/gregtom3/data/Summer2018/ECAL_energy_studies/Pions/Pions5C.root",
	     "ntp_cluster");
	TTree *t_P_10GeV_CEMC =
	    load_tree
	    ("/sphenix/user/gregtom3/data/Summer2018/ECAL_energy_studies/Pions/Pions10C.root",
	     "ntp_cluster");
	TTree *t_P_20GeV_CEMC =
	    load_tree
	    ("/sphenix/user/gregtom3/data/Summer2018/ECAL_energy_studies/Pions/Pions20C.root",
	     "ntp_cluster");

	TTree *t_E_1GeV_CEMC =
	    load_tree
	    ("/sphenix/user/gregtom3/data/Summer2018/ECAL_energy_studies/Electrons/Electrons1C.root",
	     "ntp_cluster");
	TTree *t_E_2GeV_CEMC =
	    load_tree
	    ("/sphenix/user/gregtom3/data/Summer2018/ECAL_energy_studies/Electrons/Electrons2C.root",
	     "ntp_cluster");
	TTree *t_E_5GeV_CEMC =
	    load_tree
	    ("/sphenix/user/gregtom3/data/Summer2018/ECAL_energy_studies/Electrons/Electrons5C.root",
	     "ntp_cluster");
	TTree *t_E_10GeV_CEMC =
	    load_tree
	    ("/sphenix/user/gregtom3/data/Summer2018/ECAL_energy_studies/Electrons/Electrons10C.root",
	     "ntp_cluster");
	TTree *t_E_20GeV_CEMC =
	    load_tree
	    ("/sphenix/user/gregtom3/data/Summer2018/ECAL_energy_studies/Electrons/Electrons20C.root",
	     "ntp_cluster");

	/* EEMC */
	TH1F *h_P_1GeV_EEMC = (TH1F *) h_base_p->Clone();
	TH1F *h_P_2GeV_EEMC = (TH1F *) h_base_p->Clone();
	TH1F *h_P_5GeV_EEMC = (TH1F *) h_base_p->Clone();
	TH1F *h_P_10GeV_EEMC = (TH1F *) h_base_p->Clone();
	TH1F *h_P_20GeV_EEMC = (TH1F *) h_base_p->Clone();

	TH1F *h_E_1GeV_EEMC = (TH1F *) h_base_e->Clone();
	TH1F *h_E_2GeV_EEMC = (TH1F *) h_base_e->Clone();
	TH1F *h_E_5GeV_EEMC = (TH1F *) h_base_e->Clone();
	TH1F *h_E_10GeV_EEMC = (TH1F *) h_base_e->Clone();
	TH1F *h_E_20GeV_EEMC = (TH1F *) h_base_e->Clone();

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

	TTree *t_P_1GeV_EEMC =
	    load_tree
	    ("/sphenix/user/gregtom3/data/Summer2018/ECAL_energy_studies/Pions/Pions1E.root",
	     "ntp_cluster");
	TTree *t_P_2GeV_EEMC =
	    load_tree
	    ("/sphenix/user/gregtom3/data/Summer2018/ECAL_energy_studies/Pions/Pions2E.root",
	     "ntp_cluster");
	TTree *t_P_5GeV_EEMC =
	    load_tree
	    ("/sphenix/user/gregtom3/data/Summer2018/ECAL_energy_studies/Pions/Pions5E.root",
	     "ntp_cluster");
	TTree *t_P_10GeV_EEMC =
	    load_tree
	    ("/sphenix/user/gregtom3/data/Summer2018/ECAL_energy_studies/Pions/Pions10E.root",
	     "ntp_cluster");
	TTree *t_P_20GeV_EEMC =
	    load_tree
	    ("/sphenix/user/gregtom3/data/Summer2018/ECAL_energy_studies/Pions/Pions20E.root",
	     "ntp_cluster");

	TTree *t_E_1GeV_EEMC =
	    load_tree
	    ("/sphenix/user/gregtom3/data/Summer2018/ECAL_energy_studies/Electrons/Electrons1E.root",
	     "ntp_cluster");
	TTree *t_E_2GeV_EEMC =
	    load_tree
	    ("/sphenix/user/gregtom3/data/Summer2018/ECAL_energy_studies/Electrons/Electrons2E.root",
	     "ntp_cluster");
	TTree *t_E_5GeV_EEMC =
	    load_tree
	    ("/sphenix/user/gregtom3/data/Summer2018/ECAL_energy_studies/Electrons/Electrons5E.root",
	     "ntp_cluster");
	TTree *t_E_10GeV_EEMC =
	    load_tree
	    ("/sphenix/user/gregtom3/data/Summer2018/ECAL_energy_studies/Electrons/Electrons10E.root",
	     "ntp_cluster");
	TTree *t_E_20GeV_EEMC =
	    load_tree
	    ("/sphenix/user/gregtom3/data/Summer2018/ECAL_energy_studies/Electrons/Electrons20E.root",
	     "ntp_cluster");

	/* *
	 * Filling Histograms and normalizing
	 */

	/* Electrons and Pions 1 GeV CEMC */
	fill_histogram(h_P_1GeV_CEMC, t_P_1GeV_CEMC, 0.3, true);
	fill_histogram(h_E_1GeV_CEMC, t_E_1GeV_CEMC, 0.3, true);
	/* Electrons and Pions 2 GeV CEMC */
	fill_histogram(h_P_2GeV_CEMC, t_P_2GeV_CEMC, 0.3, true);
	fill_histogram(h_E_2GeV_CEMC, t_E_2GeV_CEMC, 0.3, true);
	/* Electrons and Pions 5 GeV CEMC */
	fill_histogram(h_P_5GeV_CEMC, t_P_5GeV_CEMC, 0.3, true);
	fill_histogram(h_E_5GeV_CEMC, t_E_5GeV_CEMC, 0.3, true);
	/* Electrons and Pions 10 GeV CEMC */
	fill_histogram(h_P_10GeV_CEMC, t_P_10GeV_CEMC, 0.3, true);
	fill_histogram(h_E_10GeV_CEMC, t_E_10GeV_CEMC, 0.3, true);
	/* Electrons and Pions 20 GeV CEMC */
	fill_histogram(h_P_20GeV_CEMC, t_P_20GeV_CEMC, 0.3, true);
	fill_histogram(h_E_20GeV_CEMC, t_E_20GeV_CEMC, 0.3, true);
	/* Electrons and Pions 1 GeV EEMC */
	fill_histogram(h_P_1GeV_EEMC, t_P_1GeV_EEMC, 0.3, true);
	fill_histogram(h_E_1GeV_EEMC, t_E_1GeV_EEMC, 0.3, true);
	/* Electrons and Pions 2 GeV EEMC */
	fill_histogram(h_P_2GeV_EEMC, t_P_2GeV_EEMC, 0.3, true);
	fill_histogram(h_E_2GeV_EEMC, t_E_2GeV_EEMC, 0.3, true);
	/* Electrons and Pions 5 GeV EEMC */
	fill_histogram(h_P_5GeV_EEMC, t_P_5GeV_EEMC, 0.3, true);
	fill_histogram(h_E_5GeV_EEMC, t_E_5GeV_EEMC, 0.3, true);
	/* Electrons and Pions 10 GeV EEMC */
	fill_histogram(h_P_10GeV_EEMC, t_P_10GeV_EEMC, 0.3, true);
	fill_histogram(h_E_10GeV_EEMC, t_E_10GeV_EEMC, 0.3, true);
	/* Electrons and Pions 20 GeV EEMC */
	fill_histogram(h_P_20GeV_EEMC, t_P_20GeV_EEMC, 0.3, true);
	fill_histogram(h_E_20GeV_EEMC, t_E_20GeV_EEMC, 0.3, true);

	/* *
	 * Saving histograms to PNG 
	 */

	histogram_to_png(h_P_1GeV_CEMC, h_E_1GeV_CEMC, h_P_1GeV_EEMC,
			 h_E_1GeV_EEMC, "1GeV", "test1.png", "CEMC 1GeV",
			 "EEMC 1GeV");
	histogram_to_png(h_P_2GeV_CEMC, h_E_2GeV_CEMC, h_P_2GeV_EEMC,
			 h_E_2GeV_EEMC, "2GeV", "test2.png", "CEMC 2GeV",
			 "EEMC 2GeV");
	histogram_to_png(h_P_5GeV_CEMC, h_E_5GeV_CEMC, h_P_5GeV_EEMC,
			 h_E_5GeV_EEMC, "5GeV", "test3.png", "CEMC 5GeV",
			 "EEMC 5GeV");
	histogram_to_png(h_P_10GeV_CEMC, h_E_10GeV_CEMC, h_P_10GeV_EEMC,
			 h_E_10GeV_EEMC, "10GeV", "test4.png", "CEMC 10GeV",
			 "EEMC 10GeV");
	histogram_to_png(h_P_20GeV_CEMC, h_E_20GeV_CEMC, h_P_20GeV_EEMC,
			 h_E_20GeV_EEMC, "20GeV", "test5.png", "CEMC 20GeV",
			 "EEMC 20GeV");

	/* *
	 * Comparison of detector performance as a function of particle and energy
	 */

	/* Pions CEMC */
	h_P_1GeV_CEMC->SetTitle
	    ("#pi^{-} CEMC - 1GeV(B):2GeV(G):5GeV(Y):10GeV(O):20GeV(R)");
	h_P_1GeV_CEMC->SetLineColor(kBlue);
	h_P_2GeV_CEMC->SetLineColor(kGreen);
	h_P_5GeV_CEMC->SetLineColor(kYellow);
	h_P_10GeV_CEMC->SetLineColor(kOrange);
	h_P_20GeV_CEMC->SetLineColor(kRed);
	TCanvas *c1 = new TCanvas("c1", "Pions in CEMC", 600, 400);
	TImage *t1 = TImage::Create();
	c1->SetLogy();
	h_P_1GeV_CEMC->Draw();
	h_P_2GeV_CEMC->Draw("SAME");
	h_P_5GeV_CEMC->Draw("SAME");
	h_P_10GeV_CEMC->Draw("SAME");
	h_P_20GeV_CEMC->Draw("SAME");
	t1->FromPad(c1);
	t1->WriteImage("P_CEMC.png");
	delete c1;
	delete t1;
	/* Electrons CEMC */
	h_E_1GeV_CEMC->SetTitle
	    ("e^{-} CEMC - 1GeV(B):2GeV(G):5GeV(Y):10GeV(O):20GeV(R)");
	h_E_1GeV_CEMC->SetLineColor(kBlue);
	h_E_2GeV_CEMC->SetLineColor(kGreen);
	h_E_5GeV_CEMC->SetLineColor(kYellow);
	h_E_10GeV_CEMC->SetLineColor(kOrange);
	h_E_20GeV_CEMC->SetLineColor(kRed);
	TCanvas *c2 = new TCanvas("c2", "Electrons in CEMC", 600, 400);
	TImage *t2 = TImage::Create();
	c2->SetLogy();
	h_E_1GeV_CEMC->Draw();
	h_E_2GeV_CEMC->Draw("SAME");
	h_E_5GeV_CEMC->Draw("SAME");
	h_E_10GeV_CEMC->Draw("SAME");
	h_E_20GeV_CEMC->Draw("SAME");
	t2->FromPad(c2);
	t2->WriteImage("E_CEMC.png");
	delete c2;
	delete t2;
	/* Pions EEMC */
	h_P_1GeV_EEMC->SetTitle
	    ("#pi^{-} EEMC - 1GeV(B):2GeV(G):5GeV(Y):10GeV(O):20GeV(R)");
	h_P_1GeV_EEMC->SetLineColor(kBlue);
	h_P_2GeV_EEMC->SetLineColor(kGreen);
	h_P_5GeV_EEMC->SetLineColor(kYellow);
	h_P_10GeV_EEMC->SetLineColor(kOrange);
	h_P_20GeV_EEMC->SetLineColor(kRed);
	TCanvas *c3 = new TCanvas("c3", "Pions in EEMC", 600, 400);
	TImage *t3 = TImage::Create();
	c3->SetLogy();
	h_P_1GeV_EEMC->Draw();
	h_P_2GeV_EEMC->Draw("SAME");
	h_P_5GeV_EEMC->Draw("SAME");
	h_P_10GeV_EEMC->Draw("SAME");
	h_P_20GeV_EEMC->Draw("SAME");
	t3->FromPad(c3);
	t3->WriteImage("P_EEMC.png");
	delete c3;
	delete t3;
	/* Electrons EEMC */
	h_E_1GeV_EEMC->SetTitle
	    ("e^{-} EEMC - 1GeV(B):2GeV(G):5GeV(Y):10GeV(O):20GeV(R)");
	h_E_1GeV_EEMC->SetLineColor(kBlue);
	h_E_2GeV_EEMC->SetLineColor(kGreen);
	h_E_5GeV_EEMC->SetLineColor(kYellow);
	h_E_10GeV_EEMC->SetLineColor(kOrange);
	h_E_20GeV_EEMC->SetLineColor(kRed);
	TCanvas *c4 = new TCanvas("c4", "Electrons in EEMC", 600, 400);
	TImage *t4 = TImage::Create();
	c4->SetLogy();
	h_E_1GeV_EEMC->Draw();
	h_E_2GeV_EEMC->Draw("SAME");
	h_E_5GeV_EEMC->Draw("SAME");
	h_E_10GeV_EEMC->Draw("SAME");
	h_E_20GeV_EEMC->Draw("SAME");
	t4->FromPad(c4);
	t4->WriteImage("E_EEMC.png");
	delete c4;
	delete t4;
}
