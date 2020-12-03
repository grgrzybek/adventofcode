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

long slide(int dx, int dy, int width, int height, const char* map);

int main(int argc, char *argv[]) {
	aoc2020::Options options("Day 03", argc, argv);
	if (!options.check())
		return options.result();

	cout << "Starting " << options.program_name << endl;

	vector<string> values;

	string line;
	while (getline(*options.file(), line)) {
		aoc2020::trim(line);
		values.emplace_back(line);
	}

	int W = values[0].length();
	int H = values.size();

	char *map = new char[W * H]{}; // zero initalization

	int x = 0, y = 0;
	for (string &l : values) {
		x = 0;
		for (char c : l) {
			map[y*W + x] = c;
			x++;
		}
		y++;
	}

	// part 1

	x = 0;
	y = 0;
	int trees1 = 0;

	for (;;) {
		if (y == H) {
			break;
		}
		if (x >= W) {
			x %= W;
		}
		if (map[y*W + x] == '#') {
			trees1++;
		}
		x += 3;
		y++;
	}

	// part 2

	long trees_list[5];
	trees_list[0] = slide(1, 1, W, H, map);
	trees_list[1] = slide(3, 1, W, H, map);
	trees_list[2] = slide(5, 1, W, H, map);
	trees_list[3] = slide(7, 1, W, H, map);
	trees_list[4] = slide(1, 2, W, H, map);

	long trees2 = 1;
	for (long v : trees_list) {
		trees2 *= v;
	}

	cout << "Answer 1: " << trees1 << endl;
	cout << "Answer 2: " << trees2 << endl;

	return EXIT_SUCCESS;
}

long slide(int dx, int dy, int width, int height, const char* map) {
	int x = 0, y = 0;
	long trees = 0;
	for (;;) {
		if (y >= height) {
			break;
		}
		if (x >= width) {
			x %= width;
		}
		if (map[y*width+x] == '#') {
			trees++;
		}
		x += dx;
		y += dy;
	}
	return trees;
}
