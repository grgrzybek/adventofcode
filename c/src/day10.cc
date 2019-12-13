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

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>

#include "utils/utils.h"

using namespace std;

static void state(const char *map, int w, int h) __attribute__ ((unused));

struct {
	bool operator()(pair<double, vector<pair<int, int>>*> &p1, pair<double, vector<pair<int, int>>*> &p2) {
		return p1.first < p2.first;
	}
} Sorting;

int main(int argc, char *argv[]) {
	aoc2019::Options options("Day 10", argc, argv);
	if (!options.check())
		return options.result();

	cout << "Starting " << options.program_name << endl;

	vector<string> map1;

	string line;
	while (getline(*options.file(), line)) {
		map1.emplace_back(line);
	}

	int W = map1[0].length();
	int H = map1.size();

	char *original_map = new char[W * H]{}; // zero initalization
	vector<pair<int, int>> asteroids;

	int r = 0, c = 0;
	for (auto &s : map1) {
		c = 0;
		for (auto &ch : s) {
			original_map[r * W + c] = ch == '#' ? 1 : 0;
			if (ch == '#')
				asteroids.emplace_back(c, r);
			c++;
		}
		r++;
	}

	int max_visible = 0;

	// part1 - calculate visible asteroids

	int coord_x = -1;
	int coord_y = -1;
	for (auto &a : asteroids) {
		int visible = 0;

		int x1 = a.first;
		int y1 = a.second;
		for (auto &other : asteroids) {
			int x2 = other.first;
			int y2 = other.second;
			if (x1 == x2 && y1 == y2) {
				continue;
			}
			// check if there's something between x1:y1 and x2:y2
			int dx = ::abs(x1 - x2);
			int dy = ::abs(y1 - y2);
			int d = gcd(dx, dy);
//			cout << "GCD between " << x1 << ":" << y1 << " and " << x2 << ":" << y2 << " is " << d << endl;

			int ddx = dx / d * (x1 > x2 ? -1 : 1);
			int ddy = dy / d * (y1 > y2 ? -1 : 1);
//			cout << "x1: " << x1 << ", y1: " << y1 << endl;
//			cout << "x2: " << x2 << ", y2: " << y2 << endl;
//			cout << "ddx: " << ddx << ", ddy: " << ddy << endl;
			bool ok = true;
			for (int x = x1 + ddx, y = y1 + ddy; x != x2 || y != y2; x += ddx, y += ddy) {
//				cout << "x: " << x << "y: " << y << endl;
//				map[y*W+x] = 3;
				if (original_map[y * W + x] == 1)
					ok = false;
			}
//			state(map, W, H);
//			::memcpy((void *)map, (void *)original_map, W * H);
			if (ok)
				visible++;
		}

		if (max_visible < visible) {
			max_visible = visible;
			coord_x = x1;
			coord_y = y1;
		}
	}

	original_map[coord_y * W + coord_x] = 2;
	state(original_map, W, H);

	// part2 - calculate list of angles and associated asteroids
	// list of pairs: angle from coord_x:coord_y -> pointer to vector of coordinates of asteroids at that angle
	// in order of appearence from coord_x:coord_y
	vector<pair<double, vector<pair<int, int>>*>> shooting_list;

	char *angle_checks = new char[W * H]{};
	memcpy(angle_checks, original_map, W * H);

	for (auto &a : asteroids) {
		int x1 = a.first;
		int y1 = a.second;

		if (x1 == coord_x && y1 == coord_y)
			continue;

		if (angle_checks[y1 * W + x1] == 0) {
			// this asteroid was already checked
			continue;
		}

		// for this asteroid we'll check all asteroids at that angle - and remove them from the map
		int dx = ::abs(x1 - coord_x);
		int dy = ::abs(y1 - coord_y);
		int d = gcd(dx, dy);

		// ddx and ddy are negative/positive increments from coord_x:coord_y to the checked asteroid
		// potentially many asteroids are on this line
		int ddx = dx / d * (x1 < coord_x ? -1 : 1);
		int ddy = dy / d * (y1 < coord_y ? -1 : 1);

		// example:
		// 1. .. 2
		//  .    .
		// .. . .
		//  . @...
		// ..... .
		//  .....
		// 3..  .4
		//
		// - @-1: ddx<0, ddy<0, angle = 360°-arctan(-ddx/-ddy)
		// - @-2: ddx>0, ddy<0, angle = arctan(ddx/-ddy)
		// - @-3: ddx<0, ddy>0, angle = 180°+arctan(-ddx/ddy)
		// - @-4: ddx>0, ddy>0, angle = 180°-arctan(ddx/ddy)

		double angle = ddy == 0 ? (ddx > 0 ? M_PI-M_PI_2 : M_PI+M_PI_2) : ::atan((double)::abs(ddx)/::abs(ddy));
		if (dy != 0) {
			// 1
			if (ddx < 0 && ddy < 0)
				angle = 2*M_PI-angle;
			// 3
			if (ddx < 0 && ddy > 0)
				angle = M_PI+angle;
			// 4
			if (ddx >= 0 && ddy > 0)
				angle = M_PI-angle;
		}
//		cout << "  angle: " << angle << "(" << angle/(2*M_PI) * (double)360 << "°) (" << x1 << ":" << y1 << ")" << endl;

		vector<pair<int, int>> *coords = new vector<pair<int, int>>;
		shooting_list.emplace_back(angle, coords);

		// collect asteroids at the same angle
		int x = coord_x + ddx;
		int y = coord_y + ddy;
		for (;; x += ddx, y += ddy) {
			if (!(x >= 0 && x < W && y >= 0 && y < H))
				break;
//			cout << "    also: " << x << ":" << y << endl;
			if (angle_checks[y * W + x] == 1) {
				coords->emplace_back(x, y);
				angle_checks[y * W + x] = 0;
			}
		}
	}

	sort(shooting_list.begin(), shooting_list.end(), Sorting);

//	int i = 1;
//	for (auto &d : shooting_list) {
//		cout << i << ") " << d.first << " - " << d.second->size() << ": ";
//		for (auto &xy : *d.second) {
//			cout << xy.first << ":" << xy.second << " ";
//		}
//		cout << endl;
//		i++;
//	}

	int answer2 = 0;
	for (vector<pair<double,vector<pair<int,int>>*>>::size_type i = 0; i < 200; i++) {
		vector<pair<double,vector<pair<int,int>>*>>::size_type idx = i % shooting_list.size();
		if (i == 199) {
			answer2 = (*shooting_list[idx].second)[0].first * 100 + (*shooting_list[idx].second)[0].second;
		}
//		cout << "i: " << i << ", idx: " << idx << " - ";
//		for (auto &xy : *shooting_list[idx].second) {
//			cout << xy.first << ":" << xy.second << " ";
//		}
//		cout << endl;
		shooting_list[idx].second->erase(shooting_list[idx].second->begin());
	}

	for (auto &d : shooting_list) {
		delete d.second;
	}

	delete[] angle_checks;
	delete[] original_map;

	cout << "Answer 1: " << max_visible << " (at " << coord_x << ":" << coord_y << ")" << endl;
	cout << "Answer 2: " << answer2 << endl;

	return EXIT_SUCCESS;
}

void state(const char *map, int w, int h) {
	cout << endl;
	for (int y = 0; y < h; y++) {
		cout << " ";
		for (int x = 0; x < w; x++) {
			switch (map[y * w + x]) {
				case 0:
					cout << ' ';
					break;
				case 1:
					cout << '.';
					break;
				case 2:
					cout << '@';
					break;
				case 3:
					cout << '+';
					break;
				default:
					cout << '?';
					break;
			}
		}
		cout << endl;
	}
	cout << endl;
}
