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
#include <cstring>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    aoc2022::Options options("Day 03", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<string> rucksacks;

    while (getline(*input, line)) {
        aoc2022::trim(line);
        if (!aoc2022::has_text(line)) {
            break;
        }

        rucksacks.push_back(line);
    }

    // part 1

    int answer1 = 0;
    set<char> pocket1;
    set<char> pocket2;
    for (const auto& rucksack : rucksacks) {
        pocket1.clear();
        pocket2.clear();
        pocket1.insert(rucksack.begin(), rucksack.begin() + (long) rucksack.length() / 2);
        pocket2.insert(rucksack.begin() + (long) rucksack.length() / 2, rucksack.end());

        for (auto item: pocket2) {
            if (pocket1.contains(item)) {
                if (item >= 'a' && item <= 'z') {
                    answer1 += item - 'a' + 1;
                } else {
                    answer1 += item - 'A' + 27;
                }
                break;
            }
        }
    }

    // part 2

    int answer2 = 0;
    set<char> r1;
    set<char> r2;
    set<char> r3;
    int counts[26 + 26];

    for (vector<string>::size_type group = 0; group < rucksacks.size(); group += 3) {
        std::memset(counts, 0, (26 + 26) * sizeof(int));
        r1.clear();
        r2.clear();
        r3.clear();
        r1.insert(rucksacks[group].begin(), rucksacks[group].end());
        r2.insert(rucksacks[group + 1].begin(), rucksacks[group + 1].end());
        r3.insert(rucksacks[group + 2].begin(), rucksacks[group + 2].end());
        for (auto item: r1) {
            if (item >= 'a' && item <= 'z') {
                counts[item - 'a']++;
            } else {
                counts[item - 'A' + 26]++;
            }
        }
        for (auto item: r2) {
            if (item >= 'a' && item <= 'z') {
                counts[item - 'a']++;
            } else {
                counts[item - 'A' + 26]++;
            }
        }
        for (auto item: r3) {
            if (item >= 'a' && item <= 'z') {
                counts[item - 'a']++;
            } else {
                counts[item - 'A' + 26]++;
            }
        }
        for (int score = 0; score < 26 + 26; score++) {
            if (counts[score] == 3) {
                answer2 += score + 1;
                break;
            }
        }
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}
