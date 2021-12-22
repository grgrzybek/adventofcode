/*
 * Copyright 2021 Grzegorz Grzybek
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

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;
using namespace aoc2021;

void print_map_png(const byte *map, int width, int height, int step);

void
play(vector<pair<unsigned long long, unsigned long long>> &dist, unsigned long long int *u1, unsigned long long int *u2, int p1score, int p2score,
     int p1, int p2, int player);

int main(int argc, char *argv[]) {
    Options options("Day 21", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;

    int p1_initial, p2_initial;
    getline(*input, line);
    trim(line);
    sscanf(line.c_str(), "Player 1 starting position: %d", &p1_initial);
    getline(*input, line);
    trim(line);
    sscanf(line.c_str(), "Player 2 starting position: %d", &p2_initial);

    // part 1

    int p1 = p1_initial, p2 = p2_initial;
    int p1score = 0;
    int p2score = 0;

    int dice = 1;

    int moves;
    int rolls = 0;
    while (true) {
        // p1
        moves = 0;
        for (int i = 0; i < 3; i++) {
            moves += dice++;
            if (dice > 100) {
                dice = 1;
            }
        }
        rolls += 3;
        p1 += moves;
        p1 %= 10;
        if (p1 == 0) {
            p1 = 10;
        }
        p1score += p1;
        if (p1score >= 1000) {
            break;
        }

        // p2
        moves = 0;
        for (int i = 0; i < 3; i++) {
            moves += dice++;
            if (dice > 100) {
                dice = 1;
            }
        }
        rolls += 3;
        p2 += moves;
        p2 %= 10;
        if (p2 == 0) {
            p2 = 10;
        }
        p2score += p2;
        if (p2score >= 1000) {
            break;
        }
    }

    int answer1 = 0;
    if (p1score > p2score) {
        answer1 = p2score * rolls;
    } else {
        answer1 = p1score * rolls;
    }

    // part 2

    /*
     *  1--2--3-[4]
     *  |         |
     * 10         5
     *  |         |
     *  9-(8)-7---6
     */

    // [4] - player 1
    // (8) - player 2

    /*
     * a player may roll 3 - 9:
     * 1,1,1 - 3
     * 1,1,2 - 4
     * 1,1,3 - 5
     * 1,2,1 - 4
     * 1,2,2 - 5
     * 1,2,3 - 6
     * 1,3,1 - 5
     * 1,3,2 - 6
     * 1,3,3 - 7
     * 2,1,1 - 4
     * 2,1,2 - 5
     * 2,1,3 - 6
     * 2,2,1 - 5
     * 2,2,2 - 6
     * 2,2,3 - 7
     * 2,3,1 - 6
     * 2,3,2 - 7
     * 2,3,3 - 8
     * 3,1,1 - 5
     * 3,1,2 - 6
     * 3,1,3 - 7
     * 3,2,1 - 6
     * 3,2,2 - 7
     * 3,2,3 - 8
     * 3,3,1 - 7
     * 3,3,2 - 8
     * 3,3,3 - 9
     *
     * By final output:
     * -
     * 1,1,1 - 3
     * -
     * 1,1,2 - 4
     * 1,2,1 - 4
     * 2,1,1 - 4
     * -
     * 1,1,3 - 5
     * 1,2,2 - 5
     * 1,3,1 - 5
     * 2,1,2 - 5
     * 2,2,1 - 5
     * 3,1,1 - 5
     * -
     * 1,2,3 - 6
     * 1,3,2 - 6
     * 2,1,3 - 6
     * 2,2,2 - 6
     * 2,3,1 - 6
     * 3,1,2 - 6
     * 3,2,1 - 6
     * -
     * 1,3,3 - 7
     * 2,2,3 - 7
     * 2,3,2 - 7
     * 3,1,3 - 7
     * 3,2,2 - 7
     * 3,3,1 - 7
     * -
     * 2,3,3 - 8
     * 3,2,3 - 8
     * 3,3,2 - 8
     * -
     * 3,3,3 - 9
     * -
     */

    /*
     * may roll (27 permutations):
     * 3 - 1
     * 4 - 3
     * 5 - 6
     * 6 - 7
     * 7 - 6
     * 8 - 3
     * 9 - 1
     */

    /*
     * player 1 wins if it has:
     * points | stands | rolls
     * -------+--------+------
     *      0 |
     *      1 |
     *      2 |
     *      3 |
     *      4 |
     *      5 |
     *      6 |
     *      7 |
     *      8 |
     *      9 |
     *     10 |
     *     11 |
     *     12 |
     *     13 |
     *     14 |
     *     15 |
     *     16 |
     *     17 |
     *     18 | 1-10   | 3-9
     *     19 | 1-10   | 3-9
     *     20 | 1-10   | 3-9
     */

    vector<pair<unsigned long long, unsigned long long>> distribution = {
            make_pair(3ull, 1ull),
            make_pair(4ull, 3ull),
            make_pair(5ull, 6ull),
            make_pair(6ull, 7ull),
            make_pair(7ull, 6ull),
            make_pair(8ull, 3ull),
            make_pair(9ull, 1ull)
    };

    unsigned long long u1 = 0ll, u2 = 0ll;
    play(distribution, &u1, &u2, 0, 0, p1_initial, p2_initial, 0);

//    cout << "p1: " << u1 << ", p2: " << u2 << endl;

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << max(u1, u2) << endl;

    return EXIT_SUCCESS;
}

void
play(vector<pair<unsigned long long, unsigned long long>> &dist, unsigned long long int *u1, unsigned long long int *u2, int p1score, int p2score,
     int p1, int p2, int player) { // NOLINT(misc-no-recursion)
    for (auto &p: dist) {

        if (player == 0) {
            unsigned long long p1s = p1score;
            unsigned long long p1p = p1;
            p1p += p.first;
            p1p %= 10;
            if (p1p == 0) {
                p1p = 10;
            }
            p1s += p1p;
            if (p1s >= 21) {
                *u1 += p.second;
            } else {
                unsigned long long u1l = 0;
                unsigned long long u2l = 0;
                play(dist, &u1l, &u2l, p1s, p2score, p1p, p2, 1);
                *u1 += u1l * p.second;
                *u2 += u2l * p.second;
            }
        } else {
            unsigned long long p2s = p2score;
            unsigned long long p2p = p2;
            p2p += p.first;
            p2p %= 10;
            if (p2p == 0) {
                p2p = 10;
            }
            p2s += p2p;
            if (p2s >= 21) {
                *u2 += p.second;
            } else {
                unsigned long long u1l = 0;
                unsigned long long u2l = 0;
                play(dist, &u1l, &u2l, p1score, p2s, p1, p2p, 0);
                *u1 += u1l * p.second;
                *u2 += u2l * p.second;
            }
        }
    }
}
