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

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    aoc2024::Options options("Day 07", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;

    vector<long> results;
    vector<vector<long>*> numbers;

    while (getline(*input, line)) {
        aoc2024::trim(line);
        if (line.empty()) {
            break;
        }
        size_t colon = line.find(':');
        results.push_back(stol(line.substr(0, colon)));
        auto args = new vector<long>;
        numbers.push_back(args);
        istringstream iss(line.substr(colon + 1));
        string v;
        while (!iss.eof()) {
            iss >> v;
            args->push_back(stol(v));
        }
    }

//    for (size_t i = 0; i < results.size(); i++) {
//        cout << results.at(i) << ": ";
//        for (auto &a: *(numbers.at(i))) {
//            cout << a << " ";
//        }
//        cout << endl;
//    }

    // part 1

    long answer1 = 0;

    for (size_t idx = 0; idx < results.size(); idx++) {
        long result = results.at(idx);
        auto args = numbers[idx];

        int max = (1 << (args->size() - 1));
        bool match = false;
        for (int v = 0; v < max; v++) {
            int bit = 1;
            long a = args->at(0);
            for (size_t arg = 1; arg < args->size(); arg++) {
                if (v & bit) {
                    // 1 == *, 0 == +
                    a *= args->at(arg);
                } else {
                    a += args->at(arg);
                }
                bit <<= 1;
            }
            if (result == a) {
                match = true;
                break;
            }
        }
        if (match) {
            answer1 += result;
        }
    }

    // part 2

    long answer2 = 0;

    for (size_t idx = 0; idx < results.size(); idx++) {
        cout << "checking " << (idx + 1) << " of " << results.size() << endl;
        long result = results.at(idx);
        auto args = numbers[idx];

        bool match = false;
        int max = pow(3, (args->size() - 1));
        for (int v = 0; v < max; v++) {
            string v3 = "";
            int _v = v;
            while (_v > 0) {
                v3 = to_string(_v % 3) + v3;
                _v /= 3;
            }
            while (v3.length() < args->size() - 1) {
                v3 = "0" + v3;
            }
//            cout << " - checking " << v3 << endl;
            int bit = 0;
            long a = args->at(0);
            for (size_t arg = 1; arg < args->size(); arg++) {
                if (v3[bit] == '0') {
                    // 2 == ||, 1 == *, 0 == +
//                    cout << "    - " << a << " * " << args->at(arg) << endl;
                    a *= args->at(arg);
                } else if (v3[bit] == '1') {
//                    cout << "    - " << a << " + " << args->at(arg) << endl;
                    a += args->at(arg);
                } else if (v3[bit] == '2') {
                    // concatenation
//                    cout << "    - " << a << " || " << args->at(arg) << endl;
                    string as = to_string(args->at(arg));
                    a *= pow(10, as.length());
                    a += args->at(arg);
                }
                bit++;
            }
            if (result == a) {
                match = true;
                break;
            }
        }
        if (match) {
            answer2 += result;
        }
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    for (auto &v: numbers) {
        delete v;
    }

    return EXIT_SUCCESS;
}
