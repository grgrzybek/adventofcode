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
#include <set>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

int travel(const char *board, int w, int h, int x, int y, int value, set<pair<int, int>> *s);

int main(int argc, char *argv[]) {
    aoc2024::Options options("Day 10", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    int w = 0, h = 0;

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
    int y = 0;
    int x = 0;
    for (auto &l: lines) {
        strncpy(board + (y++ * w), l.c_str(), w);
    }

    vector<pair<int, int>> starts;

    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            if (board[y * w + x] == '0') {
                starts.emplace_back(x, y);
            }
//            cout << board[y * w + x];
        }
//        cout << endl;
    }

    // part 1

    int answer1 = 0;

    for (auto &p: starts) {
        auto s = new set<pair<int, int>>();
        cout << "starting from " << p.first << ", " << p.second << endl;
        travel(board, w, h, p.first, p.second, 0, s);
        answer1 += (int) s->size();
        delete s;
    }

    // part 2

    int answer2 = 0;

    for (auto &p: starts) {
        auto s = new set<pair<int, int>>();
        cout << "starting from " << p.first << ", " << p.second << endl;
        answer2 += travel(board, w, h, p.first, p.second, 0, s);
        delete s;
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    free(board);

    return EXIT_SUCCESS;
}

int travel(const char *board, int w, int h, int x, int y, int value, set<pair<int, int>> *s) {
    cout << " - at " << x << ", " << y << " (value: " << value << ")" << endl;
    if (value == 9) {
        cout << "ending at " << x << ", " << y << endl;
        s->emplace(x, y);
        return 1;
    }
    int result = 0;
    if (y > 0 && board[(y - 1) * w + x] - '0' == value + 1) {
        // up
        cout << "can go up. value: " << value << ", x: " << x << ", y: " << y << endl;
        result += travel(board, w, h, x, y - 1, value + 1, s);
    }
    if (y < h - 1 && board[(y + 1) * w + x] - '0' == value + 1) {
        // down
        cout << "can go down. value: " << value << ", x: " << x << ", y: " << y << endl;
        result += travel(board, w, h, x, y + 1, value + 1, s);
    }
    if (x > 0 && board[y * w + x - 1] - '0' == value + 1) {
        // left
        cout << "can go left. value: " << value << ", x: " << x << ", y: " << y << endl;
        result += travel(board, w, h, x - 1, y, value + 1, s);
    }
    if (x < w - 1 && board[y * w + x + 1] - '0' == value + 1) {
        // right
        cout << "can go right. value: " << value << ", x: " << x << ", y: " << y << endl;
        result += travel(board, w, h, x + 1, y, value + 1, s);
    }
    return result;
}
