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
#include <iostream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    aoc2022::Options options("Day 02", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<char> player_a;
    vector<char> player_b;

    while (getline(*input, line)) {
        aoc2022::trim(line);
        if (!aoc2022::has_text(line)) {
            break;
        }

        player_a.push_back(line[0]);
        player_b.push_back(line[2]);
    }

    // part 1

    int answer1 = 0;
    auto pa = player_a.begin();
    auto pb = player_b.begin();
    while (pa != player_a.end()) {
        if (*pa == 'A') {
            // rock (1)
            if (*pb == 'X') {
                // rock - draw (3)
                answer1 += 4;
            } else if (*pb == 'Y') {
                // paper - win (6)
                answer1 += 8;
            } else {
                // scissors - loose (0)
                answer1 += 3;
            }
        } else if (*pa == 'B') {
            // paper (2)
            if (*pb == 'X') {
                // rock - loose (0)
                answer1 += 1;
            } else if (*pb == 'Y') {
                // paper - draw (3)
                answer1 += 5;
            } else {
                // scissors - win (6)
                answer1 += 9;
            }
        } else {
            // scissors (3)
            if (*pb == 'X') {
                // rock - win (6)
                answer1 += 7;
            } else if (*pb == 'Y') {
                // paper - loose (0)
                answer1 += 2;
            } else {
                // scissors - draw (3)
                answer1 += 6;
            }
        }
        pa++;
        pb++;
    }

    // part 2

    int answer2 = 0;
    pa = player_a.begin();
    pb = player_b.begin();
    while (pa != player_a.end()) {
        if (*pa == 'A') {
            // rock (1)
            if (*pb == 'X') {
                // loose - scissors
                answer2 += 0 + 3;
            } else if (*pb == 'Y') {
                // draw - rock
                answer2 += 3 + 1;
            } else {
                // win - paper
                answer2 += 6 + 2;
            }
        } else if (*pa == 'B') {
            // paper (2)
            if (*pb == 'X') {
                // loose - rock
                answer2 += 0 + 1;
            } else if (*pb == 'Y') {
                // draw - paper
                answer2 += 3 + 2;
            } else {
                // win - scissors
                answer2 += 6 + 3;
            }
        } else {
            // scissors (3)
            if (*pb == 'X') {
                // loose - paper
                answer2 += 0 + 2;
            } else if (*pb == 'Y') {
                // draw - scissors
                answer2 += 3 + 3;
            } else {
                // win - rock
                answer2 += 6 + 1;
            }
        }
        pa++;
        pb++;
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}
