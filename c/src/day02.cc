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
#include <ostream>
#include <fstream>
#include <sstream>
#include <string>

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
static void state(vector<int> &numbers) __attribute__ ((unused));

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

	vector<int> numbers;

	while (*input) {
		getline(*input, line);
		if (line.length()) {
			string::size_type pos1 = 0;
			string::size_type pos2 = 0;
			pos2 = line.find(",", pos1);
			while ((pos2 = line.find(",", pos1)) != string::npos) {
				numbers.push_back(std::stoi(line.substr(pos1, pos2 - pos1)));
				pos1 = pos2+1;
			}
			numbers.push_back(std::stoi(line.substr(pos1, line.length())));
		}
	}

	delete input;

	vector<int> numbers_copy(numbers);

	numbers[1] = 12; // noun
	numbers[2] = 2;  // verb

	int pos = 0;
	int end = 0;
	for (;;) {
		switch (numbers[pos]) {
			case 1:
				numbers[numbers[pos+3]] = numbers[numbers[pos+1]] + numbers[numbers[pos+2]];
				break;
			case 2:
				numbers[numbers[pos+3]] = numbers[numbers[pos+1]] * numbers[numbers[pos+2]];
				break;
			case 99:
				end = 1;
				break;
		}
		pos += 4;
		if (end) {
			break;
		}
	}

	int answer1 = numbers[0];

	int n = 0;
	int v = 0;
	end = 0;
	for (;;) {
		int end2 = 0;
		for (n = 0; n <= 99 && !end2; n++) {
			cout << "noun: " << n << endl;
			for (v = 0; v <= 99 && !end2; v++) {
				numbers.assign(numbers_copy.begin(), numbers_copy.end());
				numbers[1] = n; // noun
				numbers[2] = v; // verb
				int pos = 0;
				int end = 0;
				for (;;) {
					switch (numbers[pos]) {
						case 1:
							numbers[numbers[pos+3]] = numbers[numbers[pos+1]] + numbers[numbers[pos+2]];
							break;
						case 2:
							numbers[numbers[pos+3]] = numbers[numbers[pos+1]] * numbers[numbers[pos+2]];
							break;
						case 99:
							end = 1;
							break;
					}
					pos += 4;
					if (end) {
						break;
					}
				}
				if (numbers[0] == 19690720) {
					n--;
					v--;
					end2 = 1;
				}
			}
		}
		if (end2) {
			break;
		}
	}

	int answer2 = (100 * n) + v;

	cout << "Answer 1: " << answer1 << endl;
	cout << "Answer 2: " << answer2 << endl;

	return EXIT_SUCCESS;
}

static void print_help(void) {
	cout << "Usage: " << program_name << " [OPTION]...\n";

	cout << "Day 02.\n\n";

	cout << "-h, --help      display this help and exit\n";
	cout << "-v, --version   display version information and exit\n\n";

	cout << "-f, --file=FILE puzzle input (file)\n\n";

	cout << "Report bugs to <" << PACKAGE_BUGREPORT << "." << endl;
}

void state(vector<int> &numbers) {
	for (auto n = numbers.cbegin(); n != numbers.end(); n++) {
		if (n != numbers.cbegin())
			cout << "|";
		cout << *n;
	}
	cout << endl;
}
