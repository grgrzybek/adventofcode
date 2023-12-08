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
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <numeric>

#include "utils/utils.h"

using namespace std;

unsigned long lcm(unsigned long l1, unsigned long l2);

int main(int argc, char *argv[]) {
    aoc2023::Options options("Day 08", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    string key;
    string to_l;
    string to_r;

    string instructions;
    map<string, pair<string, string>> desert;
    vector<string> keys;

    getline(*input, instructions);
    aoc2023::trim(instructions);
    getline(*input, line);

    // AAA = (BBB, BBB)
    while (getline(*input, line)) {
        aoc2023::trim(line);
//        cout << "[" << line << "]" << endl;
        istringstream iss(line);
        iss >> key;
        string::size_type pos = line.find('(');
        line = line.substr(pos + 1);
        iss.clear();
        iss.str(line);
        iss >> to_l;
        to_l = to_l.substr(0, to_l.length() - 1);
        iss >> to_r;
        to_r = to_r.substr(0, to_r.length() - 1);

        desert[key] = make_pair(to_l, to_r);

        if (key[key.length() - 1] == 'A') {
            keys.emplace_back(key);
        }
    }

    // part 1

    int answer1 = 1;

//    for (auto &e: desert) {
//        key = e.first;
//        to_l = e.second.first;
//        to_r = e.second.second;
//        cout << key << " -> " << to_l << "/" << to_r << endl;
//    }

    size_t pos = 0;
    size_t size = instructions.length();
    key = "AAA";
    while (true) {
        if (!desert.contains(key)) {
            break;
        }
        auto dir = desert[key];
        key = instructions[pos++ % size] == 'L' ? dir.first : dir.second;
        if (key == "ZZZ") {
            break;
        }
        answer1++;
    }

    // part 2

    unsigned long answer2 = 1;

    size_t size_steps = keys.size();
    vector<unsigned long> cycles;

    pos = 0;
    for (size_t k = 0; k < size_steps; k++) {
        key = keys[k];
        cycles.emplace_back(0);
        while (true) {
            auto dir = desert[keys[k]];
            char where = instructions[pos++];
            if (pos == size) {
                pos = 0;
            }
            keys[k] = where == 'L' ? dir.first : dir.second;
            cycles[k]++;
            if (keys[k][keys[k].length() - 1] == 'Z') {
                break;
            }
        }
    }

//    for (auto &l: cycles) {
//        cout << "length: " << l << endl;
//    }

    for (auto &l: cycles) {
        answer2 = lcm(answer2, l);
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}

unsigned long lcm(unsigned long l1, unsigned long l2) {
    return l1 * l2 / gcd(l1, l2);
}
