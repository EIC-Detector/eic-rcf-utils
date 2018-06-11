#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <dirent.h>
#include <regex>
#include <getopt.h>
#include <string>

enum command_line_options
{
	option_detector = 0,
};


void process_directory(const char *const detector, const char *const directory);

	int
main (int argc, char *argv[])
{
	static const struct option long_options[] {
		{"detector", required_argument, NULL, option_detector},
			{0, 0, 0, 0}
	};

	const char *detector = NULL;

	for (;;)
	{
		int option_index = 0;

		int c = getopt_long (argc, argv, "", long_options, &option_index);
		if (c == -1)
			break;

		switch (c)
		{
			case option_detector:
				detector = optarg;
				break;
		}
	}
	if (!detector) {
		std::fputs("Error: must specify a detector name.\n", stderr);
		std::exit(EXIT_FAILURE);
	}

	if (optind == argc)
		process_directory (detector, ".");

	for (int i = optind; i < argc; ++i) 
		process_directory (detector, argv[i]);
}

void process_directory(const char *const detector, const char *const directory)  {
	std::printf("Looking through: %s\n", directory);

	std::regex root_file_pattern {".*" + std::string (detector) + ".*"};
	DIR *cwd = opendir (directory);
	if (!cwd) {
		std::fputs("Error NULL\n", stderr);
		exit(EXIT_FAILURE);
	}
	struct dirent *d;


	while (d = readdir (cwd))
		if (std::regex_match (d->d_name, root_file_pattern))
			std::printf ("%s matches\n", d->d_name);
}

