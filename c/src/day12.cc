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
    aoc2024::Options options("Day 12", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    int w = 0, h = 0;
    int w2 = 0, h2 = 0;

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

    char *board1 = (char *) malloc(w * h);

    int y = 0;
    int x = 0;
    for (auto &l: lines) {
        strncpy(board1 + (y++ * w), l.c_str(), w);
    }
    w2 = w + 2;
    h2 = h + 2;

    // part 1 & 2

    int answer1 = 0;
    int answer2 = 0;

    set<pair<int, int>> plots1;
    set<pair<int, int>> plots2;
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            plots1.emplace(x, y);
            plots2.emplace(x, y);
        }
    }

    deque<pair<int, int> *> q;
    while (!plots1.empty()) {
        for (auto &el: q) {
            delete el;
        }
        q.clear();
        auto p = *plots1.begin();
//        cout << "checking from " << p.first << "," << p.second << " (to check: " << plots.size() << ")\n";
        q.emplace_back(new pair<int, int>(p.first, p.second));
        int area = 0;
        int peri = 0;
        int peri2 = 0;
        vector<pair<int, int>> to_clear;
//        cout << "----\n";
//        for (y = 0; y < h; y++) {
//            for (x = 0; x < w; x++) {
//                cout << board1[y * w + x];
//            }
//            cout << endl;
//        }

        while (!q.empty()) {
            auto xy = q.front();
            q.pop_front();
            plots1.erase(*xy);
            x = xy->first;
            y = xy->second;
//            cout << "  matching " << x << "," << y << " (q size: " << q.size() << ")" << endl;
            delete xy;
            char kind = board1[y * w + x];
            if (kind == '#' || kind == '~') {
                continue;
            }
            to_clear.emplace_back(x, y);
            board1[y * w + x] = '~';
            area++;
            if (x > 0) {
                // to the left
                if (board1[y * w + x - 1] == kind) {
                    q.emplace_back(new pair<int, int>(x - 1, y));
                } else if (board1[y * w + x - 1] != '~') {
//                    cout << "  increase because there's " << board1[y * w + x - 1] << " on the left" << endl;
                    peri++;
                }
            } else {
//                cout << "  increase because (checking left) x=" << x << endl;
                peri++;
            }
            if (x < w - 1) {
                // to the right
                if (board1[y * w + x + 1] == kind) {
                    q.emplace_back(new pair<int, int>(x + 1, y));
                } else if (board1[y * w + x + 1] != '~') {
//                    cout << "  increase because there's " << board1[y * w + x + 1] << " on the right" << endl;
                    peri++;
                }
            } else {
//                cout << "  increase because (checking right) x=" << x << endl;
                peri++;
            }
            if (y > 0) {
                // to the top
                if (board1[(y - 1) * w + x] == kind) {
                    q.emplace_back(new pair<int, int>(x, y - 1));
                } else if (board1[(y - 1) * w + x] != '~') {
//                    cout << "  increase because there's " << board1[(y - 1) * w + x] << " on the top" << endl;
                    peri++;
                }
            } else {
//                cout << "  increase because (checking top) y=" << y << endl;
                peri++;
            }
            if (y < h - 1) {
                // to the bottom
                if (board1[(y + 1) * w + x] == kind) {
                    q.emplace_back(new pair<int, int>(x, y + 1));
                } else if (board1[(y + 1) * w + x] != '~') {
//                    cout << "  increase because there's " << board1[(y + 1) * w + x] << " on the bottom" << endl;
                    peri++;
                }
            } else {
//                cout << "  increase because (checking bottom) y=" << y << endl;
                peri++;
            }
        }
        char *board2 = (char *) malloc(w2 * h2);
        memset(board2, '.', w2 * h2);
        for (auto &tc: to_clear) {
            board1[tc.second * w + tc.first] = '#';
            board2[(tc.second + 1) * w2 + tc.first + 1] = '#';
        }
//        cout << "\n----------------------------------\n";
//        for (y = 0; y < h2; y++) {
//            for (x = 0; x < w2; x++) {
//                cout << board2[y * w2 + x];
//            }
//            cout << endl;
//        }
        // top -> down
        for (y = 0; y < h2 - 1 ; y++) {
            bool had_side = false;
            bool had_increased_side = false;
            for (x = 1; x < w2 - 1; x++) {
//                cout << "checking X " << x << "," << y << endl;
                if (board2[y * w2 + x] != board2[(y + 1) * w2 + x]) {
                    had_side = true;
                    // top is different than down, so we have a fence
                    if ((board2[y * w2 + x] != board2[y * w2 + x + 1]) || (board2[(y + 1) * w2 + x] != board2[(y + 1) * w2 + x + 1])) {
//                        cout << "  new side!\n";
                        had_increased_side = true;
                        peri2++;
                    }
                }
            }
            if (had_side && !had_increased_side) {
//                cout << "  new side (wide)!\n";
                peri2++;
            }
        }
        // left -> right
        for (x = 0; x < w2 - 1; x++) {
            bool had_side = false;
            bool had_increased_side = false;
            for (y = 1; y < h2 - 1; y++) {
//                cout << "checking Y " << x << "," << y << endl;
                if (board2[y * w2 + x] != board2[y * w2 + x + 1]) {
                    had_side = true;
                    // left is different than right, so we have a fence
                    if ((board2[y * w2 + x] != board2[(y + 1) * w2 + x]) || (board2[y * w2 + x + 1] != board2[(y + 1) * w2 + x + 1])) {
//                        cout << "  new side!\n";
                        had_increased_side = true;
                        peri2++;
                    }
                }
            }
            if (had_side && !had_increased_side) {
//                cout << "  new side (wide)!\n";
                peri2++;
            }
        }

        free(board2);
//        cout << "  increasing by " << area << "*" << peri << endl;
        answer1 += (area * peri);
        answer2 += (area * peri2);
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    free(board1);

    return EXIT_SUCCESS;
}
