/*
 * Copyright 2025 Grzegorz Grzybek
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

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <complex>

#include "utils/utils.h"

int nsize(long i);

using namespace std;

int main(int argc, char *argv[]) {
    aoc2025::Options options("Day 02", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    string range;
    vector<pair<long, long>> ranges;

    while (getline(*input, line)) {
        aoc2025::trim(line);
        istringstream iss(line);

        while (getline(iss, range, ',')) {
            unsigned long pos = range.find('-');
            if (pos != string::npos) {
                long l1 = stol(range.substr(0, pos));
                long l2 = stol(range.substr(pos + 1));
                ranges.emplace_back(l1, l2);
            }
        }
    }

    // part 1

    long answer1 = 0;
    for (auto p: ranges) {
        long v = p.first;
        while (v <= p.second) {
            int digits = nsize(v);
            if (digits % 2 == 0) {
                if ((v % (int) std::pow(10, digits / 2)) == (v / (int) std::pow(10, digits / 2))) {
                    answer1 += v;
                }
            }
            v++;
        }
    }

    // part 2

    long answer2 = 0;
    char buf[256];
    for (auto p: ranges) {
        long v = p.first;
        while (v <= p.second) {
            int digits = nsize(v);
            for (int d = 1; d < digits; d++) {
                if (digits % d == 0) {
                    memset(buf, 0, 256);
                    snprintf(buf, 255, "%ld", v);
                    auto s = string(buf);
                    long subv = stol(s.substr(0, d), nullptr, 10);
                    int pos = d;
                    bool same = pos < digits;
                    while (pos < digits) {
                        if (stol(s.substr(pos, d), nullptr, 10) != subv) {
                            same = false;
                            break;
                        }
                        pos += d;
                    }
                    if (same) {
                        answer2 += v;
                        break;
                    }
                }
            }
            v++;
        }
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}

int nsize(long v) {
    int s = 0;
    while (v > 0) {
        v /= 10;
        s++;
    }
    return s;
}
