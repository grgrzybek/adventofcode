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
#include <vector>

#include "utils/utils.h"

using namespace std;

struct wall {
    int x1, y1, x2, y2;

    void print() const {
        cout << "[" << x1 << "," << y1 << "] -> [" << x2 << "," << y2 << "]\n";
    }
};

void print_cave(char *cave, int w, int h, int dx, int dy);

int main(int argc, char *argv[]) {
    aoc2022::Options options("Day 14", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<wall> lines;

    int minx = 500;
    int miny = 0;
    int maxx = 500;
    int maxy = 0;

    int x1 = -1, y1 = -1, x2 = -1, y2 = -1;
    while (getline(*input, line)) {
        if (!aoc2022::has_text(line)) {
            continue;
        }
        size_t start = 0, pos = 0;
        while (pos < line.length()) {
            pos = line.find(',', pos);
            string s = line.substr(start, pos);
            x2 = stoi(s);
            start = pos + 1;
            pos = line.find(' ', pos);
            s = line.substr(start, pos);
            y2 = stoi(s);
            if (x2 > maxx) {
                maxx = x2;
            }
            if (x2 < minx) {
                minx = x2;
            }
            if (y2 > maxy) {
                maxy = y2;
            }
            if (y2 < miny) {
                miny = y2;
            }
            start = pos + 4;
            if (x1 != -1) {
                lines.push_back({ .x1 = x1, .y1 = y1, .x2 = x2, .y2 = y2 });
            }
            x1 = x2;
            y1 = y2;
        }
        x1 = -1;
        y1 = -1;
    }

    int w = maxx - minx + 1;
    int dx = -minx;
    int h = maxy - miny + 1;
    int dy = -miny;
    char cave1[w * h];
    memset(cave1, '.', sizeof(char) * w * h);

    for (auto &l: lines) {
        if (l.x1 == l.x2) {
            if (l.y1 < l.y2) {
                for (int y = l.y1; y <= l.y2; y++) {
                    cave1[(y + dy) * w + (l.x1 + dx)] = '#';
                }
            } else {
                for (int y = l.y2; y <= l.y1; y++) {
                    cave1[(y + dy) * w + (l.x1 + dx)] = '#';
                }
            }
        } else {
            if (l.x1 < l.x2) {
                for (int x = l.x1; x <= l.x2; x++) {
                    cave1[(l.y1 + dy) * w + (x + dx)] = '#';
                }
            } else {
                for (int x = l.x2; x <= l.x1; x++) {
                    cave1[(l.y1 + dy) * w + (x + dx)] = '#';
                }
            }
        }
    }

    // part 1

    int answer1 = 0;

    int sx, sy, nsx, nsy;
    char cell;
    bool end = false;
    while (true) {
        sx = 500, sy = 0;
        while (true) {
            nsx = sx;
            nsy = sy + 1;
            if (nsy > maxy) {
                end = true;
                break;
            }
            cell = cave1[(nsy + dy) * w + (nsx + dx)];
            if (cell == '#' || cell == '+') {
                // maybe fall left?
                nsx = sx - 1;
                cell = cave1[(nsy + dy) * w + (nsx + dx)];
                if (cell == '#' || cell == '+') {
                    // maybe fall right?
                    nsx = sx + 1;
                    cell = cave1[(nsy + dy) * w + (nsx + dx)];
                    if (cell == '#' || cell == '+') {
                        // end of falling - fell on top
                        cave1[(sy + dy) * w + (sx + dx)] = '+';
                        answer1++;
                        break;
                    } else {
                        // fell right
                    }
                } else {
                    // fell left
                }
            }
            sx = nsx;
            sy = nsy;
        }
        if (end) {
            break;
        }
    }

    print_cave(cave1, w, h, dx, dy);

    // part 2

    int answer2 = 0;

    h = maxy - miny + 1 + 2;
    w = 2 * h - 1;
    dx += h / 2 - 1;
    char *cave2 = new char[w * h];
    memset(cave2, '.', sizeof(char) * w * h);
    lines.push_back({ .x1 = 500 - h + 1, .y1 = h - 1, .x2 = 500 + h - 1, .y2 = h - 1 });

    for (auto &l: lines) {
        if (l.x1 == l.x2) {
            if (l.y1 < l.y2) {
                for (int y = l.y1; y <= l.y2; y++) {
                    cave2[(y + dy) * w + (l.x1 + dx)] = '#';
                }
            } else {
                for (int y = l.y2; y <= l.y1; y++) {
                    cave2[(y + dy) * w + (l.x1 + dx)] = '#';
                }
            }
        } else {
            if (l.x1 < l.x2) {
                for (int x = l.x1; x <= l.x2; x++) {
                    cave2[(l.y1 + dy) * w + (x + dx)] = '#';
                }
            } else {
                for (int x = l.x2; x <= l.x1; x++) {
                    cave2[(l.y1 + dy) * w + (x + dx)] = '#';
                }
            }
        }
    }

    while (true) {
        sx = 500, sy = 0;
        if (cave2[(sy + dy + 1) * w + (sx + dx)] != '.' && cave2[(sy + dy + 1) * w + (sx + dx - 1)] != '.'
            && cave2[(sy + dy + 1) * w + (sx + dx + 1)] != '.') {
            break;
        }
        while (true) {
            nsx = sx;
            nsy = sy + 1;
            if (cave2[(sy + dy + 1) * w + (sx + dx)] != '.' && cave2[(sy + dy + 1) * w + (sx + dx - 1)] != '.'
                && cave2[(sy + dy + 1) * w + (sx + dx + 1)] != '.') {
                if (cave2[(sy + dy) * w + (sx + dx)] == '.') {
                    cave2[(sy + dy) * w + (sx + dx)] = '+';
                } else {
                    break;
                }
            }
            cell = cave2[(nsy + dy) * w + (nsx + dx)];
            if (cell == '#' || cell == '+') {
                // maybe fall left?
                nsx = sx - 1;
                cell = cave2[(nsy + dy) * w + (nsx + dx)];
                if (cell == '#' || cell == '+') {
                    // maybe fall right?
                    nsx = sx + 1;
                    cell = cave2[(nsy + dy) * w + (nsx + dx)];
                    if (cell == '#' || cell == '+') {
                        // end of falling - fell on top
                        cave2[(sy + dy) * w + (sx + dx)] = '+';
                        answer2++;
                        break;
                    } else {
                        // fell right
                    }
                } else {
                    // fell left
                }
            }
            sx = nsx;
            sy = nsy;
        }
    }

    print_cave(cave2, w, h, dx, dy);

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 + 1 << endl;

    delete[] cave2;

    return EXIT_SUCCESS;
}

void print_cave(char *cave, int w, int h, int dx, int dy) {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (x - dx == 500 && y - dy == 0) {
                cout << "+";
            } else {
                cout << cave[y * w + x];
            }
        }
        cout << "\n";
    }
}
