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
#include <iostream>
#include <list>
#include <sstream>
#include <string>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    aoc2024::Options options("Day 11", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;


    getline(*input, line);
    istringstream iss(line);
    list<long> values;

    long v;
    while (!iss.eof()) {
        iss >> v;
        values.push_back(v);
    }

    // part 1

    size_t answer1 = 0;

    ostringstream oss;
    for (int i = 0; i < 25; i++) {
        for (auto it = values.begin(); it != values.end(); it++) {
            v = *it;
            if (v == 0) {
                *it = 1;
            } else {
                oss.str("");
                oss << *it;
                unsigned long size = oss.str().length();
                if (size % 2 == 0) {
                    int v1 = stoi(oss.str().substr(0, size / 2));
                    int v2 = stoi(oss.str().substr(size / 2));
                    *it = v1;
                    it++;
                    it = values.insert(it, v2);
                } else {
                    *it *= 2024;
                }
            }
        }

//        for (auto it = values.begin(); it != values.end(); it++) {
//            cout << *it << " ";
//        }
//        cout << endl;
    }

    answer1 = values.size();

    // part 2

    int answer2 = 0;

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}
