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
#include <cstdio>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    aoc2022::Options options("Day 04", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    // 2-4,6-8
    // 2-3,4-5
    // 5-7,7-9
    // 2-8,3-7
    // 6-6,4-6
    // 2-6,4-8

    ifstream *input = options.file();

    string line;
    vector<int *> assignments;

    while (getline(*input, line)) {
        aoc2022::trim(line);
        if (!aoc2022::has_text(line)) {
            break;
        }

        int *a = new int[4];
        sscanf(line.c_str(), "%u-%u,%u-%u", a, a + 1, a + 2, a + 3); // NOLINT(cert-err34-c)
        assignments.push_back(a);
    }

    // part 1

    int answer1 = 0;

    for (auto a: assignments) {
        if ((a[0] <= a[2] && a[1] >= a[3]) || (a[0] >= a[2] && a[1] <= a[3])) {
            answer1++;
        }
    }

    // part 2

    int answer2 = 0;

    for (auto a: assignments) {
        if ((a[0] >= a[2] && a[0] <= a[3])
            || (a[1] >= a[2] && a[1] <= a[3])
            || (a[2] >= a[0] && a[2] <= a[1])
            || (a[3] >= a[0] && a[3] <= a[1])) {
            answer2++;
        }
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    for (auto a: assignments) {
        delete[] a;
    }

    return EXIT_SUCCESS;
}
