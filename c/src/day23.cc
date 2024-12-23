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
#include <sstream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    aoc2024::Options options("Day 23", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;

    set<string> computers_set;
    set<pair<string, string>> pairs;

    while (getline(*input, line)) {
        aoc2024::trim(line);
        if (line.empty()) {
            break;
        }
        string c1 = line.substr(0, 2);
        string c2 = line.substr(3, 2);
        pairs.emplace(c1, c2);
        pairs.emplace(c2, c1);
        computers_set.insert(c1);
        computers_set.insert(c2);
    }

    cout << "Computers: " << computers_set.size() << endl;
    cout << "Connections: " << pairs.size() << endl;

    vector<string> computers(computers_set.begin(), computers_set.end());
    sort(computers.begin(), computers.end());

    // part 1

    long answer1 = 0;

    // sets of connected computers for each computer (sorted)
    map<string, set<string> *> connected;

    for (auto &c: computers) {
        auto group = new set<string>;
        connected[c] = group;

        for (auto &p: pairs) {
            if (p.first == c) {
                group->insert(p.second);
            }
            if (p.second == c) {
                group->insert(p.first);
            }
        }
    }

    set<set<string>> threes;
    for (auto & c : computers) {
        vector<string> v(connected[c]->begin(), connected[c]->end());
        for (int a = 0; a < (int) v.size(); a++) {
            for (int b = 0; b < (int) v.size(); b++) {
                if (a == b) {
                    continue;
                }
                if (pairs.contains(make_pair(v[a], v[b]))) {
                    threes.insert(set<string>{c, v[a], v[b]});
                }
            }
        }
    }

    for (auto &t: threes) {
        bool match = false;
        for (auto &c: t) {
            match |= c[0] == 't';
        }
        if (match) {
            answer1++;
        }
    }

    // part 2

    string answer2;

    int min = 0;
    ostringstream oss;

    vector<set<string> *> groups;
    for (auto &c : computers) {
        cout << "checking \"" << c << "\"\n";
        auto group = new set<string>;
        group->insert(c);
        groups.push_back(group);
        cout << "   ";
        for (auto &other: *connected[c]) {
            cout << " " << other;
        }
        cout << endl;
        vector<string> v(connected[c]->begin(), connected[c]->end());
        bool *matrix = new bool[v.size() * v.size()];
        fill(matrix, matrix + (v.size() * v.size()), false);
        int matched = 0;
        for (int a = 0; a < (int) v.size(); a++) {
            for (int b = 0; b < (int) v.size(); b++) {
                if (a == b) {
                    matrix[a * v.size() + b] = true;
                    matched++;
                } else {
                    if (pairs.contains(make_pair(v[a], v[b]))) {
                        matrix[a * v.size() + b] = true;
                        matched++;
                    }
                }
            }
        }
        cout << "  matched: " << matched << endl;
        cout << "   ";
        for (int a = 0; a < (int) v.size(); a++) {
            cout << " " << v[a];
        }
        cout << endl;
        if (matched > min) {
            min = matched;
            for (int a = 0; a < (int) v.size(); a++) {
                cout << " " << v[a];
                int fine = 0;
                for (int b = 0; b < (int) v.size(); b++) {
                    cout << " " << (matrix[a * v.size() + b] ? "+ " : "- ");
                    if (matrix[a * v.size() + b]) {
                        fine++;
                    }
                }
                cout << endl;
                cout << "     fine: " << fine << endl;
                if (fine + 1 == (int) v.size()) {
                    group->insert(v[a]);
                }
            }
            oss.clear();
            oss.str("");

            for (auto &x: *group) {
                cout << "     - " << x << endl;
                oss << "," << x;
            }
            answer2 = oss.str().substr(1);
        }
        delete[] matrix;
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    for (auto &s: connected) {
        delete s.second;
    }
    for (auto &g: groups) {
        delete g;
    }

    return EXIT_SUCCESS;
}
