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

#include <cstring>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include "utils/utils.h"

void print_ocean(int *ocean, int w, int h);

using namespace std;
using namespace aoc2021;

int main(int argc, char *argv[]) {
    Options options("Day 03", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;

    // 4-element arrays - x1,y1 -> x2,y2
    vector<int *> pairs;

    int minx = numeric_limits<int>::max(), miny = numeric_limits<int>::max();
    int maxx = 0, maxy = 0;

    while (getline(*input, line)) {
        trim(line);

        int *array = new int[4];
        sscanf(line.c_str(), "%d,%d -> %d,%d", array, array + 1, array + 2,
               array + 3);
        if (array[0] < minx) {
            minx = array[0];
        }
        if (array[2] < minx) {
            minx = array[2];
        }
        if (array[1] < miny) {
            miny = array[1];
        }
        if (array[3] < miny) {
            miny = array[3];
        }
        if (array[0] > maxx) {
            maxx = array[0];
        }
        if (array[2] > maxx) {
            maxx = array[2];
        }
        if (array[1] > maxy) {
            maxy = array[1];
        }
        if (array[3] > maxy) {
            maxy = array[3];
        }

        pairs.push_back(array);
    }

    int width = maxx - minx + 1;
    int height = maxy - miny + 1;

    cout << minx << ":" << miny << " -> " << maxx << ":" << maxy << endl;

    // part 1

    int *ocean = new int[width * height];
    memset(ocean, 0, width * height);

    int x1, x2, y1, y2;
    for (int *&pair: pairs) {
        x1 = pair[0] - minx;
        x2 = pair[2] - minx;
        y1 = pair[1] - miny;
        y2 = pair[3] - miny;

        if (x1 == x2 || y1 == y2) {
            // straight line
            if (x1 == x2) {
                // vertical line
                if (y2 > y1) {
                    for (int y = y1; y <= y2; y++) {
                        ocean[width * y + x1]++;
                    }
                } else {
                    for (int y = y2; y <= y1; y++) {
                        ocean[width * y + x1]++;
                    }
                }
            } else {
                // horizontal line
                if (x2 > x1) {
                    for (int x = x1; x <= x2; x++) {
                        ocean[width * y1 + x]++;
                    }
                } else {
                    for (int x = x2; x <= x1; x++) {
                        ocean[width * y1 + x]++;
                    }
                }
            }
        }
    }

    int answer1 = 0;

    for (int v = 0; v < width * height; v++) {
        if (ocean[v] > 1) {
            answer1++;
        }
    }

    // part 2 - additionally the diagonal lines

    for (int *&pair: pairs) {
        x1 = pair[0] - minx;
        x2 = pair[2] - minx;
        y1 = pair[1] - miny;
        y2 = pair[3] - miny;

        if (!(x1 == x2 || y1 == y2)) {
            // diagonal line
            int dx = x1 < x2 ? 1 : -1;
            int dy = y1 < y2 ? 1 : -1;
            cout << "Diagonal line " << x1 << ":" << y1 << " -> " << x2 << ":" << y2 << endl;
            for (int x = x1, y = y1; x != x2 + dx && y != y2 + dy; x += dx, y += dy) {
                cout << " - " << x << ":" << y << endl;
                ocean[width * y + x]++;
            }
        }
    }

    int answer2 = 0;

    for (int v = 0; v < width * height; v++) {
        if (ocean[v] > 1) {
            answer2++;
        }
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    delete[] ocean;
    for (int *&ptr: pairs) {
        delete[] ptr;
    }

    return EXIT_SUCCESS;
}

void print_ocean(int *ocean, int w, int h) {
    for (int i = 0; i < w * h; i++) {
        if (i % w == 0) {
            cout << endl;
        }
        if (ocean[i] == 0) {
            cout << ".";
        } else {
            cout << ocean[i];
        }
    }
    cout << endl;
}
