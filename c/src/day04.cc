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
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

struct game {
    int id;
    int count;
    int score;
    set<int> *winning;
    set<int> *given;

    friend ostream &operator<<(ostream &os, const game &g);

    ~game() {
        delete winning;
        delete given;
    }
};

ostream &operator<<(ostream &os, const game &g) {
    os << g.id << " - id: " << g.id << "(" << g.winning->size() << "/" << g.given->size() << "), score: " << g.score << ", count: " << g.count;
    return os;
}

int main(int argc, char *argv[]) {
    aoc2023::Options options("Day 04", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    string token;
    int n;
    vector<game*> games;

    while (getline(*input, line)) {
        aoc2023::trim(line);

        auto *g = new game;
        g->count = 1;
        g->winning = new set<int>;
        g->given = new set<int>;

        // Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
        string::size_type sep = line.find('|');
        string p1 = line.substr(0, sep - 1);
        string p2 = line.substr(sep + 1);

        istringstream iss1(p1);
        iss1 >> token; // Card
        iss1 >> g->id;
        iss1 >> token; // :
        while (true) {
            iss1 >> n;
            g->winning->insert(n);
            if (iss1.eof()) {
                break;
            }
        }

        istringstream iss2(p2);
        while (true) {
            iss2 >> n;
            g->given->insert(n);
            if (iss2.eof()) {
                break;
            }
        }

        games.emplace_back(g);
    }

//    for (auto &g: games) {
//        cout << *g << endl;
//    }

    // part 1

    int answer1 = 0;

    for (size_t i = 0; i < games.size(); i++) {
        auto g = games[i];
        set<int> s = set<int>(*g->given);
        for (auto v: *g->winning) {
            s.erase(v);
        }
        if (s.size() < g->given->size()) {
            int score = 1 << (int)(g->given->size() - s.size() - 1);
            g->score = score;
            answer1 += score;
        }
        // for part 2
        for (size_t j = 0; j < g->given->size() - s.size(); j++) {
            games[i + j + 1]->count += g->count;
        }
    }

    // part 2

    int answer2 = 0;

    for (auto &g: games) {
        answer2 += g->count;
//        cout << *g << endl;
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    for (auto &g: games) {
        delete g;
    }

    return EXIT_SUCCESS;
}
