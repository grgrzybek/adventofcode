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
#include <cstring>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>
#include <numeric>

#include "utils/utils.h"

using namespace std;

struct pattern {
    char *map;
    int w, h;

    ~pattern() {
        free(map);
    }

    friend ostream& operator<<(ostream& os, pattern &p);
};

ostream& operator<<(ostream& os, pattern &p) {
    os << p.w << "x" << p.h << endl;
    for (int y = 0; y < p.h; y++) {
        for (int x = 0; x < p.w; x++) {
            cout << p.map[y * p.w + x];
        }
        cout << endl;
    }
    return os;
}

int main(int argc, char *argv[]) {
    aoc2023::Options options("Day 13", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<string> lines;

    vector<pattern *> patterns;

    size_t w, h;
    h = 0;
    while (true) {
        getline(*input, line);
        aoc2023::trim(line);
        if (!line.empty()) {
            w = line.size();
            lines.emplace_back(line);
            h++;
        } else {
            // next pattern
            auto p = new pattern;
            p->w = w;
            p->h = h;
            p->map = (char *) malloc(sizeof(char) * w * h + 1);
            p->map[w * h] = 0;
            h = 0;
            for (auto &l: lines) {
                strncpy(p->map + (h * w), l.c_str(), w);
                h++;
            }
            lines.clear();
            patterns.emplace_back(p);
            h = 0;
        }
        if (input->eof()) {
            break;
        }
    }

//    for (auto &p: patterns) {
//        cout << p->w << "x" << p->h << ": " << p->map << endl;
//    }

    // part 1

    unsigned long answer1 = 0;

    int l1, l2;
    for (auto &p: patterns) {
//        cout << *p;
        bool found = false;
        // comparing lines
        for (int split = 0; split < p->h - 1; split++) {
            bool same = true;
            l1 = split;
            l2 = split + 1;
            while (true) {
                if (l1 < 0 || l2 >= p->h) {
                    break;
                }
                if (strncmp(p->map + (l1 * p->w), p->map + (l2 * p->w), p->w) != 0) {
                    same = false;
                    break;
                }
                l1--;
                l2++;
            }
            if (same) {
                found = true;
                answer1 += 100 * (split + 1);
//                cout << "split horizontally: " << split + 1 << endl << endl;
                break;
            }
        }
        if (!found) {
            // comparing columns
            for (int split = 0; split < p->w - 1; split++) {
                bool same = true;
                l1 = split;
                l2 = split + 1;
                while (true) {
                    if (l1 < 0 || l2 >= p->w) {
                        break;
                    }
                    for (int y = 0; y < p->h; y++) {
                        if (p->map[y * p->w + l1] != p->map[y * p->w + l2]) {
                            same = false;
                            break;
                        }
                    }
                    if (!same) {
                        break;
                    }
                    l1--;
                    l2++;
                }
                if (same) {
                    answer1 += (split + 1);
//                    cout << "split vertically: " << split + 1 << endl << endl;
                    break;
                }
            }
        }
    }

    // part 2

    unsigned long answer2 = 0;

    for (auto &p: patterns) {
//        cout << *p;
        bool found = false;
        // comparing lines
        for (int split = 0; split < p->h - 1; split++) {
            int differences = 0;
            l1 = split;
            l2 = split + 1;
            while (true) {
                if (l1 < 0 || l2 >= p->h) {
                    break;
                }
                for (int x = 0; x < p->w; x++) {
                    if (p->map[l1 * p->w + x] != p->map[l2 * p->w + x]) {
                        differences++;
                    }
                }
                l1--;
                l2++;
            }
            if (differences == 1) {
                found = true;
                answer2 += 100 * (split + 1);
//                cout << "split horizontally: " << split + 1 << endl << endl;
                break;
            }
        }
        if (!found) {
            // comparing columns
            for (int split = 0; split < p->w - 1; split++) {
                int differences = 0;
                l1 = split;
                l2 = split + 1;
                while (true) {
                    if (l1 < 0 || l2 >= p->w) {
                        break;
                    }
                    for (int y = 0; y < p->h; y++) {
                        if (p->map[y * p->w + l1] != p->map[y * p->w + l2]) {
                            differences++;
                        }
                    }
                    l1--;
                    l2++;
                }
                if (differences == 1) {
                    answer2 += (split + 1);
//                    cout << "split vertically: " << split + 1 << endl << endl;
                    break;
                }
            }
        }
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    for (auto &p: patterns) {
        delete p;
    }

    return EXIT_SUCCESS;
}
