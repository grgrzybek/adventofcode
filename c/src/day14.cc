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
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>
#include <numeric>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    aoc2023::Options options("Day 14", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<string> lines;

    char *map;
    int w, h = 0;

    while (getline(*input, line)) {
        aoc2023::trim(line);
        w = line.length();
        h++;
        lines.emplace_back(line);
    }

    map = (char *) malloc(sizeof(char) * w * h + 1);
    map[w * h] = 0;
    for (int y = 0; y < h; y++) {
        strncpy(map + (y * w), lines[y].c_str(), w);
    }

    // part 1

    cout << "Before" << endl;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            cout << map[y * w + x];
        }
        cout << endl;
    }

    unsigned long answer1 = 0;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (map[y * w + x] == 'O') {
                // move it north/up
                for (int yy = y - 1; yy >= 0; yy--) {
                    if (map[yy * w + x] == '.') {
                        map[yy * w + x] = 'O';
                        map[(yy + 1) * w + x] = '.';
                    } else {
                        break;
                    }
                }
            }
        }
    }

    cout << "\nAfter" << endl;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            cout << map[y * w + x];
        }
        cout << endl;
    }

    for (int y = 0, l = h; y < h; y++, l--) {
        for (int x = 0; x < w; x++) {
            if (map[y * w + x] == 'O') {
                answer1 += l;
            }
        }
    }

    // part 2

    unsigned long answer2 = 0;

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    free(map);

    return EXIT_SUCCESS;
}
