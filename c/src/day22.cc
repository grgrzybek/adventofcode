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

int main(int argc, char *argv[]) {
    aoc2024::Options options("Day 22", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;

    vector<long> seeds;

    while (getline(*input, line)) {
        aoc2024::trim(line);
        if (line.empty()) {
            break;
        }
        seeds.emplace_back(stol(line));
    }

    // part 1

    long answer1 = 0;

    long v;
    for (long sn: seeds) {
//        cout << "seed " << sn << ":\n";
        for (int i = 0; i < 2000; i++) {
            v = sn << 6;
            sn ^= v;
            sn &= 0xffffff;
            v = sn >> 5;
            sn ^= v;
            sn &= 0xffffff;
            v = sn << 11;
            sn ^= v;
            sn &= 0xffffff;
//            cout << "  " << sn << endl;
        }
        answer1 += sn;
    }

    // part 2

    long answer2 = 0;

    vector<array<int, 2001> *> secrets_sequences;
    vector<array<int, 2000> *> secrets_deltas;
    for (long sn: seeds) {
        auto seq = new array<int, 2001>;
        secrets_sequences.emplace_back(seq);
        (*seq)[0] = (int) sn % 10;

        auto deltas = new array<int, 2000>;
        secrets_deltas.emplace_back(deltas);

        for (int i = 0; i < 2000; i++) {
            v = sn << 6;
            sn ^= v;
            sn &= 0xffffff;
            v = sn >> 5;
            sn ^= v;
            sn &= 0xffffff;
            v = sn << 11;
            sn ^= v;
            sn &= 0xffffff;
            (*seq)[i + 1] = (int) sn % 10;
            (*deltas)[i] = (*seq)[i + 1] - (*seq)[i];
        }
    }

//    for (int i = 0; i < 10; i++) {
//        if (i == 0) {
//            cout << secrets_sequences[0]->at(i) << endl;
//        } else {
//            cout << secrets_sequences[0]->at(i) << " : " << secrets_deltas[0]->at(i - 1) << endl;
//        }
//    }

    // each seed/sequence/deltas will be checked to find 4-elem sequences leading to prices 1-9
    vector<map<string, int>*> maps;
    map<string, int> seq_to_price;
    vector<map<int, vector<string>>*> rmaps;
    for (int seed = 0; seed < (int) seeds.size(); seed++) {
        cout << "checking seed " << seeds[seed] << endl;
        auto m = new map<string, int>();
        maps.emplace_back(m);
        auto rm = new map<int, vector<string>>();
        rmaps.emplace_back(rm);

        for (int idx = 4; idx < 2001; idx++) {
            // for price at idx=4 we check deltas at indices 0,1,2,3
            int price = secrets_sequences[seed]->at(idx);
            // -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
            //  a,  b,  c,  d,  e,  f,  g,  h,  i, j, k, l, m, n, o, p, q, r, s
            string key = "    ";
            key[0] = (char) (secrets_deltas[seed]->at(idx - 4) + 9 + 'a');
            key[1] = (char) (secrets_deltas[seed]->at(idx - 3) + 9 + 'a');
            key[2] = (char) (secrets_deltas[seed]->at(idx - 2) + 9 + 'a');
            key[3] = (char) (secrets_deltas[seed]->at(idx - 1) + 9 + 'a');
            if (!m->contains(key)) {
//                cout << "  new sequence \"" << key << "\": " << price;
                seq_to_price[key] += price;
//                cout << " (total: " << seq_to_price[key] << ")\n";
            }
            (*m)[key] = price;
            (*rm)[price].emplace_back(key);
        }
    }

//    for (auto &m: *maps[0]) {
//        cout << m.first << ": " << m.second << endl;
//    }

    for (auto &p: seq_to_price) {
        if (p.second > answer2) {
            answer2 = p.second;
        }
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    for (auto &ss: secrets_sequences) {
        delete ss;
    }
    for (auto &sd: secrets_deltas) {
        delete sd;
    }
    for (auto &m: maps) {
        delete m;
    }
    for (auto &m: rmaps) {
        delete m;
    }

    return EXIT_SUCCESS;
}
