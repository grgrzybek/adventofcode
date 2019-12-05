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

	if (lose || optind < argc - 1 || !input) {
		if (optind < argc) {
			cerr << program_name << ": extra operand: " << argv[optind] << endl;
		}
		cerr << "Try `" << program_name << " --help' for more information." << endl;
		return EXIT_FAILURE;
	}

	int input_value = ::atoi(argv[optind]);

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

	ostringstream output;

//	state(numbers);

	vector<int>::size_type pos = 0;
	bool end = false;
	while (pos < numbers.size()) {
//		cout << "pos: " << pos << endl;
		int op = numbers[pos];
		int instruction = op % 100;
		int m1 = op % 1000 / 100;
		int m2 = op % 10000 / 1000;

		int arg1 = 0, arg2 = 0;

		switch (instruction) {
			case 1: // ADD
				arg1 = m1 ? numbers[pos+1] : numbers[numbers[pos+1]];
				arg2 = m2 ? numbers[pos+2] : numbers[numbers[pos+2]];
				numbers[numbers[pos+3]] = arg1 + arg2;
				pos += 4;
				break;
			case 2: // MUL
				arg1 = m1 ? numbers[pos+1] : numbers[numbers[pos+1]];
				arg2 = m2 ? numbers[pos+2] : numbers[numbers[pos+2]];
				numbers[numbers[pos+3]] = arg1 * arg2;
				pos += 4;
				break;
			case 3: // READ
				numbers[numbers[pos+1]] = input_value;
				pos += 2;
				break;
			case 4: // WRITE
				output << " " << (m1 ? numbers[pos+1] : numbers[numbers[pos+1]]);
				pos += 2;
				break;
			case 5: // jump-if-true
				if (m1 ? numbers[pos+1] : numbers[numbers[pos+1]])
					pos = m2 ? numbers[pos+2] : numbers[numbers[pos+2]];
				else
					pos += 3;
				break;
			case 6: // jump-if-false
				if (!(m1 ? numbers[pos+1] : numbers[numbers[pos+1]]))
					pos = m2 ? numbers[pos+2] : numbers[numbers[pos+2]];
				else
					pos += 3;
				break;
			case 7: // less-than
				arg1 = m1 ? numbers[pos+1] : numbers[numbers[pos+1]];
				arg2 = m2 ? numbers[pos+2] : numbers[numbers[pos+2]];
				numbers[numbers[pos+3]] = arg1 < arg2 ? 1 : 0;
				pos += 4;
				break;
			case 8: // equals
				arg1 = m1 ? numbers[pos+1] : numbers[numbers[pos+1]];
				arg2 = m2 ? numbers[pos+2] : numbers[numbers[pos+2]];
				numbers[numbers[pos+3]] = arg1 == arg2 ? 1 : 0;
				pos += 4;
				break;
			case 99:
				end = true;
				break;
		}
		if (end) {
			break;
		}
	}

	cout << "Answer 1: " << output.str() << endl;

	return EXIT_SUCCESS;
}

static void print_help(void) {
	cout << "Usage: " << program_name << " [OPTION]... <input_value>\n";

	cout << "Day 05.\n\n";

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
