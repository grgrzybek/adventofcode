/*
 * Copyright 2024 Grzegorz Grzybek
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

const char *XMAS = "XMAS";

int checkXMAS(const char *board, int x, int y, int w, int h);

int checkX_MAS(const char *board, int x, int y, int w, int h);

int checkDirection(const char *board, int x, int y, int w, int h, int dx, int dy);

int main(int argc, char *argv[]) {
    aoc2024::Options options("Day 04", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    int w = 0, h = 0;
    int x, y;

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
//    for (y = 0; y < h; y++) {
//        for (x = 0; x < w; x++) {
//            cout << board[y * w + x];
//        }
//        cout << endl;
//    }

    // part 1

    int answer1 = 0;

    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            answer1 += checkXMAS(board, x, y, w, h);
        }
    }

    // part 2

    int answer2 = 0;

    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            answer2 += checkX_MAS(board, x, y, w, h);
        }
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    free(board);

    return EXIT_SUCCESS;
}

int checkXMAS(const char *board, int x, int y, int w, int h) {
    if (board[y * w + x] != 'X') {
        return 0;
    }
    int result = 0;
    if (x <= w - 4) {
        // can check right
        result += checkDirection(board, x, y, w, h, 1, 0);

        if (y >= 3) {
            // can check right top
            result += checkDirection(board, x, y, w, h, 1, -1);
        }
        if (y <= h - 4) {
            // can check right down
            result += checkDirection(board, x, y, w, h, 1, 1);
        }
    }
    if (y >= 3) {
        // can check top
        result += checkDirection(board, x, y, w, h, 0, -1);
    }
    if (y <= h - 4) {
        // can check down
        result += checkDirection(board, x, y, w, h, 0, 1);
    }
    if (x >= 3) {
        // can check left
        result += checkDirection(board, x, y, w, h, -1, 0);

        if (y >= 3) {
            // can check left top
            result += checkDirection(board, x, y, w, h, -1, -1);
        }
        if (y <= h - 4) {
            // can check left down
            result += checkDirection(board, x, y, w, h, -1, 1);
        }
    }

    return result;
}


int checkX_MAS(const char *board, int x, int y, int w, int h) {
    int result = 0;
    if (x <= w - 3 && y <= h - 3) {
        if (board[(y + 1) * w + x + 1] != 'A') {
            return 0;
        }

        // can check
        // M.S
        // .A.
        // M.S
        if (board[y * w + x] == 'M' && board[(y + 2) * w + x] == 'M') {
            if (board[y * w + x + 2] == 'S' && board[(y + 2) * w + x + 2] == 'S') {
                result++;
            }
        }

        // or
        // M.M
        // .A.
        // S.S
        if (board[y * w + x] == 'M' && board[y * w + x + 2] == 'M') {
            if (board[(y + 2) * w + x] == 'S' && board[(y + 2) * w + x + 2] == 'S') {
                result++;
            }
        }

        // or
        // S.M
        // .A.
        // S.M
        if (board[y * w + x + 2] == 'M' && board[(y + 2) * w + x + 2] == 'M') {
            if (board[y * w + x] == 'S' && board[(y + 2) * w + x] == 'S') {
                result++;
            }
        }

        // or
        // S.S
        // .A.
        // M.M
        if (board[(y + 2) * w + x] == 'M' && board[(y + 2) * w + x + 2] == 'M') {
            if (board[y * w + x] == 'S' && board[y * w + x + 2] == 'S') {
                result++;
            }
        }
    }

    return result;
}

int checkDirection(const char *board, int x, int y, int w, int h, int dx, int dy) {
    int result = 1;
    int _x = x, _y = y;
    for (int i = 0; i < 4; i++, _x += dx, _y += dy) {
        if (XMAS[i] != board[_y * w + _x]) {
            result = 0;
            break;
        }
    }
    return result;
}
