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

	while (*input) {
		line.clear();
		getline(*input, line);
		if (line.length()) {
			string o1 = line.substr(0, 3);
			string o2 = line.substr(4, 3); // is unique
			data[o2] = new list<string*>;
			data[o2]->push_back(new string(o1)); // direct orbit of
		}
	}
	delete input;

//	cout << "Orbits " << data.size() << endl;

	int checked = 0;
	// collect orbits
	for (map<string, list<string*>*>::const_iterator e = data.cbegin(); e != data.end(); e++) {
//		const string *k = &(e->first);
		list<string*> *l = e->second;
//		cout << "checking \"" << *k << "\"" << endl;
		for (;;) {
			if (l) {
				l = data[*l->front()];
				if (l)
					e->second->push_back(l->front());
			} else {
				break;
			}
		}
		checked++;
	}


	int answer1 = 0;

	// calculate orbits
	for (map<string, list<string*>*>::const_iterator e = data.cbegin(); e != data.end(); e++) {
		if (e->second) {
			answer1 += e->second->size();
		}
	}

	// calculate YOU-SAN distance

	list<string*> *my_orbits = data["YOU"];
	list<string*> *santa_orbits = data["SAN"];

//	for (auto &my_orbit : *my_orbits) {
//		cout << "-" << *my_orbit;
//	}
//	cout << endl;
//	for (auto &santa_orbit : *santa_orbits) {
//		cout << "-" << *santa_orbit;
//	}
//	cout << endl;

	string *base = NULL;
	for (auto &my_orbit : *my_orbits) {
		for (auto &san_orbit : *santa_orbits) {
			if (*my_orbit == *san_orbit) {
				base = my_orbit;
			}
			if (base)
				break;
		}
		if (base)
			break;
	}
	cout << "common: " << *base << endl;

	int answer2 = 0;
	list<string*>::const_iterator it;

	it = my_orbits->cbegin();
	for (; it != my_orbits->cend(); it++) {
		cout << "  " << **it << endl;
		if (**it == *base)
			break;
		cout << "  " << **it << endl;
		answer2++;
	}
	it = santa_orbits->cbegin();
	for (; it != santa_orbits->cend(); it++) {
		cout << "  " << **it << endl;
		if (**it == *base)
			break;
		answer2++;
	}

	// clean
	for (map<string, list<string*>*>::const_iterator e = data.cbegin(); e != data.end(); e++) {
//		cout << "k: " << e->first << " -> " << e->second->size() << endl;
		if (e->second) {
//			cout << e->first;
			for (list<string*>::const_iterator it = e->second->cbegin(); it != e->second->end(); it++) {
//				cout << " - " << **it;
				// free only first elements
				delete *it;
				break;
			}
//			cout << endl;
			delete e->second;
		}
	}

	cout << "Answer 1: " << answer1 << endl;
	cout << "Answer 2: " << answer2 << endl;

	return EXIT_SUCCESS;
}

static void print_help(void) {
	cout << "Usage: " << program_name << " [OPTION]...\n";

	cout << "Day 06.\n\n";

	cout << "-h, --help      display this help and exit\n";
	cout << "-v, --version   display version information and exit\n\n";

	cout << "-f, --file=FILE puzzle input (file)\n\n";

	cout << "Report bugs to <" << PACKAGE_BUGREPORT << "." << endl;
}
