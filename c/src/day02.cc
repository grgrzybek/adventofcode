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
#include <sstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

struct round {
    int game_id = 0;
    set<int*> *sets = nullptr;

    ~round() {
        for (auto s: *sets) {
            delete s;
        }
        delete sets;
    }
};

int main(int argc, char *argv[]) {
    aoc2023::Options options("Day 02", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    string token;
    int number;
    vector<round *> rounds;

    while (getline(*input, line)) {
        aoc2023::trim(line);

        istringstream iss(line);
        auto *r = new round;
        r->sets = new set<int*>;
        // Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
        iss >> token; // Game
        iss >> r->game_id;
        iss >> token; // :

        int* set = new int[3];
        r->sets->insert(set);
        while (!iss.eof()) {
            iss >> number;
            iss >> token;
            char last = token[token.length() - 1];
            if (last == ';' || last == ',') {
                token = token.substr(0, token.length() - 1);
            }
            switch (token.length()) {
                case 3: // red
                    set[0] = number;
                    break;
                case 4: // blue
                    set[2] = number;
                    break;
                case 5: // green
                    set[1] = number;
                    break;
            }
            if (last == ';') {
                set = new int[3] { 0, 0, 0 };
                r->sets->insert(set);
            }
        }
        rounds.emplace_back(r);
    }

    // part 1

    int answer1 = 0;
    for (auto r: rounds) {
//        cout << "id: " << r->game_id << ", size=" << r->sets->size() << endl;
        bool ok = true;
        for (const auto &set: *r->sets) {
//            cout << " " << set[0] << ":" << set[1] << ":" << set[2] << endl;
            if (set[0] > 12 || set[1] > 13 || set[2] > 14) {
                ok = false;
                break;
            }
        }
        if (ok) {
            answer1 += r->game_id;
        }
    }

    // part 2

    int answer2 = 0;
    for (auto r: rounds) {
        int max_cubes[] = { 0, 0, 0 };
        for (const auto &set: *r->sets) {
            for (int i = 0; i < 3; i++) {
                if (max_cubes[i] < set[i]) {
                    max_cubes[i] = set[i];
                }
            }
        }
        answer2 += (max_cubes[0] * max_cubes[1] * max_cubes[2]);
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    for (auto r: rounds) {
        delete r;
    }

    return EXIT_SUCCESS;
}
