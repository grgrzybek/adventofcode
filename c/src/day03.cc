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

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    aoc2025::Options options("Day 03", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<string> lines;

    while (getline(*input, line)) {
        aoc2025::trim(line);
        lines.push_back(line);
    }

    // part 1

    long answer1 = 0;
    for (const auto &l: lines) {
        string::const_iterator p1 = max_element(l.cbegin(), l.cend() - 1 , [](char c1, char c2) {
            return c1 < c2;
        });
        string::const_iterator p2 = max_element(p1 + 1, l.cend(), [](char c1, char c2) {
            return c1 < c2;
        });

        char buf[3] = { *p1, *p2, '\0' };
        int v = static_cast<int>(strtol(buf, nullptr, 10));
        answer1 += v;
    }

    // part 2

    long answer2 = 0;
    for (const auto &l: lines) {
        char buf[13];
        memset(buf, 0, 13);
        string::const_iterator p1 = l.cbegin();
        for (int limit = 11; limit >= 0; limit--) {
            p1 = max_element(p1, l.cend() - limit , [](char c1, char c2) {
                return c1 < c2;
            });
            buf[11 - limit] = *p1;
            p1++;
        }

        cout << buf << endl;
        long v = strtol(buf, nullptr, 10);
        answer2 += v;
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}
