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
static void state(long long *numbers, int size) __attribute__ ((unused));
static void ensure_capacity(long long **numbers, size_t *mem_size, int idx);
static long long val(int mod, long long *numbers, int pos, int base);

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

	size_t mem_size = 0;
	long long *numbers = NULL;

	int idx = 0;
	long long v = 0LL;
	while (*input) {
		line.clear();
		getline(*input, line);
		if (line.length()) {
			string::size_type pos1 = 0;
			string::size_type pos2 = 0;
			pos2 = line.find(",", pos1);
			while ((pos2 = line.find(",", pos1)) != string::npos) {
				v = std::stoll(line.substr(pos1, pos2 - pos1));
				ensure_capacity(&numbers, &mem_size, idx);
				numbers[idx++] = v;
				pos1 = pos2+1;
			}
			v = std::stoll(line.substr(pos1, pos2 - pos1));
			ensure_capacity(&numbers, &mem_size, idx);
			numbers[idx++] = v;
		}
	}

	delete input;

	ostringstream output;

//	state(numbers, idx);

	vector<int>::size_type pos = 0;
	bool end = false;
	int base = 0;

	for (;;) {
		cout << endl << "pos: " << pos << ", base: " << base << endl;
		int op = numbers[pos];
		int instruction = op % 100;
		int m1 = op % 1000 / 100;
		int m2 = op % 10000 / 1000;
		int m3 = op % 100000 / 10000;

		long long arg1 = 0, arg2 = 0;

		switch (instruction) {
			case 1: // ADD
				cout << "================== ADD " << numbers[pos] << "," << numbers[pos+1] << "," << numbers[pos+2] << "," << numbers[pos+3] << endl;
				arg1 = val(m1, numbers, pos+1, base);
				arg2 = val(m2, numbers, pos+2, base);
				cout << "    " << arg1 << "+" << arg2 << " -> " << numbers[pos+3] + (m3 == 2 ? base : 0) << endl;
				ensure_capacity(&numbers, &mem_size, numbers[pos+3] + (m3 == 2 ? base : 0));
				numbers[numbers[pos+3] + (m3 == 2 ? base : 0)] = arg1 + arg2;
				pos += 4;
				break;
			case 2: // MUL
				cout << "================== MUL " << numbers[pos] << "," << numbers[pos+1] << "," << numbers[pos+2] << "," << numbers[pos+3] << endl;
				arg1 = val(m1, numbers, pos+1, base);
				arg2 = val(m2, numbers, pos+2, base);
				cout << "    " << arg1 << "*" << arg2 << " -> " << numbers[pos+3] + (m3 == 2 ? base : 0) << endl;
				ensure_capacity(&numbers, &mem_size, numbers[pos+3] + (m3 == 2 ? base : 0));
				numbers[numbers[pos+3] + (m3 == 2 ? base : 0)] = arg1 * arg2;
				pos += 4;
				break;
			case 3: // READ
				cout << "================== READ " << numbers[pos] << "," << numbers[pos+1] << endl;
				arg1 = val(m1, numbers, pos+1, base);
				if (m1 == 0) {
					ensure_capacity(&numbers, &mem_size, numbers[pos+1]);
					cout << "    " << "read: " << input_value << " -> " << numbers[pos+1] << endl;
					numbers[numbers[pos+1]] = input_value;
				} else if (m1 == 2) {
					ensure_capacity(&numbers, &mem_size, numbers[pos+1] + base);
					cout << "    " << "read: " << input_value << " -> " << numbers[pos+1] + base << endl;
					numbers[numbers[pos+1] + base] = input_value;
				}
				pos += 2;
				break;
			case 4: // WRITE
				cout << "================== WRITE " << numbers[pos] << "," << numbers[pos+1] << endl;
				arg1 = val(m1, numbers, pos+1, base);
				cout << "    " << "write: " << arg1 << endl;
				output << " " << arg1;
				pos += 2;
				break;
			case 5: // jump-if-true
				cout << "================== JUMP-IF-TRUE " << numbers[pos] << "," << numbers[pos+1] << "," << numbers[pos+2] << endl;
				arg1 = val(m1, numbers, pos+1, base);
				arg2 = val(m2, numbers, pos+2, base);
				if (arg1) {
					cout << "    " << arg1 << ": pos = " << arg2	 << endl;
					pos = (int)arg2;
				} else {
					pos += 3;
					cout << "    " << arg1 << ": pos+=3 = " << pos << endl;
				}
				break;
			case 6: // jump-if-false
				cout << "================== JUMP-IF-FALSE " << numbers[pos] << "," << numbers[pos+1] << "," << numbers[pos+2] << endl;
				arg1 = val(m1, numbers, pos+1, base);
				arg2 = val(m2, numbers, pos+2, base);
				if (!arg1) {
					cout << "    !" << arg1 << ": pos = " << arg2	 << endl;
					pos = (int)arg2;
				} else {
					pos += 3;
					cout << "    " << arg1 << ": pos+=3 = " << pos << endl;
				}
				break;
			case 7: // less-than
				cout << "================== LESS-THAN " << numbers[pos] << "," << numbers[pos+1] << "," << numbers[pos+2] << "," << numbers[pos+3] << endl;
				arg1 = val(m1, numbers, pos+1, base);
				arg2 = val(m2, numbers, pos+2, base);
				ensure_capacity(&numbers, &mem_size, numbers[pos+3] + (m3 == 2 ? base : 0));
				cout << "    " << arg1 << "<" << arg2 << " " << (arg1 < arg2 ? 1 : 0) << " -> " << numbers[pos+3] + (m3 == 2 ? base : 0) << endl;
				numbers[numbers[pos+3] + (m3 == 2 ? base : 0)] = arg1 < arg2 ? 1 : 0;
				pos += 4;
				break;
			case 8: // equals
				cout << "================== EQUALS " << numbers[pos] << "," << numbers[pos+1] << "," << numbers[pos+2] << "," << numbers[pos+3] << endl;
				arg1 = val(m1, numbers, pos+1, base);
				arg2 = val(m2, numbers, pos+2, base);
				ensure_capacity(&numbers, &mem_size, numbers[pos+3] + (m3 == 2 ? base : 0));
				cout << "    " << arg1 << "==" << arg2 << " " << (arg1 == arg2 ? 1 : 0) << " -> " << numbers[pos+3] + (m3 == 2 ? base : 0) << endl;
				numbers[numbers[pos+3] + (m3 == 2 ? base : 0)] = arg1 == arg2 ? 1 : 0;
				pos += 4;
				break;
			case 9: // change base
				cout << "================== BASE " << numbers[pos] << "," << numbers[pos+1] << endl;
				arg1 = val(m1, numbers, pos+1, base);
				cout << "    " << "base " << base << "+" << arg1 << endl;
				base += (int)arg1;
				pos += 2;
				break;
			case 99:
				cout << "================== END " << numbers[pos] << endl;
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

	cout << "Day 09.\n\n";

	cout << "-h, --help      display this help and exit\n";
	cout << "-v, --version   display version information and exit\n\n";

	cout << "-f, --file=FILE puzzle input (file)\n\n";

	cout << "Report bugs to <" << PACKAGE_BUGREPORT << "." << endl;
}

void ensure_capacity(long long **numbers, size_t *mem_size, int idx) {
//	cout << "numbers: " << *numbers << endl;
//	cout << "mem_size: " << *mem_size << endl;
//	cout << "idx: " << idx << endl;
	if (!*numbers) {
		// for simplicity - one chunk
		cout << "expanding from " << *mem_size << " to " << 4096 << endl;
		*numbers = (long long *)::calloc((size_t)4096, sizeof(long long));
		*mem_size = 4096;
	}
	if ((size_t)idx >= *mem_size) {
		int chunks = idx / 4096 + 1;
		long long *new_numbers = (long long *)::calloc(chunks * 4096, sizeof(long long));
		::memcpy((void *)new_numbers, (const void *)numbers, *mem_size);
		cout << "expanding from " << *mem_size << " to " << chunks*4096 << " (idx=" << idx << ")" << endl;
		*mem_size = chunks * 4096;
		::free(*numbers);
		*numbers = new_numbers;
	}
}

long long val(int mod, long long *numbers, int pos, int base) {
	switch (mod) {
	case 0: // absolute
		return numbers[numbers[pos]];
	case 1: // immediate
		return numbers[pos];
	case 2: // relative
		cout << "          - taking from " << (numbers[pos] + base) << endl;
		return numbers[numbers[pos] + base];
	default:
		return 0LL;
	}
}

void state(long long *numbers, int size) {
	for (int i=0; i<size; i++) {
		if (i != 0)
			cout << "|";
		cout << numbers[i];
	}
	cout << endl;
}
