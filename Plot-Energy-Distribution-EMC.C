#include <iostream>
#include <cstdlib>
#include <memory>
#include <sstream>
#include <string>
#include <cstring>

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TLegend.h"

#define NELEMS(arr) (sizeof(arr)/sizeof(arr[0]))

/* Directory where data is stored for plots */
const char *const data_directory =
    "/sphenix/user/gregtom3/data/Summer2018/ECAL_energy_studies";
/* The energy levels we have in GeV */
const static int energy_levels[] = { 1, 2, 5, 10, 20 };

/* The particle types we have */
enum particle_type { electron, pion };
/* The detectors we have */
enum detector { cemc, eemc, femc };

TTree *load_tree(const char *const file_name, const char *const tree_name);
void fill_histogram(TH1F * const h, TTree * const t, const Float_t min_value);
void histogram_to_png(TH1F * const h_pion, TH1F * const h_electron,
		      const char *const title, const char *const save_file_name,
		      const char *const pion_label,
		      const char *const electron_label);
void display_histogram(TH1F * const h, const char *const name,
		       const char *const title);
char *generate_file_path(const particle_type p, const int particle_energy_gev,
			 const detector d);
char *generate_save_file_name(const detector d);
char *generate_title(const detector d);
char *generate_label(const particle_type p, const detector d);

void Plot_Energy_Distribution_EMC()
{

	/* 
	 * sPHENIX Style
	 */

	gROOT->LoadMacro
	    ("/sphenix/user/gregtom3/SBU/research/macros/macros/sPHENIXStyle/sPhenixStyle.C");
	SetsPhenixStyle();

	gROOT->SetBatch(kTRUE);

	/*
	 * Base Histogram (Recreated from Matching Plots)
	 */

	TH1F *h_base = new TH1F("h_base", "", 25, 0.0, 21);
	TH1F *h_base_e = (TH1F *) h_base->Clone();
	TH1F *h_base_p = (TH1F *) h_base->Clone();
	h_base_e->SetLineColor(kRed);
	h_base_p->SetLineColor(kBlue);

	/* CEMC */
	TH1F *const h_pion_cemc = h_base_p->Clone();
	TH1F *const h_electron_cemc = h_base_e->Clone();
	h_pion_cemc->SetName("Pion-CEMC");
	h_electron_cemc->SetName("Electron-CEMC");

	/* EEMC */
	TH1F *const h_pion_eemc = h_base_p->Clone();
	TH1F *const h_electron_eemc = h_base_e->Clone();
	h_pion_eemc->SetName("Pion-EEMC");
	h_electron_cemc->SetName("Electron-EEMC");

	/* FEMC */
	TH1F *const h_pion_femc = h_base_p->Clone();
	TH1F *const h_electron_femc = h_base_e->Clone();
	h_pion_eemc->SetName("Pion-FEMC");
	h_electron_cemc->SetName("Electron-FEMC");

	/* iterate through energy levels and create plots for Pions and Electrons
	 * in the CEMC and EEMC detectors */
	for (size_t i = 0; i < NELEMS(energy_levels); ++i) {
		/* CEMC */
		TTree *const t_pion_cemc =
		    load_tree(generate_file_path(pion, energy_levels[i], cemc),
			      "ntp_cluster");
		TTree *const t_electron_cemc =
		    load_tree(generate_file_path
			      (electron, energy_levels[i], cemc),
			      "ntp_cluster");

		fill_histogram(h_pion_cemc, t_pion_cemc, 0.3);
		fill_histogram(h_electron_cemc, t_electron_cemc, 0.3);

		/* EEMC */
		TTree *const t_pion_eemc =
		    load_tree(generate_file_path(pion, energy_levels[i], eemc),
			      "ntp_cluster");
		TTree *const t_electron_eemc =
		    load_tree(generate_file_path
			      (electron, energy_levels[i], eemc),
			      "ntp_cluster");

		fill_histogram(h_pion_eemc, t_pion_eemc, 0.3);
		fill_histogram(h_electron_eemc, t_electron_eemc, 0.3);

		/* FEMC */
		TTree *const t_pion_femc =
		    load_tree(generate_file_path(pion, energy_levels[i], femc),
			      "ntp_cluster");
		TTree *const t_electron_femc =
		    load_tree(generate_file_path
			      (electron, energy_levels[i], femc),
			      "ntp_cluster");

		fill_histogram(h_pion_femc, t_pion_femc, 0.3);
		fill_histogram(h_electron_femc, t_electron_femc, 0.3);

	}

//      display_histogram(h_pion_cemc, "Pions CEMC","Pions CEMC");
//      display_histogram(h_electron_cemc, "Electrons CEMC","Electrons CEMC");
//      display_histogram(h_pion_eemc, "Pions EEMC","Pions FEMC");
//      display_histogram(h_electron_eemc, "Electrons EEMC","Electrons EMC");
//      display_histogram(h_pion_femc, "Pions FEMC","Pions FEMC");
//      display_histogram(h_electron_femc, "Electrons FEMC", "Electrons FEMC");

	histogram_to_png(h_pion_cemc, h_electron_cemc,
			 generate_title(cemc),
			 generate_save_file_name(cemc),
			 generate_label(pion, cemc),
			 generate_label(electron, cemc));
	histogram_to_png(h_pion_eemc, h_electron_eemc,
			 generate_title(eemc),
			 generate_save_file_name(eemc),
			 generate_label(pion, eemc),
			 generate_label(electron, eemc));
	histogram_to_png(h_pion_femc, h_electron_femc,
			 generate_title(femc),
			 generate_save_file_name(femc),
			 generate_label(pion, femc),
			 generate_label(electron, femc));
}

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
void fill_histogram(TH1F * const h, TTree * const t, const Float_t min_value)
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
			h->Fill(measured_energy);
	}

	h->SetXTitle("em_cluster_e");
	h->SetYTitle("entries / #scale[0.5]{#sum} entries      ");
}

void display_histogram(TH1F * const h, const char *const name,
		       const char *const title)
{
	TCanvas *const t = new TCanvas(name, title, 600, 400);
	t->SetLogy();
	h->Scale(1 / h->GetEntries());
	h->Draw();
}

void histogram_to_png(TH1F * const h_pion, TH1F * const h_electron,
		      const char *const title, const char *const save_file_name,
		      const char *const pion_label,
		      const char *const electron_label)
{
	TCanvas cPNG("cPNG", title, 1200, 400);
	TImage *img = TImage::Create();

	cPNG.Divide(2, 1);

	cPNG.cd(1);
//      h_pion->GetYaxis()->SetRangeUser(0.0001, 1);
	gPad->SetLogy();
	gPad->SetLeftMargin(0.3);
	h_pion->Scale(1 / h_pion->GetEntries());
	h_pion->Draw();
	gPad->RedrawAxis();

	auto pion_legend = new TLegend(0.80, 1, 1, 0.9, pion_label);
	pion_legend->Draw();

	cPNG.cd(2);
//      h_electron->GetYaxis()->SetRangeUser(0.0001, 1);
	gPad->SetLogy();
	gPad->SetLeftMargin(0.3);
	h_electron->Scale(1 / h_electron->GetEntries());
	h_electron->Draw();
	gPad->RedrawAxis();

	auto electron_legend = new TLegend(0.80, 1, 1, 0.9, electron_label);
	electron_legend->Draw();

	img->FromPad(&cPNG);
	img->WriteImage(save_file_name);

	delete img;
}

char *strdup(const char *s)
{
	char *const t = new char[strlen(s) + 1];
	return strcpy(t, s);
}

char *generate_file_path(const particle_type p,
			 const int particle_energy_gev, const detector d)
{
	std::stringstream path;
	path << data_directory;

	switch (p) {
	case electron:
		path << "/Electrons/Electrons";
		break;
	case pion:
		path << "/Pions/Pions";
		break;
	}

	path << particle_energy_gev;
	switch (d) {
	case cemc:
		path << "C";
		break;
	case eemc:
		path << "E";
		break;
	case femc:
		path << "F";
		break;
	}

	path << ".root";

	return strdup(path.str().c_str());
}

char *generate_save_file_name(const detector d)
{
	std::stringstream name;
	name << "Electron-Pion-";
	switch (d) {
	case cemc:
		name << "CEMC";
		break;
	case eemc:
		name << "EEMC";
		break;
	case femc:
		name << "FEMC";
		break;
	}

	name << ".png";

	return strdup(name.str().c_str());
}

char *generate_title(const detector d)
{
	std::stringstream title;
	switch (d) {
	case cemc:
		title << "CEMC";
		break;
	case eemc:
		title << "EEMC";
		break;
	case femc:
		title << "FEMC";
		break;
	}

	return strdup(title.str().c_str());
}

char *generate_label(const particle_type p, const detector d)
{
	std::stringstream label;

	switch (p) {
	case electron:
		label << "Electrons ";
		break;
	case pion:
		label << "Pions ";
		break;
	}

	switch (d) {
	case cemc:
		label << "CEMC";
		break;
	case eemc:
		label << "EEMC";
		break;
	case femc:
		label << "FEMC";
		break;
	}

	return strdup(label.str().c_str());
}
