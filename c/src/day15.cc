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
#include <sstream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

struct robot {
    int x, y, vx, vy;

    friend ostream &operator<<(ostream &os, const robot &r);
};

ostream &operator<<(ostream &os, const robot &r) {
    os << "robot: [" << r.x << "," << r.y << "], v: [" << r.vx << "," << r.vy << "]";
    return os;
}

bool move_block(int x, int y, int dy, char *const board, int w, int h, bool move);

int main(int argc, char *argv[]) {
    aoc2024::Options options("Day 15", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    int w = 0, h = 0;
    int w2 = 0;
    int rx = 0, ry = 0;

    vector<string> lines;
    string moves;

    bool reading_map = true;
    while (getline(*input, line)) {
        aoc2024::trim(line);
        if (line.empty()) {
            reading_map = false;
            continue;
        }
        if (reading_map) {
            w = static_cast<int>(line.length());
            w2 = w * 2;
            h++;
            lines.emplace_back(line);
        } else {
            moves += line;
        }
    }

    char *board = (char *) malloc(w * h);
    char *board2 = (char *) malloc(w2 * h);

    int y = 0;
    int x = 0;
    for (auto &l: lines) {
        strncpy(board + (y * w), l.c_str(), w);
        int idx = 0;
        for (char c: l) {
            switch (c) {
                case '#':
                    board2[y * w2 + idx] = '#';
                    board2[y * w2 + idx + 1] = '#';
                    break;
                case 'O':
                    board2[y * w2 + idx] = '[';
                    board2[y * w2 + idx + 1] = ']';
                    break;
                case '.':
                    board2[y * w2 + idx] = '.';
                    board2[y * w2 + idx + 1] = '.';
                    break;
                case '@':
                    board2[y * w2 + idx] = '@';
                    board2[y * w2 + idx + 1] = '.';
                    break;
                default:
                    break;
            }
            idx+=2;
        }
        y++;
    }

//    cout << "--- map ---\n";
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
//            cout << board[y * w + x];
            if (board[y * w + x] == '@') {
                rx = x;
                ry = y;
            }
        }
//        cout << endl;
    }
//    cout << "--- moves ---\n";
//    cout << "[" << moves << "]" << endl;

    // part 1

    long answer1 = 0;

    for (char c: moves) {
        switch (c) {
            case '^': {
                for (y = ry - 1; board[y * w + rx] != '#'; y--) {
                    if (board[y * w + rx] == '.') {
                        for (int y2 = y; y2 < ry; y2++) {
//                            cout << board[y * w + rx] << " ^ " << board[(y + 1) * w + rx] << endl;
                            board[y2 * w + rx] = board[(y2 + 1) * w + rx];
                        }
                        board[ry * w + rx] = '.';
                        ry--;
                        break;
                    }
                }
                break;
            }
            case 'v': {
                for (y = ry + 1; board[y * w + rx] != '#'; y++) {
                    if (board[y * w + rx] == '.') {
                        for (int y2 = y; y2 > ry; y2--) {
//                            cout << board[y * w + rx] << " V " << board[(y - 1) * w + rx] << endl;
                            board[y2 * w + rx] = board[(y2 - 1) * w + rx];
                        }
                        board[ry * w + rx] = '.';
                        ry++;
                        break;
                    }
                }
                break;
            }
            case '<': {
                for (x = rx - 1; board[ry * w + x] != '#'; x--) {
                    if (board[ry * w + x] == '.') {
                        for (int x2 = x; x2 < rx; x2++) {
//                            cout << board[ry * w + rx] << " < " << board[ry * w + rx + 1] << endl;
                            board[ry * w + x2] = board[ry * w + x2 + 1];
                        }
                        board[ry * w + rx] = '.';
                        rx--;
                        break;
                    }
                }
                break;
            }
            case '>': {
                for (x = rx + 1; board[ry * w + x] != '#'; x++) {
                    if (board[ry * w + x] == '.') {
                        for (int x2 = x; x2 > rx; x2--) {
//                            cout << board[ry * w + x2] << " > " << board[ry * w + rx - 1] << endl;
                            board[ry * w + x2] = board[ry * w + x2 - 1];
                        }
                        board[ry * w + rx] = '.';
                        rx++;
                        break;
                    }
                }
                break;
            }
            default:
                break;
        }
//        cout << "--- map (after " << c << ") ---\n";
//        for (y = 0; y < h; y++) {
//            for (x = 0; x < w; x++) {
//                cout << board[y * w + x];
//            }
//            cout << endl;
//        }
    }

    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            if (board[y * w + x] == 'O') {
                answer1 += (100 * y + x);
            }
        }
//        cout << endl;
    }

    // part 2

//        cout << "--- map 2 ---\n";
        for (y = 0; y < h; y++) {
            for (x = 0; x < w2; x++) {
//                cout << board2[y * w2 + x];
                if (board2[y * w2 + x] == '@') {
                    rx = x;
                    ry = y;
                }
            }
//            cout << endl;
        }
    //    cout << "--- moves ---\n";
    //    cout << "[" << moves << "]" << endl;

    long answer2 = 0;

    // < and > are the same
    // ^ and v are the problem...
    for (char c: moves) {
        switch (c) {
            case '^': {
                if (board2[(ry - 1) * w2 + rx] == '.') {
                    // easy
                    board2[(ry - 1) * w2 + rx] = '@';
                    board2[ry * w2 + rx] = '.';
                    ry--;
                } else if (board2[(ry - 1) * w2 + rx] == '#') {
                    // easier - nothing to do
                } else {
                    // up we can see '[' or ']'
                    if (move_block(rx, ry - 1, -1, board2, w2, h, false)) {
                        move_block(rx, ry - 1, -1, board2, w2, h, true);
                        board2[(ry - 1) * w2 + rx] = '@';
                        board2[ry * w2 + rx] = '.';
                        ry--;
                    }
                }
                break;
            }
            case 'v': {
                if (board2[(ry + 1) * w2 + rx] == '.') {
                    // easy
                    board2[(ry + 1) * w2 + rx] = '@';
                    board2[ry * w2 + rx] = '.';
                    ry++;
                } else if (board2[(ry + 1) * w2 + rx] == '#') {
                    // easier - nothing to do
                } else {
                    // down we can see '[' or ']'
                    if (move_block(rx, ry + 1, 1, board2, w2, h, false)) {
                        move_block(rx, ry + 1, 1, board2, w2, h, true);
                        board2[(ry + 1) * w2 + rx] = '@';
                        board2[ry * w2 + rx] = '.';
                        ry++;
                    }
                }
                break;
            }
            case '<': {
                for (x = rx - 1; board2[ry * w2 + x] != '#'; x--) {
                    if (board2[ry * w2 + x] == '.') {
                        for (int x2 = x; x2 < rx; x2++) {
//                            cout << board2[ry * w2 + rx] << " < " << board2[ry * w2 + rx + 1] << endl;
                            board2[ry * w2 + x2] = board2[ry * w2 + x2 + 1];
                        }
                        board2[ry * w2 + rx] = '.';
                        rx--;
                        break;
                    }
                }
                break;
            }
            case '>': {
                for (x = rx + 1; board2[ry * w2 + x] != '#'; x++) {
                    if (board2[ry * w2 + x] == '.') {
                        for (int x2 = x; x2 > rx; x2--) {
//                            cout << board2[ry * w2 + x2] << " > " << board2[ry * w2 + rx - 1] << endl;
                            board2[ry * w2 + x2] = board2[ry * w2 + x2 - 1];
                        }
                        board2[ry * w2 + rx] = '.';
                        rx++;
                        break;
                    }
                }
                break;
            }
            default:
                break;
        }
        cout << "--- map (after " << c << ") ---\n";
        for (y = 0; y < h; y++) {
            for (x = 0; x < w2; x++) {
                cout << board2[y * w2 + x];
            }
            cout << endl;
        }
    }

    for (y = 0; y < h; y++) {
        for (x = 0; x < w2; x++) {
            if (board2[y * w2 + x] == '[') {
                answer2 += (100 * y + x);
            }
        }
//        cout << endl;
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    free(board);
    free(board2);

    return EXIT_SUCCESS;
}

bool move_block(int x, int y, int dy, char *const board, int w, int h, bool move) {
    // x,y is left or right part of a box "[]"
    if (board[y * w + x] == ']') {
        x--;
    }
    // x,y is now left part of a box "[]"
    if (board[(y + dy) * w + x] == '.' && board[(y + dy) * w + x + 1] == '.') {
        // easy move
        if (move) {
            board[(y + dy) * w + x] = '[';
            board[(y + dy) * w + x + 1] = ']';
            board[y * w + x] = '.';
            board[y * w + x + 1] = '.';
        }
        return true;
    } else if (board[(y + dy) * w + x] == '#' || board[(y + dy) * w + x + 1] == '#') {
        // can't move
        return false;
    } else if (board[(y + dy) * w + x] == '[' && board[(y + dy) * w + x + 1] == ']') {
        // one block to move
        if (move_block(x, y + dy, dy, board, w, h, false)) {
            if (move) {
                move_block(x, y + dy, dy, board, w, h, true);
                board[(y + dy) * w + x] = '[';
                board[(y + dy) * w + x + 1] = ']';
                board[y * w + x] = '.';
                board[y * w + x + 1] = '.';
            }
            return true;
        }
    } else {
        // one or two blocks above/below us
        if (board[(y + dy) * w + x] == ']' && board[(y + dy) * w + x + 1] == '.') {
            // only left
            if (move_block(x - 1, y + dy, dy, board, w, h, false)) {
                if (move) {
                    move_block(x - 1, y + dy, dy, board, w, h, true);
                    board[(y + dy) * w + x] = '[';
                    board[(y + dy) * w + x + 1] = ']';
                    board[y * w + x] = '.';
                    board[y * w + x + 1] = '.';
                }
                return true;
            }
        } else if (board[(y + dy) * w + x] == '.' && board[(y + dy) * w + x + 1] == '[') {
            // only right
            if (move_block(x + 1, y + dy, dy, board, w, h, false)) {
                if (move) {
                    move_block(x + 1, y + dy, dy, board, w, h, true);
                    board[(y + dy) * w + x] = '[';
                    board[(y + dy) * w + x + 1] = ']';
                    board[y * w + x] = '.';
                    board[y * w + x + 1] = '.';
                }
                return true;
            }
        } else {
            // left AND right
            if (move_block(x - 1, y + dy, dy, board, w, h, false) && move_block(x + 1, y + dy, dy, board, w, h, false)) {
                if (move) {
                    move_block(x - 1, y + dy, dy, board, w, h, true);
                    move_block(x + 1, y + dy, dy, board, w, h, true);
                    board[(y + dy) * w + x] = '[';
                    board[(y + dy) * w + x + 1] = ']';
                    board[y * w + x] = '.';
                    board[y * w + x + 1] = '.';
                }
                return true;
            }
        }
    }
    return false;
}
