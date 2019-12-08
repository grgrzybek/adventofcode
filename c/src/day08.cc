/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <istream>
#include <limits>
#include <ostream>
#include <fstream>
#include <sstream>
#include <string>

#include <list>
#include <map>
#include <vector>

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
	map<string, list<string*>*> data;

	const int W = 25;
	const int H = 6;

	vector<int*> image;
	image.push_back(new int[W*H]);

	int pixel = 0;
	while (*input) {
		line.clear();
		getline(*input, line);
		if (line.length()) {
			const char *l = line.c_str();
			for (const char *p=l; p<l+line.length(); p++) {
				vector<int*>::size_type layer_number = pixel/(W*H) + 1;
//				cout << "layer: " << layer_number << endl;;
				int *layer = NULL;
				if (image.size() < layer_number) {
					image.push_back(new int[W*H]);
				}
				layer = image[layer_number - 1];
				layer[pixel % (W*H)] = *p - '0';
				pixel++;
			}
		}
	}
	delete input;

	int min_zeros = numeric_limits<int>().max();
	int min_ones = 0;
	int min_twos = 0;

	for (auto &l : image) {
		int zeros = 0;
		int ones = 0;
		int twos = 0;
		for (int p=0; p<W*H; p++) {
			if (l[p] == 0)
				zeros++;
			if (l[p] == 1)
				ones++;
			if (l[p] == 2)
				twos++;
//			cout << l[p];
		}
//		cout << endl;
//		cout << "0: " << zeros << ", 1: " << ones << ", 2: " << twos << endl;
		if (min_zeros > zeros) {
			min_zeros = zeros;
			min_ones = ones;
			min_twos = twos;
		}
		delete l;
	}

	int final_image[W*H];
	for (int p=0; p<W*H; p++) {
		final_image[p] = 1;
	}
	for (vector<int*>::const_reverse_iterator it = image.rbegin(); it != image.rend(); it++) {
		for (int p=0; p<W*H; p++) {
			if ((*it)[p] < 2)
				final_image[p] = (*it)[p];
		}
	}
	for (int p=01; p<=W*H; p++) {
		switch (final_image[p-1]) {
		case 0:
			cout << '.';
			break;
		case 1:
			cout << '#';
			break;
		case 2:
			cout << ' ';
			break;
		}
		if (p % W == 0)
			cout << endl;
	}

	cout << "Answer 1: " << min_ones*min_twos << endl;

	return EXIT_SUCCESS;
}

static void print_help(void) {
	cout << "Usage: " << program_name << " [OPTION]...\n";

	cout << "Day 08.\n\n";

	cout << "-h, --help      display this help and exit\n";
	cout << "-v, --version   display version information and exit\n\n";

	cout << "-f, --file=FILE puzzle input (file)\n\n";

	cout << "Report bugs to <" << PACKAGE_BUGREPORT << "." << endl;
}
