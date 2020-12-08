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
#include <memory>
#include <regex>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
	aoc2020::Options options("Day 05", argc, argv);
	if (!options.check())
		return options.result();

	cout << "Starting " << options.program_name << endl;

	vector<string> lines;

	string line;
	while (getline(*options.file(), line)) {
		aoc2020::trim(line);
		if (line.empty()) {
			break;
		}
		lines.push_back(line);
	}

//	for (auto &p : lines) {
//		cout << ">> " << p << endl;
//	}

	// part 1

	int highest1 = 0;
	vector<int> seats;

	for (auto &p : lines) {
		int vmin = 1, vmax = 128;
		int hmin = 1, hmax = 8;
		for (int i = 0; i < 7; i++) {
			if (p.c_str()[i] == 'F') {
				vmax -= (vmax - vmin + 1) / 2;
			} else if (p.c_str()[i] == 'B') {
				vmin += (vmax - vmin + 1) / 2;
			}
		}
		for (int i = 7; i < 10; i++) {
			if (p.c_str()[i] == 'L') {
				hmax -= (hmax - hmin + 1) / 2;
			} else if (p.c_str()[i] == 'R') {
				hmin += (hmax - hmin + 1) / 2;
			}
		}
		hmax--;
		vmax--;

		seats.push_back(vmax * 8 + hmax);
		if (highest1 < (vmax * 8 + hmax)) {
			highest1 = vmax * 8 + hmax;
		}
	}

	// part 2

	sort(seats.begin(), seats.end());
	int s1 = seats[0];
	int my_seat = 0;
	for (auto &s : seats) {
		if (s == s1 + 2) {
			my_seat = s - 1;
			break;
		}
		s1 = s;
	}

	cout << "Answer 1: " << highest1 << endl;
	cout << "Answer 2: " << my_seat << endl;

	return EXIT_SUCCESS;
}
