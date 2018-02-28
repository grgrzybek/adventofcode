/*
 * day04.cc
 *
 *  Created on: Feb 27, 2018
 *      Author: ggrzybek
 */

#include <cstdlib>
#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
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
	set<string> tokens1, tokens2;
	int valid_count1 = 0;
	int valid_count2 = 0;

	while (*input) {
		getline(*input, line);
		istringstream iss(line);
		tokens1.clear();
		tokens2.clear();
//		copy(istream_iterator<string>(iss), istream_iterator<string>(), inserter(tokens1, tokens1.begin()));
		int words = 0;
		for (istream_iterator<string> t = istream_iterator<string>(iss); t != istream_iterator<string>(); t++, words++) {
			tokens1.insert(*t);
			string s = *t;
			sort(s.begin(), s.end());
			tokens2.insert(s);
		}
		if (words != 0 && tokens1.size() == words)
			valid_count1++;
		if (words != 0 && tokens2.size() == words)
			valid_count2++;
	}

	delete input;

	cout << "Answer 1: " << valid_count1 << endl;
	cout << "Answer 2: " << valid_count2 << endl;

	return EXIT_SUCCESS;
}

static void print_help(void) {
	cout << "Usage: " << program_name << " [OPTION]...\n";

	cout << "Day 04.\n\n";

	cout << "-h, --help      display this help and exit\n";
	cout << "-v, --version   display version information and exit\n\n";

	cout << "-f, --file=FILE puzzle input (file)\n\n";

	cout << "Report bugs to <" << PACKAGE_BUGREPORT << "." << endl;
}
