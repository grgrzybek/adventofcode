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
#include <limits>
#include <list>
#include <set>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;
using namespace aoc2021;

void calculate_distances(const int *map, int *distances, int width, int height, int start, list<int> &to_check,
                         set<int> &to_check_set);

int main(int argc, char *argv[]) {
    Options options("Day 15", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<string> lines;
    int width = 0;
    int height = 0;

    while (getline(*input, line)) {
        trim(line);
        lines.push_back(line);
        height++;
        if (width == 0) {
            width = (int) line.length();
        }
    }

    cout << width << "x" << height << endl;
    int map[width * height];

    int x = 0, y = 0;
    for (auto &l: lines) {
        x = 0;
        for (auto &c: l) {
            map[y * width + x++] = c - '0';
        }
        y++;
    }

    // part 1

    set<pair<int, int>> visited;
    visited.emplace(make_pair(0, 0));

    // map of distances (total risks starting from 0,0)
    int distances[width * height];
    for (int i = 0; i < width * height; i++) {
        distances[i] = numeric_limits<int>::max();
    }
    distances[0] = 0;

    set<int> to_check_set;
    to_check_set.emplace(0);
    list<int> to_check;
    to_check.push_back(0);
    int distance = 1;
    while (!to_check.empty()) {
        calculate_distances(map, distances, width, height, distance < width ? distance++ : -1, to_check, to_check_set);
    }

    int answer1 = distances[width * height - 1];

    // part 2

    int width2 = width * 5;
    int height2 = height * 5;
    int map2[width2 * height2];

    x = 0, y = 0;
    int v, v1;
    for (auto &l: lines) {
        x = 0;
        for (auto &c: l) {
            v = c - '0';
            for (int y1 = 0; y1 < 5; y1++) {
                for (int x1 = 0; x1 < 5; x1++) {
                    v1 = v + y1 + x1;
                    while (v1 > 9) {
                        v1 -= 9;
                    }
                    map2[(y + height * y1) * width2 + (x + width * x1)] = v1;
                }
            }
            x++;
        }
        y++;
    }

    int *distances2 = new int[width2 * height2];
    for (int i = 0; i < width2 * height2; i++) {
        distances2[i] = numeric_limits<int>::max();
    }
    distances2[0] = 0;

    to_check_set.clear();
    to_check_set.emplace(0);
    to_check.clear();
    to_check.push_back(0);
    distance = 1;
    while (!to_check.empty()) {
        calculate_distances(map2, distances2, width2, height2, distance < width2 ? distance++ : -1, to_check, to_check_set);
    }

    int answer2 = distances2[width2 * height2 - 1];

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    delete[] distances2;

    return EXIT_SUCCESS;
}

void calculate_distances(const int *map, int *distances, int width, int height, int start, list<int> &to_check,
                         set<int> &to_check_set) { // NOLINT(misc-no-recursion)
    if (to_check.empty()) {
        return;
    }
    int xy = *(to_check.begin());
    int x = xy >> 16;
    int y = xy & 0xFFFF;

    to_check.pop_front();
    to_check_set.erase(xy);

    // first, if needed, add right and bottom points to check
    if (start >= 0 && start < width) {
        for (int _y = 0; _y <= start; _y++) {
            int v = start << 16 | _y;
            if (!to_check_set.contains(v)) {
                to_check.push_back(v);
                to_check_set.emplace(v);
            }
        }
        for (int _x = 0; _x <= start; _x++) {
            int v = _x << 16 | start;
            if (!to_check_set.contains(v)) {
                to_check.push_back(v);
                to_check_set.emplace(v);
            }
        }
    }

    // change surronding points

    int d = distances[width * y + x];
    if (x > 0) {
        int risk_left = map[width * y + x - 1];
        int distance_left = distances[width * y + x - 1];
        if (distance_left > risk_left + d) {
            distances[width * y + x - 1] = risk_left + d;
            int v = (x - 1) << 16 | y;
            if (!to_check_set.contains(v)) {
                to_check.push_back(v);
                to_check_set.emplace(v);
            }
        }
    }
    if (x < width - 1) {
        int risk_right = map[width * y + x + 1];
        int distance_right = distances[width * y + x + 1];
        if (distance_right > risk_right + d) {
            distances[width * y + x + 1] = risk_right + d;
            int v = (x + 1) << 16 | y;
            if (!to_check_set.contains(v)) {
                to_check.push_back(v);
                to_check_set.emplace(v);
            }
        }
    }
    if (y > 0) {
        int risk_top = map[width * (y - 1) + x];
        int distance_top = distances[width * (y - 1) + x];
        if (distance_top > risk_top + d) {
            distances[width * (y - 1) + x] = risk_top + d;
            int v = x << 16 | (y - 1);
            if (!to_check_set.contains(v)) {
                to_check.push_back(v);
                to_check_set.emplace(v);
            }
        }
    }
    if (y < height - 1) {
        int risk_bottom = map[width * (y + 1) + x];
        int distance_bottom = distances[width * (y + 1) + x];
        if (distance_bottom > risk_bottom + d) {
            distances[width * (y + 1) + x] = risk_bottom + d;
            int v = x << 16 | (y + 1);
            if (!to_check_set.contains(v)) {
                to_check.push_back(v);
                to_check_set.emplace(v);
            }
        }
    }
}
