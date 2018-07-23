#include <iostream>
#include <fstream>
#include <cstdlib>
#include <memory>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <cmath>
#include <utility>
#include <vector>
#include <numeric>
#include <iterator>
#include <algorithm>

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

TTree *load_tree(const char *const file_name, const char *const tree_name);
const char *fasttrack_file_path = NULL;

using namespace std;

double get_std(std::vector<double>& d) {
	std::sort(std::begin(d), std::end(d));
	/* Remove outliers, determined by a generous cut of 2.5 * IQR */
	const double fence_size {2.5};

	double q1 {d[d.size() / 4]};
	double q3 {d[(d.size() * 3) / 4]};
	double iqr {q3 - q1};
	const double fence_min {q1 - fence_size * iqr};
	const double fence_max {q3 + fence_size * iqr};


	d.erase(std::remove_if(std::begin(d), std::end(d), [fence_min, fence_max](double x) {
		return !(fence_min <= x && x <= fence_max);
	}), end(d));

	const double total {accumulate(std::begin(d), std::end(d), 0.0, std::plus<double>())};
	const double mean {total / d.size()};
	double std {std::accumulate(std::begin(d), std::end(d), 0.0, [mean](double acc, double x) {
			return acc + std::pow(x - mean, 2);
	})};
	std /= d.size();
	std = std::sqrt(std);

	return std;
}

void Plot_Sigma()
{
	SetsPhenixStyle();
	gROOT->SetBatch(true);

	TTree *const tracks {load_tree(fasttrack_file_path, "tracks")};

	Long64_t ntracks {tracks->GetEntries()};
	Float_t gpx, gpy, gpz, px, py, pz;
	tracks->SetBranchAddress("gpx", &gpx);
	tracks->SetBranchAddress("gpy", &gpy);
	tracks->SetBranchAddress("gpz", &gpz);
	tracks->SetBranchAddress("px", &px);
	tracks->SetBranchAddress("py", &py);
	tracks->SetBranchAddress("pz", &pz);
	if (tracks->LoadTree(0) < 0)
		return;

	tracks->GetEntry(0);
	const double gmom {sqrt(gpx * gpx + gpy * gpy + gpz * gpz)};
	const double geta {0.5 *log((gmom + gpz) / (gmom - gpz))};
	const double gtheta {2 * atan(exp(-geta))};

	vector <double> moms;
	vector <double> thetas;
	vector<double> phis;
	for (Long64_t i {0}; i < ntracks; ++i) {
		if (tracks->LoadTree(i) < 0)
			break;

		tracks->GetEntry(i);
		if (px == -9999 || py == -9999 || pz == -9999) 
			continue;

		const double mom {sqrt(px * px + py * py + pz * pz)};
		const double eta {0.5 * log((mom + pz) / (mom - pz))};
		const double theta {2 * atan(exp(-eta))};
		const double gphi {atan2(gpy, gpx)};
		const double phi {atan2(py, px)};

		moms.push_back(mom);
		thetas.push_back(theta);
		phis.push_back(phi - gphi);

	}

	
	if (!moms.size())
		return;


	std::ofstream fout {"Momentum_Sigma.csv"};
	double std {get_std(moms)};
	if (std == std) // check for NaN
		fout << gmom << ',' << gtheta << ',' << std << '\n';
	fout.close();

	fout.open("Theta_Sigma.csv");
	std = get_std(thetas);
	if (std == std) // check for NaN
		fout << gmom << ',' << gtheta << ',' << std << '\n';
	fout.close();

	fout.open("Phi_Sigma.csv");
	std = get_std(phis);
	if (std == std) // check for NaN
		fout << gmom << ',' << gtheta << ',' << std << '\n';
	fout.close();

	gApplication->Terminate(0); 
}

TTree *load_tree(const char *const file_name, const char *const tree_name)
{
	return (TTree *) (new TFile(file_name, "READ"))->Get(tree_name);
}

int main(int argc, char *argv[])
{
	if (argc > 1) {
		fasttrack_file_path = argv[1];
		argc = 1;
	}

	TApplication app {"Sigma Plots", &argc, argv};
	Plot_Sigma();
	app.Run();
	return 0;
}
