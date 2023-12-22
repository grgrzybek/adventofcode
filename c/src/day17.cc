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

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <numeric>

#include "utils/utils.h"

using namespace std;

struct path {
    int x, y;
    // where did it come from (1 - went right/bottom, -1 - went left/up)
    int dx, dy;
    int straight_line;
    int current_loss;
    list<pair<int, int>> *track;
};

int main(int argc, char *argv[]) {
    aoc2023::Options options("Day 17", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<string> lines;

    char *map;
    int *cost_N;
    int *cost_E;
    int *cost_S;
    int *cost_W;
    int w = 0, h = 0;

    while (getline(*input, line)) {
        w = (int) line.length();
        h++;
        lines.emplace_back(line);
    }

    map = (char *) malloc(sizeof(char) * w * h);
    cost_N = (int *) malloc(sizeof(int) * w * h);
    cost_E = (int *) malloc(sizeof(int) * w * h);
    cost_S = (int *) malloc(sizeof(int) * w * h);
    cost_W = (int *) malloc(sizeof(int) * w * h);
    for (int y = 0; y < h; y++) {
        strncpy(map + (y * w), lines[y].c_str(), w);
    }

    // part 1

    long answer1 = 0;

    for (int i = 0; i < w * h; i++) {
        cost_N[i] = (int) numeric_limits<int>::max();
        cost_E[i] = (int) numeric_limits<int>::max();
        cost_S[i] = (int) numeric_limits<int>::max();
        cost_W[i] = (int) numeric_limits<int>::max();
    }
    // start in 0,0 at cost = 0
    *cost_N = 0;
    *cost_E = 0;
    *cost_S = 0;
    *cost_W = 0;

    int min_loss1 = 0;
    int min_loss2 = 0;
    bool right = true;
    int x = 1, y = 0;
    while (true) {
        min_loss1 += map[y * w + x] - '0';
        min_loss2 += map[y * w + x] - '0';
        if (right) {
            if (x < w - 1) {
                x++;
            }
        } else {
            if (y < h - 1) {
                y++;
            }
        }
        right = !right;
        if (x == w - 1 && y == h - 1) {
            break;
        }
    }

    auto q = new deque<path *>;
    q->push_back(
            new path{ .x = 0, .y = 0, .dx = 0, .dy = 0, .straight_line = 0, .current_loss = 0/*, .track = new list<pair<int, int>>*/ });

    int heat_loss;
    string pause;
    while (!q->empty()) {
        auto p = q->back();
        q->pop_back();
//        p->track->emplace_back(p->x, p->y);

        if (p->x == w - 1 && p->y == h - 1 && p->current_loss < min_loss1) {
            min_loss1 = p->current_loss;
//            cout << "New min loss (" << p->x << ":" << p->y << ", total_loss: " << p->current_loss << ", qs: " << q->size() << ")" << endl;
        }
        if (p->current_loss > min_loss1) {
            continue;
        }

        if (p->dx != 1 && p->x > 0) {
            // didn't come from left, so may go left
            int sl = p->straight_line;
            int tsl = 0;
            int cost = p->current_loss;
            bool turn = p->dx == 0;
            int c = 0;
            while (((turn && tsl < 3) || sl < 3) && p->x - c > 0) {
                heat_loss = map[p->y * w + p->x - 1 - c] - '0';
                if (cost_W[p->y * w + p->x - 1 - c] > cost + heat_loss) {
                    if (cost + heat_loss <= min_loss1) {
                        q->push_back(new path{ .x = p->x - 1 - c, .y = p->y, .dx = -1, .dy = 0,
                                .straight_line = turn ? tsl + 1 : sl + 1, .current_loss = cost + heat_loss/*,
                                .track = new list<pair<int, int>>(*p->track)*/ });
                    }
                    cost_W[p->y * w + p->x - 1 - c] = cost + heat_loss;
                }
                cost += heat_loss;
                sl++;
                tsl++;
                c++;
            }
        }
        if (p->dx != -1 && p->x < w - 1) {
            // may go right
            int sl = p->straight_line;
            int tsl = 0;
            int cost = p->current_loss;
            bool turn = p->dx == 0;
            int c = 0;
            while (((turn && tsl < 3) || sl < 3) && p->x + c < w - 1) {
                heat_loss = map[p->y * w + p->x + 1 + c] - '0';
                if (cost_E[p->y * w + p->x + 1 + c] > cost + heat_loss) {
                    if (cost + heat_loss <= min_loss1) {
                        q->push_back(new path{ .x = p->x + 1 + c, .y = p->y, .dx = 1, .dy = 0,
                                .straight_line = turn ? tsl + 1 : sl + 1, .current_loss = cost + heat_loss/*,
                                .track = new list<pair<int, int>>(*p->track)*/ });
                    }
                    cost_E[p->y * w + p->x + 1 + c] = cost + heat_loss;
                }
                cost += heat_loss;
                sl++;
                tsl++;
                c++;
            }
        }
        if (p->dy != 1 && p->y > 0) {
            // may go up
            int sl = p->straight_line;
            int tsl = 0;
            int cost = p->current_loss;
            bool turn = p->dy == 0;
            int c = 0;
            while (((turn && tsl < 3) || sl < 3) && p->y - c > 0) {
                heat_loss = map[(p->y - 1 - c) * w + p->x] - '0';
                if (cost_N[(p->y - 1 - c) * w + p->x] > cost + heat_loss) {
                    if (cost + heat_loss <= min_loss1) {
                        q->push_back(new path{ .x = p->x, .y = p->y - 1 - c, .dx = 0, .dy = -1,
                                .straight_line = turn ? tsl + 1 : sl + 1, .current_loss = cost + heat_loss/*,
                                .track = new list<pair<int, int>>(*p->track)*/ });
                    }
                    cost_N[(p->y - 1 - c) * w + p->x] = cost + heat_loss;
                }
                cost += heat_loss;
                sl++;
                tsl++;
                c++;
            }
        }
        if (p->dy != -1 && p->y < h - 1) {
            // may go down
            int sl = p->straight_line;
            int tsl = 0;
            int cost = p->current_loss;
            bool turn = p->dy == 0;
            int c = 0;
            while (((turn && tsl < 3) || sl < 3) && p->y + c < h - 1) {
                heat_loss = map[(p->y + 1 + c) * w + p->x] - '0';
                if (cost_S[(p->y + 1 + c) * w + p->x] > cost + heat_loss) {
                    if (cost + heat_loss <= min_loss1) {
                        q->push_back(new path{ .x = p->x, .y = p->y + 1 + c, .dx = 0, .dy = 1,
                                .straight_line = turn ? tsl + 1 : sl + 1, .current_loss = cost + heat_loss/*,
                                .track = new list<pair<int, int>>(*p->track)*/ });
                    }
                    cost_S[(p->y + 1 + c) * w + p->x] = cost + heat_loss;
                }
                cost += heat_loss;
                sl++;
                tsl++;
                c++;
            }
        }

        delete p->track;
        delete p;
    }

    answer1 = min(min(cost_N[w * h - 1], cost_S[w * h - 1]), min(cost_E[w * h - 1], cost_W[w * h - 1]));

//    for (y = 0; y < h; y++) {
//        for (x = 0; x < w; x++) {
//            cout << setw(3) << setfill(' ')
//                 << min(min(cost_N[y * w + x], cost_S[y * w + x]), min(cost_E[y * w + x], cost_W[y * w + x])) << " ";
//        }
//        cout << endl;
//    }

    // part 2

    long answer2 = 0;

    for (int i = 0; i < w * h; i++) {
        cost_N[i] = (int) numeric_limits<int>::max();
        cost_E[i] = (int) numeric_limits<int>::max();
        cost_S[i] = (int) numeric_limits<int>::max();
        cost_W[i] = (int) numeric_limits<int>::max();
    }
    // start in 0,0 at cost = 0
    *cost_N = 0;
    *cost_E = 0;
    *cost_S = 0;
    *cost_W = 0;
    
    delete q;

    q = new deque<path *>;
    q->push_back(
            new path{ .x = 0, .y = 0, .dx = 0, .dy = 0, .straight_line = 0, .current_loss = 0/*, .track = new list<pair<int, int>>*/ });

    while (!q->empty()) {
//        auto p = q->back();
//        q->pop_back();
        auto p = q->front();
        q->pop_front();
//        p->track->emplace_back(p->x, p->y);

        if (p->x == w - 1 && p->y == h - 1 && p->current_loss < min_loss2) {
            min_loss2 = p->current_loss;
//            cout << "New min loss (" << p->x << ":" << p->y << ", total_loss: " << p->current_loss << ", qs: " << q->size() << ")" << endl;
        }
        if (p->current_loss > min_loss2) {
            continue;
        }

//        cout << "Cost N\n";
//        for (y = 0; y < h; y++) {
//            for (x = 0; x < w; x++) {
//                if (cost_N[y * w + x] == numeric_limits<int>::max()) {
//                    cout << "... ";
//                } else {
//                    cout << setw(3) << setfill(' ') << cost_N[y * w + x] << " ";
//                }
//            }
//            cout << endl;
//        }
//        cout << "Cost E\n";
//        for (y = 0; y < h; y++) {
//            for (x = 0; x < w; x++) {
//                if (cost_E[y * w + x] == numeric_limits<int>::max()) {
//                    cout << "... ";
//                } else {
//                    cout << setw(3) << setfill(' ') << cost_E[y * w + x] << " ";
//                }
//            }
//            cout << endl;
//        }
//        cout << "Cost S\n";
//        for (y = 0; y < h; y++) {
//            for (x = 0; x < w; x++) {
//                if (cost_S[y * w + x] == numeric_limits<int>::max()) {
//                    cout << "... ";
//                } else {
//                    cout << setw(3) << setfill(' ') << cost_S[y * w + x] << " ";
//                }
//            }
//            cout << endl;
//        }
//        cout << "Cost W\n";
//        for (y = 0; y < h; y++) {
//            for (x = 0; x < w; x++) {
//                if (cost_W[y * w + x] == numeric_limits<int>::max()) {
//                    cout << "... ";
//                } else {
//                    cout << setw(3) << setfill(' ') << cost_W[y * w + x] << " ";
//                }
//            }
//            cout << endl;
//        }
//
//        cout << endl;
//        cout << "Checking (" << p->x << ":" << p->y << ", total_loss: " << p->current_loss << ", qs: " << q->size() << ", pl:" << p->straight_line << ")" << endl;
//        getline(cin, pause);

        if (p->dx != 1 && p->x > 0) {
            // didn't come from left, so may go left
            int sl = p->straight_line;
            int tsl = 0;
            int cost = p->current_loss;
            bool turn = p->dx == 0;
            int c = 0;
            while (((turn && tsl < 10) || sl < 10) && p->x - c > 0) {
                heat_loss = map[p->y * w + p->x - 1 - c] - '0';
                if (cost_W[p->y * w + p->x - 1 - c] > cost + heat_loss) {
                    if (((turn && tsl + 1 > 3) || (!turn && sl + 1 > 3)) && cost + heat_loss <= min_loss2) {
                        q->push_back(new path{ .x = p->x - 1 - c, .y = p->y, .dx = -1, .dy = 0,
                                .straight_line = turn ? tsl + 1 : sl + 1, .current_loss = cost + heat_loss/*,
                                .track = new list<pair<int, int>>(*p->track)*/ });
                    }
                    if (((turn && tsl + 1 > 3) || (!turn && sl + 1 > 3))) {
                        cost_W[p->y * w + p->x - 1 - c] = cost + heat_loss;
                    }
                }
                cost += heat_loss;
                sl++;
                tsl++;
                c++;
            }
        }
        if (p->dx != -1 && p->x < w - 1) {
            // may go right
            int sl = p->straight_line;
            int tsl = 0;
            int cost = p->current_loss;
            bool turn = p->dx == 0;
            int c = 0;
            while (((turn && tsl < 10) || sl < 10) && p->x + c < w - 1) {
                heat_loss = map[p->y * w + p->x + 1 + c] - '0';
                if (cost_E[p->y * w + p->x + 1 + c] > cost + heat_loss) {
                    if (((turn && tsl + 1 > 3) || (!turn && sl + 1 > 3)) && cost + heat_loss <= min_loss2) {
                        q->push_back(new path{ .x = p->x + 1 + c, .y = p->y, .dx = 1, .dy = 0,
                                .straight_line = turn ? tsl + 1 : sl + 1, .current_loss = cost + heat_loss/*,
                                .track = new list<pair<int, int>>(*p->track)*/ });
                    }
                    if (((turn && tsl + 1 > 3) || (!turn && sl + 1 > 3))) {
                        cost_E[p->y * w + p->x + 1 + c] = cost + heat_loss;
                    }
                }
                cost += heat_loss;
                sl++;
                tsl++;
                c++;
            }
        }
        if (p->dy != 1 && p->y > 0) {
            // may go up
            int sl = p->straight_line;
            int tsl = 0;
            int cost = p->current_loss;
            bool turn = p->dy == 0;
            int c = 0;
            while (((turn && tsl < 10) || sl < 10) && p->y - c > 0) {
                heat_loss = map[(p->y - 1 - c) * w + p->x] - '0';
                if (cost_N[(p->y - 1 - c) * w + p->x] > cost + heat_loss) {
                    if (((turn && tsl + 1 > 3) || (!turn && sl + 1 > 3)) && cost + heat_loss <= min_loss2) {
                        q->push_back(new path{ .x = p->x, .y = p->y - 1 - c, .dx = 0, .dy = -1,
                                .straight_line = turn ? tsl + 1 : sl + 1, .current_loss = cost + heat_loss/*,
                                .track = new list<pair<int, int>>(*p->track)*/ });
                    }
                    if (((turn && tsl + 1 > 3) || (!turn && sl + 1 > 3))) {
                        cost_N[(p->y - 1 - c) * w + p->x] = cost + heat_loss;
                    }
                }
                cost += heat_loss;
                sl++;
                tsl++;
                c++;
            }
        }
        if (p->dy != -1 && p->y < h - 1) {
            // may go down
            int sl = p->straight_line;
            int tsl = 0;
            int cost = p->current_loss;
            bool turn = p->dy == 0;
            int c = 0;
            while (((turn && tsl < 10) || sl < 10) && p->y + c < h - 1) {
                heat_loss = map[(p->y + 1 + c) * w + p->x] - '0';
                if (cost_S[(p->y + 1 + c) * w + p->x] > cost + heat_loss) {
                    if (((turn && tsl + 1 > 3) || (!turn && sl + 1 > 3)) && cost + heat_loss <= min_loss2) {
                        q->push_back(new path{ .x = p->x, .y = p->y + 1 + c, .dx = 0, .dy = 1,
                                .straight_line = turn ? tsl + 1 : sl + 1, .current_loss = cost + heat_loss/*,
                                .track = new list<pair<int, int>>(*p->track)*/ });
//                        cout << " - pushed " << p->x << ":" << (p->y + 1 + c) << endl;
                    }
                    if (((turn && tsl + 1 > 3) || (!turn && sl + 1 > 3))) {
                        cost_S[(p->y + 1 + c) * w + p->x] = cost + heat_loss;
                    }
                }
                cost += heat_loss;
                sl++;
                tsl++;
                c++;
            }
        }

        delete p->track;
        delete p;
    }

    answer2 = min(min(cost_N[w * h - 1], cost_S[w * h - 1]), min(cost_E[w * h - 1], cost_W[w * h - 1]));

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    delete q;
    free(map);
    free(cost_N);
    free(cost_E);
    free(cost_S);
    free(cost_W);

    return EXIT_SUCCESS;
}
