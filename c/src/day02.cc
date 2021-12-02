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
using namespace aoc2021;

int main(int argc, char *argv[]) {
    Options options("Day 02", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<string> commands;
    vector<int> values;
    int count = 0;

    while (getline(*input, line)) {
        trim(line);
        int v = 0;
        string cmd;

        // number per line
        istringstream iss(line);
        iss >> cmd >> v;
        commands.push_back(cmd);
        values.push_back(v);
        count++;
    }

    // part 1

    int h = 0, v = 0;
    for (int i = 0; i < count; i++) {
        if (commands[i] == "forward") {
            h += values[i];
        }
        if (commands[i] == "up") {
            v -= values[i];
        }
        if (commands[i] == "down") {
            v += values[i];
        }
    }

    int answer1 = h * v;

    // part 2

    h = 0, v = 0;
    int aim = 0;
    for (int i = 0; i < count; i++) {
        if (commands[i] == "forward") {
            h += values[i];
            v += aim*values[i];
        }
        if (commands[i] == "up") {
            aim -= values[i];
        }
        if (commands[i] == "down") {
            aim += values[i];
        }
    }

    int answer2 = h * v;

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}
