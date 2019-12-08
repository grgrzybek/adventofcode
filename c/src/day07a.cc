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
#include <iomanip>
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
static void calculate_combinations(int, int, int, int, bool[], int[], vector<int*>*);
static int run(vector<int> &numbers, int *inputs);
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

	string line;

	vector<int> numbers;

	while (*input) {
		line.clear();
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

	//	state(numbers);

	vector<int*>* combinations = new vector<int*>;

	int SIZE = 5;
	bool slots[SIZE]; // on stack
	int it[SIZE]; // on stack
	for (int i=0; i<SIZE; i++) {
		slots[i] = false;
		it[i] = -1;
	}
	calculate_combinations(0, SIZE-1, 0, SIZE, slots, it, combinations);

	cout << "combinations 1: " << combinations->size() << endl;

//	combinations->clear();
//	int tab[] { 1,0,4,3,2 };
//	combinations->push_back(tab);

	int max_output = 0;
	for (vector<int*>::const_iterator it = combinations->cbegin(); it != combinations->cend(); it++) {
		// a copy of a program has to be run once for each combination of phase settings
		numbers.clear();
		numbers.assign(numbers_copy.cbegin(), numbers_copy.cend());

		int output = 0;
		for (int thruster = 0; thruster < SIZE; thruster++) {
			int inputs[2] = { (*it)[thruster], output };
			output = run(numbers, inputs);
		}
		if (output > max_output)
			max_output = output;
	}

	for (vector<int*>::const_iterator it = combinations->cbegin(); it != combinations->cend(); it++) {
		delete *it;
	}
	delete combinations;

	cout << "Answer 1: " << max_output << endl;

	return EXIT_SUCCESS;
}

static void print_help(void) {
	cout << "Usage: " << program_name << " [OPTION]... <input_value>\n";

	cout << "Day 07a.\n\n";

	cout << "-h, --help      display this help and exit\n";
	cout << "-v, --version   display version information and exit\n\n";

	cout << "-f, --file=FILE puzzle input (file)\n\n";

	cout << "Report bugs to <" << PACKAGE_BUGREPORT << "." << endl;
}

void calculate_combinations(int from, int to, int start, int size, bool slots[], int it[], vector<int*>* result) {
	bool slots2[size]; // on stack
	for (int i=from; i<=to; i++) {
		for (int j=from; j<=to; j++) {
			slots2[j-from] = slots[j-from];
		}
		slots2[i-from] = true;
		if (!slots[i-from]) {
//			for (int i=0; i<start; i++)
//				cout << " ";
			// if number was not yet used
//			cout << "n: " << i << ", start: " << start << endl;
			int *tab = new int[size];
			for (int i=0; i<start; i++) {
				// values from previous iteration
				tab[i] = it[i];
			}
			tab[start] = i;
			if (start == size-1) {
				// end of calculations
				result->push_back(tab);
//				for (int i=0; i<size; i++) {
//					cout << tab[i];
//				}
//				cout << endl;
			} else {
				calculate_combinations(from, to, start+1, size, slots2, tab, result);
				delete[] tab;
			}
		}
	}
}

int run(vector<int> &numbers, int *inputs) {
	vector<int>::size_type pos = 0;
	int input = 0; // will be 0 for phase setting and 1 for input value
	int output = 0;
	bool end = false;
	while (pos < numbers.size()) {
	//				state(numbers);
		//		cout << "pos: " << pos << endl;
		int op = numbers[pos];
		int instruction = op % 100;
		int m1 = op % 1000 / 100;
		int m2 = op % 10000 / 1000;

		int arg1 = 0, arg2 = 0;

		switch (instruction) {
		case 1: // ADD
			arg1 = m1 ? numbers[pos + 1] : numbers[numbers[pos + 1]];
			arg2 = m2 ? numbers[pos + 2] : numbers[numbers[pos + 2]];
	//					if (m1)
	//						cout << arg1;
	//					else
	//						cout << arg1 << "(" << numbers[pos + 1] << ")";
	//					cout << " + ";
	//					if (m2)
	//						cout << arg2;
	//					else
	//						cout << arg2 << "(" << numbers[pos + 2] << ")";
	//					cout << " -> " << numbers[pos + 3] << endl;
			numbers[numbers[pos + 3]] = arg1 + arg2;
			pos += 4;
			break;
		case 2: // MUL
			arg1 = m1 ? numbers[pos + 1] : numbers[numbers[pos + 1]];
			arg2 = m2 ? numbers[pos + 2] : numbers[numbers[pos + 2]];
	//					if (m1)
	//						cout << arg1;
	//					else
	//						cout << arg1 << "(" << numbers[pos + 1] << ")";
	//					cout << " * ";
	//					if (m2)
	//						cout << arg2;
	//					else
	//						cout << arg2 << "(" << numbers[pos + 2] << ")";
	//					cout << " -> " << numbers[pos + 3] << endl;
			numbers[numbers[pos + 3]] = arg1 * arg2;
			pos += 4;
			break;
		case 3: // READ
//			cout << "READ " << inputs[input] << " -> " << numbers[pos + 1] << endl;
			numbers[numbers[pos + 1]] = inputs[input++];
			pos += 2;
			break;
		case 4: // WRITE
			output = m1 ? numbers[pos + 1] : numbers[numbers[pos + 1]];
//			cout << "O: " << inputs[1] << endl;
			pos += 2;
			break;
		case 5: // jump-if-true
			if (m1 ? numbers[pos + 1] : numbers[numbers[pos + 1]])
				pos = m2 ? numbers[pos + 2] : numbers[numbers[pos + 2]];
			else
				pos += 3;
			break;
		case 6: // jump-if-false
			if (!(m1 ? numbers[pos + 1] : numbers[numbers[pos + 1]]))
				pos = m2 ? numbers[pos + 2] : numbers[numbers[pos + 2]];
			else
				pos += 3;
			break;
		case 7: // less-than
			arg1 = m1 ? numbers[pos + 1] : numbers[numbers[pos + 1]];
			arg2 = m2 ? numbers[pos + 2] : numbers[numbers[pos + 2]];
			numbers[numbers[pos + 3]] = arg1 < arg2 ? 1 : 0;
			pos += 4;
			break;
		case 8: // equals
			arg1 = m1 ? numbers[pos + 1] : numbers[numbers[pos + 1]];
			arg2 = m2 ? numbers[pos + 2] : numbers[numbers[pos + 2]];
			numbers[numbers[pos + 3]] = arg1 == arg2 ? 1 : 0;
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
	return output;
}

void state(vector<int> &numbers) {
	int i = 0;
	for (auto n = numbers.cbegin(); n != numbers.end(); n++) {
		if (n != numbers.cbegin())
			cout << "|";
		cout << std::setw(5) << i++;
	}
	cout << endl;
	for (auto n = numbers.cbegin(); n != numbers.end(); n++) {
		if (n != numbers.cbegin())
			cout << "|";
		cout << std::setw(5) << *n;
	}
	cout << endl;
}
