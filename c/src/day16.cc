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

struct reindeer {
    int x, y, dx, dy, score, turns;
    char *board_copy;

    reindeer() = default;

    reindeer(const reindeer *r, int w, int h, int dx, int dy);

    void draw_map(int w, int h) const;

    ~reindeer() {
        free(board_copy);
    }

    friend ostream &operator<<(ostream &os, const reindeer &r);
};

reindeer::reindeer(const reindeer *r, int w, int h, int dx, int dy) {
    this->x = r->x;
    this->y = r->y;
    this->dx = r->dx;
    this->dy = r->dy;
    this->score = r->score;
    this->turns = r->turns;
    this->board_copy = (char *) malloc(sizeof(char) * w * h);
    memcpy(this->board_copy, r->board_copy, sizeof(char) * w * h);

    if (!(this->dx == dx && this->dy == dy)) {
        // we have to turn first
        if (this->dx == dx || this->dy == dy) {
            // weird - we have to turn twice, but let's handle...
            // like ">" (1, 0) -> "<" (-1, 0)
            // like "^" (0, -1) -> "v" (0, 1)
            this->score += 2000;
            this->turns += 2;
        } else {
            // normal turn
            // like ">" (1, 0) -> "v" (0, 1) or -> "^" (0, -1)
            this->score += 1000;
            this->turns++;
        }
    }
    this->dx = dx;
    this->dy = dy;
    this->board_copy[this->y * w + this->x] = 'x';
    // now move
    this->x += this->dx;
    this->y += this->dy;
    this->score++;
}

void reindeer::draw_map(int w, int h) const {
    for (int _y = 0; _y < h; _y++) {
        for (int _x = 0; _x < w; _x++) {
            if (_x == x && _y == y) {
                if (dx == 0) {
                    if (dy == -1) {
                        cout << "^";
                    } else {
                        cout << "v";
                    }
                } else {
                    if (dx == -1) {
                        cout << "<";
                    } else {
                        cout << ">";
                    }
                }
            } else {
                cout << this->board_copy[_y * w + _x];
            }
        }
        cout << endl;
    }
}

ostream &operator<<(ostream &os, const reindeer &r) {
    os << "reindeer: [" << r.x << "," << r.y << " (" << r.score << ")] ";
    if (r.dx == 0) {
        if (r.dy == -1) {
            os << "^";
        } else {
            os << "v";
        }
    } else {
        if (r.dx == -1) {
            os << "<";
        } else {
            os << ">";
        }
    }
    return os;
}

int main(int argc, char *argv[]) {
    aoc2024::Options options("Day 16", argc, argv);
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

    cout << "--- map ---\n";
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            cout << board[y * w + x];
            if (board[y * w + x] == 'S') {
                sx = x;
                sy = y;
            }
            if (board[y * w + x] == 'E') {
                ex = x;
                ey = y;
                // to make life easier
                board[y * w + x] = '.';
            }
        }
        cout << endl;
    }
    cout << "S: " << sx << "," << sy << endl;
    cout << "E: " << ex << "," << ey << endl;

    // part 1

    long answer1 = 0;

    auto r = new reindeer;
    r->x = sx;
    r->y = sy;
    r->dx = 1;
    r->dy = 0;
    r->score = 0;
    r->board_copy = (char *) malloc(sizeof(char) * w * h);
    memcpy(r->board_copy, board, sizeof(char) * w * h);

    auto q = deque<reindeer *>();
    q.emplace_back(r);

    int min_score = numeric_limits<int>::max();
    int min_turns = numeric_limits<int>::max();

    while (!q.empty()) {
        r = q.front();
        q.pop_front();

        if (r->x == ex && r->y == ey) {
            if (min_score > r->score) {
                min_score = r->score;
                min_turns = r->turns;
                cout << "one winner: " << *r << endl;
                r->draw_map(w, h);
            }
            delete r;
            continue;
        }

//        cout << "q size: " << q.size() << endl;

        auto b = r->board_copy;

        // prefer going in existing direction
        // up, down, left, right
        reindeer* arr[4] { nullptr, nullptr, nullptr, nullptr };

        if (b[(r->y - 1) * w + r->x] == '.') {
            // can go up
            auto rn = new reindeer(r, w, h, 0, -1);
            if (rn->score >= min_score || rn->turns >= min_turns) {
                delete rn;
            } else {
                arr[0] = rn;
            }
        }
        if (b[(r->y + 1) * w + r->x] == '.') {
            // can go down
            auto rn = new reindeer(r, w, h, 0, 1);
            if (rn->score >= min_score || rn->turns >= min_turns) {
                delete rn;
            } else {
                arr[1] = rn;
            }
        }
        if (b[r->y * w + r->x - 1] == '.') {
            // can go left
            auto rn = new reindeer(r, w, h, -1, 0);
            if (rn->score >= min_score || rn->turns >= min_turns) {
                delete rn;
            } else {
                arr[2] = rn;
            }
        }
        if (b[r->y * w + r->x + 1] == '.') {
            // can go right
            auto rn = new reindeer(r, w, h, 1, 0);
            if (rn->score >= min_score || rn->turns >= min_turns) {
                delete rn;
            } else {
                arr[3] = rn;
            }
        }

        for (int idx = 0; idx < 4; idx++) {
            if (arr[idx] && r->dx == arr[idx]->dx && r->dy == arr[idx]->dy) {
                q.emplace_front(arr[idx]);
                arr[idx] = nullptr;
            }
        }
        // emplace remaining ones
        for (int idx = 0; idx < 4; idx++) {
            if (arr[idx]) {
                q.emplace_front(arr[idx]);
                arr[idx] = nullptr;
            }
        }

        delete r;
    }

    answer1 = min_score;

    // part 2

    long answer2 = 0;

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    free(board);

    return EXIT_SUCCESS;
}
