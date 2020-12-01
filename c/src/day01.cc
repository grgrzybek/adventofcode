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

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
	aoc2020::Options options("Day 01", argc, argv);
	if (!options.check())
		return options.result();

	cout << "Starting " << options.program_name << endl;

	ifstream *input = options.file();

	vector<int> values;

	string line;
	while (getline(*input, line)) {
		aoc2020::trim(line);
		int v = 0;

		// number per line
		istringstream iss(line);
		iss >> v;
		values.push_back(v);
	}

	// part 1

	int idx1 = 0;
	int idx2, idx3;
	int n1a = 0, n1b = 0;
	for (auto v1 = values.begin(); v1 != values.end(); v1++) {
		idx2 = 0;
		for (auto v2 = values.begin(); v2 != values.end(); v2++) {
			if (idx1 != idx2) {
				if (*v1 + *v2 == 2020) {
					n1a = *v1;
					n1b = *v2;
					break;
				}
			}
			idx2++;
		}
		idx1++;
		if (n1a != 0) {
			break;
		}
	}

	// part 1

	idx1 = 0;
	int n2a = 0, n2b = 0, n2c = 0;
	for (auto v1 = values.begin(); v1 != values.end(); v1++) {
		idx2 = 0;
		for (auto v2 = values.begin(); v2 != values.end(); v2++) {
			idx3 = 0;
			for (auto v3 = values.begin(); v3 != values.end(); v3++) {
				if (idx1 != idx2 && idx1 != idx3 && idx2 != idx3) {
					if (*v1 + *v2 + *v3 == 2020) {
						n2a = *v1;
						n2b = *v2;
						n2c = *v3;
						break;
					}
				}
				idx3++;
			}
			idx2++;
			if (n2a != 0) {
				break;
			}
		}
		idx1++;
		if (n2a != 0) {
			break;
		}
	}

	cout << "Answer 1: " << n1a*n1b << endl;
	cout << "Answer 2: " << n2a*n2b*n2c << endl;

	return EXIT_SUCCESS;
}
