/*
 * Copyright 2021 Grzegorz Grzybek
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

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    aoc2021::Options options("Day 01", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<int> numbers;

    while (getline(*input, line)) {
        aoc2021::trim(line);
        int v = 0;

        // number per line
        istringstream iss(line);
        iss >> v;
        numbers.push_back(v);
    }

    // part 1

    int answer1 = 0;
    for (vector<int>::size_type i = 0; i < numbers.size(); i++) {
        if (i > 0 && numbers[i] > numbers[i - 1]) {
            answer1++;
        }
    }

    // part 2

    for (vector<int>::size_type i = numbers.size() - 1; i >= 2; i--) {
        vector<int>::size_type from = i - 2;
        int sum = 0;
        for (vector<int>::size_type j = from; j <= i; j++) {
            sum += numbers[j];
        }
        numbers[i] = sum;
    }

    int answer2 = 0;
    for (vector<int>::size_type i = 2; i < numbers.size(); i++) {
        if (i > 2 && numbers[i] > numbers[i - 1]) {
            answer2++;
        }
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}
