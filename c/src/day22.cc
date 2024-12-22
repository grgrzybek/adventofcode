/*
 * Copyright 2024 Grzegorz Grzybek
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

#include <cstdlib>
#include <cstring>
#include <deque>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    aoc2024::Options options("Day 22", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;

    vector<long> seeds;

    while (getline(*input, line)) {
        aoc2024::trim(line);
        if (line.empty()) {
            break;
        }
        seeds.emplace_back(stol(line));
    }

    // part 1

    long answer1 = 0;

    long v;
    for (long sn: seeds) {
//        cout << "seed " << sn << ":\n";
        for (int i = 0; i < 2000; i++) {
            v = sn << 6;
            sn ^= v;
            sn &= 0xffffff;
            v = sn >> 5;
            sn ^= v;
            sn &= 0xffffff;
            v = sn << 11;
            sn ^= v;
            sn &= 0xffffff;
//            cout << "  " << sn << endl;
        }
        answer1 += sn;
    }

    // part 2

    long answer2 = 0;

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}
