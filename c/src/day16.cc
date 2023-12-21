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
#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <numeric>

#include "utils/utils.h"

using namespace std;

struct beam {
    int dx, dy, x, y;
};

void show_map(char *map, int w, int h);
void show_energy(const unsigned int *map, int w, int h);

int main(int argc, char *argv[]) {
    aoc2023::Options options("Day 16", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<string> lines;

    char *map;
    unsigned int *energy;
    int w, h = 0;

    while (getline(*input, line)) {
        aoc2023::trim(line);
        w = (int) line.length();
        h++;
        lines.emplace_back(line);
    }

    map = (char *) malloc(sizeof(char) * w * h);
    energy = (unsigned int *) malloc(sizeof(unsigned int) * w * h);
    for (int y = 0; y < h; y++) {
        strncpy(map + (y * w), lines[y].c_str(), w);
    }
    memset((void *) energy, (int) 0, w * h);

    // part 1

    unsigned long answer1 = 0;

    deque<beam> beams;
    beams.emplace_back(beam{ .dx = 1, .dy = 0, .x = 0, .y = 0 });

    while (!beams.empty()) {
        auto b = beams.front();
        beams.pop_front();

        auto dx = b.dx;
        auto dy = b.dy;
        auto x = b.x;
        auto y = b.y;

        // energy: 0xNNEESSWW
        unsigned char e = energy[y * w + x];
        int new_e = dx == 0 ? (dy == 1 ? 0x00001100 : 0x11000000) : (dx == 1 ? 0x00110000 : 0x00000011);
        if (e & new_e) {
            // we already went in that direction
            continue;
        }
        energy[y * w + x] |= new_e;

//        cout << "-----------\n\n";
//        show_energy(energy, w, h);

        char field = map[y * w + x];
        switch (field) {
            case '.':
                // keep going
                if ((dx == -1 && x > 0) || (dx == 1 && x < w - 1) || (dy == -1 && y > 0) || (dy == 1 && y < h - 1)) {
                    beams.emplace_back(beam{ .dx = dx, .dy = dy, .x = x + dx, .y = y + dy });
                }
                break;
            case '/':
                if (dy == -1) {
                    // N->E
                    if (x < w - 1) {
                        beams.emplace_back(beam{ .dx = 1, .dy = 0, .x = x + 1, .y = y });
                    }
                } else if (dx == 1) {
                    // E->N
                    if (y > 0) {
                        beams.emplace_back(beam{ .dx = 0, .dy = -1, .x = x, .y = y - 1 });
                    }
                } else if (dy == 1) {
                    // S->W
                    if (x > 0) {
                        beams.emplace_back(beam{ .dx = -1, .dy = 0, .x = x - 1, .y = y });
                    }
                } else if (dx == -1) {
                    // W->S
                    if (y < h - 1) {
                        beams.emplace_back(beam{ .dx = 0, .dy = 1, .x = x, .y = y + 1 });
                    }
                }
                break;
            case '\\':
                if (dy == -1) {
                    // N->W
                    if (x > 0) {
                        beams.emplace_back(beam{ .dx = -1, .dy = 0, .x = x - 1, .y = y });
                    }
                } else if (dx == 1) {
                    // E->S
                    if (y < h - 1) {
                        beams.emplace_back(beam{ .dx = 0, .dy = 1, .x = x, .y = y + 1 });
                    }
                } else if (dy == 1) {
                    // S->E
                    if (x < w - 1) {
                        beams.emplace_back(beam{ .dx = 1, .dy = 0, .x = x + 1, .y = y });
                    }
                } else if (dx == -1) {
                    // W->N
                    if (y > 0) {
                        beams.emplace_back(beam{ .dx = 0, .dy = -1, .x = x, .y = y - 1 });
                    }
                }
                break;
            case '|':
                if (dx == 0) {
                    // keep going
                    if ((dy == -1 && y > 0) || (dy == 1 && y < h - 1)) {
                        beams.emplace_back(beam{ .dx = dx, .dy = dy, .x = x + dx, .y = y + dy });
                    }
                } else {
                    // split up/down
                    if (y > 0) {
                        beams.emplace_back(beam{ .dx = 0, .dy = -1, .x = x, .y = y - 1 });
                    }
                    if (y < h - 1) {
                        beams.emplace_back(beam{ .dx = 0, .dy = 1, .x = x, .y = y + 1 });
                    }
                }
                break;
            case '-':
                if (dy == 0) {
                    // keep going
                    if ((dx == -1 && x > 0) || (dx == 1 && x < w - 1)) {
                        beams.emplace_back(beam{ .dx = dx, .dy = 0, .x = x + dx, .y = y });
                    }
                } else {
                    // split left/right
                    if (x > 0) {
                        beams.emplace_back(beam{ .dx = -1, .dy = 0, .x = x - 1, .y = y });
                    }
                    if (x < w - 1) {
                        beams.emplace_back(beam{ .dx = 1, .dy = 0, .x = x + 1, .y = y });
                    }
                }
                break;
            default:
                break;
        }
    }

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (energy[y * w + x] != 0) {
                answer1++;
            }
        }
    }

    // part 2

    unsigned long answer2 = 0;

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    free(map);
    free(energy);

    return EXIT_SUCCESS;
}

void show_map(char *map, int w, int h) {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            cout << map[y * w + x];
        }
        cout << endl;
    }
    cout << endl;
}

void show_energy(const unsigned int *map, int w, int h) {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            unsigned int c = map[y * w + x];
            int count = 0;
            count += (c & 0x11000000) ? 1 : 0;
            count += (c & 0x00110000) ? 1 : 0;
            count += (c & 0x00001100) ? 1 : 0;
            count += (c & 0x00000011) ? 1 : 0;
            if (count == 0) {
                cout << '.';
            } else if (count == 1) {
                if (c & 0x11000000) {
                    cout << '^';
                }
                if (c & 0x00110000) {
                    cout << '>';
                }
                if (c & 0x00001100) {
                    cout << 'v';
                }
                if (c & 0x00000011) {
                    cout << '<';
                }
            } else {
                cout << count;
            }
        }
        cout << endl;
    }
    cout << endl;
}
