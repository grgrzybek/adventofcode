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

#include <map>
#include <vector>
#include <numeric>

#include "utils/utils.h"

using namespace std;

struct Body {
	int id = 0;
	int x, y, z;
	int vx = 0, vy = 0, vz = 0;

	Body(int _id, int _x, int _y, int _z) : id(_id), x(_x), y(_y), z(_z) {}

	friend basic_ostream<char>& operator<<(basic_ostream<char> &os, Body &b) {
		return os << b.id << ") " << b.x << ":" << b.y << ":" << b.z << " (" << b.vx << " - " << b.vy << " - " << b.vz << ")";
	}
};

int main(int argc, char *argv[]) {
	aoc2019::Options options("Day 12b", argc, argv);
	if (!options.check())
		return options.result();

	cout << "Starting " << options.program_name << endl;

	string line;
	int body_id = 0;
	vector<Body> moons;

	while (getline(*options.file(), line)) {
		aoc2019::trim(line);
		cout << line << endl;

		int x, y, z;
		// <x=3, y=5, z=-1>
		::sscanf(line.c_str(), "<x=%d, y=%d, z=%d>", &x, &y, &z);

		moons.emplace_back(++body_id, x, y, z);
	}

	cout << "--- Moons:\n";
	for (Body &moon : moons)
		cout << moon << endl;

	vector<int> initial_state_x;
	vector<int> initial_state_y;
	vector<int> initial_state_z;
	vector<long> rc = { -1L, -1L, -1L };

	for (Body &m : moons) {
		initial_state_x.push_back(m.x);
		initial_state_y.push_back(m.y);
		initial_state_z.push_back(m.z);
	}

	long step = 0;
	bool end;
	for (;;) {
		if (step > 0) {
			if (rc[0] == -1) { // x
				bool same = true;
				same &= initial_state_x[0] == moons[0].x;
				same &= initial_state_x[1] == moons[1].x;
				same &= initial_state_x[2] == moons[2].x;
				same &= initial_state_x[3] == moons[3].x;
				same &= 0 == moons[0].vx;
				same &= 0 == moons[1].vx;
				same &= 0 == moons[2].vx;
				same &= 0 == moons[3].vx;
				if (same)
					rc[0] = step;
			}
			if (rc[1] == -1) { // y
				bool same = true;
				same &= initial_state_y[0] == moons[0].y;
				same &= initial_state_y[1] == moons[1].y;
				same &= initial_state_y[2] == moons[2].y;
				same &= initial_state_y[3] == moons[3].y;
				same &= 0 == moons[0].vy;
				same &= 0 == moons[1].vy;
				same &= 0 == moons[2].vy;
				same &= 0 == moons[3].vy;
				if (same)
					rc[1] = step;
			}
			if (rc[2] == -1) { // z
				bool same = true;
				same &= initial_state_z[0] == moons[0].z;
				same &= initial_state_z[1] == moons[1].z;
				same &= initial_state_z[2] == moons[2].z;
				same &= initial_state_z[3] == moons[3].z;
				same &= 0 == moons[0].vz;
				same &= 0 == moons[1].vz;
				same &= 0 == moons[2].vz;
				same &= 0 == moons[3].vz;
				if (same)
					rc[2] = step;
			}
		}
		end = rc[0] != -1 && rc[1] != -1 && rc[2] != -1;
		if (end)
			break;

		// alter velocities
		for (Body &m1 : moons) {
			for (Body &m2 : moons) {
				if (m1.id == m2.id)
					continue;
				if (m1.x != m2.x)
					m1.vx += (m1.x < m2.x ? 1 : -1);
				if (m1.y != m2.y)
					m1.vy += (m1.y < m2.y ? 1 : -1);
				if (m1.z != m2.z)
					m1.vz += (m1.z < m2.z ? 1 : -1);
			}
		}
		// alter positions
		for (Body &m1 : moons) {
			m1.x += m1.vx;
			m1.y += m1.vy;
			m1.z += m1.vz;
		}

		step++;
	}

	cout << "Answer 1: " << lcm(lcm(rc[0], rc[1]), rc[2]) << endl;
	cout << "Answer 1: " << rc[0]*rc[1]*rc[2] << endl;

	return EXIT_SUCCESS;
}
