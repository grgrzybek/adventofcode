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
#include <map>
#include <set>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

struct cheat {
    int x = 0, y = 0;
};

struct cheat_gate {
    // x:y is a '.' field and dx:dy is a direction of wall for starting a cheat
    int x = 0, y = 0, dx = 0, dy = 0;
};

struct probe {
    int x, y;
    int score = 0;
    set<pair<int, int>> *path;

    probe(int x, int y, int score) {
//        cout << "Creating probe at " << x << "," << y << endl;
        this->x = x;
        this->y = y;
        this->score = score;
        path = new set<pair<int, int>>;
    }

    ~probe() {
//        cout << "Deleting probe at " << x << "," << y << endl;
        path->clear();
        delete path;
    }
};

long track(const char *const &board, int w, int h, int sx, int sy, int ex, int ey, int *honest_scores, int savings, bool trial = false);
//long track2(const char *const &board, int w, int h, int sx, int sy, int ex, int ey, int cx, int cy, int *honest_scores, int savings);

int main(int argc, char *argv[]) {
    aoc2024::Options options("Day 20", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    bool real = options.file_name()[options.file_name().length() - 5] == '0';

    string line;
    int w = 0, h = 0;
    int sx = 0, sy = 0;
    int ex = 0, ey = 0;

    const int SAVINGS = real ? 100 : 72;

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

    auto honest_scores = (int *) malloc(sizeof(int) * w * h);
    fill(honest_scores, honest_scores + (w * h), numeric_limits<int>::max());
    honest_scores[sy * w + sx] = 0;

    long honest_cost = track(board, w, h, sx, sy, ex, ey, honest_scores, SAVINGS, true);

    long answer1 = 0;

    vector<cheat> cheats;
    for (y = 1; y < h - 1; y++) {
        for (x = 1; x < w - 1; x++) {
//            cout << "checking " << x << "," << y << endl;
            if (board[y * w + x] == '#') {
                // potential cheat
                if (board[(y - 1) * w + x] == '.' && board[(y + 1) * w + x] == '.') {
                    cheats.emplace_back(cheat{.x = x, .y = y});
                }
                if (board[y * w + x - 1] == '.' && board[y * w + x + 1] == '.') {
                    cheats.emplace_back(cheat{.x = x, .y = y});
                }
            }
        }
    }
    cout << "potential cheats 1: " << cheats.size() << endl;

//    for (auto &c: cheats) {
//        // open the gate
//        board[c.y * w + c.x] = '.';
//        long cheated = track(board, w, h, sx, sy, ex, ey, honest_scores, SAVINGS);
//        if (honest_cost - cheated >= SAVINGS) {
//            answer1++;
//        }
//        // close the gate
//        board[c.y * w + c.x] = '#';
//    }

    // part 2

    size_t answer2 = 0;

//    cout << "--- honest score ---\n";
//    for (y = 0; y < h; y++) {
//        for (x = 0; x < w; x++) {
//            if (board[y * w + x] == '#') {
//                printf(" ###");
//            } else {
//                printf(" %3d", honest_scores[y * w + x]);
//            }
//        }
//        printf("\n");
//    }

    // now every '#' that has '.' on any side can be a potential cheat gate.
    // include borders too!
    vector<cheat_gate> cheats2;
//    for (y = 1; y < h - 1; y++) {
//        for (x = 1; x < w - 1; x++) {
//            if (board[y * w + x] == '#') {
//                // potential cheat
//                if (y > 0 && board[(y - 1) * w + x] == '.') {
//                    if (!(x == ex && y - 1 == ey)) {
//                        cheats2.emplace_back(cheat_gate({.x = x, .y = y - 1, .dx = 0, .dy = 1}));
//                    }
//                }
//                if (y < h - 1 && board[(y + 1) * w + x] == '.') {
//                    if (!(x == ex && y + 1 == ey)) {
//                        cheats2.emplace_back(cheat_gate({.x = x, .y = y + 1, .dx = 0, .dy = -1}));
//                    }
//                }
//                if (x > 0 && board[y * w + x - 1] == '.') {
//                    if (!(x - 1 == ex && y == ey)) {
//                        cheats2.emplace_back(cheat_gate({.x = x - 1, .y = y, .dx = 1, .dy = 0}));
//                    }
//                }
//                if ((x < w - 1 && board[y * w + x + 1] == '.')) {
//                    if (!(x + 1 == ex && y == ey)) {
//                        cheats2.emplace_back(cheat_gate({.x = x + 1, .y = y, .dx = -1, .dy = 0}));
//                    }
//                }
//            }
//        }
//    }
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
//            if (board[y * w + x] == '#') {
//                // potential cheat
//                if (y > 0 && board[(y - 1) * w + x] == '.') {
//                    cheats2.emplace_back(cheat_gate({.x = x, .y = y - 1, .dx = 0, .dy = 1}));
//                }
//                if (y < h - 1 && board[(y + 1) * w + x] == '.') {
//                    cheats2.emplace_back(cheat_gate({.x = x, .y = y + 1, .dx = 0, .dy = -1}));
//                }
//                if (x > 0 && board[y * w + x - 1] == '.') {
//                    cheats2.emplace_back(cheat_gate({.x = x - 1, .y = y, .dx = 1, .dy = 0}));
//                }
//                if ((x < w - 1 && board[y * w + x + 1] == '.')) {
//                    cheats2.emplace_back(cheat_gate({.x = x + 1, .y = y, .dx = -1, .dy = 0}));
//                }
//            }
            if (board[y * w + x] == '.') {
                // we'll try cheating on every field
                cheats2.emplace_back(cheat_gate({.x = x, .y = y}));
            }
        }
    }
//    cheats2.clear();
//    cheats2.emplace_back(cheat_gate({.x = 1, .y = 3, .dx = 0, .dy = 1}));
    bool debug = true;
//    bool debug = false;
    cout << "potential cheats 2: " << cheats2.size() << endl;

//    auto honest_scores_cheated = (int *) malloc(sizeof(int) * w * h);
//    copy(honest_scores, honest_scores + (w * h), honest_scores_cheated);
//    for (auto &c: cheats2) {
//        honest_scores_cheated[c.y * w + c.x] = 123123123;
//    }
//    cout << "--- honest score with cheats ---\n";
//    for (y = 0; y < h; y++) {
//        for (x = 0; x < w; x++) {
//            if (board[y * w + x] == '#') {
//                printf(" ###");
//            } else {
//                if (honest_scores_cheated[y * w + x] == 123123123) {
//                    printf(" !! ");
//                } else {
//                    printf(" %3d", honest_scores[y * w + x]);
//                }
//            }
//        }
//        printf("\n");
//    }
//    free(honest_scores_cheated);

    // for each cheat2 we check all <= 20 exits from the wall and check the savings
    int cc = 1;
    for (auto &c: cheats2) {
//        cout << "  checking cheat2 starting at " << c.x << "," << c.y << " (" << cc << " of " << cheats2.size() << ")";
//        if (c.dx == 0) {
//            if (c.dy == -1) {
//                cout << " ^\n";
//            } else {
//                cout << " v\n";
//            }
//        } else {
//            if (c.dx == -1) {
//                cout << " <\n";
//            } else {
//                cout << " >\n";
//            }
//        }
//        cout << endl;
        cc++;

        // review all <=20ps manhattan disntace fields
        int cheat_exits = 0;
        int xa = c.x - 20;
        if (xa < 0) {
            xa = 0;
        }
        int xb = c.x + 20;
        if (xb > w - 1) {
            xb = w - 1;
        }
        int ya = c.y - 20;
        if (ya < 0) {
            ya = 0;
        }
        int yb = c.y + 20;
        if (yb > h - 1) {
            yb = h - 1;
        }
//        cout << "    checking x: " << xa << "-" << xb << ", y: " << ya << "-" << yb << endl;
        for (y = ya; y <= yb; y++) {
            for (x = xa; x <= xb; x++) {
                int dist = abs(x - c.x) + abs(y - c.y);
//                cout << "      x: " << x << ", y: " << y << ", dist: " << dist << endl;
                if (dist <= 20) {
                    if (board[y * w + x] == '.' && honest_scores[y * w + x] >= honest_scores[c.y * w + c.x] + dist + SAVINGS) {
//                        cout << "    can cheat into " << x << "," << y << endl;
//                        cout << "      was " << honest_scores[y * w + x] << " now " << honest_scores[c.y * w + c.x] + dist << endl;
//                        cout << "        saved " << honest_scores[y * w + x] - (honest_scores[c.y * w + c.x] + dist) << endl;

                        cheat_exits++;
                    }
                }
            }
        }
        answer2 += cheat_exits;
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    free(board);
    free(honest_scores);

    return EXIT_SUCCESS;
}

long track(const char *const &board, int w, int h, int sx, int sy, int ex, int ey, int *honest_scores, int savings, bool trial) {
    auto scores = trial ? honest_scores : (int *) malloc(sizeof(int) * w * h);
    copy(honest_scores, honest_scores + (w * h), scores);
//    fill(scores, scores + (w * h), numeric_limits<int>::max());
//    scores[sy * w + sx] = 0;

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
            q->emplace_front(xy.first, xy.second - 1);
        }
        if (board[(xy.second + 1) * w + xy.first] == '.' && scores[(xy.second + 1) * w + xy.first] > scores[xy.second * w + xy.first] + 1) {
            // can go down
            scores[(xy.second + 1) * w + xy.first] = scores[xy.second * w + xy.first] + 1;
            q->emplace_front(xy.first, xy.second + 1);
        }
        if (board[xy.second * w + xy.first - 1] == '.' && scores[xy.second * w + xy.first - 1] > scores[xy.second * w + xy.first] + 1) {
            // can go left
            scores[xy.second * w + xy.first - 1] = scores[xy.second * w + xy.first] + 1;
            q->emplace_front(xy.first - 1, xy.second);
        }
        if (board[xy.second * w + xy.first + 1] == '.' && scores[xy.second * w + xy.first + 1] > scores[xy.second * w + xy.first] + 1) {
            // can go right
            scores[xy.second * w + xy.first + 1] = scores[xy.second * w + xy.first] + 1;
            q->emplace_front(xy.first + 1, xy.second);
        }
    }

    int res = scores[ey * w + ex];
    if (!trial) {
        free(scores);
    }
    delete q;

    return res;
}

//long track2(const char *const &board, int w, int h, int sx, int sy, int ex, int ey, int cx, int cy, int *honest_scores, int savings) {
//    auto scores = (int *) malloc(sizeof(int) * w * h);
//    copy(honest_scores, honest_scores + (w * h), scores);
//
////    cout << " ------- map -------- \n";
////    for (int y = 0; y < h; y++) {
////        for (int x = 0; x < w; x++) {
////            cout << board[y * w + x];
////        }
////        cout << endl;
////    }
//
//    auto q = new deque<probe>();
//    q->emplace_front(probe{.x = sx, .y = sy});
//
//    while (!q->empty()) {
//        auto p = q->front();
//        q->pop_front();
//
//        if (p.x == ex && p.y == ey) {
//            continue;
//        }
//
//        if (p.y > 0 && scores[(p.y - 1) * w + p.x] > scores[p.y * w + p.x] + 1) {
//            if (board[(p.y - 1) * w + p.x] == '.') {
//                // can go up without cheating
//                scores[(p.y - 1) * w + p.x] = scores[p.y * w + p.x] + 1;
//                q->emplace_front(probe{.x = p.x, .y = p.y - 1, .cheating = p.cheating && p.cheat > 0, .cheated = p.cheat == 0, .cheat = p.cheating ? 0 : p.cheat});
//            } else {
//                // there's a wall up, so check if we can cheat
//                if (!p.cheated && p.y - 1 == cy && p.x == cx) {
//                    // start cheating
//                    scores[(p.y - 1) * w + p.x] = scores[p.y * w + p.x] + 1;
//                    q->emplace_front(probe{.x = p.x, .y = p.y - 1, .cheating = true, .cheated = false, .cheat = p.cheat - 1});
//                } else if (p.cheating && p.cheat > 0) {
//                    // continue cheating
//                    scores[(p.y - 1) * w + p.x] = scores[p.y * w + p.x] + 1;
//                    q->emplace_front(probe{.x = p.x, .y = p.y - 1, .cheating = true, .cheated = p.cheat == 1, .cheat = p.cheat - 1});
//                }
//            }
//        }
//        if (p.y < h - 1 && scores[(p.y + 1) * w + p.x] > scores[p.y * w + p.x] + 1) {
//            if (board[(p.y + 1) * w + p.x] == '.') {
//                // can go down without cheating
//                scores[(p.y + 1) * w + p.x] = scores[p.y * w + p.x] + 1;
//                q->emplace_front(probe{.x = p.x, .y = p.y + 1, .cheating = p.cheating && p.cheat > 0, .cheated = p.cheat == 0, .cheat = p.cheating ? 0 : p.cheat});
//            } else {
//                // there's a wall down, so check if we can cheat
//                if (!p.cheated && p.y + 1 == cy && p.x == cx) {
//                    // start cheating
//                    scores[(p.y + 1) * w + p.x] = scores[p.y * w + p.x] + 1;
//                    q->emplace_front(probe{.x = p.x, .y = p.y + 1, .cheating = true, .cheated = false, .cheat = p.cheat - 1});
//                } else if (p.cheating && p.cheat > 0) {
//                    // continue cheating
//                    scores[(p.y + 1) * w + p.x] = scores[p.y * w + p.x] + 1;
//                    q->emplace_front(probe{.x = p.x, .y = p.y + 1, .cheating = true, .cheated = p.cheat == 1, .cheat = p.cheat - 1});
//                }
//            }
//        }
//        if (p.x > 0 && scores[p.y * w + p.x - 1] > scores[p.y * w + p.x] + 1) {
//            if (board[p.y * w + p.x - 1] == '.') {
//                // can go left without cheating
//                scores[p.y * w + p.x - 1] = scores[p.y * w + p.x] + 1;
//                q->emplace_front(probe{.x = p.x - 1, .y = p.y, .cheating = p.cheating && p.cheat > 0, .cheated = p.cheat == 0, .cheat = p.cheating ? 0 : p.cheat});
//            } else {
//                // there's a wall left, so check if we can cheat
//                if (!p.cheated && p.y == cy && p.x - 1 == cx) {
//                    // start cheating
//                    scores[p.y * w + p.x - 1] = scores[p.y * w + p.x] + 1;
//                    q->emplace_front(probe{.x = p.x - 1, .y = p.y, .cheating = true, .cheated = false, .cheat = p.cheat - 1});
//                } else if (p.cheating && p.cheat > 0) {
//                    // continue cheating
//                    scores[p.y * w + p.x - 1] = scores[p.y * w + p.x] + 1;
//                    q->emplace_front(probe{.x = p.x - 1, .y = p.y, .cheating = true, .cheated = p.cheat == 1, .cheat = p.cheat - 1});
//                }
//            }
//        }
//        if (p.x < w - 1 && scores[p.y * w + p.x + 1] > scores[p.y * w + p.x] + 1) {
//            if (board[p.y * w + p.x + 1] == '.') {
//                // can go right without cheating
//                scores[p.y * w + p.x + 1] = scores[p.y * w + p.x] + 1;
//                q->emplace_front(probe{.x = p.x + 1, .y = p.y, .cheating = p.cheating && p.cheat > 0, .cheated = p.cheat == 0, .cheat = p.cheating ? 0 : p.cheat});
//            } else {
//                // there's a wall right, so check if we can cheat
//                if (!p.cheated && p.y == cy && p.x + 1 == cx) {
//                    // start cheating
//                    scores[p.y * w + p.x + 1] = scores[p.y * w + p.x] + 1;
//                    q->emplace_front(probe{.x = p.x + 1, .y = p.y, .cheating = true, .cheated = false, .cheat = p.cheat - 1});
//                } else if (p.cheating && p.cheat > 0) {
//                    // continue cheating
//                    scores[p.y * w + p.x + 1] = scores[p.y * w + p.x] + 1;
//                    q->emplace_front(probe{.x = p.x + 1, .y = p.y, .cheating = true, .cheated = p.cheat == 1, .cheat = p.cheat - 1});
//                }
//            }
//        }
//    }
//
//    int res = scores[ey * w + ex];
//    free(scores);
//    delete q;
//
//    return res;
//}
