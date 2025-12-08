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

#include <cmath>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

struct junction_box {
    long x, y, z;

    friend ostream& operator<<(ostream&, junction_box &);
};

ostream& operator<<(ostream& os, junction_box &j) {
    return os << "[" << j.x << "," << j.y << "," << j.z << "]";
}

unsigned long distance(junction_box &b1, junction_box &b2);

int main(int argc, char *argv[]) {
    aoc2025::Options options("Day 08", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<junction_box> junctions;

    while (getline(*input, line)) {
        aoc2025::trim(line);
        junction_box b{};
        string::size_type p1 = 0, p2 = 0;
        p2 = line.find(',');
        b.x = stol(line.substr(0, p2));
        p1 = p2 + 1;
        p2 = line.find(',', p1);
        b.y = stol(line.substr(p1, p2));
        p1 = p2 + 1;
        b.z = stol(line.substr(p1));
        junctions.emplace_back(b);
    }

    // part 1

    list<unsigned long>::size_type answer1 = 0;

    vector<list<vector<junction_box>::size_type> *> circuits(junctions.size());
    vector<junction_box>::size_type pos = 0, pos2 = 0/*, idx = 0*/;
    auto map = new vector<junction_box>::size_type[(size_t) circuits.size()];
    fill(map, map + circuits.size(), -1);

    for (pos = 0; pos < junctions.size(); pos++) {
        auto l = new list<vector<junction_box>::size_type>();
        l->push_back(pos);
        map[pos] = pos;
        circuits[pos] = l;
    }

    unsigned long previous_min = 0;
    int i = 0;
    while (i < 1000) {
        unsigned long min = numeric_limits<unsigned long>::max();
        vector<junction_box>::size_type j1 = 0, j2 = 0;
        for (pos = 0; pos < junctions.size(); pos++) {
            for (pos2 = pos + 1; pos2 < junctions.size(); pos2++) {
                if (pos == pos2) {
                    continue;
                }
                unsigned long dist = distance(junctions[pos], junctions[pos2]);
                if (min > dist && dist > previous_min) {
                    j1 = pos;
                    j2 = pos2;
                    min = dist;
                }
            }
        }
        previous_min = min;
        i++;

        if (map[j1] == map[j2]) {
            // already in the same circuit
            continue;
        }

        list<unsigned long> *to = map[j1] < map[j2] ? circuits[map[j1]] : circuits[map[j2]];
        list<unsigned long> *from = map[j1] < map[j2] ? circuits[map[j2]] : circuits[map[j1]];
        for (auto &v: *from) {
            to->push_back(v);
            map[v] = map[j1] < map[j2] ? map[j1] : map[j2];
        }
        from->clear();
    }

    vector<list<unsigned long>::size_type> sizes;
    for (auto &l: circuits) {
        sizes.push_back(l->size());
    }
    sort(sizes.begin(), sizes.end(), [](list<unsigned long>::size_type s1, list<unsigned long>::size_type s2){
        return s1 > s2;
    });
    answer1 = sizes[0] * sizes[1] * sizes[2];

    // part 2

    long answer2 = 0;
    while (true) {
        unsigned long min = numeric_limits<unsigned long>::max();
        vector<junction_box>::size_type j1 = 0, j2 = 0;
        for (pos = 0; pos < junctions.size(); pos++) {
            for (pos2 = pos + 1; pos2 < junctions.size(); pos2++) {
                if (pos == pos2) {
                    continue;
                }
                unsigned long dist = distance(junctions[pos], junctions[pos2]);
                if (min > dist && dist > previous_min) {
                    j1 = pos;
                    j2 = pos2;
                    min = dist;
                }
            }
        }
        previous_min = min;
        i++;

        if (map[j1] == map[j2]) {
            // already in the same circuit
            continue;
        }

        list<unsigned long> *to = map[j1] < map[j2] ? circuits[map[j1]] : circuits[map[j2]];
        list<unsigned long> *from = map[j1] < map[j2] ? circuits[map[j2]] : circuits[map[j1]];
        for (auto &v: *from) {
            to->push_back(v);
            map[v] = map[j1] < map[j2] ? map[j1] : map[j2];
        }
        from->clear();

        int non_empty_count = 0;
        for (auto &l: circuits) {
            if (!l->empty()) {
                non_empty_count++;
            }
        }
        if (non_empty_count == 1) {
            cout << "non empty: " << non_empty_count << endl;
            answer2 = junctions[j1].x * junctions[j2].x;
            break;
        }
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    delete[] map;
    for (auto &l1: circuits) {
        delete l1;
    }

    return EXIT_SUCCESS;
}

unsigned long distance(junction_box &b1, junction_box &b2) {
    return (b1.x - b2.x) * (b1.x - b2.x) + (b1.y - b2.y) * (b1.y - b2.y) + (b1.z - b2.z) * (b1.z - b2.z);
}
