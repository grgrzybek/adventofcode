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

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    aoc2024::Options options("Day 06", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    int w = 0, h = 0;
    int x, y;
    int x1, y1;

    vector<string> lines;

    while (getline(*input, line)) {
        aoc2024::trim(line);
        if (line.empty()) {
            break;
        }
        w = static_cast<int>(line.length());
        h++;
        lines.emplace_back(line);
    }

    char *board = (char *) malloc(w * h);
    y = 0;
    for (auto &l: lines) {
        strncpy(board + (y++ * w), l.c_str(), w);
    }
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            if (board[y * w + x] == '^') {
                x1 = x;
                y1 = y;
            }
            cout << board[y * w + x];
        }
        cout << endl;
    }

    // part 1

    int answer1 = 0;

    int dx = 0;
    int dy = -1;
    while (true) {
        board[y1 * w + x1] = 'o';
        if (dx == 0 && dy == -1 && y1 == 0) {
            // up
            break;
        }
        if (dx == 1 && dy == 0 && x1 == w - 1) {
            // right
            break;
        }
        if (dx == 0 && dy == 1 && y1 == h - 1) {
            // down
            break;
        }
        if (dx == -1 && dy == 0 && x1 == 0) {
            // left
            break;
        }
        if (board[(y1 + dy) * w + x1 + dx] == '#') {
            // turn right
            if (dx == 0) {
                dx = -dy;
                dy = 0;
            } else {
                dy = dx;
                dx = 0;
            }
        } else {
            // move
            x1 += dx;
            y1 += dy;
        }
    }

    for (int i = 0; i < w * h; i++) {
        if (board[i] == 'o') {
            answer1++;
        }
    }

    // part 2

    int answer2 = 0;

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    free(board);

    return EXIT_SUCCESS;
}
