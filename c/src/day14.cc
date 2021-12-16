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
#include <limits>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;
using namespace aoc2021;

int main(int argc, char *argv[]) {
    Options options("Day 14", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    list<char> polymer1;
    list<char> polymer2;
    map<pair<char, char>, char> rules;

    getline(*input, line);
    trim(line);
    for (char c: line) {
        polymer1.push_back(c);
        polymer2.push_back(c);
    }
    getline(*input, line);

    while (getline(*input, line)) {
        trim(line);
        char c1, c2, c3;
        sscanf(line.c_str(), "%c%c -> %c", &c1, &c2, &c3);
        rules[make_pair(c1, c2)] = c3;
    }

    // part 1

    for (int i = 0; i < 10; i++) {
        auto it = polymer1.begin();
        while (true) {
            char c1 = *it++;
            if (it == polymer1.end()) {
                break;
            }
            char c2 = *it;
            for (auto &rule: rules) {
                if (rule.first.first == c1 && rule.first.second == c2) {
                    polymer1.insert(it, rule.second);
                    break;
                }
            }
        }
    }

    map<char, unsigned long long> counts;
    for (char c: polymer1) {
        if (!counts.contains(c)) {
            counts[c] = 0;
        }
        counts[c]++;
    }
    unsigned long long max = 0;
    unsigned long long min = numeric_limits<unsigned long long>::max();

    for (auto &e: counts) {
        if (max < e.second) {
            max = e.second;
        }
        if (min > e.second) {
            min = e.second;
        }
    }

    unsigned long long answer1 = max - min;

    // part 2

    map<pair<char, char>, unsigned long long> counts2_pairs;
    map<char, unsigned long long> counts2;

    auto it = polymer2.begin();
    while (true) {
        char c1 = *it++;
        if (!counts2.contains(c1)) {
            counts2[c1] = 0;
        }
        counts2[c1]++;
        if (it == polymer2.end()) {
            break;
        }
        char c2 = *it;
        auto p = make_pair(c1, c2);
        if (!counts2_pairs.contains(p)) {
            counts2_pairs[p] = 0;
        }
        counts2_pairs[p]++;
    }

    for (int i = 0; i < 40; i++) {
        map<pair<char, char>, unsigned long long> new_counts2;
        for (auto &p: counts2_pairs) {
            for (auto &rule: rules) {
                if (rule.first.first == p.first.first && rule.first.second == p.first.second) {
                    // matching rule creates two new pairs
                    pair<char, char> p1 = make_pair(p.first.first, rule.second);
                    pair<char, char> p2 = make_pair(rule.second, p.first.second);
                    if (!new_counts2.contains(p1)) {
                        new_counts2[p1] = 0;
                    }
                    new_counts2[p1] += p.second;
                    if (!new_counts2.contains(p2)) {
                        new_counts2[p2] = 0;
                    }
                    new_counts2[p2] += p.second;
                    if (!counts2.contains(rule.second)) {
                        counts2[rule.second] = 0;
                    }
                    counts2[rule.second] += p.second;

                    break;
                }
            }
        }

        counts2_pairs.clear();
        for (auto &p: new_counts2) {
            if (!counts2_pairs.contains(p.first)) {
                counts2_pairs[p.first] = 0;
            }
            counts2_pairs[p.first] += p.second;
        }
    }

    max = 0;
    min = numeric_limits<unsigned long long>::max();

    for (auto &e: counts2) {
        if (max < e.second) {
            max = e.second;
        }
        if (min > e.second) {
            min = e.second;
        }
    }

    unsigned long long answer2 = max - min;

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}
