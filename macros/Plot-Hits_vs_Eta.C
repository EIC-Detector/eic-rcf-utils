#include <iostream>
#include <cstdlib>
#include <memory>
#include <string>
#include <cstring>
#include <vector>
#include <set>
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
#include "TGraphErrors.h"
#include "/sphenix/user/gregtom3/SBU/research/macros/macros/sPHENIXStyle/sPhenixStyle.C"

#define STR(X) #X
#define XSTR(X) STR(X)
#define NELEMS(a) (sizeof(a)/sizeof(a[0]))


TTree *load_tree(const char *const file_name, const char *const tree_name);
void fill_histogram(TH1F * const h, TTree * const t);
const char *const svtx_file_path {"/sphenix/user/giorgian/hits-per-eta/hitcount.root"};

const char *const hit_containers[] {"G4HIT_EGEM_0", "G4HIT_EGEM_1", "G4HIT_EGEM_3", 
	"G4HIT_FGEM_0", "G4HIT_FGEM_1", "G4HIT_FGEM_2", "G4HIT_FGEM_3","G4HIT_FGEM_4", "G4HIT_MAPS", 
	"G4HIT_SVTX"};

void Plot_Hit_Count() {
	SetsPhenixStyle();
	gROOT->SetBatch(true);

	for (size_t i {0}; i < NELEMS(hit_containers); ++i) {
		const std::string tree_name {std::string(hit_containers[i]) + "_normalized"};
		TTree *const hits {load_tree(svtx_file_path, tree_name.c_str())};


		Double_t eta, hit_count;
		hits->SetBranchAddress("eta", &eta);
		hits->SetBranchAddress("hit_count", &hit_count);

		Long64_t nentries {hits->GetEntries()};
		Double_t *x {new Double_t[nentries]};
		Double_t *y {new Double_t[nentries]};
		for (Long64_t i {0}; i < nentries; ++i) {
			if (hits->LoadTree(i) < 0)
				break;
			hits->GetEntry(i);
			x[i] = eta;
			y[i] = hit_count;
		}


		TCanvas c {"hits", "Hits", gStyle->GetCanvasDefW(), gStyle->GetCanvasDefH()};
		TGraph *gr {new TGraphErrors(nentries, x, y)};
		gr->SetMarkerColor(kBlue);
		gr->SetMarkerStyle(21);
		gr->SetMarkerSize(0.5);
		gr->GetXaxis()->SetTitle("#eta");
		gr->GetYaxis()->SetTitle("Hit Count");
//		gr->Draw("ALP");

		hits->Draw("eta","hit_count");

		TLegend *l {new TLegend(0.65, 0.9, 0.95, 0.8, "Detector")};
		l->SetTextSize(0.03);
//		l->AddEntry(gr, "Normalized Hit Count", "l");
		l->AddEntry(hits, hit_containers[i], "l");
		l->Draw();
		gPad->RedrawAxis();


		TImage *const img {TImage::Create()};
		img->FromPad(&c);
		img->WriteImage((std::string(hit_containers[i]) + ".png").c_str());
	}
	gApplication->Terminate(0);
}

void fill_histogram(TH1F * const h, TTree * const t)
{
	Float_t px, py, pz;
	t->SetBranchAddress("px", &px);
	t->SetBranchAddress("py", &py);
	t->SetBranchAddress("pz", &pz);

	Int_t nentries = Int_t(t->GetEntries());

	for (Int_t i = 0; i < nentries; ++i) {
		if (t->LoadTree(i) < 0)
			break;

		t->GetEntry(i);
		const double mom {std::sqrt(px * px + py * py + pz * pz)};
		const double eta {0.5 *std::log((mom + pz) / (mom - pz))};
		h->Fill(eta);
	}
}

TTree *load_tree(const char *const file_name, const char *const tree_name)
{
	return (TTree *) (new TFile(file_name, "READ"))->Get(tree_name);
}

int main(int argc, char *argv[]) {
	TApplication app("Hit Plots", &argc, argv);
	Plot_Hit_Count();
	app.Run();
	return 0;
}

