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

#include <algorithm>
#include <cstdio>
#include <deque>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    aoc2022::Options options("Day 05", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    //     [D]
    // [N] [C]
    // [Z] [M] [P]
    //  1   2   3
    //
    // move 1 from 2 to 1
    // move 3 from 1 to 3
    // move 2 from 2 to 1
    // move 1 from 1 to 2

    ifstream *input = options.file();

    string line;
    vector<string> instructions;
    bool analyzing_moves = false;

    size_t stack_count = 0;
    deque<char> stacks1[9];
    deque<char> stacks2[9];

    while (getline(*input, line)) {
        if (!aoc2022::has_text(line)) {
            // switch to moves
            analyzing_moves = true;
            continue;
        }

        char check = line.length() >= 2 ? line[1] : ' ';
        if (!analyzing_moves) {
            if (check >= '1' && check <= '9') {
                // last line before the moves
                size_t pos = 1;
                while (pos < line.length() && line[pos] >= '1' && line[pos] <= '9') {
                    stack_count++;
                    pos += 4;
                }
            } else {
                // crates
                size_t q = 0;
                size_t crate = 1;
                while (crate < line.length()) {
                    if (line[crate] >= 'A' && line[crate] <= 'Z') {
                        stacks1[q].push_front(line[crate]);
                        stacks2[q].push_front(line[crate]);
                    }
                    crate += 4;
                    q++;
                }
            }
        } else {
            instructions.push_back(line);
        }
    }

    int how_many = 0, from = 0, to = 0;
    string skip;

    // part 1
    for (auto const &instruction: instructions) {
        istringstream ins(instruction);
        ins >> skip >> how_many >> skip >> from >> skip >> to;

        for (int i = 0; i < how_many; i++) {
            char c = stacks1[from - 1].back();
            stacks1[from - 1].pop_back();
            stacks1[to - 1].push_back(c);
        }
    }
    ostringstream answer1;
    for (size_t i=0; i<stack_count; i++) {
        answer1 << stacks1[i].back();
    }

    // part 2

    for (auto const &instruction: instructions) {
        istringstream ins(instruction);
        ins >> skip >> how_many >> skip >> from >> skip >> to;

        for (int i = 0; i < how_many; i++) {
            char c = stacks2[from - 1][stacks2[from - 1].size() - how_many + i];
            stacks2[to - 1].push_back(c);
        }
        for (int i = 0; i < how_many; i++) {
            stacks2[from - 1].pop_back();
        }
    }
    ostringstream answer2;
    for (size_t i=0; i<stack_count; i++) {
        answer2 << stacks2[i].back();
    }

    cout << "Answer 1: " << answer1.str() << endl;
    cout << "Answer 2: " << answer2.str() << endl;

    return EXIT_SUCCESS;
}
