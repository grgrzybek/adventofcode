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

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    aoc2024::Options options("Day 05", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;

    vector<pair<int, int>> rules;
    vector<list<int> *> full_rules;
    vector<vector<int> *> updates;

    bool reading_rules = true;
    while (getline(*input, line)) {
        aoc2024::trim(line);
        if (line.empty()) {
            if (reading_rules) {
                reading_rules = false;
                continue;
            } else {
                break;
            }
        }

        if (reading_rules) {
            int i1, i2;
            sscanf(line.c_str(), "%d|%d", &i1, &i2);
            rules.emplace_back(i1, i2);
//            cout << i1 << "|" << i2 << endl;
        } else {
            istringstream iss(line);
            string n;
            auto v = new vector<int>;
            while (getline(iss, n, ',')) {
                v->push_back(stoi(n));
                if (iss.eof()) {
                    break;
                }
            }
            updates.push_back(v);
        }
    }

    // part 1

    int answer1 = 0;

    vector<int> correct_lines;
    vector<int> incorrect_lines;

    int count = 0;
    for (auto &update: updates) {
        // check if rule applies
        bool ok = true;
        for (auto &pair: rules) {
            bool cont = ranges::find_if(*update, [pair](auto v) { return pair.first == v; }) != update->end();
            cont &= ranges::find_if(*update, [pair](auto v) { return pair.second == v; }) != update->end();
            if (cont) {
                for (size_t pos = 0; pos < update->size(); pos++) {
                    int v = update->at(pos);
                    if (v == pair.first) {
                        // check if pair.second is not before
                        for (size_t p2 = 0; p2 < pos; p2++) {
                            if (update->at(p2) == pair.second) {
                                ok = false;
                                break;
                            }
                        }
                    } else if (v == pair.second) {
                        // check if pair.first is not after
                        for (size_t p2 = pos+1; p2 < update->size(); p2++) {
                            if (update->at(p2) == pair.first) {
                                ok = false;
                                break;
                            }
                        }
                    }
                    if (!ok) {
                        break;
                    }
                }
                if (!ok) {
                    break;
                }
            }
            if (!ok) {
                break;
            }
        }
        if (ok) {
            answer1 += update->at(update->size() / 2);
            correct_lines.push_back(count);
        } else {
            incorrect_lines.push_back(count);
        }
        count++;
    }

    // part 2

    int answer2 = 0;

    // the rules are not distinct!
    // 11|18
    // 18|15
    // 15|11

    for (auto &l: incorrect_lines) {
        auto update = updates[l];
        vector<int> correct_order;

        auto all_pages_set = set<int>();
        for (auto &p: rules) {
            bool cont = ranges::find_if(*update, [p](auto v) { return p.first == v; }) != update->end();
            cont &= ranges::find_if(*update, [p](auto v) { return p.second == v; }) != update->end();
            if (cont) {
                all_pages_set.insert(p.first);
                all_pages_set.insert(p.second);
            }
        }
        auto all_pages = vector<int>();
        all_pages.insert(all_pages.begin(), all_pages_set.begin(), all_pages_set.end());

        sort(all_pages.begin(), all_pages.end(), [rules](const int &el1, const int &el2) {
            for (auto &pair: rules) {
                if (pair.first == el1 && pair.second == el2) {
                    return true;
                }
            }
            return false;
        });

        for (auto &p: all_pages) {
            auto f = find_if(update->begin(), update->end(), [p](int el) { return p == el; });
            if (f < update->end()) {
                correct_order.push_back(p);
            }
        }
        cout << "--- incorrect order " << l << "\n";
        for (auto &p: *update) {
            cout << p << " ";
        }
        cout << endl;
        cout << "--- correct order\n";
        for (auto &p: correct_order) {
            cout << p << " ";
        }
        cout << endl;
        cout << endl;
        answer2 += correct_order.at(correct_order.size() / 2);
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    for (auto &v: updates) {
        delete v;
    }
    for (auto &v: full_rules) {
        delete v;
    }

    return EXIT_SUCCESS;
}
