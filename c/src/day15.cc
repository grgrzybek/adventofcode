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
#include <list>
#include <vector>
#include <unordered_set>

#include "utils/utils.h"

using namespace std;

struct sensor_beacon {
    long sx, sy, bx, by;
    long range;

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
    vector<sensor_beacon> sensors_and_beacons;

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

        sensors_and_beacons.push_back(sb);
    }

    // part 1

    // Sensor at x=2, y=18: closest beacon is at x=-2, y=15
    // Sensor at x=9, y=16: closest beacon is at x=10, y=16
    // ...
//    const int ROW = 10;
    const int ROW = 2000000;

    unordered_set<long> not_beacons;
    for (auto &sb: sensors_and_beacons) {
        sb.range = abs((long) (sb.sx - sb.bx)) + abs((long) (sb.sy - sb.by));
        long dx = sb.sy - ROW;
        if (abs((long) dx) <= sb.range) {
//            sb.print();
//            cout << "   range: " << range << "\n";
            // this sensor sees some some x coords in checked row
            for (long x = sb.sx - sb.range + abs((long) dx); x <= sb.sx + sb.range - abs((long) dx); x++) {
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

    long answer2 = 0;

    vector<pair<long, long>> range;
    long dy = 0;

    const long SIZE = 4000000;

    for (long y = 0; y <= SIZE; y++) {
        range.clear();
        range.emplace_back(-2, -1);
        for (auto &sb: sensors_and_beacons) {
            dy = abs((long) (sb.sy - y));
            if (dy > sb.range) {
                continue;
            }
            range.emplace_back(sb.sx - (sb.range - dy), sb.sx + (sb.range - dy));
        }
        range.emplace_back(SIZE + 1, SIZE + 2);
        sort(range.begin(), range.end(), [] (auto &p1, auto &p2) {
            return p1.first < p2.first;
        });
        list<pair<long, long>> l(range.begin(), range.end());

        long pos = -1;
        bool change = false;
        while (!l.empty()) {
            change = false;
            auto it = l.begin();
            while (it != l.end()) {
                if (it->second <= pos) {
                    l.erase(it++);
                    change = true;
                    continue;
                } else if ((it->first <= pos || it->first == 0) && it->second > pos) {
                    pos = it->second;
                    change = true;
                }
                it++;
            }
            if (!change && pos < SIZE) {
                answer2 = (l.begin()->first - 1l) * 4000000l + y;
                break;
            }
        }
        if (answer2 > 0) {
            break;
        }
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}
