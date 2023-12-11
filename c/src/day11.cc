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
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <numeric>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    aoc2023::Options options("Day 11", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<string> lines;
    size_t w = 0, h = 0, x, y;

    vector<size_t> galaxies_in_columns;
    vector<size_t> galaxies_in_lines;

    while (getline(*input, line)) {
        aoc2023::trim(line);
        if (w == 0) {
            w = line.length();
            galaxies_in_columns.reserve(w);
            galaxies_in_columns.insert(galaxies_in_columns.begin(), w, 0);
            cout << "Size: " << galaxies_in_columns.size() << endl;
        }

        if (line.find('#') == string::npos) {
            // expand immediately
            line = line.replace(line.begin(), line.end(), w, '~');
            lines.emplace_back(line);
            lines.emplace_back(line);
            galaxies_in_lines.push_back(0);
            galaxies_in_lines.push_back(0);
            h++;
        } else {
            lines.emplace_back(line);
            string::size_type p = line.find('#');
            int c = 0;
            while (p != string::npos) {
                galaxies_in_columns[p]++;
                p = line.find('#', p + 1);
                c++;
            }
            galaxies_in_lines.push_back(c);
        }
        h++;
    }

    for (auto &c: galaxies_in_columns) {
        if (c == 0) {
            w++;
        }
    }

    cout << "Galaxy size: " << w << "x" << h << endl;

    char *galaxy = (char *) malloc(w * h * sizeof(char));

    vector<pair<int, int>> galaxies;

    for (y = 0; y < h; y++) {
        line = lines[y];
        x = 0;
        for (size_t p = 0; p < line.size(); p++) {
            if (line[p] == '#') {
                galaxies.emplace_back(x, y);
            }
            if (galaxies_in_columns[p] == 0) {
                galaxy[y * w + x++] = '~';
                galaxy[y * w + x++] = '~';
            } else {
                galaxy[y * w + x++] = line[p];
            }
        }
    }

    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            cout << galaxy[y * w + x];
        }
        cout << endl;
    }

//    cout << "Galaxies\n";
//    for (auto &p: galaxies) {
//        cout << " - " << p.first << ":" << p.second << endl;
//    }

    // part 1

    unsigned long answer1 = 0;
    unsigned long dist = 0;

    for (auto &g1: galaxies) {
//        cout << "Checking " << _g1.first << ":" << _g1.second << endl;
        for (auto &g2: galaxies) {
            if (g1.first == g2.first && g1.second == g2.second) {
                continue;
            }
//            cout << "    with " << _g2.first << ":" << _g2.second << endl;
            dist = abs(g2.first - g1.first) + abs(g2.second - g1.second);
            answer1 += dist;
        }
    }

    answer1 /= 2;

    // part 2

    unsigned long answer2 = 0;

    size_t _g1, _g2;
    for (_g1 = 0; _g1 < galaxies.size(); _g1++) {
        auto g1 = galaxies[_g1];
//        cout << "Checking " << g1.first << ":" << g1.second << endl;
        for (_g2 = _g1 + 1; _g2 < galaxies.size(); _g2++) {
            auto g2 = galaxies[_g2];
//            cout << "    with " << g2.first << ":" << g2.second << endl;
            int dx = g1.first <= g2.first ? 1 : -1;
            int dy = g1.second <= g2.second ? 1 : -1;

            dist = 0;
            x = g1.first;
            y = g1.second;
            for (; (int)x != g2.first; x += dx) {
                if (galaxy[y * w + x] == '~') {
                    dist += 500000; // because duplicated space
                } else {
                    dist++;
                }
            }
            for (; (int)y != g2.second; y += dy) {
                if (galaxy[y * w + x] == '~') {
                    dist += 500000; // because duplicated space
                } else {
                    dist++;
                }
            }
//            cout << "     - dist: " << dist << endl;
            answer2 += dist;
        }
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    free(galaxy);

    return EXIT_SUCCESS;
}
