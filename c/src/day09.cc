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

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

struct rect {
    long x1; long x2; long y1; long y2;

    friend ostream& operator<<(ostream& os, const rect& r);
};

ostream& operator<<(ostream& os, const rect& r) {
    return os << r.x1 << " " << r.x2 << " " << r.y1 << " " << r.y2;
}

int main(const int argc, char *argv[]) {
    aoc2025::Options options("Day 09", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<pair<long, long>> pairs;

    while (getline(*input, line)) {
        aoc2025::trim(line);
        const string::size_type pos = line.find(',');
        pairs.emplace_back(stol(line.substr(0, pos)), stol(line.substr(pos + 1)));
    }

    // part 1

    long answer1 = 0;
    for (auto &[x1, y1]: pairs) {
        for (auto &[x2, y2]: pairs) {
            if (x1 == x2 && y1 == y2) {
                continue;
            }
            if (const long area = (abs(x2 - x1) + 1) * (abs(y2 - y1) + 1); answer1 < area) {
                answer1 = area;
            }
        }
    }

    // part 2

    long answer2 = 0;

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}
