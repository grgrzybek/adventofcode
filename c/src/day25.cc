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
#include <limits>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

struct pins {
    bool key;
    int v[5] = { 0, 0, 0, 0, 0 };
    friend ostream& operator<<(ostream &os, pins &p);
};

ostream& operator<<(ostream &os, pins &p) {
    os << (p.key ? "key: " : "lock: ") << p.v[0] << "," << p.v[1] << "," << p.v[2] << "," << p.v[3] << "," << p.v[4];
    return os;
}

int main(int argc, char *argv[]) {
    aoc2024::Options options("Day 25", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    pins *p = nullptr;

    vector<pins *> locks;
    vector<pins *> keys;

    while (true) {
        if (input->eof()) {
            break;
        }
        if (p == nullptr) {
            p = new pins;
            getline(*input, line);
            aoc2024::trim(line);
            if (line == "#####") {
                // #####
                // ##.##
                // .#.##
                // ...##
                // ...#.
                // ...#.
                // .....
                cout << "new lock\n";
                p->key = false;
                locks.emplace_back(p);
            } else if (line == ".....") {
                // .....
                // #....
                // #....
                // #...#
                // #.#.#
                // #.###
                // #####

                cout << "new key\n";
                p->key = true;
                keys.emplace_back(p);
            }
        }
        if (line.empty()) {
            p = nullptr;
            continue;
        }

        for (int i = 0; i < 5; i++) {
            getline(*input, line);
            aoc2024::trim(line);
            for (int j = 0; j < (int) line.length(); j++) {
                if (line[j] == '#') {
                    p->v[j]++;
                }
            }
        }
        // reading bottom line of the definition
        getline(*input, line);
        aoc2024::trim(line);
        if (input->eof()) {
            break;
        }
        getline(*input, line);
        aoc2024::trim(line);
    }

//    cout << "locks:\n";
//    for (auto &_p: locks) {
//        cout << *_p << endl;
//    }
//    cout << "keys:\n";
//    for (auto &_p: keys) {
//        cout << *_p << endl;
//    }

    // part 1

    long answer1 = 0;

    for (auto &l: locks) {
        for (auto &k: keys) {
            bool fit = true;
            for (int i = 0; i < 5; i++) {
                if (l->v[i] + k->v[i] > 5) {
                    fit = false;
                    break;
                }
            }
            if (fit) {
                answer1++;
            }
        }
    }

    // part 2

    long answer2 = 0;

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    for (auto &_p: locks) {
        delete _p;
    }
    for (auto &_p: keys) {
        delete _p;
    }

    return EXIT_SUCCESS;
}
