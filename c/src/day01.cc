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

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
	aoc2019::Options options("Day 01", argc, argv);
	if (!options.check())
		return options.result();

	cout << "Starting " << options.program_name << endl;

	ifstream *input = options.file();

	int fuel1 = 0;
	int fuel2 = 0;

	string line;
	while (getline(*input, line)) {
		aoc2019::trim(line);
		int module_mass = 0;

		// number per line
		istringstream iss(line);
		iss >> module_mass;

		int _f = (module_mass / 3) - 2;
		fuel1 += _f;

		int added_mass = _f;
		for (;;) {
			_f = (added_mass / 3) - 2;
			if (_f > 0) {
				added_mass = _f;
				fuel2 += _f;
			} else {
				break;
			}
		}
	}

	cout << "Answer 1: " << fuel1 << endl;
	cout << "Answer 2: " << fuel1 + fuel2 << endl;

	return EXIT_SUCCESS;
}
