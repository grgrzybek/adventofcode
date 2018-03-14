/*
 * day06.cc
 *
 *  Created on: Mar 14, 2018
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
#include <set>
#include <unordered_map>
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
static string to_string(vector<int>);

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

	vector<int> tab;

	if (*input) {
		string line, v;
		getline(*input, line);
		if (line.length()) {
			istringstream tokens(line);
			while (tokens >> v)
				tab.push_back(strtol(v.c_str(), NULL, 10));
		}
	}

	delete input;
	vector<int> tab2(tab); // to reuse it in part2

	set<string> collect;
	int steps1 = 0;

	while (true) {
		string v = to_string(tab);
		set<string>::size_type c = collect.count(v);
		if (c)
			break;
		steps1++;
		collect.insert(v);

		// find max
		int max = 0;
		int idx = -1;
		for (auto t=tab.cbegin(); t<tab.cend(); t++) {
			if (max < *t) {
				max = *t;
				idx = t - tab.cbegin();
			}
		}

		// redustribute
		tab[idx] = 0;
		while (max > 0) {
			idx++;
			idx %= tab.size();
			tab[idx]++;
			max--;
		}
	}

	tab.swap(tab2);

	unordered_map<string, int> collect2;
	int steps2 = 0, count = 0;

	while (true) {
		string v = to_string(tab);
		set<string>::size_type c = collect2.count(v);
		if (c) {
			steps2 = count - collect2[v];
			break;
		}
		collect2[v] = count;
		count++;

		// find max
		int max = 0;
		int idx = -1;
		for (auto t=tab.cbegin(); t<tab.cend(); t++) {
			if (max < *t) {
				max = *t;
				idx = t - tab.cbegin();
			}
		}

		// redustribute
		tab[idx] = 0;
		while (max > 0) {
			idx++;
			idx %= tab.size();
			tab[idx]++;
			max--;
		}
	}

	cout << "Answer 1: " << steps1 << endl;
	cout << "Answer 2: " << steps2 << endl;

	return EXIT_SUCCESS;
}

static string to_string(vector<int> tab) {
	// C++17
//	for(auto &item : as_const(tab)) {
//		cout << item << endl;
//	}
	stringstream ss;
	// C++14
	for (auto t = tab.cbegin(); t != tab.end(); t++) {
		ss << "|" << *t;
	}

	return ss.str();
	// Classic
//	for (vector<int>::const_iterator t = tab.cbegin(); t != tab.end(); t++) {
//		cout << (*t + 1) << endl;
//	}
}

static void print_help(void) {
	cout << "Usage: " << program_name << " [OPTION]...\n";

	cout << "Day 06.\n\n";

	cout << "-h, --help      display this help and exit\n";
	cout << "-v, --version   display version information and exit\n\n";

	cout << "-f, --file=FILE puzzle input (file)\n\n";

	cout << "Report bugs to <" << PACKAGE_BUGREPORT << "." << endl;
}
