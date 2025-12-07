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
#include <deque>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    aoc2025::Options options("Day 07", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<string> lines;
    int w = 0, h = 0;
    int x, y, sx, sy;

    while (getline(*input, line)) {
        aoc2025::trim(line);
        w = (int) line.length();
        lines.emplace_back(line);
        h++;
    }

    cout << w << "x" << h << endl;
    auto manifold = unique_ptr<char[]>(new char[w * h]);
    y = 0;

    for (auto &l: lines) {
        x = 0;
        for (auto &c: l) {
            manifold[y * w + x++] = c;
            if (y == 0 && c == 'S') {
                sx = x - 1;
                sy = y;
            }
        }
        y++;
    }

//    for (y = 0; y < h; y++) {
//        for (x = 0; x < w; x++) {
//            if (x == sx && y == sy) {
//                cout << '#';
//            } else {
//                cout << manifold[y * w + x];
//            }
//        }
//        cout << endl;
//    }

    // part 1

    long answer1 = 0;
    auto s1 = make_unique<set<pair<int, int>>>();
    auto s2 = make_unique<set<pair<int, int>>>();
    y = 2;
    s1->emplace(sx, sy + 2);
    while (y < h) {
        s2->clear();
        for (auto &p: *s1) {
            for (x = 0; x < w; x++) {
                if (x == p.first && y == p.second) {
                    if (manifold[y * w + x] == '^') {
                        s2->emplace(x - 1, y + 1);
                        s2->emplace(x + 1, y + 1);
                        answer1++;
                    } else {
                        s2->emplace(x, y + 1);
                    }
                }
            }
        }
        s1->clear();
        s1->insert(s2->cbegin(), s2->cend());
        y++;
    }

    // part 2

    long answer2 = 0;
    sy += 2;
    auto l1 = unique_ptr<long[]>(new long[w]);
    auto l2 = unique_ptr<long[]>(new long[w]);
    fill(l1.get(), l1.get() + w, 0L);
    l1[sx] = 1;
    y = 2;
    while (y < h) {
        fill(l2.get(), l2.get() + w, 0L);
        for (x = 0; x < w; x++) {
            if (manifold[y * w + x] == '^') {
                if (x > 0) {
                    l2[x - 1] += l1[x];
                }
                if (x < w - 1) {
                    l2[x + 1] += l1[x];
                }
            } else {
                l2[x] += l1[x];
            }
        }
        swap(l1, l2);
        y++;
    }
    for (x = 0; x < w; x++) {
        answer2 += l1[x];
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}
