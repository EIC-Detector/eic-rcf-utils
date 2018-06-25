/* To compile this program, run:
 * g++ -std=c++11 -o merge-trees merge-trees.cpp `root-config --cflags --libs`
 * Usage: merge-trees [OPTIONS]... [FILE]...
 * Merge trees from specified FILEs into a single ROOT file.
 *
 * --out, --out=		path where to save merged trees
 * --tree, --tree=		which tree to read from files
 * --help			print this message
*/
#include <iostream>
#include <cstdlib>
#include <vector>
#include <getopt.h>

#include "TFile.h"
#include "TChain.h"

enum command_line_options {
	option_out = 1,
	option_tree,
	option_help,
};

int main(int argc, char *argv[])
{
	static const struct option long_options[] {
		{"out", required_argument, NULL, option_out}, 
		{"tree", required_argument, NULL, option_tree}, 
		{"help", no_argument, NULL, option_help}, 
		{0, 0, 0, 0}
	};

	const char *out = "Merged.root";
	std::vector < const char *>trees;

	for (;;) {
		int option_index = 0;

		int c =
		    getopt_long(argc, argv, "", long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
		case option_out:
			out = optarg;
			break;
		case option_tree:
			trees.push_back(optarg);
			break;
		case option_help:
			const char *const help_message =
			    "Usage: merge-trees [OPTIONS]... [FILE]...\n"
			    "Merge trees from specified FILEs into a single ROOT file.\n\n"
			    "--out, --out=\t\t\tpath where to save merged trees\n"
			    "--tree, --tree=\t\t\twhich tree to read from files\n"
			    "--help\t\t\t\tprint this message\n";
			std::cout << help_message;
			std::exit(EXIT_SUCCESS);
		}
	}

	if (optind == argc) {
		std::cerr << "Error: no files specified to merge.\n";
		std::exit(EXIT_FAILURE);
	}

	if (trees.size() == 0)
		trees.push_back("ntp_cluster");

	TFile *const f = new TFile(out, "RECREATE");

	for (size_t i = 0; i < trees.size(); ++i) {
		TChain *const t = new TChain(trees[i]);
		for (int j = optind; j < argc; ++j) {
			t->Add(argv[j]);
			std::
			    cout << "Added (" << trees[i] << "): " << argv[j] <<
			    '\n';
		}

		t->CloneTree(-1, "fast");
		delete t;
	}

	f->Write();

	std::exit(EXIT_SUCCESS);
}
