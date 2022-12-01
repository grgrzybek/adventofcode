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
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    aoc2022::Options options("Day 01", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<int> numbers;
    int single_elf = 0;

    while (getline(*input, line)) {
        aoc2022::trim(line);
        if (!aoc2022::has_text(line)) {
            numbers.emplace_back(single_elf);
            single_elf = 0;
            continue;
        }

        int v = 0;
        // number per line
        istringstream iss(line);
        iss >> v;
        single_elf += v;
    }
    numbers.emplace_back(single_elf);

    // part 1

    int answer1 = 0;
    for (auto elf: numbers) {
        if (elf > answer1) {
            answer1 = elf;
        }
    }

    // part 2

    int answer2 = 0;
    sort(numbers.begin(), numbers.end(), [](int a, int b) {
        return a > b;
    });
    answer2 = numbers[0] + numbers[1] + numbers[2];

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}
