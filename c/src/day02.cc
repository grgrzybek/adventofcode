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

static void state(vector<int> &numbers) __attribute__ ((unused));

int main(int argc, char *argv[]) {
	aoc2019::Options options("Day 02", argc, argv);
	if (!options.check())
		return options.result();

	cout << "Starting " << options.program_name << endl;

	vector<int> numbers;

	string line;
	while (getline(*options.file(), line)) {
		aoc2019::trim(line);
		string::size_type pos1 = 0;
		string::size_type pos2 = 0;
		while ((pos2 = line.find(',', pos1)) != string::npos) {
			numbers.push_back(std::stoi(line.substr(pos1, pos2 - pos1)));
			pos1 = pos2 + 1;
		}
		numbers.push_back(std::stoi(line.substr(pos1, line.length())));
	}

	vector<int> numbers_copy(numbers);

	numbers[1] = 12; // noun
	numbers[2] = 2;  // verb

	int pos = 0;
	bool end1 = false;
	for (;;) {
		switch (numbers[pos]) {
			case 1:
				numbers[numbers[pos + 3]] = numbers[numbers[pos + 1]] + numbers[numbers[pos + 2]];
				break;
			case 2:
				numbers[numbers[pos + 3]] = numbers[numbers[pos + 1]] * numbers[numbers[pos + 2]];
				break;
			case 99:
				end1 = true;
				break;
		}
		pos += 4;
		if (end1) {
			break;
		}
	}

	int answer1 = numbers[0];

	int n = 0;
	int v = 0;
	for (;;) {
		bool end2 = false;
		for (n = 0; n <= 99 && !end2; n++) {
			cout << "noun: " << n << endl;
			for (v = 0; v <= 99 && !end2; v++) {
				numbers.assign(numbers_copy.begin(), numbers_copy.end());
				numbers[1] = n; // noun
				numbers[2] = v; // verb
				pos = 0;
				end1 = false;
				for (;;) {
					switch (numbers[pos]) {
						case 1:
							numbers[numbers[pos + 3]] = numbers[numbers[pos + 1]] + numbers[numbers[pos + 2]];
							break;
						case 2:
							numbers[numbers[pos + 3]] = numbers[numbers[pos + 1]] * numbers[numbers[pos + 2]];
							break;
						case 99:
							end1 = true;
							break;
					}
					pos += 4;
					if (end1) {
						break;
					}
				}
				if (numbers[0] == 19690720) {
					n--;
					v--;
					end2 = true;
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

void state(vector<int> &numbers) {
	for (auto n = numbers.cbegin(); n != numbers.end(); n++) {
		if (n != numbers.cbegin())
			cout << "|";
		cout << *n;
	}
	cout << endl;
}
