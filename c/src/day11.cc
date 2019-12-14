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

#include <map>

#include "utils/intcode.h"
#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
	aoc2019::Options options("Day 11", argc, argv);
	if (!options.check())
		return options.result();

	cout << "Starting " << options.program_name << endl;

	aoc2019::IntcodeProgram original_program(options.file());

	aoc2019::IntcodeProgram program(original_program);
	program.debug(false);

	map<string, int> painting;
	ostringstream oss;

	int input0;
	int x = 0, y = 0;    // initial position (0,0)
	int dx = 0, dy = -1; // initial heading (up)
	int tdx = 0, tdy = 0;
	vector<long long> result;
	while (!program.is_finished()) {
		oss.str("");
		oss << x << ":" << y;
		tdx = dx;
		tdy = dy;
		string key = oss.str();

		if (painting.find(key) == painting.end()) {
			// not visited, but is on black
			input0 = 0;
		} else {
			// input based on the color of current panel
			input0 = painting[key] == 0 ? 0 : 1;
		}

		result.clear();
		program.run(input0, result, 2);

		int color = (int) result[0]; // 0 - black, 1 - white
		int turn = (int) result[1];  // 0 - left 90°, 1 - right 90°

//		cout << "painting key " << key << ", color " << color << ", turning " << turn << endl;
		painting[key] = color;

		if (turn == 0) {
			// turn left
			if (tdx == 0) {
				dy = 0;
				dx = tdy;
			} else {
				dy = -tdx;
				dx = 0;
			}
		} else if (turn == 1) {
			// turn right
			if (tdx == 0) {
				dy = 0;
				dx = -tdy;
			} else {
				dy = tdx;
				dx = 0;
			}
		}

		x += dx;
		y += dy;
//		cout << "moving x: " << x << ", y: " << y << endl;
	}

	int answer1 = painting.size();

	cout << "Answer 1: " << answer1 << endl;

	aoc2019::IntcodeProgram program2(original_program);
	program2.debug(false);

	painting.clear();

	input0 = -1;
	while (!program2.is_finished()) {
		oss.str("");
		oss << x << " " << y;
		tdx = dx;
		tdy = dy;
		string key = oss.str();

		if (input0 == -1) {
			// starting panel is white
			input0 = 1;
		} else if (painting.find(key) == painting.end()) {
			// not visited, but is on black
			input0 = 0;
		} else {
			// input based on the color of current panel
			input0 = painting[key] == 0 ? 0 : 1;
		}

		result.clear();
		program2.run(input0, result, 2);

		int color = (int) result[0]; // 0 - black, 1 - white
		int turn = (int) result[1];  // 0 - left 90°, 1 - right 90°

//		cout << "painting key " << key << ", color " << color << ", turning " << turn << endl;
		painting[key] = color;

		if (turn == 0) {
			// turn left
			if (tdx == 0) {
				dy = 0;
				dx = tdy;
			} else {
				dy = -tdx;
				dx = 0;
			}
		} else if (turn == 1) {
			// turn right
			if (tdx == 0) {
				dy = 0;
				dx = -tdy;
			} else {
				dy = tdx;
				dx = 0;
			}
		}

		x += dx;
		y += dy;
//		cout << "moving x: " << x << ", y: " << y << endl;
	}

	int minx = numeric_limits<int>::max();
	int miny = numeric_limits<int>::max();
	int maxx = numeric_limits<int>::min();
	int maxy = numeric_limits<int>::min();

	int _x = 0, _y = 0;
	for (auto &coord : painting) {
		istringstream iss(coord.first);
		iss >> _x >> _y;
//		cout << _x << ":" << _y << endl;
		if (_x > maxx)
			maxx = _x;
		if (_x < minx)
			minx = _x;
		if (_y > maxy)
			maxy = _y;
		if (_y < miny)
			miny = _y;
	}

	int W = maxx - minx + 1;
	int H = maxy - miny + 1;
	int *tab = new int[W * H]{};

	cout << W << "x" << H << endl;

	for (auto &coord : painting) {
		istringstream iss(coord.first);
		iss >> _x >> _y;

		_x -= minx;
		_y -= miny;

		tab[_y * W + _x] = coord.second;
	}

	for (y = 0; y < maxy-miny+1; y++) {
		for (x = 0; x < maxx-minx+1; x++) {
			if (tab[y * W + x] == 1) {
				cout << '#';
			} else {
				cout << '.';
			}
		}
		cout << endl;
	}

	delete[] tab;

	return EXIT_SUCCESS;
}
