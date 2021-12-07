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
    Options options("Day 06", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;

    getline(*input, line);
    trim(line);

    vector<int> lantern_fish;

    istringstream str(line);
    while (!str.eof()) {
        int v;
        char c;
        str >> v;
        lantern_fish.push_back(v);
        str >> c;
    }

    // part 1

    for (int i = 0; i < 80; i++) {
        unsigned long s = lantern_fish.size();
        for (unsigned long lf = 0; lf < s; lf++) {
            if (lantern_fish[lf] == 0) {
                lantern_fish[lf] = 6;
                lantern_fish.push_back(8);
            } else {
                lantern_fish[lf]--;
            }
        }
    }

    unsigned long answer1 = lantern_fish.size();

    // part 2

    str.clear();
    str.str(line);
    unsigned long long *map1 = new unsigned long long[9]{};
    unsigned long long *map2 = new unsigned long long[9]{};

    while (!str.eof()) {
        unsigned long long v;
        char c;
        str >> v;
        map1[v]++;
        str >> c;
    }

    for (int i = 0; i < 256; i++) {
        // alter map2
        for (int j = 1; j < 9; j++) {
            // countdown
            map2[j - 1] = map1[j];
        }
        map2[8] = map1[0];
        map2[6] += map1[0];

        // swap pointers
        swap(map1, map2);
    }

    unsigned long long answer2 = 0;
    for (int j = 0; j < 9; j++) {
        answer2 += map1[j];
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    delete[] map1;
    delete[] map2;

    return EXIT_SUCCESS;
}
