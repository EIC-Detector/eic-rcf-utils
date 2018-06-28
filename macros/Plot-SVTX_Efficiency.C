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
#include "TGraph.h"
#include "/sphenix/user/gregtom3/SBU/research/macros/macros/sPHENIXStyle/sPhenixStyle.C"


TTree *load_tree(const char *const file_name, const char *const tree_name);
const char *const svtx_file_path = "/sphenix/user/giorgian/muons/svtx.root";

const double MOMENTUM_MARGIN = 0.01;

void Plot_SVTX_Efficiency() {
	SetsPhenixStyle();

	TTree *const ntp_gtrack {load_tree(svtx_file_path, "ntp_gtrack")};
	TTree *const ntp_track {load_tree(svtx_file_path, "ntp_track")};


	Long64_t ngtrack {ntp_gtrack->GetEntries()};
	Long64_t ntrack {ntp_track->GetEntries()};

	TH1F* h_geta {new TH1F("h_geta", "Psuedorapidity count", 100, -5, 5)};
	h_geta->SetLineColor(kBlue);
	Float_t geta;
	ntp_gtrack->SetBranchAddress("geta", &geta);
	for (Long64_t i {0}; i < ngtrack; ++i) {
		if (ntp_gtrack->LoadTree(i) < 0)
			break;
		ntp_gtrack->GetEntry(i);
		h_geta->Fill(geta);
	}
TH1F* h_geta_p {new TH1F("h_geta_p", "Psuedorapidity count", 100, -5, 5)}; h_geta_p->SetLineColor(kRed);
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
		&& fabs((pz - gpz)/gpz) < MOMENTUM_MARGIN)
			h_geta_p->Fill(geta);
	}

	const Long64_t nbins {h_geta->GetSize() - 2}; /* -2 for underflow and overflow */
	Double_t *x {new Double_t[nbins]};
	Double_t *y {new Double_t[nbins]};
	Long64_t top = 0;
	for (Long64_t i = 0; i < nbins; ++i) {
		if (h_geta->GetBinContent(i + 1) != 0) {
			x[top] = h_geta->GetBinCenter(i + 1);
			y[top++] = h_geta_p->GetBinContent(i + 1) * 1.0 / h_geta->GetBinContent(i + 1);
			std::cout << "x[" << top << "]: " << x[top] << '\t' << "y[" << top << "]: " << y[top] << '\n';
		}
	}
	TGraph *gr {new TGraph(nbins, x, y)};

	TCanvas* c {new TCanvas("c", "SVTX Efficiency", 1200, 400)};
	c->Divide(1, 2);
	c->cd(1);
	h_geta->GetYaxis()->SetRangeUser(0, 1500);
	h_geta->Draw();
	h_geta_p->Draw("SAME");
	c->cd(2);
	gr->Draw("AC*");
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

