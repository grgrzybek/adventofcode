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

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <iostream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    aoc2024::Options options("Day 18", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    bool real = options.file_name()[options.file_name().length() - 5] == '8';

    string line;
    const int SIZE = real ? 71 : 7;
    const int STEPS = real ? 1024 : 12;
    int w = SIZE, h = SIZE;

    cout << "checking " << STEPS << " steps for size " << SIZE << endl;

    vector<pair<int, int>> coords;

    while (getline(*input, line)) {
        aoc2024::trim(line);
        if (line.empty()) {
            break;
        }
        string::size_type comma = line.find(',');
        coords.emplace_back(stoi(line.substr(0, comma)), stoi(line.substr(comma + 1)));
    }

    char *board = (char *) malloc(w * h);
    memset(board, (int) '.', w * h);

    int nanos = 0;
    for (auto &l: coords) {
        if (nanos == STEPS) {
            break;
        }
//        cout << l.first << ":" << l.second << endl;
        board[l.second * w + l.first] = '#';
        nanos++;
    }

    int corrupted = 0;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (board[y * w + x] == '#') {
                corrupted++;
            }
            printf("%c", board[y * w + x]);
        }
        printf("\n");
    }
    printf("corrupted: %d, nanos: %d\n", corrupted, nanos);

    // part 1

    long answer1 = 0;

    auto costs = (unsigned int *) malloc(sizeof(unsigned int) * w * h);
    fill(costs, costs + (w * h), numeric_limits<unsigned int>::max());

    deque<pair<int, int>> q;
    costs[0] = 0;
    q.emplace_front(0, 0);

    while (!q.empty()) {
        auto &xy = q.front();
        q.pop_front();

        int x = xy.first;
        int y = xy.second;
        printf("  cost at %d:%d: %d\n", x, y, costs[y * w + x]);

        if (y > 0) {
            if (board[(y - 1) * w + x] != '#' && costs[(y - 1) * w + x] > costs[y * w + x] + 1) {
                // can go up
                costs[(y - 1) * w + x] = costs[y * w + x] + 1;
                q.emplace_front(x, y - 1);
            }
        }
        if (y < h - 1) {
            if (board[(y + 1) * w + x] != '#' && costs[(y + 1) * w + x] > costs[y * w + x] + 1) {
                // can go down
                costs[(y + 1) * w + x] = costs[y * w + x] + 1;
                q.emplace_front(x, y + 1);
            }
        }
        // left
        if (x > 0) {
            if (board[y * w + x - 1] != '#' && costs[y * w + x - 1] > costs[y * w + x] + 1) {
                // can go left
                costs[y * w + x - 1] = costs[y * w + x] + 1;
                q.emplace_front(x - 1, y);
            }
        }
        // right
        if (x < w - 1) {
            if (board[y * w + x + 1] != '#' && costs[y * w + x + 1] > costs[y * w + x] + 1) {
                // can go right
                costs[y * w + x + 1] = costs[y * w + x] + 1;
                q.emplace_front(x + 1, y);
            }
        }
    }

    answer1 = costs[h * w - 1];

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (board[y * w + x] == '#') {
                printf(" ###");
            } else {
                printf(" %03d", costs[y * w + x]);
            }
        }
        printf("\n");
    }

    // part 2

    long answer2 = 0;

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    free(costs);
    free(board);

    return EXIT_SUCCESS;
}
