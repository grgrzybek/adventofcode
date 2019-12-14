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

#include <vector>

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
	aoc2019::Options options("Day 12a", argc, argv);
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

	for (int step = 0; step < 1000; step++) {
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
	}

	int energy = 0;
	for (Body &m : moons) {
		energy += (abs(m.x) + abs(m.y) + abs(m.z)) * (abs(m.vx) + abs(m.vy) + abs(m.vz));
	}

	cout << "Answer 1: " << energy << endl;

	return EXIT_SUCCESS;
}
