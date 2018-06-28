#include <iostream>
#include <cstdlib>
#include <memory>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <utility>

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TColor.h"
#include "TImage.h"
#include "TApplication.h"

TTree *load_tree(const char *const file_name, const char *const tree_name);
const char *const svtx_file_path = "/sphenix/user/giorgian/muons/svtx.root";

const double MOMENTUM_MARGIN = 0.01;

void Plot_SVTX_Efficiency() {

	TTree *const ntp_gtrack {load_tree(svtx_file_path, "ntp_gtrack")};
	TTree *const ntp_track {load_tree(svtx_file_path, "ntp_track")};


	Long64_t ngtrack {ntp_gtrack->GetEntries()};
	Long64_t ntrack {ntp_track->GetEntries()};

	TH1F* h_geta {new TH1F("h_geta", "Psuedorapidity count", 100, -10, 10)};
	h_geta->SetLineColor(kBlue);
	Float_t geta;
	ntp_gtrack->SetBranchAddress("geta", &geta);
	std::unordered_map<Float_t, unsigned> gtrack_count;
	for (Long64_t i {0}; i < ngtrack; ++i) {
		if (ntp_gtrack->LoadTree(i) < 0)
			break;
		ntp_gtrack->GetEntry(i);
		++gtrack_count[geta];
		h_geta->Fill(geta);
	}


	TH1F* h_geta_p {new TH1F("h_geta_p", "Psuedorapidity count", 100, -10, 10)};
	h_geta_p->SetLineColor(kRed);
	Float_t gpx, gpy, gpz, px, py, pz;
	ntp_track->SetBranchAddress("geta", &geta);
	ntp_track->SetBranchAddress("gpx", &gpx);
	ntp_track->SetBranchAddress("gpy", &gpy);
	ntp_track->SetBranchAddress("gpz", &gpz);
	ntp_track->SetBranchAddress("px", &px);
	ntp_track->SetBranchAddress("py", &py);
	ntp_track->SetBranchAddress("pz", &pz);
	std::unordered_map<Float_t, unsigned> track_count;
	for (Long64_t i = 0; i < ntrack; ++i) {
		if (ntp_track->LoadTree(i) < 0)
			break;

		ntp_track->GetEntry(i);
		if (fabs((px - gpx)/gpx) < MOMENTUM_MARGIN
		&& fabs((py - gpy)/gpy) < MOMENTUM_MARGIN
		&& fabs((pz - gpz)/gpz) < MOMENTUM_MARGIN) {
			++track_count[geta];
			h_geta_p->Fill(geta);
		}
	}

	TH1F* h_geta_count {new TH1F("h_geta_count", "SVTX Efficiency", 100, -3, 3)};
	for (const auto& e: gtrack_count) {
		std::cout << "Eta(" << e.first << "): " << track_count[e.first] * 1.0 / e.second << '\n';
		h_geta_count->Fill(e.first, track_count[e.first] * 1.0 / e.second);
	}
//	h_geta_count->Scale(1 / h_geta_count->GetEntries());

	TCanvas* c {new TCanvas("c", "SVTX Efficiency", 1200, 400)};
	c->Divide(1, 2);
	c->cd(1);
	h_geta->Draw();
	h_geta_p->Draw("SAME");
	c->cd(2);
	h_geta_count->Draw();
}

TTree *load_tree(const char *const file_name, const char *const tree_name)
{
	return (TTree *) (new TFile(file_name, "READ"))->Get(tree_name);
}

int main(int argc, char *argv[]) {
	TApplication app("SVTX Efficiency Plots", &argc, argv);
	Plot_SVTX_Efficiency();
	app.Run();
	return 0;
}

