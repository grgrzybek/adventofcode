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
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    aoc2023::Options options("Day 06", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    string token;
    int n;
    vector<int> times;
    unsigned long one_time = 0;
    vector<int> distances;
    unsigned long one_distance = 0;

    getline(*input, line);
    aoc2023::trim(line);

    // Time:      7  15   30
    string::size_type pos = line.find(':');
    line = line.substr(pos + 1);

    istringstream iss(line);
    ostringstream oss;
    while (!iss.eof()) {
        iss >> n;
        times.emplace_back(n);
        oss << n;
    }
    iss.clear();
    iss.str(oss.str());
    iss >> one_time;
    cout << "one_time: " << one_time << endl;

    getline(*input, line);
    aoc2023::trim(line);

    // Distance:  9  40  200
    pos = line.find(':');
    line = line.substr(pos + 1);

    iss.clear();
    iss.str(line);
    oss.clear();
    oss.str("");
    while (!iss.eof()) {
        iss >> n;
        distances.emplace_back(n);
        oss << n;
    }
    iss.clear();
    iss.str(oss.str());
    iss >> one_distance;
    cout << "one_distance: " << one_distance << endl;

    for (auto &t: times) {
        cout << t << endl;
    }

    // part 1

    int answer1 = 1;
    unsigned long d;
    int count = 0;

    for (int i = 0; i < (int) times.size(); i++) {
        int time = times[i];
        unsigned long distance = distances[i];

//        cout << "checking time " << time << endl;
        for (int t = 0; t <= time; t++) {
            d = t * (time - t);
//            cout << "    t=" << t << ", d=" << d << endl;
            if (d > distance) {
                count++;
            }
        }
        answer1 *= count;
    }

    // part 2

    int answer2 = 0;
    count = 0;

    for (unsigned long t = 0; t <= one_time; t++) {
        d = t * (one_time - t);
//            cout << "    t=" << t << ", d=" << d << endl;
        if (d > one_distance) {
            count++;
        }
    }
    answer2 = count;

//    cout << "one time: " << one_time << endl;
//    cout << "one distance: " << one_distance << endl;

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}
