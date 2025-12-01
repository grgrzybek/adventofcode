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
    aoc2025::Options options("Day 01", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<int> numbers;

    while (getline(*input, line)) {
        aoc2025::trim(line);

        const char *p = line.c_str();
        int v = (int) strtol(p + 1, nullptr, 10);
        if (*p == 'L') {
            v = -v;
        }
        numbers.emplace_back(v);
    }

    // part 1

    int answer1 = 0;
    int pos = 50;

    for (int number : numbers) {
        pos = (pos + number) % 100;
        if (pos == 0) {
            answer1++;
        }
    }

    // part 2

    int answer2 = 0;
    pos = 50;

    for (int number : numbers) {
        if (number > 0) {
            // right
            pos += number;
            answer2 += pos / 100;
            pos %= 100;
        } else {
            // left
            if (pos == 0) {
                answer2--;
            }
            pos += number;
            while (pos <= 0) {
                pos += 100;
                answer2++;
            }
            pos %= 100;
        }
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}
