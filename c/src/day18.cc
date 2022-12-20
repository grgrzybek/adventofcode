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
#include <limits>
#include <queue>
#include <map>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

struct p3d {
    int x, y, z;

    p3d(int x, int y, int z) : x(x), y(y), z(z) {}

    bool operator==(const p3d &c2) const {
        bool res = x == c2.x && y == c2.y && z == c2.z;
        return res;
    }

    bool operator<(const p3d &c2) const {
        if (z != c2.z) {
            return z < c2.z;
        }
        if (y != c2.y) {
            return y < c2.y;
        }
        if (x != c2.x) {
            return x < c2.x;
        }
        return false;
    }
};

int bfs(char *cube, int w, int h, int d, int x, int y, int z) {
    queue<p3d> q;
    q.emplace(x, y, z);
    cube[(z * w * h) + (y * w) + x] = 's'; // for steam
    int answer = 0;
    char c;
    while (!q.empty()) {
        p3d p = q.front();
        q.pop();

        if (p.x > 0) {
            // check left
            c = cube[(p.z * w * h) + (p.y * w) + p.x - 1];
            if (c == '.') {
                q.emplace(p.x - 1, p.y, p.z);
                cube[(p.z * w * h) + (p.y * w) + p.x - 1] = 's';
            } else if (c == 'L') {
                answer++;
            }
        }
        if (p.x < w - 1) {
            // check right
            c = cube[(p.z * w * h) + (p.y * w) + p.x + 1];
            if (c == '.') {
                q.emplace(p.x + 1, p.y, p.z);
                cube[(p.z * w * h) + (p.y * w) + p.x + 1] = 's';
            } else if (c == 'L') {
                answer++;
            }
        }
        if (p.y > 0) {
            // check up
            c = cube[(p.z * w * h) + ((p.y - 1) * w) + p.x];
            if (c == '.') {
                q.emplace(p.x, p.y - 1, p.z);
                cube[(p.z * w * h) + ((p.y - 1) * w) + p.x] = 's';
            } else if (c == 'L') {
                answer++;
            }
        }
        if (p.y < h - 1) {
            // check down
            c = cube[(p.z * w * h) + ((p.y + 1) * w) + p.x];
            if (c == '.') {
                q.emplace(p.x, p.y + 1, p.z);
                cube[(p.z * w * h) + ((p.y + 1) * w) + p.x] = 's';
            } else if (c == 'L') {
                answer++;
            }
        }
        if (p.z > 0) {
            // check above
            c = cube[((p.z - 1) * w * h) + (p.y * w) + p.x];
            if (c == '.') {
                q.emplace(p.x, p.y, p.z - 1);
                cube[((p.z - 1) * w * h) + (p.y * w) + p.x] = 's';
            } else if (c == 'L') {
                answer++;
            }
        }
        if (p.z < d - 1) {
            // check below
            c = cube[((p.z + 1) * w * h) + (p.y * w) + p.x];
            if (c == '.') {
                q.emplace(p.x, p.y, p.z + 1);
                cube[((p.z + 1) * w * h) + (p.y * w) + p.x] = 's';
            } else if (c == 'L') {
                answer++;
            }
        }

        cube[(p.z * w * h) + (p.y * w) + p.x] = 'S'; // for steam
    }

    return answer;
}

int main(int argc, char *argv[]) {
    aoc2022::Options options("Day 18", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<p3d> coords;

    int x, y, z;
    while (getline(*input, line)) {
        aoc2022::trim(line);
        if (!aoc2022::has_text(line)) {
            continue;
        }
        size_t start = 0, pos = 0;
        while (pos < line.length()) {
            pos = line.find(',');
            x = stoi(line.substr(0, pos));
            start = pos + 1;
            pos = line.find(',', start);
            y = stoi(line.substr(start, pos - start));
            start = pos + 1;
            z = stoi(line.substr(start));
            coords.emplace_back(x, y, z);
            pos = line.length();
        }
    }

    // part 1

    int answer1 = 0;

    map<p3d, bool> space;

    pair<int, int> xx(numeric_limits<int>::max(), numeric_limits<int>::min());
    pair<int, int> yy(numeric_limits<int>::max(), numeric_limits<int>::min());
    pair<int, int> zz(numeric_limits<int>::max(), numeric_limits<int>::min());

    for (auto &c: coords) {
        answer1 += 6;

        if (space.contains(p3d(c.x, c.y, c.z - 1))) {
            answer1 -= 2;
        }
        if (space.contains(p3d(c.x, c.y, c.z + 1))) {
            answer1 -= 2;
        }
        if (space.contains(p3d(c.x, c.y - 1, c.z))) {
            answer1 -= 2;
        }
        if (space.contains(p3d(c.x, c.y + 1, c.z))) {
            answer1 -= 2;
        }
        if (space.contains(p3d(c.x - 1, c.y, c.z))) {
            answer1 -= 2;
        }
        if (space.contains(p3d(c.x + 1, c.y, c.z))) {
            answer1 -= 2;
        }

        if (xx.first > c.x) {
            xx.first = c.x;
        }
        if (xx.second < c.x) {
            xx.second = c.x;
        }
        if (yy.first > c.y) {
            yy.first = c.y;
        }
        if (yy.second < c.y) {
            yy.second = c.y;
        }
        if (zz.first > c.z) {
            zz.first = c.z;
        }
        if (zz.second < c.z) {
            zz.second = c.z;
        }

        space[c] = true;
    }

    // part 2

    int answer2 = 0;

    int w = xx.second - xx.first + 1 + 2; // width
    int h = yy.second - yy.first + 1 + 2; // height
    int d = zz.second - zz.first + 1 + 2; // depth


    char cube[w * h * d];
    memset(cube, '.', sizeof(char) * w * h * d);

    int dx = -xx.first;
    int dy = -yy.first;
    int dz = -zz.first;

    for (z = zz.first-1; z <= zz.second+1; z++) {
        for (y = yy.first-1; y <= yy.second+1; y++) {
            for (x = xx.first-1; x <= xx.second+1; x++) {
                if (space.contains(p3d(x, y, z))) {
                    cube[(h * w * (z + dz + 1)) + (w * (y + dy + 1)) + (x + dx + 1)] = 'L';
                }
            }
        }
    }

    answer2 = bfs(cube, w, h, d, 0, 0, 0);

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}
