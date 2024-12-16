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
#include <set>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

struct score {
    unsigned int up = 0, left = 0, right = 0, down = 0;
};

struct reindeer {
    int x, y, dx, dy;
    unsigned int score;

    vector<pair<int, int>> *path;

    reindeer() = default;
    ~reindeer() {
        delete path;
    }

    reindeer(const reindeer *r, int w, int h, int dx, int dy);

    friend ostream &operator<<(ostream &os, const reindeer &r);
};

reindeer::reindeer(const reindeer *r, int w, int h, int dx, int dy) {
    this->x = r->x;
    this->y = r->y;
    this->dx = r->dx;
    this->dy = r->dy;
    this->score = r->score;
    this->path = new vector<pair<int, int>>;
    this->path->assign(r->path->begin(), r->path->end());

    if (!(this->dx == dx && this->dy == dy)) {
        // we have to turn first
        if (this->dx == dx || this->dy == dy) {
            // weird - we have to turn twice, but let's handle...
            // like ">" (1, 0) -> "<" (-1, 0)
            // like "^" (0, -1) -> "v" (0, 1)
            this->score += 2000;
        } else {
            // normal turn
            // like ">" (1, 0) -> "v" (0, 1) or -> "^" (0, -1)
            this->score += 1000;
        }
    }
    this->dx = dx;
    this->dy = dy;
    // now move
    this->x += this->dx;
    this->y += this->dy;
    this->path->emplace_back(this->x, this->y);
    this->score++;
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

//    cout << "--- map ---\n";
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
//            cout << board[y * w + x];
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
//        cout << endl;
    }
//    cout << "S: " << sx << "," << sy << endl;
//    cout << "E: " << ex << "," << ey << endl;

    // part 1

    long answer1 = 0;

    auto r = new reindeer;
    r->x = sx;
    r->y = sy;
    r->dx = 1;
    r->dy = 0;
    r->score = 0;
    r->path = new vector<pair<int, int>>;
    r->path->emplace_back(sx, sy);

    auto scores = (score *) malloc(sizeof(struct score) * w * h);
    memset(scores, 0xff, sizeof(struct score) * w * h);
    scores[sy * w + sx].right = 0;

    auto q = deque<reindeer *>();
    q.emplace_back(r);

    unsigned int min_score = numeric_limits<unsigned int>::max();

    set<pair<int, int>> seats;
    while (!q.empty()) {
        r = q.front();
        q.pop_front();

//        cout << "checking " << *r << endl;
        if (r->x == ex && r->y == ey) {
            cout << "reached end\n";
            char *board2 = (char *) malloc(w * h);
            memcpy(board2, board, w * h);

            for (auto &p: *r->path) {
                board2[p.second * w + p.first] = 'O';
            }
            for (y = 0; y < h; y++) {
                for (x = 0; x < w; x++) {
                    cout << board2[y * w + x];
                }
                cout << endl;
            }
            free(board2);
            if (min_score > r->score) {
                min_score = r->score;
                // new score, so new seats for this lowest (for now) score
                seats.clear();
                seats.insert(r->path->begin(), r->path->end());
                cout << "new score: " << min_score << ", seats: " << seats.size() << endl;
            } else if (min_score == r->score) {
                // another potential path for this score
                cout << "score: " << min_score << ", seats now: " << seats.size() << endl;
                seats.insert(r->path->begin(), r->path->end());
                cout << "    seats after adding new path: " << seats.size() << endl;
            }
            delete r;
            continue;
        }

        // prefer going in existing direction
        // up, down, left, right
        reindeer* arr[4] { nullptr, nullptr, nullptr, nullptr };

        if (board[(r->y - 1) * w + r->x] == '.') {
            // can go up
            auto rn = new reindeer(r, w, h, 0, -1);
            if (rn->score > scores[rn->y * w + rn->x].up) {
//                cout << "not going up from " << *r << " to " << *rn << endl;
                delete rn;
            } else {
//                cout << "   going up from " << *r << " to " << *rn << endl;
                scores[rn->y * w + rn->x].up = rn->score;
                arr[0] = rn;
            }
        }
        if (board[(r->y + 1) * w + r->x] == '.') {
            // can go down
            auto rn = new reindeer(r, w, h, 0, 1);
            if (rn->score > scores[rn->y * w + rn->x].down) {
                delete rn;
            } else {
//                cout << "   going down from " << *r << " to " << *rn << endl;
                scores[rn->y * w + rn->x].down = rn->score;
                arr[1] = rn;
            }
        }
        if (board[r->y * w + r->x - 1] == '.') {
            // can go left
            auto rn = new reindeer(r, w, h, -1, 0);
            if (rn->score > scores[rn->y * w + rn->x].left) {
                delete rn;
            } else {
//                cout << "   going left from " << *r << " to " << *rn << endl;
                scores[rn->y * w + rn->x].left = rn->score;
                arr[2] = rn;
            }
        }
        if (board[r->y * w + r->x + 1] == '.') {
            // can go right
            auto rn = new reindeer(r, w, h, 1, 0);
            if (rn->score > scores[rn->y * w + rn->x].right) {
//                cout << "   not going right from " << *r << " to " << *rn << " target: " << scores[rn->y * w + rn->x] << endl;
                delete rn;
            } else {
//                cout << "   going right from " << *r << " to " << *rn << endl;
                scores[rn->y * w + rn->x].right = rn->score;
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

    answer1 = min(min(scores[ey * w + ex].left, scores[ey * w + ex].right), min(scores[ey * w + ex].up, scores[ey * w + ex].up));

    // part 2

    size_t answer2 = seats.size();

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
