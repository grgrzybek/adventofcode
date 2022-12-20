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
#include <list>
#include <string>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    aoc2022::Options options("Day 20", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    list<pair<int, int>> numbers;

    int idx = 0;
    while (getline(*input, line)) {
        aoc2022::trim(line);
        if (!aoc2022::has_text(line)) {
            continue;
        }
        numbers.emplace_back(idx++, stoi(line));
    }

    // part 1

    int answer1 = 0;

    for (int i=0; i<idx; i++) {
        auto start = numbers.begin();
        auto pos = start;
        auto end = numbers.end();
        while (pos != end) {
            if (pos->first == i) {
                break;
            }
            pos++;
        }
        auto p = *pos;
        int move = p.second;
        if (p.second == 0) {
            continue;
        }
        auto next = pos;
        next++;
        numbers.erase(pos);
        pos = next;
        while (move > 0) {
            if (pos == end) {
                pos = numbers.begin();
            }
            pos++;
            move--;
        }
        while (move < 0) {
            pos--;
            if (pos == numbers.begin()) {
                pos = end;
            }
            move++;
        }
        numbers.insert(pos, make_pair(p.first, p.second));
    }
    cout << "end state: ";
    for (auto &pp: numbers) {
        cout << pp.second << " ";
    }
    cout << "\n";

    auto pos = numbers.begin();
    while (pos != numbers.end()) {
        if (pos->second == 0) {
            break;
        }
        pos++;
    }
    pos++;
    for (int i = 1; i <= 3000; i++) {
        if (pos == numbers.end()) {
            pos = numbers.begin();
        }
        if (i % 1000 == 0) {
            cout << "adding: " << pos->second << "\n";
            answer1 += pos->second;
        }
        pos++;
    }

    // part 2

    int answer2 = 0;

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}
