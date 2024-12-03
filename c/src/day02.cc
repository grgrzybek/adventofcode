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

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    aoc2024::Options options("Day 02", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<vector<int> *> reports;

    int n;
    while (getline(*input, line)) {
        aoc2024::trim(line);
        if (line.empty()) {
            break;
        }
        istringstream iss(line);
        auto r = new vector<int>;
        reports.emplace_back(r);

        while (!iss.eof()) {
            iss >> n;
            r->push_back(n);
        }
    }

    // part 1

    int answer1 = 0;

    for (auto &r : reports) {
        int delta = 0;
        bool ok = true;
        int previous = -1;
        for (auto &v : *r) {
            if (previous == -1) {
                previous = v;
                continue;
            } else {
                if (delta == 0) {
                    // we don't know if it's increasing or decreasing
                    if (abs(v - previous) < 1 || abs(v - previous) > 3) {
                        // bad
                        ok = false;
                        break;
                    } else {
                        delta = v > previous ? 1 : -1;
                    }
                } else {
                    if (((v - previous) * delta < 1 || (v - previous) * delta > 3)) {
                        ok = false;
                        break;
                    }
                }
                previous = v;
            }
        }
        if (ok) {
            answer1++;
        }
    }

    // part 2

    int answer2 = 0;

    for (auto &r : reports) {
        for (size_t skip = 0; skip < r->size(); skip++) {
            int delta = 0;
            bool ok = true;
            int previous = -1;
            size_t elem = -1;
            for (auto &v : *r) {
                elem++;
                if (skip == elem) {
                    continue;
                }
                if (previous == -1) {
                    previous = v;
                    continue;
                } else {
                    if (delta == 0) {
                        // we don't know if it's increasing or decreasing
                        if (abs(v - previous) < 1 || abs(v - previous) > 3) {
                            // bad
                            ok = false;
                            break;
                        } else {
                            delta = v > previous ? 1 : -1;
                        }
                    } else {
                        if (((v - previous) * delta < 1 || (v - previous) * delta > 3)) {
                            ok = false;
                            break;
                        }
                    }
                    previous = v;
                }
            }
            if (ok) {
                answer2++;
                break;
            }
        }
    }

    for (auto &r : reports) {
        delete r;
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}
