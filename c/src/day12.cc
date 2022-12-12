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
#include <deque>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

#include "utils/utils.h"


using namespace std;

void bfs(queue<pair<int, int>> &q, int *distance, const char *map, size_t w, size_t h);

int main(int argc, char *argv[]) {
    aoc2022::Options options("Day 12", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<string> map_data;
    size_t w = 0, h;

    while (getline(*input, line)) {
        map_data.emplace_back(line);
        if (w == 0) {
            w = line.length();
        }
    }
    h = map_data.size();

    size_t yS = -1, xS = -1;
    size_t xE = -1, yE = -1;
    char map[w * h];
    size_t row = 0, col = 0;
    for (auto &l: map_data) {
        col = 0;
        for (char c: l) {
            if (c == 'S') {
                xS = col;
                yS = row;
                c = 'a';
            }
            if (c == 'E') {
                xE = col;
                yE = row;
                c = 'z';
            }
            map[row * w + col++] = c;
        }
        row++;
    }

    // part 1

    int distance[w * h];

    queue<pair<int, int>> q;
    for (size_t i = 1; i < w * h; i++) {
        distance[i] = numeric_limits<int>::max();
    }
    distance[yS * w + xS] = 0;
    q.emplace(xS, yS);
    bfs(q, distance, map, w, h);

    int answer1 = distance[yE * w + xE];

    // part 2

    int answer2 = numeric_limits<int>::max();

    vector<pair<int, int>> lowest_points;
    for (row = 0; row < h; row++) {
        for (col = 0; col < w; col++) {
            if (map[row * w + col] == 'a') {
                lowest_points.emplace_back(col, row);
            }
        }
    }
    for (auto &p: lowest_points) {
        while (!q.empty()) {
            q.pop();
        }
        for (size_t i = 0; i < w * h; i++) {
            distance[i] = numeric_limits<int>::max();
        }
        distance[p.second * w + p.first] = 0;
        q.emplace(p.first, p.second);
        bfs(q, distance, map, w, h);
        if (distance[yE * w + xE] < answer2) {
            answer2 = distance[yE * w + xE];
        }
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}

void bfs(queue<pair<int, int>> &q, int *distance, const char *map, size_t w, size_t h) {
    while (!q.empty()) {
        auto xy = q.front();
        q.pop();

        size_t x = xy.first, y = xy.second;
        int f1 = map[y * w + x] - 'a';
        int f2 = -1;

        int current_distance = distance[y * w + x];

        if (y > 0) {
            // can go up
            f2 = map[(y - 1) * w + x] - 'a';
            if (f2 - f1 <= 1 && distance[(y - 1) * w + x] > distance[y * w + x] + 1) {
                distance[(y - 1) * w + x] = current_distance + 1;
                q.emplace(x, y - 1);
            }
        }
        if (y < h - 1) {
            // can go down
            f2 = map[(y + 1) * w + x] - 'a';
            if (f2 - f1 <= 1 && distance[(y + 1) * w + x] > distance[y * w + x] + 1) {
                distance[(y + 1) * w + x] = current_distance + 1;
                q.emplace(x, y + 1);
            }
        }
        if (x > 0) {
            // can go left
            f2 = map[y * w + x - 1] - 'a';
            if (f2 - f1 <= 1 && distance[y * w + x - 1] > distance[y * w + x] + 1) {
                distance[y * w + x - 1] = current_distance + 1;
                q.emplace(x - 1, y);
            }
        }
        if (x < w - 1) {
            // can go right
            f2 = map[y * w + x + 1] - 'a';
            if (f2 - f1 <= 1 && distance[y * w + x + 1] > distance[y * w + x] + 1) {
                distance[y * w + x + 1] = current_distance + 1;
                q.emplace(x + 1, y);
            }
        }
    }
}
