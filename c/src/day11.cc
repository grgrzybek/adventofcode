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

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;
using namespace aoc2021;

int make_pool(int *map, int x, int y, int width, int height);

void flash_surrounding(int *map, int x, int y, int width, int height);

void print_map(int *map, int width, int height);

int main(int argc, char *argv[]) {
    Options options("Day 11", argc, argv);
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
        if (width == 0) {
            width = (int) line.length();
        }
        height++;
    }

    cout << "Size: " << width << "x" << height << endl;

    int *map1 = new int[width * height];
    int *map2 = new int[width * height];

    int row = 0;
    for (string &l: lines) {
        for (int col = 0; col < width; col++) {
            map1[row * width + col] = l[col] - '0';
        }
        row++;
    }

    // part 1 & 2

    int answer1 = 0;
    int answer2 = 0;

    for (int i = 0; i < 1200; i++) {
        for (int c = 0; c < width * height; c++) {
            map2[c] = map1[c] + 1;
        }
        cout << "========== After step " << i << " ==========" << endl;
        print_map(map1, width, height);
        while (true) {
            bool flash = false;
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    if (map2[y * width + x] > 9) {
                        flash = true;
                        flash_surrounding(map2, x, y, width, height);
                        if (i < 100) {
                            answer1++;
                        }
                    }
                }
            }
            if (!flash) {
                break;
            }
        }

        bool zero = true;
        for (int j = 0; j < width * height; j++) {
            if (map2[j] != 0) {
                zero = false;
                break;
            }
        }
        if (zero) {
            answer2 = i+1;
            break;
        }

        swap(map1, map2);
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}

void flash_surrounding(int *map, int x, int y, int width, int height) {
    if (x > 0) {
        // left column
        if (y > 0 && map[(y - 1) * width + x - 1] > 0) {
            // top left
            map[(y - 1) * width + x - 1]++;
        }
        // left
        if (map[y * width + x - 1] > 0) {
            map[y * width + x - 1]++;
        }
        if (y < height - 1 && map[(y + 1) * width + x - 1] > 0) {
            // bottom left
            map[(y + 1) * width + x - 1]++;
        }
    }
    // same column
    if (y > 0 && map[(y - 1) * width + x] > 0) {
        // top
        map[(y - 1) * width + x]++;
    }
    // this
    map[y * width + x] = 0;
    if (y < height - 1 && map[(y + 1) * width + x] > 0) {
        // bottom left
        map[(y + 1) * width + x]++;
    }
    if (x < width - 1) {
        // right column
        if (y > 0 && map[(y - 1) * width + x + 1] > 0) {
            // top right
            map[(y - 1) * width + x + 1]++;
        }
        // right
        if (map[y * width + x + 1] > 0) {
            map[y * width + x + 1]++;
        }
        if (y < height - 1 && map[(y + 1) * width + x + 1] > 0) {
            // bottom right
            map[(y + 1) * width + x + 1]++;
        }
    }
}

void print_map(int *map, int width, int height) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            cout << setw(1) << setfill(' ') << map[y*width + x];
        }
        cout << endl;
    }
}
