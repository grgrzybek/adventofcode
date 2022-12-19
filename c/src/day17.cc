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
#include <cstring>
#include <iostream>
#include <list>
#include <vector>

#include "utils/utils.h"

using namespace std;

string rock1 = "####";
string rock1_space = ".......";
string rock2 = ".#."
               "###"
               ".#.";
string rock2_space = "......."
                     "......."
                     ".......";
string rock3 = "..#"
               "..#"
               "###";
string rock3_space = "......."
                     "......."
                     ".......";
string rock4 = "#"
               "#"
               "#"
               "#";
string rock4_space = "......."
                     "......."
                     "......."
                     ".......";
string rock5 = "##"
               "##";
string rock5_space = "......."
                     ".......";

struct shape {
    string &rock;
    string &rock_space;
    int width;
    int height;

    shape(string &rock, string &rock_space, int width, int height) : rock(rock), rock_space(rock_space), width(width), height(height) {}
};

unsigned long simulate(long shapes, vector<shape> &rocks, string &winds) {
    long size = 55000;

    char *well = new char[7 * size * 4];
    memset(well, '.', sizeof(char) * 7 * size * 4);

    long bottom = size * 4;
    long current_bottom = bottom;
    size_t current_rock = 0;
    size_t current_wind = 0;

    string *well_fragment = nullptr;

    long height_diff = 0;

    bool we_have_a_pattern = false;
    string pattern;
    string match;

    long rock_count = 0;
    long prev_rock = 0;
    long prev_height = 0;

    while (rock_count < shapes) {
        if (bottom >= current_bottom + 1000) {
            if (!we_have_a_pattern) {
                // let's remember current top 20 rows
                pattern.assign(well + current_bottom * 7, 20 * 7);
                we_have_a_pattern = true;
            } else {
                match.assign(well + current_bottom * 7, 20 * 7);
                if (pattern == match) {
                    if (prev_rock == 0 && prev_height == 0) {
                        prev_rock = rock_count;
                        prev_height = bottom - current_bottom;
                    } else {
                        long skip_cycles = (shapes - rock_count) / (rock_count - prev_rock);
                        rock_count += skip_cycles * (rock_count - prev_rock);
                        height_diff += skip_cycles * (bottom - current_bottom - prev_height);
                    }
                }
            }
        }
        shape &s = rocks[current_rock++];

        int indent = 2;
        long top_row = current_bottom - s.height - 3;

        while (true) {
            char dir = winds[current_wind++];

            delete well_fragment;
            well_fragment = new string(s.rock_space);
            *well_fragment += ".......";

            // draw current well fragment
            for (int row = 0; row <= s.height; row++) {
                for (int x = 0; x < 7; x++) {
                    if (row + top_row >= bottom) {
                        (*well_fragment)[row * 7 + x] = '#';
                    } else {
                        (*well_fragment)[row * 7 + x] = well[(row + top_row) * 7 + x];
                    }
                }
            }
            int dx = dir == '>' ? 1 : -1;
            int new_indent = indent + dx;
            if (new_indent + s.width > 7) {
                new_indent = 7 - s.width;
            }
            if (new_indent < 0) {
                new_indent = 0;
            }
            // check if a shape can be drawn after the wind blows
            bool ok = true;
            for (int row = 0; row < s.height; row++) {
                for (int x = new_indent; x < s.width + new_indent; x++) {
                    char wc = (*well_fragment)[row * 7 + x];
                    char sc = s.rock[row * s.width + x - new_indent];
                    if (wc != '.' && sc == '#') {
                        ok = false;
                        break;
                    }
                }
                if (!ok) {
                    break;
                }
            }
            if (ok) {
                // the wind moved the shape
                indent = new_indent;
            }
            ok = true;
            // check if a shape can be drawn after falling down
            for (int row = 1; row <= s.height; row++) {
                for (int x = indent; x < s.width + indent; x++) {
                    char wc = (*well_fragment)[row * 7 + x];
                    char sc = s.rock[(row - 1) * s.width + (x - indent)];
                    if (wc != '.' && sc == '#') {
                        ok = false;
                        break;
                    }
                }
                if (!ok) {
                    break;
                }
            }
            if (ok) {
                top_row++;
            } else {
                // draw at current position in the well
                for (int row = 0; row < s.height; row++) {
                    for (int x = indent; x < s.width + indent; x++) {
                        if (s.rock[row * s.width + x - indent] == '#') {
                            well[(top_row + row) * 7 + x] = '1' + (char)(current_rock-1);
                        }
                    }
                }
                current_bottom -= s.height;
                bool finish = false;
                for (; current_bottom < bottom; current_bottom++) {
                    for (int x = 0; x < 7; x++) {
                        if (well[current_bottom * 7 + x] != '.') {
                            finish = true;
                            break;
                        }
                    }
                    if (finish) {
                        break;
                    }
                }
            }

            current_wind %= winds.length();
            if (!ok) {
                break;
            }
        }

        current_rock %= 5;
        rock_count++;
    }

    unsigned long answer = bottom - current_bottom + height_diff;

    delete[] well;

    return answer;
}

int main(int argc, char *argv[]) {
    aoc2022::Options options("Day 17", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string winds;
    getline(*input, winds);

    vector<shape> rocks;
    rocks.emplace_back(rock1, rock1_space, 4, 1);
    rocks.emplace_back(rock2, rock2_space, 3, 3);
    rocks.emplace_back(rock3, rock3_space, 3, 3);
    rocks.emplace_back(rock4, rock4_space, 1, 4);
    rocks.emplace_back(rock5, rock5_space, 2, 2);

    // part 1

    unsigned long answer1 = simulate(2022, rocks, winds);
    cout << "Answer 1: " << answer1 << endl;

    // part 2

    unsigned long answer2 = simulate(1000000000000, rocks, winds);
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}
