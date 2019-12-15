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
	aoc2019::Options options("Day 13", argc, argv);
	if (!options.check())
		return options.result();

	cout << "Starting " << options.program_name << endl;

	aoc2019::IntcodeProgram original_program(options.file());

	aoc2019::IntcodeProgram program(original_program);
	program.debug(false);

	map<string, int> painting;
	ostringstream oss;

	int x = 0, y = 0, tile = 0;
	vector<long long> result;

	int ball[2] = {-1, -1};
	int pad[2] = {-1, -1};

	int minx = numeric_limits<int>::max();
	int miny = numeric_limits<int>::max();
	int maxx = numeric_limits<int>::min();
	int maxy = numeric_limits<int>::min();

	int blocks = 0;
	while (!program.is_finished()) {
		result.clear();
		program.run(1, result, 3);

		x = (int) result[0];
		y = (int) result[1];
		tile = (int) result[2];

		if (minx > x)
			minx = x;
		if (maxx < x)
			maxx = x;
		if (miny > y)
			miny = y;
		if (maxy < y)
			maxy = y;

		oss.str("");
		oss << x << " " << y;
		string key = oss.str();

//		cout << "coords " << key << ", tile " << tile << endl;
		painting[key] = tile;
		if (tile == 2)
			blocks++;

		if (tile == 3) {
			// pad
			pad[0] = x;
			pad[1] = y;
		} else if (tile == 4) {
			// ball
			ball[0] = x;
			ball[1] = y;
		}
	}

	int W = maxx - minx + 1;
	int H = maxy - miny + 1;

	char *screen = new char[W * H]{};

	for (auto &e : painting) {
		istringstream iss(e.first);
		iss >> x >> y;

		if (painting.find(e.first) != painting.cend()) {
			switch (painting[e.first]) {
				case 0: // empty
					screen[y * W + x] = ' ';
					break;
				case 1: // wall
					screen[y * W + x] = '#';
					break;
				case 2: // block
					screen[y * W + x] = 'o';
					break;
				case 3: // paddle
					screen[y * W + x] = '-';
					break;
				case 4: // ball
					screen[y * W + x] = '@';
					break;
				default:
					break;
			}
		}
	}

	cout << endl;
	for (int y = 0; y < H; y++) {
		cout << " ";
		for (int x = 0; x < W; x++) {
			cout << (char) screen[y * W + x];
		}
		cout << endl;
	}
	cout << endl;

	cout << "ball at " << ball[0] << ":" << ball[1] << ", pad at " << pad[0] << ":" << pad[1] << endl;

	cout << "Answer 1: " << blocks << endl;

	aoc2019::IntcodeProgram program2(original_program);
	program2.debug(false);
	// insert coin
	program2.get_memory()[0] = 2;

	int input0;

	int score = 0;

	while (!program2.is_finished()) {
		result.clear();

		input0 = ball[0] - pad[0];
		program2.run(input0, result, 3);

		x = (int) result[0];
		y = (int) result[1];

		if (x == -1 && y == 0)
			score = (int) result[2];
		if (result[2] == 3) {
			pad[0] = x;
		}
		if (result[2] == 4) {
			ball[0] = x;
		}
	}

	cout << "Answer 2: " << score << endl;

	delete[] screen;

	return EXIT_SUCCESS;
}
