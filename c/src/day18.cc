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
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <numeric>

#include "utils/utils.h"

using namespace std;

struct instruction {
    int dx, dy;

    instruction(int _dx, int _dy) : dx(_dx), dy(_dy) {}
};

int main(int argc, char *argv[]) {
    aoc2023::Options options("Day 18", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<instruction> lines;

    int w = 0, h = 0;
    int x, y, dx, dy;
    int x1 = numeric_limits<int>::max();
    int x2 = numeric_limits<int>::min();
    int y1 = numeric_limits<int>::max();
    int y2 = numeric_limits<int>::min();

    char dir;
    int v;
    while (getline(*input, line)) {
        istringstream iss(line);
        iss >> dir;
        iss >> v;
        // R 6 (#70c710)
        // D 5 (#0dc571)
        // L 2 (#5713f0)
        // D 2 (#d2c081)
        if (dir == 'U') {
            lines.emplace_back(0, -v);
        } else if (dir == 'D') {
            lines.emplace_back(0, v);
        } else if (dir == 'R') {
            lines.emplace_back(v, 0);
        } else if (dir == 'L') {
            lines.emplace_back(-v, 0);
        }
    }

    x = 0;
    y = 0;
    for (auto &in: lines) {
        x += in.dx;
        y += in.dy;

        if (x < x1) {
            x1 = x;
        }
        if (x > x2) {
            x2 = x;
        }
        if (y < y1) {
            y1 = y;
        }
        if (y > y2) {
            y2 = y;
        }
    }
    w = x2 - x1 + 1 + 2;
    h = y2 - y1 + 1 + 2;

    cout << x1 << ":" << y1 << " - " << x2 << ":" << y2 << endl;
    cout << "w = " << w << ", h = " << h << endl;

    char *map = (char *) malloc(sizeof(char) * w * h);
    memset(map, '.', w * h);

    // part 1

    long answer1 = 0;

    x = -x1 + 1;
    y = -y1 + 1;

    map[y * w + x] = '#';
    for (auto &in: lines) {
        if (in.dx != 0) {
            dx = in.dx > 0 ? 1 : -1;
            x1 = x;
            x2 = x + in.dx + dx;
            for (x = x1; x != x2; x += dx) {
                map[y * w + x] = '#';
            }
            x -= dx;
        } else {
            dy = in.dy > 0 ? 1 : -1;
            y1 = y;
            y2 = y + in.dy + dy;
            for (y = y1; y != y2; y += dy) {
                map[y * w + x] = '#';
            }
            y -= dy;
        }
    }

//    for (y = 0; y < h; y++) {
//        for (x = 0; x < w; x++) {
//            auto c = map[y * w + x];
//            cout << c;
//        }
//        cout << endl;
//    }

    deque<pair<int, int>> q;
    q.emplace_back(0, 0);
    while (!q.empty()) {
        auto p = q.back();
        q.pop_back();

        x = p.first;
        y = p.second;
        map[y * w + x] = '~';

        if (x > 0 && map[y * w + x - 1] == '.') {
            q.emplace_back(x - 1, y);
        }
        if (y > 0 && map[(y - 1) * w + x] == '.') {
            q.emplace_back(x, y - 1);
        }
        if (x < w - 1 && map[y * w + x + 1] == '.') {
            q.emplace_back(x + 1, y);
        }
        if (y < h - 1 && map[(y + 1) * w + x] == '.') {
            q.emplace_back(x, y + 1);
        }
    }

//    for (y = 0; y < h; y++) {
//        for (x = 0; x < w; x++) {
//            auto c = map[y * w + x];
//            cout << c;
//        }
//        cout << endl;
//    }

    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            if (map[y * w + x] != '~') {
                answer1++;
            }
        }
    }

    // part 2

    long answer2 = 0;

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    free(map);

    return EXIT_SUCCESS;
}
