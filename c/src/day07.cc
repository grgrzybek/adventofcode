/*
 * Copyright 2021 Grzegorz Grzybek
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;
using namespace aoc2021;

int main(int argc, char *argv[]) {
    Options options("Day 07", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;

    getline(*input, line);
    trim(line);

    vector<int> crabs;
    int min = numeric_limits<int>::max(), max = 0;

    istringstream str(line);
    while (!str.eof()) {
        int v;
        char c;
        str >> v;
        crabs.push_back(v);
        if (v > max) {
            max = v;
        }
        if (v < min) {
            min = v;
        }
        str >> c;
    }

    // part 1

    int min_fuel = numeric_limits<int>::max();
    for (int i = min; i < max; i++) {
        int fuel = 0;
        for (int &c: crabs) {
            fuel += abs(c - i);
        }
        if (fuel < min_fuel) {
            min_fuel = fuel;
        }
    }

    int answer1 = min_fuel;

    // part 2

    min_fuel = numeric_limits<int>::max();
    for (int i = min; i < max; i++) {
        int fuel = 0;
        int to = 0;
        int df = 0;
        for (int &c: crabs) {
            if (c > i) {
                // sum from i to c
                to = c-i;
                df = to*(to+1)/2;
                fuel += df;
            } else if (i > c) {
                // sum from c to i
                to = i-c;
                df = to*(to+1)/2;
                fuel += df;
            }
        }
        if (fuel < min_fuel) {
            min_fuel = fuel;
        }
    }

    int answer2 = min_fuel;

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}
