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
#include <iostream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    aoc2025::Options options("Day 04", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<string> lines;

    int w, h = 0;
    int x, y;

    while (getline(*input, line)) {
        aoc2025::trim(line);
        lines.push_back(line);
        w = (int) line.length();
        h++;
    }

    char *storage = new char[w * h];

    x = 0; y = 0;
    for (auto &l: lines) {
        for (x = 0; x < (int) l.length(); x++) {
            storage[y * w + x] = l[x];
        }
        y++;
    }

    cout << w << " x " << h << endl;

    // part 1

    long answer1 = 0;
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            if (storage[y * w + x] != '@') {
                continue;
            }
            int c = 0;
            if (y > 0) {
                if (x > 0 && storage[(y - 1) * w + x - 1] == '@') {
                    c++;
                }
                if (storage[(y - 1) * w + x] == '@') {
                    c++;
                }
                if (x < w - 1 && storage[(y - 1) * w + x + 1] == '@') {
                    c++;
                }
            }
            if (x > 0 && storage[y * w + x - 1] == '@') {
                c++;
            }
            if (x < w - 1 && storage[y * w + x + 1] == '@') {
                c++;
            }
            if (y < h - 1) {
                if (x > 0 && storage[(y + 1) * w + x - 1] == '@') {
                    c++;
                }
                if (storage[(y + 1) * w + x] == '@') {
                    c++;
                }
                if (x < w - 1 && storage[(y + 1) * w + x + 1] == '@') {
                    c++;
                }
            }
            if (c < 4) {
                answer1++;
            }
        }
    }

    // part 2

    long answer2 = 0;
    bool change = true;
    while (change) {
        change = false;
        vector<pair<int, int>> to_remove;
        for (y = 0; y < h; y++) {
            for (x = 0; x < w; x++) {
                if (storage[y * w + x] != '@') {
                    continue;
                }
                int c = 0;
                if (y > 0) {
                    if (x > 0 && storage[(y - 1) * w + x - 1] == '@') {
                        c++;
                    }
                    if (storage[(y - 1) * w + x] == '@') {
                        c++;
                    }
                    if (x < w - 1 && storage[(y - 1) * w + x + 1] == '@') {
                        c++;
                    }
                }
                if (x > 0 && storage[y * w + x - 1] == '@') {
                    c++;
                }
                if (x < w - 1 && storage[y * w + x + 1] == '@') {
                    c++;
                }
                if (y < h - 1) {
                    if (x > 0 && storage[(y + 1) * w + x - 1] == '@') {
                        c++;
                    }
                    if (storage[(y + 1) * w + x] == '@') {
                        c++;
                    }
                    if (x < w - 1 && storage[(y + 1) * w + x + 1] == '@') {
                        c++;
                    }
                }
                if (c < 4) {
                    to_remove.emplace_back(x, y);
                }
            }
        }
        if (!to_remove.empty()) {
            change = true;
            answer2 += (int) to_remove.size();
            for (auto &p: to_remove) {
                storage[p.second * w + p.first] = '.';
            }
        }
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}
