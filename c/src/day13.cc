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
#include <deque>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

struct machine {
    long ax, ay, bx, by, px, py;

    friend ostream &operator<<(ostream &os, const machine &m);
};

ostream &operator<<(ostream &os, const machine &m) {
    os << "price: [" << m.px << "," << m.py << "], A: [" << m.ax << "," << m.ay << "], B: [" << m.bx << "," << m.by << "]";
    return os;
}

int main(int argc, char *argv[]) {
    aoc2024::Options options("Day 13", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    string tok1;
    string tok2;
    istringstream iss;
    long x, y;

    vector<machine *> machines;

    machine *m;
    while (getline(*input, line)) {
        aoc2024::trim(line);
        if (line.empty()) {
            continue;
        }
//        cout << "[" << line << "]\n";
        if (line.starts_with("Button A")) {
            m = new machine;
            machines.push_back(m);
            iss.clear();
            iss.str(line.substr(10));
            iss >> tok1; // X+N,
            iss >> tok2; // Y+Y
            x = stoi(tok1.substr(2, tok1.length() - 1));
            y = stoi(tok2.substr(2));
            m->ax = x;
            m->ay = y;
        }
        if (line.starts_with("Button B")) {
            iss.clear();
            iss.str(line.substr(10));
            iss >> tok1; // X+N,
            iss >> tok2; // Y+Y
            x = stoi(tok1.substr(2, tok1.length() - 1));
            y = stoi(tok2.substr(2));
            m->bx = x;
            m->by = y;
        }
        if (line.starts_with("Prize")) {
            iss.clear();
            iss.str(line.substr(7));
            iss >> tok1; // X=N,
            iss >> tok2; // Y=Y
            x = stoi(tok1.substr(2, tok1.length() - 1));
            y = stoi(tok2.substr(2));
            m->px = x;
            m->py = y;
        }
    }

//    for (auto &machine: machines) {
//        cout << *machine << endl;
//    }

    // part 1

    long answer1 = 0;

    for (auto &e: machines) {
        // | ax bx | = | px |
        // | ay by | = | py |
        long w = e->ax * e->by - e->bx * e->ay;
        long wa = e->px * e->by - e->bx * e->py;
        long wb = e->ax * e->py - e->px * e->ay;
        if (w == 0) {
            // no solution
            continue;
        } else {
            long a = wa / w;
            long b = wb / w;
            if (a >= 0 && b >= 0 && a <= 100 && b <= 100) {
                // we need to skip non-integer solutions
                if ((a * e->ax + b * e->bx == e->px) && (a * e->ay + b * e->by == e->py)) {
                    answer1 += 3 * a + b;
                }
            }
        }
    }

    // part 2

    long answer2 = 0;

    for (auto &e: machines) {
        // | ax bx | = | px |
        // | ay by | = | py |
        e->px += 10000000000000;
        e->py += 10000000000000;
        long w = e->ax * e->by - e->bx * e->ay;
        long wa = e->px * e->by - e->bx * e->py;
        long wb = e->ax * e->py - e->px * e->ay;
        if (w == 0) {
            // no solution
            continue;
        } else {
            long a = wa / w;
            long b = wb / w;
//            cout << *e << endl;
//            cout << "a: " << a << ", b: " << b << endl;
            if (a >= 0 && b >= 0) {
                // we need to skip non-integer solutions
                if ((a * e->ax + b * e->bx == e->px) && (a * e->ay + b * e->by == e->py)) {
                    answer2 += 3 * a + b;
                }
            }
        }
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    for (auto &dm: machines) {
        delete dm;
    }

    return EXIT_SUCCESS;
}
