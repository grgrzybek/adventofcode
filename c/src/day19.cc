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
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

#include "utils/utils.h"

using namespace std;

int build_pattern(string &pattern, string &current, vector<string> &towels);
void build_patterns2(string &pattern, vector<string> &towels, map<string, long> &cache);

int main(int argc, char *argv[]) {
    aoc2024::Options options("Day 19", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    string tok;

    vector<string> towels;
    vector<string> patterns;

    bool reading_towels = true;
    while (getline(*input, line)) {
        aoc2024::trim(line);
        if (line.empty()) {
            reading_towels = false;
            continue;
        }
        if (reading_towels) {
            string::size_type p1 = 0;
            string::size_type p2;
            while (true) {
                p2 = line.find(',', p1);
                if (p2 == string::npos) {
                    // last towel
                    towels.push_back(line.substr(p1));
                    break;
                }
                towels.push_back(line.substr(p1, p2 - p1));
//                cout << "found towel: " << line.substr(p1, p2 - p1) << endl;
                p1 = p2 + 2;
            }
        } else {
            patterns.push_back(line);
        }
    }

//    for (auto &t: towels) {
//        cout << t << endl;
//    }

    // part 1

    long answer1 = 0;

    std::sort(towels.begin(), towels.end(), [](string t1, string t2) {
        if (t1.length() > t2.length()) {
            return true;
        }
        if (t1.length() < t2.length()) {
            return false;
        }
        return t1.compare(t2) < 0;
    });
    for (auto &t: towels) {
        cout << "t: " << t << endl;
    }

    for (auto &p: patterns) {
//        cout << "Checking pattern \"" << p << "\"\n";

        string current = "";
        answer1 += build_pattern(p, current, towels);
    }

    // part 2

    long answer2 = 0;

    for (auto &p: patterns) {
        cout << "Checking pattern \"" << p << "\"\n";

        map<string, long> cache;
        build_patterns2(p, towels, cache);
        answer2 += cache[p];
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}

int build_pattern(string &pattern, string &current, vector<string> &towels) {
//    cout << "Checking pattern \"" << pattern << "\" based on current \"" << current << "\"\n";
    for (auto &c: towels) {
        if (pattern.starts_with(current + c)) {
            string nc = current + c;
            if (pattern == nc) {
                return 1;
            }
            int found = build_pattern(pattern, nc, towels);
            if (found) {
                return 1;
            }
        }
    }
    return 0;
}

void build_patterns2(string &pattern, vector<string> &towels, map<string, long> &cache) {
    if (pattern == "") {
        return;
    }
    if (cache.contains(pattern)) {
        return;
    }
    cout << "   Caching pattern \"" << pattern << "\"\n";
    for (auto &c: towels) {
        if (pattern == c) {
            // the remaining pattern is simply this towel
            cache[pattern] = 1;
        } else if (pattern.starts_with(c)) {
            string shorter = pattern.substr(c.length());
            cout << "      Splitting to \"" << c << "\" and \"" << shorter << "\"\n";
            build_patterns2(shorter, towels, cache);
        }
    }
    for (auto &c: towels) {
        if (pattern.starts_with(c)) {
            string shorter = pattern.substr(c.length());
            cache[pattern] += cache[shorter];
        }
    }
}
