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

#include <vector>

#include <getopt.h>
#include <config.h>

using namespace std;

static const char *program_name;

static const struct option longopts[] = {
		{ "help", no_argument, NULL, 'h' },
		{ "version", no_argument, NULL, 'v' },
};

static void print_help(void);

int main(int argc, char *argv[]) {

	program_name = argv[0];

	int optc = 0;

	// my input: 145852-616942

	while ((optc = getopt_long(argc, argv, "hv", longopts, NULL)) != -1) {
		switch (optc) {
		case 'h':
			print_help();
			return EXIT_SUCCESS;
		case 'v':
			printf("%s\n", PACKAGE_STRING);
			return EXIT_SUCCESS;
		}
	}

	int n1 = ::atoi(argv[optind++]);
	int n2 = ::atoi(argv[optind++]);

	int possible_passwords1 = 0;
	int possible_passwords2 = 0;

	for (int n = n1; n <= n2; n++) {
		string s = std::to_string(n);
		const char *tab = s.c_str();
		bool ok1 = true;
		bool ok2 = false;
		bool ok3 = false;
		for (int i = 0; i < 6; i++) {
			if (i < 5 && tab[i] > tab[i+1])
				ok1 = false;
		}
		if (ok1) {
			int i = 0;
			for (; i < 5; i++) {
				if (tab[i] == tab[i+1]) {
					ok2 = true;
					if (!ok3) {
						ok3 = true;
						if (i < 4) {
							if (tab[i] == tab[i+2]) {
								// found group of more than 2, move to different value
								ok3 = false;
								int add = 2;
								while (i+add < 6 && tab[i] == tab[i+add])
									add++;
								i += add;
								i--;
							}
						}
					}
				}
			}
		}
		if (ok1 && ok2) {
			possible_passwords1++;
		}
		if (ok1 && ok3)
			possible_passwords2++;
	}

	cout << "Answer 1: " << possible_passwords1 << endl;
	cout << "Answer 2: " << possible_passwords2 << endl;

	return EXIT_SUCCESS;
}

static void print_help(void) {
	cout << "Usage: " << program_name << " [OPTION]...\n";

	cout << "Day 04.\n\n";

	cout << "-h, --help      display this help and exit\n";
	cout << "-v, --version   display version information and exit\n\n";

	cout << "Report bugs to <" << PACKAGE_BUGREPORT << "." << endl;
}
