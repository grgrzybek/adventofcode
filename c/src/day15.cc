/*
 * Copyright 2022 Grzegorz Grzybek
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
#include <cstring>
#include <iostream>
#include <set>
#include <vector>

#include "utils/utils.h"

using namespace std;

struct sensor_beacon {
    int sx, sy, bx, by;

    void print() const {
        cout << "[" << sx << "," << sy << "] sees [" << bx << "," << by << "]\n";
    }
};

int main(int argc, char *argv[]) {
    aoc2022::Options options("Day 15", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<sensor_beacon> seonsors_and_beacons;

    while (getline(*input, line)) {
        if (!aoc2022::has_text(line)) {
            continue;
        }
        sensor_beacon sb{};

        size_t start = line.find('=');
        size_t pos = line.find(',', start);
        string s = line.substr(start + 1, pos - start - 1);
        sb.sx = stoi(s);

        start = pos + 4;
        pos = line.find(':', start);
        s = line.substr(start, pos - start);
        sb.sy = stoi(s);

        start = pos + 25;
        pos = line.find(',', start);
        s = line.substr(start, pos - start);
        sb.bx = stoi(s);

        start = pos + 4;
        s = line.substr(start);
        sb.by = stoi(s);

        seonsors_and_beacons.push_back(sb);
    }

    // part 1

    // Sensor at x=2, y=18: closest beacon is at x=-2, y=15
    // Sensor at x=9, y=16: closest beacon is at x=10, y=16
    // ...
    const int ROW = 2000000;
    set<int> not_beacons;
    for (auto &sb: seonsors_and_beacons) {
        int range = abs(sb.sx - sb.bx) + abs(sb.sy - sb.by);
        int dx = sb.sy - ROW;
        if (abs(dx) <= range) {
//            sb.print();
//            cout << "   range: " << range << "\n";
            // this sensor sees some some x coords in checked row
            for (int x = sb.sx - range + abs(dx); x <= sb.sx + range - abs(dx); x++) {
                if (!(sb.bx == x && sb.by == ROW)) {
                    // it's a non-beacon field
                    not_beacons.insert(x);
                }
            }
//            cout << "      size so far: " << not_beacons.size() << "\n";
        }
    }

    size_t answer1 = not_beacons.size();

    // part 2

    int answer2 = 0;


    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 + 1 << endl;

    return EXIT_SUCCESS;
}
