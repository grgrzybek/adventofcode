/*
 * Copyright 2023 Grzegorz Grzybek
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

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    aoc2023::Options options("Day 01", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<string> lines;

    while (getline(*input, line)) {
        aoc2023::trim(line);
        lines.emplace_back(line);
    }

    // part 1

    int answer1 = 0;
    for (const auto &l: lines) {
        int d1 = -1;
        int d2 = -1;
        for (char c : l) {
            if (c - '0' >= 0 && c - '0' <= 9) {
                if (d1 == -1) {
                    d1 = c - '0';
                }
                d2 = c - '0';
            }
        }
        if (d1 > 0 && d2 > 0) {
            answer1 += (10 * d1 + d2);
        }
    }

    // part 2
    vector<string> numbers = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

    int answer2 = 0;
    for (const auto &l: lines) {
//        cout << "line: " << l << endl;
        int d1 = -1;
        int d2 = -1;
        int idx = 0;
        string::size_type idx1 = string::npos;
        string::size_type idx2 = string::npos;
        for (char c : l) {
            if (c - '0' >= 0 && c - '0' <= 9) {
                if (d1 == -1) {
                    d1 = c - '0';
                    idx1 = idx;
                }
                d2 = c - '0';
                idx2 = idx;
            }
            idx++;
        }

//        cout << "    " << d1 << " " << d2 << endl;
        for (string::size_type p = 0; p < l.length(); p++) {
            idx = 1;
            for (const auto &n: numbers) {
                string::size_type pos = l.find(n, p);
                if (pos != string::npos) {
//                    cout << "        " << n << ": " << pos << " (" << idx << ", " << idx1 << ", " << idx2 << ")" << endl;
                    if (idx1 == string::npos || pos < idx1) {
                        d1 = idx;
                        idx1 = pos;
//                        cout << " - " << d1 << "," << idx1 << endl;
                    }
                    if (idx2 == string::npos || pos > idx2) {
                        d2 = idx;
                        idx2 = pos;
//                        cout << " - " << d2 << "," << idx2 << endl;
                    }
                }
                idx++;
            }
        }
//        cout << "    " << d1 << " " << d2 << endl;
        if (d1 > 0 && d2 > 0) {
            answer2 += (10 * d1 + d2);
        }
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}
