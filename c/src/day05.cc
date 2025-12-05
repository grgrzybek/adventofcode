/*
 * Copyright 2025 Grzegorz Grzybek
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
#include <iostream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    aoc2025::Options options("Day 05", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<pair<long, long>> ranges;
    vector<long> ingredients;
    bool reading_ingredients = false;
    
    while (getline(*input, line)) {
        aoc2025::trim(line);
        if (line.empty()) {
            reading_ingredients = true;
            continue;
        }
        if (!reading_ingredients) {
            unsigned long dash = line.find('-');
            long f = stol(line.substr(0, dash));
            long t = stol(line.substr(dash + 1));
            ranges.emplace_back(f, t);
        } else {
            ingredients.emplace_back(stol(line));
        }
    }

    // part 1

    long answer1 = 0;
    for (auto &i: ingredients) {
        for (auto &r: ranges) {
            if (i >= r.first && i <= r.second) {
                answer1++;
                break;
            }
        }
    }

    // part 2

    long answer2 = 0;
    auto ranges1 = new vector<pair<long, long>>;
    auto ranges2 = new vector<pair<long, long>>;
    for (auto &r: ranges) {
        ranges1->emplace_back(r);
    }

    while (true) {
//        cout << "---" << endl;
        for (auto &r1: *ranges1) {
//            cout << "checking " << r1.first << "-" << r1.second << endl;
            bool merge = false;
            for (auto &r2: *ranges2) {
                if (r1.first >= r2.first && r1.first <= r2.second) {
                    if (r2.second <= r1.second) {
//                        cout << "  changed " << r2.first << "-" << r2.second << " to " << r2.first << "-" << r1.second << endl;
                        r2.second = r1.second;
                    }
                    merge = true;
                } else if (r1.second >= r2.first && r1.second <= r2.second) {
                    if (r2.first >= r1.first) {
//                        cout << "  changed " << r2.first << "-" << r2.second << " to " << r1.first << "-" << r2.second << endl;
                        r2.first = r1.first;
                    }
                    merge = true;
                } else if (r1.first >= r2.first && r1.second <= r2.second) {
//                    cout << "  contained " << r1.first << "-" << r1.second << " in " << r2.first << "-" << r2.second << endl;
                    merge = true;
                } else if (r1.first <= r2.first && r1.second >= r2.second) {
//                    cout << "  replaced " << r2.first << "-" << r2.second << " with " << r1.first << "-" << r1.second << endl;
                    r2.first = r1.first;
                    r2.second = r1.second;
                    merge = true;
                }
            }
            if (!merge) {
//                cout << "  copied " << r1.first << "-" << r1.second << endl;
                ranges2->emplace_back(r1);
            } else {
//                cout << "  merged " << r1.first << "-" << r1.second << endl;
            }
        }
        swap(ranges1, ranges2);
        if (ranges1->size() == ranges2->size()) {
            bool same = true;
//            cout << "-" << endl;
            for (vector<pair<long, long>>::size_type i = 0; i < ranges1->size(); i++) {
//                cout << "comparing " << ranges1->at(i).first << "-" << ranges1->at(i).second << " and " << ranges2->at(i).first << "-" << ranges2->at(i).second << endl;
                if (ranges1->at(i) != ranges2->at(i)) {
                    same = false;
                }
            }
            if (same) {
                break;
            }
        } else {
//            cout << "continuing the check" << endl;
        }
        ranges2->clear();
    }

    for (auto &r: *ranges1) {
        answer2 += (r.second - r.first + 1);
//        cout << r.first << " - " << r.second << endl;
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    delete ranges1;
    delete ranges2;

    return EXIT_SUCCESS;
}
