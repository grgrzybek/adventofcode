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
#include <set>
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
    int x1, y1, x2, y2;

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
    char *board2_initial = (char *) malloc(w * h);
    int x = 0, y = 0;
    for (auto &l: lines) {
        strncpy(board + (y * w), l.c_str(), w);
        strncpy(board2_initial + (y++ * w), l.c_str(), w);
    }
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            if (board[y * w + x] == '^') {
                x1 = x;
                y1 = y;
                x2 = x;
                y2 = y;
            }
//            cout << board[y * w + x];
        }
//        cout << endl;
    }

    // part 1

    int answer1 = 0;

    set<pair<int, int>> coords;

    int dx = 0;
    int dy = -1;
    while (true) {
        board[y1 * w + x1] = 'o';
        if (!(x1 == x2 && y1 == y2)) {
            coords.insert(make_pair(x1, y1));
        }
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

    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            if (board2_initial[y * w + x] == '^' || board2_initial[y * w + x] == '.') {
                board2_initial[y * w + x] = '\0';
            }
            if (board2_initial[y * w + x] == '\0') {
                cout << '~';
            } else {
                cout << board2_initial[y * w + x];
            }
        }
        cout << endl;
    }

    for (auto &p: coords) {
        char *board2 = (char *) malloc(w * h);
        memcpy(board2, board2_initial, w * h * sizeof(char));
        board2[p.second * w + p.first] = '#';

        dx = 0;
        dy = -1;
        // reuse x1, y1 variables - starting from remembered initial x2, y2
        x1 = x2;
        y1 = y2;
        bool loop = true;
        while (true) {
            // was the guard here going in the same direction?
            // 0x00001000 - north
            // 0x00000100 - east
            // 0x00000010 - south
            // 0x00000001 - west
            char mark = board2[y1 * w + x1];
            if (dx == 0 && dy == -1) {
                if ((mark & 0x00001000) == 0x00001000) {
                    break;
                } else {
                    board2[y1 * w + x1] |= (char) 0x00001000;
                }
            }
            if (dx == 0 && dy == 1) {
                if ((mark & 0x00000010) == 0x00000010) {
                    break;
                } else {
                    board2[y1 * w + x1] |= (char) 0x00000010;
                }
            }
            if (dx == 1 && dy == 0) {
                if ((mark & 0x00000100) == 0x00000100) {
                    break;
                } else {
                    board2[y1 * w + x1] |= (char) 0x00000100;
                }
            }
            if (dx == -1 && dy == 0) {
                if ((mark & 0x00000001) == 0x00000001) {
                    break;
                } else {
                    board2[y1 * w + x1] |= (char) 0x00000001;
                }
            }
            if (dx == 0 && dy == -1 && y1 == 0) {
                // go out up
                loop = false;
                break;
            }
            if (dx == 1 && dy == 0 && x1 == w - 1) {
                // go out right
                loop = false;
                break;
            }
            if (dx == 0 && dy == 1 && y1 == h - 1) {
                // go out down
                loop = false;
                break;
            }
            if (dx == -1 && dy == 0 && x1 == 0) {
                // go out left
                loop = false;
                break;
            }
            if (!loop) {
                break;
            }
            if (board2[(y1 + dy) * w + x1 + dx] == '#') {
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

        if (loop) {
//            cout << "loop for " << p.first << ":" << p.second << endl;
            answer2++;
//            for (y = 0; y < h; y++) {
//                for (x = 0; x < w; x++) {
//                    if (board2_initial[y * w + x] == '^') {
//                        cout << "^";
//                    }
//                    if (x == p.first && y == p.second) {
//                        cout << 'X';
//                    } else if (board2[y * w + x] == '#') {
//                        cout << '#';
//                    } else if (board2[y * w + x] != '\0') {
//                        cout << "o";
//                    } else {
//                        cout << ".";
//                    }
//                }
//                cout << endl;
//            }
        }

        free(board2);
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    free(board);
    free(board2_initial);

    return EXIT_SUCCESS;
}
