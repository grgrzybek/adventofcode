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
#include <deque>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

struct cheat {
    int x1 = 0, y1 = 0, x2 = 0, y2 = 0, x = 0, y = 0;
};

long track(const char *const &board, int w, int h, int sx, int sy, int ex, int ey);

int main(int argc, char *argv[]) {
    aoc2024::Options options("Day 20", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    int w = 0, h = 0;
    int sx = 0, sy = 0;
    int ex = 0, ey = 0;

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
        strncpy(board + (y * w), l.c_str(), w);
        y++;
    }

//    cout << "--- map ---\n";
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
//            cout << board[y * w + x];
            if (board[y * w + x] == 'S') {
                sx = x;
                sy = y;
                // to make life easier
                board[y * w + x] = '.';
            }
            if (board[y * w + x] == 'E') {
                ex = x;
                ey = y;
                // to make life easier
                board[y * w + x] = '.';
            }
        }
//        cout << endl;
    }
//    cout << "S: " << sx << "," << sy << endl;
//    cout << "E: " << ex << "," << ey << endl;

    // part 1

    long honest_cost = track(board, w, h, sx, sy, ex, ey);

    long answer1 = 0;

    vector<cheat> cheats;
    for (y = 1; y < h - 1; y++) {
        for (x = 1; x < w - 1; x++) {
//            cout << "checking " << x << "," << y << endl;
            if (board[y * w + x] == '#') {
                // potential cheat
                if (board[(y - 1) * w + x] == '.' && board[(y + 1) * w + x] == '.') {
                    cheats.emplace_back(cheat{
                        .x1 = x,
                        .y1 = y - 1,
                        .x2 = x,
                        .y2 = y + 1,
                        .x = x,
                        .y = y
                    });
                }
                if (board[y * w + x - 1] == '.' && board[y * w + x + 1] == '.') {
                    cheats.emplace_back(cheat{
                        .x1 = x - 1,
                        .y1 = y,
                        .x2 = x + 1,
                        .y2 = y,
                        .x = x,
                        .y = y
                    });
                }
            }
        }
    }

    for (auto &c: cheats) {
        // open the gate
        board[c.y * w + c.x] = '.';
        int cheated = track(board, w, h, sx, sy, ex, ey);
        if (honest_cost - cheated >= 100) {
            answer1++;
        }
        // close the gate
        board[c.y * w + c.x] = '#';
    }

    // part 2

    size_t answer2 = 0;

//    for (auto &p: seats) {
//        board[p.second * w + p.first] = 'O';
//    }
//
//    for (y = 0; y < h; y++) {
//        for (x = 0; x < w; x++) {
//            cout << board[y * w + x];
//        }
//        cout << endl;
//    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    free(board);

    return EXIT_SUCCESS;
}

long track(const char *const &board, int w, int h, int sx, int sy, int ex, int ey) {
    auto scores = (int *) malloc(sizeof(int) * w * h);
    fill(scores, scores + (w * h), numeric_limits<int>::max());
    scores[sy * w + sx] = 0;

//    cout << " ------- map -------- \n";
//    for (int y = 0; y < h; y++) {
//        for (int x = 0; x < w; x++) {
//            cout << board[y * w + x];
//        }
//        cout << endl;
//    }

    auto q = new deque<pair<int, int>>();
    q->emplace_front(sx, sy);

    while (!q->empty()) {
        auto xy = q->front();
        q->pop_front();

        if (xy.first == ex && xy.second == ey) {
            continue;
        }

        if (board[(xy.second - 1) * w + xy.first] == '.' && scores[(xy.second - 1) * w + xy.first] > scores[xy.second * w + xy.first] + 1) {
            // can go up
            scores[(xy.second - 1) * w + xy.first] = scores[xy.second * w + xy.first] + 1;
            q->emplace_back(xy.first, xy.second - 1);
        }
        if (board[(xy.second + 1) * w + xy.first] == '.' && scores[(xy.second + 1) * w + xy.first] > scores[xy.second * w + xy.first] + 1) {
            // can go down
            scores[(xy.second + 1) * w + xy.first] = scores[xy.second * w + xy.first] + 1;
            q->emplace_back(xy.first, xy.second + 1);
        }
        if (board[xy.second * w + xy.first - 1] == '.' && scores[xy.second * w + xy.first - 1] > scores[xy.second * w + xy.first] + 1) {
            // can go left
            scores[xy.second * w + xy.first - 1] = scores[xy.second * w + xy.first] + 1;
            q->emplace_back(xy.first - 1, xy.second);
        }
        if (board[xy.second * w + xy.first + 1] == '.' && scores[xy.second * w + xy.first + 1] > scores[xy.second * w + xy.first] + 1) {
            // can go right
            scores[xy.second * w + xy.first + 1] = scores[xy.second * w + xy.first] + 1;
            q->emplace_back(xy.first + 1, xy.second);
        }
    }

    int res = scores[ey * w + ex];
    free(scores);
    delete q;

    return res;
}
