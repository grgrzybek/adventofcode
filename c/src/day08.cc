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

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "utils/utils.h"

unsigned long calculate_scenic_score(const int *forest, int row, int column, int w, int h);

using namespace std;

int main(int argc, char *argv[]) {
    aoc2022::Options options("Day 08", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<string> forest_map;
    size_t w = 0, h = 0;

    while (getline(*input, line)) {
        forest_map.push_back(line);
        w = line.length();
        h++;
    }
    int forest[w * h];
    bool hidden[w][h];
    std::memset(forest, 0, sizeof(int) * w * h);
    std::memset(hidden, 1, sizeof(bool) * w * h);

    for (size_t r = 0; r < h; r++) {
        for (size_t c = 0; c < w; c++) {
            forest[r * w + c] = forest_map[r][c] - '0';
        }
    }

    // part 1

    unsigned long answer1 = 0;
    int ht = 0;

    for (size_t r = 1; r < h - 1; r++) {
        ht = forest[r * w];
        for (size_t c = 1; c < w - 1; c++) {
            if (forest[r * w + c] > ht) {
                hidden[c][r] = false;
                ht = forest[r * w + c];
            }
        }
        ht = forest[r * w + w - 1];
        for (size_t c = w - 2; c > 0; c--) {
            if (forest[r * w + c] > ht) {
                hidden[c][r] = false;
                ht = forest[r * w + c];
            }
        }
    }
    for (size_t c = 1; c < w - 1; c++) {
        ht = forest[c];
        for (size_t r = 1; r < h - 1; r++) {
            if (forest[r * w + c] > ht) {
                hidden[c][r] = false;
                ht = forest[r * w + c];
            }
        }
        ht = forest[(h - 1) * w + c];
        for (size_t r = h - 2; r > 0; r--) {
            if (forest[r * w + c] > ht) {
                hidden[c][r] = false;
                ht = forest[r * w + c];
            }
        }
    }
    for (size_t r = 1; r < h - 1; r++) {
        for (size_t c = 1; c < w - 1; c++) {
            if (!hidden[c][r]) {
                answer1++;
            }
        }
    }
    answer1 += 2 * w + 2 * h - 4;

    // part 2

    unsigned long answer2 = 0;

    for (size_t r = 0; r < h; r++) {
        for (size_t c = 0; c < w; c++) {
            unsigned long distance = calculate_scenic_score(forest, (int) r, (int) c, (int) w, (int) h);
            if (distance > answer2) {
                answer2 = distance;
            }
        }
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}

unsigned long calculate_scenic_score(const int *forest, int row, int column, int w, int h) {
    int score = 1;
    int dir_score = 0;
    int tc = 0;
    int tree = forest[row * w + column];
    for (int left = column - 1; left >= 0; left--) {
        tc++;
        dir_score++;
        if (forest[row * w + left] >= tree) {
            break;
        }
    }
    score *= dir_score;
    dir_score = 0;
    for (int right = column + 1; right < w; right++) {
        tc++;
        dir_score++;
        if (forest[row * w + right] >= tree) {
            break;
        }
    }
    score *= dir_score;
    dir_score = 0;
    for (int  up = row - 1; up >= 0; up--) {
        tc++;
        dir_score++;
        if (forest[up * w + column] >= tree) {
            break;
        }
    }
    score *= dir_score;
    dir_score = 0;
    for (int down = row + 1; down < h; down++) {
        tc++;
        dir_score++;
        if (forest[down * w + column] >= tree) {
            break;
        }
    }
    score *= dir_score;

    return tc == 0 ? 0 : score;
}
