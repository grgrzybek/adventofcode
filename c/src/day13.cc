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

#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;
using namespace aoc2021;

int main(int argc, char *argv[]) {
    Options options("Day 13", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<pair<int, int>> coords;
    vector<string> folds;

    bool now_folds = false;
    while (getline(*input, line)) {
        trim(line);
        if (line.empty()) {
            now_folds = true;
            continue;
        }
        int x, y;
        char c;
        istringstream iss(line);
        iss >> x >> c >> y;

        if (!now_folds) {
            coords.emplace_back(x, y);
        } else {
            folds.push_back(line);
        }
    }

    int width = 0, height = 0;
    for (auto &p: coords) {
        if (p.first > width) {
            width = p.first;
        }
        if (p.second > height) {
            height = p.second;
        }
    }
    width++;
    height++;

    int sheet[width * height];
    memset(sheet, 0, width * height * sizeof(int));

    for (auto &p: coords) {
        sheet[p.second * width + p.first] = 1;
    }

    // part 1 & 2

    int answer1 = 0;
    char c;
    int l;
    int _width = width;

    for (auto &fold: folds) {
        sscanf(fold.c_str(), "fold along %c=%d", &c, &l);

        if (c == 'x') {
            for (int y = 0; y < height; y++) {
                for (int x = l + 1, _x = l - 1; x < _width; x++, _x--) {
                    sheet[y * width + _x] += sheet[y * width + x];
                }
            }
        } else if (c == 'y') {
            for (int y = l + 1, _y = l - 1; y < height; y++, _y--) {
                for (int x = 0; x < _width; x++) {
                    sheet[_y * width + x] += sheet[y * width + x];
                }
            }
        }

        if (answer1 == 0) {
            if (c == 'x') {
                for (int y = 0; y < height; y++) {
                    for (int x = 0; x < l; x++) {
                        if (sheet[y * width + x] > 0) {
                            answer1++;
                        }
                    }
                }
            } else if (c == 'y') {
                for (int i = 0; i < width * (height/2); i++) {
                    answer1 += sheet[i] > 0;
                }
            }
        }

        if (c == 'x') {
            _width = l;
        } else {
            height = l;
        }
    }

    cout << "after fold along " << c << "=" << l << " (w: " << _width << ", h:" << height << ")" << endl;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < _width; x++) {
            cout << (sheet[y * width + x] > 0 ? '#' : '.');
        }
        cout << endl;
    }

    cout << "Answer 1: " << answer1 << endl;

    return EXIT_SUCCESS;
}
