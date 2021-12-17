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
#include <set>
#include <string>
#include <vector>

#include "utils/utils.h"

void calculate_distances_from(int map[], int distances[], int width, int height, int x, int y);

using namespace std;
using namespace aoc2021;

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

    calculate_distances_from(map, distances, width, height, 0, 0);

    int answer1 = distances[width * height - 1];

    // part 2

    int answer2 = 0;

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}

// This function is always called for a point with a distance (total risk) already set
void calculate_distances_from(int map[], int distances[], int width, int height, int x, int y) { // NOLINT(misc-no-recursion)
    int d = distances[width * y + x];
    if (x > 0) {
        int risk_left = map[width * y + x - 1];
        int distance_left = distances[width * y + x - 1];
        if (distance_left > risk_left + d) {
            distances[width * y + x - 1] = risk_left + d;
            calculate_distances_from(map, distances, width, height, x - 1, y);
        }
    }
    if (x < width - 1) {
        int risk_right = map[width * y + x + 1];
        int distance_right = distances[width * y + x + 1];
        if (distance_right > risk_right + d) {
            distances[width * y + x + 1] = risk_right + d;
            calculate_distances_from(map, distances, width, height, x + 1, y);
        }
    }
    if (y > 0) {
        int risk_top = map[width * (y - 1) + x];
        int distance_top = distances[width * (y - 1) + x];
        if (distance_top > risk_top + d) {
            distances[width * (y - 1) + x] = risk_top + d;
            calculate_distances_from(map, distances, width, height, x, y - 1);
        }
    }
    if (y < height - 1) {
        int risk_bottom = map[width * (y + 1) + x];
        int distance_bottom = distances[width * (y + 1) + x];
        if (distance_bottom > risk_bottom + d) {
            distances[width * (y + 1) + x] = risk_bottom + d;
            calculate_distances_from(map, distances, width, height, x, y + 1);
        }
    }
}
