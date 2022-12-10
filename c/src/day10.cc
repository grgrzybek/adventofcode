/*
 * Copyright 2022 Grzegorz Grzybek
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

#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    aoc2022::Options options("Day 10", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<pair<int, int>> cycles; // length, addition (0 for noop)

    int increase;
    int ticks;
    string op;
    while (getline(*input, line)) {
        istringstream iss(line);
        iss >> op;
        if (op == "noop") {
            increase = 0;
            ticks = 1;
        } else {
            iss >> increase;
            ticks = 2;
        }
        cycles.emplace_back(ticks, increase);
    }

    // part 1

    long answer1 = 0;
    int tick = 0;
    int reg_x = 1;
    for (auto &ins: cycles) {
        for (int i = 0; i < ins.first; i++) {
            tick++;
            if (tick == 20 || tick == 60 || tick == 100 || tick == 140 || tick == 180 || tick == 220) {
                answer1 += (tick * reg_x);
            }
            if (ins.first == 2) {
                // add
                if (i == 1) {
                    // this is where addition happens
                    reg_x += ins.second;
                }
            }
        }
    }

    // part 2

    char screen[240];
    memset(screen, (int) '.', sizeof(char) * 240);

    tick = 0;
    reg_x = 1;
    for (auto &ins: cycles) {
        for (int i = 0; i < ins.first; i++) {
            int rx = tick % 40;
            if (reg_x == rx || reg_x == rx - 1 || reg_x == rx + 1) {
                screen[tick] = '#';
            }
            tick++;
            if (ins.first == 2) {
                // add
                if (i == 1) {
                    // this is where addition happens
                    reg_x += ins.second;
                }
            }
        }
    }

    cout << "Answer 1: " << answer1 << endl;

    for (int y = 0; y < 6; y++) {
        for (int x = 0; x < 40; x++) {
            cout << screen[y*40 + x];
        }
        cout << "\n";
    }

    return EXIT_SUCCESS;
}
