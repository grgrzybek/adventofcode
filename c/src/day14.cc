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
#include <set>
#include <string>
#include <vector>
#include <numeric>

#include <openssl/sha.h>
#include <iomanip>

#include "utils/utils.h"

using namespace std;

string sha(char *map, int size);
void cycle(char *map, int w, int h);
void to_string(char *map, int w, int h);

int main(int argc, char *argv[]) {
    aoc2023::Options options("Day 14", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<string> lines;

    char *map_p1, *map;
    int w, h = 0;

    while (getline(*input, line)) {
        aoc2023::trim(line);
        w = line.length();
        h++;
        lines.emplace_back(line);
    }

    map_p1 = (char *) malloc(sizeof(char) * w * h + 1);
    map = (char *) malloc(sizeof(char) * w * h + 1);
    map_p1[w * h] = 0;
    map[w * h] = 0;
    for (int y = 0; y < h; y++) {
        strncpy(map_p1 + (y * w), lines[y].c_str(), w);
        strncpy(map + (y * w), lines[y].c_str(), w);
    }

    // part 1

//    cout << "Before" << endl;
//    for (int y = 0; y < h; y++) {
//        for (int x = 0; x < w; x++) {
//            cout << map_p1[y * w + x];
//        }
//        cout << endl;
//    }

    unsigned long answer1 = 0;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (map_p1[y * w + x] == 'O') {
                // move it north/up
                for (int yy = y - 1; yy >= 0; yy--) {
                    if (map_p1[yy * w + x] == '.') {
                        map_p1[yy * w + x] = 'O';
                        map_p1[(yy + 1) * w + x] = '.';
                    } else {
                        break;
                    }
                }
            }
        }
    }

//    cout << "\nAfter" << endl;
//    for (int y = 0; y < h; y++) {
//        for (int x = 0; x < w; x++) {
//            cout << map_p1[y * w + x];
//        }
//        cout << endl;
//    }

    for (int y = 0, l = h; y < h; y++, l--) {
        for (int x = 0; x < w; x++) {
            if (map_p1[y * w + x] == 'O') {
                answer1 += l;
            }
        }
    }

    // part 2

    unsigned long answer2 = 0;

//    cout << "sha1: " << sha(map, w * h) << endl;

    auto digests = new set<string>;
    auto digests_list = new list<string>;

    string sha1 = sha(map, w * h);
    digests->insert(sha1);
    digests_list->push_back(sha1);
    int n = 0;
    bool got_cycle = false;
//    to_string(map, w, h);
//    cout << " + " << sha1 << endl;
    while (n < 1000000000) {
        cycle(map, w, h);
//        to_string(map, w, h);
        n++;
        if (!got_cycle) {
            sha1 = sha(map, w * h);
            auto r = digests->insert(sha1);
            digests_list->push_back(sha1);
//            cout << " + " << sha1 << endl;
            if (!r.second) {
                // we have a cycle
                int cs = (int) digests->size();
                cout << "cycle size: " << cs << ", n: " << n << endl;
                for (auto &s: *digests_list) {
//                    cout << " - " << s << endl;
                    if (s == sha1) {
                        break;
                    }
                    cs--;
                }
                cout << "cycle size: " << cs << ", n: " << n << endl;

                n += ((1000000000 - n) / cs) * cs;
                cout << "n: " << n << endl;
//                break;
                got_cycle = true;
//                cout << " + " << sha1 << endl;
            }
        }
    }
    sha1 = sha(map, w * h);
    cout << " = " << sha1 << endl;

    for (int y = 0, l = h; y < h; y++, l--) {
        for (int x = 0; x < w; x++) {
            if (map[y * w + x] == 'O') {
                answer2 += l;
            }
        }
    }

    cout << "\nAfter" << endl;
    to_string(map, w, h);

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    free(map);
    free(map_p1);
    delete digests;
    delete digests_list;

    return EXIT_SUCCESS;
}

string sha(char *map, int size) {
    unsigned char dgst[SHA_DIGEST_LENGTH];
    SHA1((unsigned char *)map, size, dgst);
    ostringstream oss;
    for (unsigned char c : dgst) {
        oss << hex << setfill('0') << setw(2) << (int) c;
    }
    return oss.str();
}

void cycle(char *map, int w, int h) {
    // rocks roll north, then west, then south, then east.
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (map[y * w + x] == 'O') {
                // move it north/up
                for (int yy = y - 1; yy >= 0; yy--) {
                    if (map[yy * w + x] == '.') {
                        map[yy * w + x] = 'O';
                        map[(yy + 1) * w + x] = '.';
                    } else {
                        break;
                    }
                }
            }
        }
    }
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            if (map[y * w + x] == 'O') {
                // move it west/left
                for (int xx = x - 1; xx >= 0; xx--) {
                    if (map[y * w + xx] == '.') {
                        map[y * w + xx] = 'O';
                        map[y * w + xx + 1] = '.';
                    } else {
                        break;
                    }
                }
            }
        }
    }
    for (int y = h - 1; y >= 0; y--) {
        for (int x = 0; x < w; x++) {
            if (map[y * w + x] == 'O') {
                // move it south/bottom
                for (int yy = y + 1; yy < h; yy++) {
                    if (map[yy * w + x] == '.') {
                        map[yy * w + x] = 'O';
                        map[(yy - 1) * w + x] = '.';
                    } else {
                        break;
                    }
                }
            }
        }
    }
    for (int x = w - 1; x >= 0; x--) {
        for (int y = 0; y < h; y++) {
            if (map[y * w + x] == 'O') {
                // move it east/right
                for (int xx = x + 1; xx <= w - 1; xx++) {
                    if (map[y * w + xx] == '.') {
                        map[y * w + xx] = 'O';
                        map[y * w + xx - 1] = '.';
                    } else {
                        break;
                    }
                }
            }
        }
    }
}

void to_string(char *map, int w, int h) {
    cout << "---\n";
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            cout << map[y * w + x];
        }
        cout << endl;
    }
    cout << endl;
}
