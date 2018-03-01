/*
 * day05.cc
 *
 *  Created on: Mar 01, 2018
 *      Author: ggrzybek
 */

#include <cstdlib>
#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iosfwd>

#include <getopt.h>
#include <config.h>

using namespace std;

static const char *program_name;

static const struct option longopts[] = {
		{ "help", no_argument, NULL, 'h' },
		{ "version", no_argument, NULL, 'v' },
		{ "file", required_argument, NULL, 'f' }
};

static void print_help(void);

int main(int argc, char *argv[]) {

	program_name = argv[0];
	ios::sync_with_stdio(false);

	int optc, lose = 0;
	ifstream *input = NULL;

	while ((optc = getopt_long(argc, argv, "hvf:", longopts, NULL)) != -1) {
		switch (optc) {
		case 'f':
			input = new ifstream(optarg, ios_base::in);
			break;
		case 'h':
			print_help();
			return EXIT_SUCCESS;
		case 'v':
			printf("%s\n", PACKAGE_STRING);
			return EXIT_SUCCESS;
		default:
			lose = 1;
		}
	}

	if (lose || optind < argc || !input) {
		if (optind < argc) {
			cerr << program_name << ": extra operand: " << argv[optind] << endl;
		}
		cerr << "Try `" << program_name << " --help' for more information." << endl;
		return EXIT_FAILURE;
	}

	string line;
	vector<int> tab;

	while (*input) {
		getline(*input, line);
		if (line.length())
			tab.push_back(strtol(line.c_str(), NULL, 10));
	}

	delete input;
	vector<int> tab2(tab);

	int steps1 = 0, steps2 = 0;
	int index = 0;
	while (true) {
		int ni = index + tab[index];
		steps1++;
		tab[index]++;
		if (ni < 0 || ni >= tab.size()) {
			break;
		}
		index = ni;
	}

	tab.swap(tab2);
	index = 0;
	while (true) {
		int ni = index + tab[index];
		steps2++;
		if (tab[index] >= 3) {
			tab[index]--;
		} else {
			tab[index]++;
		}
		if (ni < 0 || ni >= tab.size()) {
			break;
		}
		index = ni;
	}

	cout << "Answer 1: " << steps1 << endl;
	cout << "Answer 2: " << steps2 << endl;

	return EXIT_SUCCESS;
}

static void print_help(void) {
	cout << "Usage: " << program_name << " [OPTION]...\n";

	cout << "Day 05.\n\n";

	cout << "-h, --help      display this help and exit\n";
	cout << "-v, --version   display version information and exit\n\n";

	cout << "-f, --file=FILE puzzle input (file)\n\n";

	cout << "Report bugs to <" << PACKAGE_BUGREPORT << "." << endl;
}
