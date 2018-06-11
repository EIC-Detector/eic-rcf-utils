#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <dirent.h>
#include <regex>
#include <getopt.h>
#include <string>

#include "TFile.h"
#include "TChain.h"

enum command_line_options {
	option_detector = 1,
	option_out,
	option_tree,
};

void process_directory(TChain *const t, const char *const detector, const char *const directory);

int main(int argc, char *argv[])
{
	static const struct option long_options[] {
		{"detector", required_argument, NULL, option_detector}, 
		{"out", required_argument, NULL, option_out}, 
		{"tree", required_argument, NULL, option_tree}, 
		{0, 0, 0, 0}
	};

	const char *detector = "cemc";
	const char *out = "Merged.root";
	const char *tree = "ntp_cluster";

	for (;;) {
		int option_index = 0;

		int c =
		    getopt_long(argc, argv, "", long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
		case option_detector:
			detector = optarg;
			break;
		case option_out:
			out = optarg;
			break;
		case option_tree:
			tree = optarg;
			break;
		}
	}

	TFile *const f = new TFile(out, "RECREATE");
	TChain *const t = new TChain(tree);

	if (optind == argc)
		process_directory(t, detector, ".");

	for (int i = optind; i < argc; ++i)
		process_directory(t, detector, argv[i]);
}

void process_directory(TChain *const t, const char *const detector, const char *const directory)
{

	std::regex root_file_pattern {".*" + std::string(detector) + ".*"};

	DIR *const dir = opendir(directory);
	if (!dir) {
		std::fprintf(stderr, "Error: Could not open %s\n", directory);
		return;
	}

	std::printf("Looking through: %s\n", directory);
	const struct dirent *d;
	while (d = readdir(dir))
		if (std::regex_match(d->d_name, root_file_pattern)) {
			std::printf("Added: %s\n", d->d_name);
			t->Add(d->d_name);
		}

	closedir(dir);
}
