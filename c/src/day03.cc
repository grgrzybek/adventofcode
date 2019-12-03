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
#include <cstdlib>
#include <iostream>
#include <istream>
#include <limits>
#include <ostream>
#include <fstream>
#include <sstream>
#include <string>

#include <vector>

#include <getopt.h>
#include <config.h>

using namespace std;

static const char *program_name;

static const struct option longopts[] = {
		{ "help", no_argument, NULL, 'h' },
		{ "version", no_argument, NULL, 'v' },
		{ "file", required_argument, NULL, 'f' }
};

static void print_help(void);
static void state(vector<pair<int, int>> &steps) __attribute__ ((unused));
static void show(char *panel, int width, int height) __attribute__ ((unused));

int main(int argc, char *argv[]) {

	program_name = argv[0];

	int optc, lose = 0;
	ifstream *input = NULL;

	while ((optc = getopt_long(argc, argv, "hvf:", longopts, NULL)) != -1) {
		switch (optc) {
		case 'f':
			input = new ifstream(optarg, ios_base::in);
			break;
		case 'h':
			print_help();
			return EXIT_SUCCESS;
		case 'v':
			printf("%s\n", PACKAGE_STRING);
			return EXIT_SUCCESS;
		default:
			lose = 1;
		}
	}

	if (lose || optind < argc || !input) {
		if (optind < argc) {
			cerr << program_name << ": extra operand: " << argv[optind] << endl;
		}
		cerr << "Try `" << program_name << " --help' for more information." << endl;
		return EXIT_FAILURE;
	}


	vector<pair<int, int>> wire1;
	vector<pair<int, int>> wire2;
	vector<pair<int, int>> *p;

	// U: 0, R: 1, D: 2, L: 3
	string dirs = "URDL";

	string line;
	int w = 0;
	while (*input) {
		line.clear();
		getline(*input, line);
		if (line.length()) {
			p = w++ == 0 ? &wire1 : &wire2;
			cout << "line: " << line << endl;
			string::size_type pos1 = 0;
			string::size_type pos2 = 0;
			pos2 = line.find(",", pos1);
			while ((pos2 = line.find(",", pos1)) != string::npos) {
				char cdir = line[pos1];
				int dir = dirs.find(cdir);
				int steps = std::stoi(line.substr(pos1+1, pos2 - pos1+1));
				p->push_back(make_pair(dir, steps));
				pos1 = pos2+1;
			}
			// last step
			char cdir = line[pos1];
			int dir = dirs.find(cdir);
			int steps = std::stoi(line.substr(pos1+1, line.length()));
			p->push_back(make_pair(dir, steps));
		}
	}

	delete input;

	// panel will have 0,0 absolute coordinate in top-left corner
	// relative X grows right, Y grows up
	// relative: U: Y++, D: Y--, R: X++, L: Y--
	// absolute X grows right, Y grows bottom
	// absolute: U: Y--, D: Y++, R: X++, L: Y--

	// count boundaries using relative position of wire
	int miny=0, minx=0, maxy=0, maxx=0;
	vector<pair<int, int>> *wires[2] { &wire1, &wire2 };
	for (auto &w : wires) {
		int x=0, y=0;
		for (auto &p : *w) {
			switch (dirs[p.first]) {
			case 'U': y+=p.second; break;
			case 'R': x+=p.second; break;
			case 'D': y-=p.second; break;
			case 'L': x-=p.second; break;
			}
			if (maxx < x)
				maxx = x;
			if (maxy < y)
				maxy = y;
			if (minx > x)
				minx = x;
			if (miny > y)
				miny = y;
		}
	}

	// 2 for padding
	int width = maxx - minx + 1 + 2;
	int height = maxy - miny + 1 + 2;

	cout << "(" << minx << "," << miny << ") - (" << maxx << "," << maxy << ")" << endl;
	cout << "panel size: " << width << 'x' << height << endl;

	char* panel = new char[width * height];

	for (int _y = 0; _y < height; _y++) {
		for (int _x = 0; _x < width; _x++) {
			panel[_y*width + _x] = 0;
		}
	}
//	show(panel, width, height);

	int wire_mark = 1;
	for (auto &w : wires) {
//		cout << "--- wire " << wire_mark << endl;
		// start with relative 0,0, include padding
		int x = 1, y = 1;
		// translate to absolute in panel
		x -= minx;
		y += maxy;
		panel[y*width+x] |= wire_mark;
		// for each instruction
		for (auto &instruction : *w) {
//			cout << "x:" << x << ", y:" << y << endl;
			int dx=0, dy=0;
			switch (dirs[instruction.first]) {
			case 'U': dy=-1; break;
			case 'R': dx=1; break;
			case 'D': dy=1; break;
			case 'L': dx=-1; break;
			}

			if (dx) {
				// positive number of steps horizontally
				for (int s=0; s<instruction.second; s++) {
					x+=dx;
					panel[y*width+x] |= wire_mark;
				}
			} else if (dy) {
				// positive number of steps vertically
				for (int s=0; s<instruction.second; s++) {
					y+=dy;
					panel[y*width+x] |= wire_mark;
				}
			}
		}
		wire_mark++;
	}

//	show(panel, width, height);

	int x = 1, y = 1;
	x -= minx;
	y += maxy;

	cout << "Starting from (" << x << "," << y << ")" << endl;

	int min_distance = numeric_limits<int>().max();
	int min_steps =  numeric_limits<int>().max();

	for (int _y = height-2; _y >= 1; _y--) {
		for (int _x = 1; _x < width-1; _x++) {
			char c = panel[_y*width+ _x];
			if (c == 3) {
				cout << "Crossing at (" << _x << "," << _y << ")" << endl;
				int d = abs(_x - x) + abs(_y - y);
				if (d > 0 && d < min_distance)
					min_distance = d;
				if (d > 0) {
					// part2 - calculate steps to reach this point

					wire_mark = 1;
					int steps[] { 0, 0 };
					for (auto &w : wires) {
						// start with relative 0,0, include padding
						int x = 1, y = 1;
						// translate to absolute in panel
						x -= minx;
						y += maxy;
						// to mark start point
						panel[y*width+x] = 0;
						bool intersection_reached = false;
						for (auto &instruction : *w) {
							int dx=0, dy=0;
							switch (dirs[instruction.first]) {
							case 'U': dy=-1; break;
							case 'R': dx=1; break;
							case 'D': dy=1; break;
							case 'L': dx=-1; break;
							}

							if (dx) {
								// positive number of steps horizontally
								for (int s=0; s<instruction.second; s++) {
									x+=dx;
									steps[wire_mark-1]++;
									if (x == _x && y == _y) {
										intersection_reached = true;
										break;
									}
								}
							} else if (dy) {
								// positive number of steps vertically
								for (int s=0; s<instruction.second; s++) {
									y+=dy;
									steps[wire_mark-1]++;
									if (x == _x && y == _y) {
										intersection_reached = true;
										break;
									}
								}
							}
							if (intersection_reached) {
								break;
							}
						}
						wire_mark++;
					}
					if (steps[0] + steps[1] < min_steps)
						min_steps = steps[0] + steps[1];
				}
			}
		}
	}

	cout << "Answer 1: " << min_distance << endl;
	cout << "Answer 2: " << min_steps << endl;

	return EXIT_SUCCESS;
}

static void print_help(void) {
	cout << "Usage: " << program_name << " [OPTION]...\n";

	cout << "Day 03.\n\n";

	cout << "-h, --help      display this help and exit\n";
	cout << "-v, --version   display version information and exit\n\n";

	cout << "-f, --file=FILE puzzle input (file)\n\n";

	cout << "Report bugs to <" << PACKAGE_BUGREPORT << "." << endl;
}

void state(vector<pair<int, int>> &steps) {
	// C++17
//	for(auto &s : steps) {
//	}
	// C++14
//	for (auto s = steps.cbegin(); s != steps.end(); s++) {
//	}
	// Classic
//	for (vector<int>::const_iterator s = steps.cbegin(); s != steps.end(); s++) {
//	}

	for (auto n = steps.cbegin(); n != steps.end(); n++) {
		if (n != steps.cbegin())
			cout << "|";
		cout << n->first << '-' << n->second;
	}
	cout << endl;
}

void show(char *panel, int width, int height) {
	cout << endl;
	for (int _y = 0; _y < height; _y++) {
		cout << ' ';
		for (int _x = 0; _x < width; _x++) {
			char c = panel[_y*width+ _x];
			if (!c)
				cout << '.';
			else {
				cout << (char)('0' + c);
			}
		}
		cout << endl;
	}
	cout << endl;
}
