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

#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

struct entry {
	int min, max;
	char c;
	string password;

	entry(int _min, int _max, char _c, string &&_password) : min(_min), max(_max), c(_c), password(_password) {}
};

int main(int argc, char *argv[]) {
	aoc2020::Options options("Day 02", argc, argv);
	if (!options.check())
		return options.result();

	cout << "Starting " << options.program_name << endl;

	ifstream *input = options.file();

	vector<entry> values;

	string line;
	while (getline(*input, line)) {
		aoc2020::trim(line);

		char *p = new char[line.length()];
		int min, max;
		char c;
		sscanf(line.c_str(), "%d-%d %c: %s", &min, &max, &c, p);
		values.emplace_back(min, max, c, string(p));
		delete[] p;
	}

	// part 1

	int good1 = 0;

	for (auto &e : values) {
		int count = 0;
		for (auto &c : e.password) {
			if (c == e.c) {
				count++;
			}
		}
		if (count >= e.min && count <= e.max) {
			good1++;
		}
	}

	// part 2

	int good2 = 0;

	for (auto &e : values) {
		int count = 0;
		int idx = 1;
		for (auto &c : e.password) {
			if (c == e.c && (idx == e.min || idx == e.max)) {
				count++;
			}
			idx++;
		}
		if (count == 1) {
			good2++;
		}
	}

	cout << "Answer 1: " << good1 << endl;
	cout << "Answer 2: " << good2 << endl;

	return EXIT_SUCCESS;
}
