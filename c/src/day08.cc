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
#include <map>
#include <set>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    aoc2024::Options options("Day 08", argc, argv);
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
    for (auto &l: lines) {
        strncpy(board + (y++ * w), l.c_str(), w);
    }
//    for (y = 0; y < h; y++) {
//        for (x = 0; x < w; x++) {
//            cout << board[y * w + x];
//        }
//        cout << endl;
//    }

    map<char, vector<pair<int, int>>*> mapping;

//    for (y = 0; y < h; y++) {
//        for (x = 0; x < w; x++) {
//            cout << board[y * w + x];
//        }
//        cout << endl;
//    }

    // part 1

    for (size_t idx = 0; idx < (size_t)(w * h); idx++) {
        if (board[idx] != '.') {
            if (!mapping.contains(board[idx])) {
                mapping[board[idx]] = new vector<pair<int, int>>;
            }
            mapping[board[idx]]->emplace_back(idx % w, idx / h);
        }
    }
//    for (auto &vp: mapping) {
//        cout << vp.first << ":\n";
//        for (auto &p: *(vp.second)) {
//            cout << p.first << "," << p.second << " ";
//        }
//        cout << endl;
//    }

    set<pair<int, int>> coords;
    for (auto &e: mapping) {
        for (size_t i = 0; i < e.second->size(); i++) {
            for (size_t j = 0; j < e.second->size(); j++) {
                if (i == j) {
                    continue;
                }
                // find single antinode - another will be found when looking from the other antenna perspective
                auto &xy1 = e.second->at(i);
                auto &xy2 = e.second->at(j);
                int dx = xy2.first - xy1.first;
                int dy = xy2.second - xy1.second;
                int xan = xy2.first + dx;
                int yan = xy2.second + dy;
                if (xan >= 0 && xan < w && yan >= 0 && yan < h) {
                    coords.emplace(xan, yan);
                }
            }
        }
    }

    int answer1 = coords.size();

    // part 2

    coords.clear();
    for (auto &e: mapping) {
        for (size_t i = 0; i < e.second->size(); i++) {
            for (size_t j = 0; j < e.second->size(); j++) {
                if (i == j) {
                    continue;
                }
                // find single antinode - another will be found when looking from the other antenna perspective
                auto &xy1 = e.second->at(i);
                auto &xy2 = e.second->at(j);
                int dx = xy2.first - xy1.first;
                int dy = xy2.second - xy1.second;
                int xan = xy1.first;
                int yan = xy1.second;
                while (true) {
                    xan += dx;
                    yan += dy;
                    if (xan >= 0 && xan < w && yan >= 0 && yan < h) {
                        coords.emplace(xan, yan);
                    } else {
                        break;
                    }
                }
            }
        }
    }

    int answer2 = coords.size();

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    free(board);

    for (auto &vp: mapping) {
        delete vp.second;
    }

    return EXIT_SUCCESS;
}
