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
#include <set>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;
using namespace aoc2021;

int main(int argc, char *argv[]) {
    Options options("Day 17", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<string> lines;

    getline(*input, line);
    trim(line);
    int x1, x2, y1, y2;
    sscanf(line.c_str(), "target area: x=%d..%d, y=%d..%d", &x1, &x2, &y1, &y2);

    // part 1

    // final velocity in y axis - probe will always hit y=0, so highest velocity
    // is when from 0 it goes to y1 in next step
    int answer1 = y1, final_v_y = -y1;
    while (final_v_y != 0)
        answer1 += final_v_y--;

    cout << "Answer 1: " << answer1 << endl;

    // part 2

    int min_v_x = 0;
    int max_v_x = x2;
    int min_v_y = y1;
    int max_v_y = -y1 + 1;

    while (true) {
        int p = min_v_x * (min_v_x + 1) / 2 - 1;
        if (p >= x1 && p <= x2) {
            break;
        }
        min_v_x++;
    }

    int answer2 = 0;
    for (int vx = min_v_x; vx <= max_v_x; vx++) {
        for (int vy = min_v_y; vy <= max_v_y; vy++) {
            int x = 0, y = 0;
            int _vx = vx;
            int _vy = vy;
            while (true) {
                if (x >= x1 && x <= x2 && y >= y1 && y <= y2) {
                    answer2++;
                    break;
                }
                if (x > x2 || y < y1) {
                    break;
                }
                x += _vx;
                y += _vy;
                if (_vx > 0) {
                    _vx--;
                }
                _vy--;
            }
        }
    }

    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}
