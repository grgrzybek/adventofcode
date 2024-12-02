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
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    aoc2024::Options options("Day 01", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<int> list1, list2;

    int a, b;
    while (getline(*input, line)) {
        aoc2024::trim(line);
        istringstream iss(line);

        iss >> a >> b;
        list1.push_back(a);
        list2.push_back(b);
    }

    // part 1

    int answer1 = 0;

    sort(list1.begin(), list1.end());
    sort(list2.begin(), list2.end());

    for (size_t i = 0; i < list1.size(); i++) {
        answer1 += abs(list1[i] - list2[i]);
    }

    // part 2

    int answer2 = 0;

    for (auto &n1 : list1) {
        answer2 += n1 * (int) count_if(list2.begin(), list2.end(), [n1](int n2) { return n1 == n2; });
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}
