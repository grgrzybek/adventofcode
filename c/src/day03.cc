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
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

struct engine_part {
    int id = 0;
    int number = 0;
    int y, p1, p2;

    friend ostream &operator<<(ostream &os, const engine_part &ep);
};

ostream &operator<<(ostream &os, const engine_part &ep) {
    os << ep.number << " - " << ep.p1 << "-" << ep.p2 << ":" << ep.y;
    return os;
}

int main(int argc, char *argv[]) {
    aoc2023::Options options("Day 03", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<string> engine_lines;
    char *engine;
    char *engine2;
    int w = 0, h = 0;
    int x = 0, y = 0;

    while (getline(*input, line)) {
        aoc2023::trim(line);
        w = static_cast<int>(line.length());
        h++;
        engine_lines.emplace_back(line);
    }
    engine = static_cast<char *>(malloc(sizeof(char) * w * h));
    engine2 = static_cast<char *>(malloc(sizeof(char) * w * h));

//    cout << "lines: " << engine_lines.size() << ", w: " << w << ", h: " << h << endl;

    y = 0;
    for (auto &l: engine_lines) {
        for (x = 0; x < w; x++) {
            engine[y * w + x] = l[x];
            engine2[y * w + x] = l[x];
        }
        y++;
    }

    vector<engine_part> parts;
    vector<pair<int, int>> gears;
    map<string, int> ids;
    map<int, int> part_numbers;

    int p1, p2;
    char c;
    char buf[16];
    int id = 0;
    for (y = 0; y < h; y++) {
        p1 = 0;
        p2 = 0;
        bool number = false;
        for (x = 0; x < w; x++) {
            c = engine[y * w + x];
            if (c >= '0' && c <= '9') {
                // number
                if (!number) {
                    p1 = x;
                    p2 = x;
                    number = true;
                } else {
                    p2++;
                }
            } else {
                // nothing or symbol
                if (number) {
                    p2++;
                    // number between p1 and p2-1
                    engine_part ep = { .id = id++, .y = y, .p1 = p1, .p2 = p2 };
                    ep.number = (int) strtol(&engine[y * w + p1], nullptr, 10);
                    parts.emplace_back(ep);
                }
                number = false;
                if (c == '*') {
                    gears.emplace_back(x, y);
                }
            }
        }
        // end of line
        if (number) {
            p2++;
            // number between p1 and p2-1
            engine_part ep = { .id = id++, .y = y, .p1 = p1, .p2 = p2 };
            strncpy(buf, &engine[y * w + p1], p2 - p1);
            buf[p2-p1] = '\0';
            ep.number = (int) strtol(buf, nullptr, 10);
            parts.emplace_back(ep);
            cout << ep << endl;
        }
        number = false;
    }

    // part 1

    int answer1 = 0;

    for (auto &ep: parts) {
        // index for part 2
        for (x = ep.p1; x < ep.p2; x++) {
            sprintf(buf, "%d:%d", x, ep.y);
            string k(buf);
            ids[k] = ep.id;
            part_numbers[ep.id] = ep.number;
        }

        // continue with part 1
        bool is_part = false;
        char cp;
        if (ep.p1 + 1 == ep.p2) {
            // check 8 around
            // top
            if (ep.y > 0) {
                // left
                if (ep.p1 > 0 && engine[(ep.y - 1) * w + (ep.p1 - 1)] != '.') {
                    cp = engine[(ep.y - 1) * w + (ep.p1 - 1)];
                    is_part = true;
                }
                // top
                if (engine[(ep.y - 1) * w + ep.p1] != '.') {
                    cp = engine[(ep.y - 1) * w + ep.p1];
                    is_part = true;
                }
                // right
                if (ep.p1 < w - 1 && engine[(ep.y - 1) * w + (ep.p1 + 1)] != '.') {
                    cp = engine[(ep.y - 1) * w + (ep.p1 + 1)];
                    is_part = true;
                }
            }
            // same line
            // left
            if (ep.p1 > 0 && engine[ep.y * w + (ep.p1 - 1)] != '.') {
                cp = engine[ep.y * w + (ep.p1 - 1)];
                is_part = true;
            }
            // right
            if (ep.p1 < w - 1 && engine[ep.y * w + (ep.p1 + 1)] != '.') {
                cp = engine[ep.y * w + (ep.p1 + 1)];
                is_part = true;
            }
            // bottom
            if (ep.y < h - 1) {
                // left
                if (ep.p1 > 0 && engine[(ep.y + 1) * w + (ep.p1 - 1)] != '.') {
                    cp = engine[(ep.y + 1) * w + (ep.p1 - 1)];
                    is_part = true;
                }
                // bottom
                if (engine[(ep.y + 1) * w + ep.p1] != '.') {
                    cp = engine[(ep.y + 1) * w + ep.p1];
                    is_part = true;
                }
                // right
                if (ep.p1 < w - 1 && engine[(ep.y + 1) * w + (ep.p1 + 1)] != '.') {
                    cp = engine[(ep.y + 1) * w + (ep.p1 + 1)];
                    is_part = true;
                }
            }
        } else {
            // check each digit
            for (x = ep.p1; x < ep.p2; x++) {
                // top
                if (ep.y > 0) {
                    // left
                    if (x > 0 && engine[(ep.y - 1) * w + (x - 1)] != '.') {
                        cp = engine[(ep.y - 1) * w + (x - 1)];
                        is_part = true;
                    }
                    // top
                    if (engine[(ep.y - 1) * w + x] != '.') {
                        cp = engine[(ep.y - 1) * w + x];
                        is_part = true;
                    }
                    // right
                    if (x < w - 1 && engine[(ep.y - 1) * w + (x + 1)] != '.') {
                        cp = engine[(ep.y - 1) * w + (x + 1)];
                        is_part = true;
                    }
                }
                // same line
                // left
                if (x > 0 && x == ep.p1 && engine[ep.y * w + (x - 1)] != '.') {
                    cp = engine[ep.y * w + (x - 1)];
                    is_part = true;
                }
                // right
                if (x < w - 1 && x + 1 == ep.p2 && engine[ep.y * w + (x + 1)] != '.') {
                    cp = engine[ep.y * w + (x + 1)];
                    is_part = true;
                }
                // bottom
                if (ep.y < h - 1) {
                    // left
                    if (x > 0 && engine[(ep.y + 1) * w + (x - 1)] != '.') {
                        cp = engine[(ep.y + 1) * w + (x - 1)];
                        is_part = true;
                    }
                    // bottom
                    if (engine[(ep.y + 1) * w + x] != '.') {
                        cp = engine[(ep.y + 1) * w + x];
                        is_part = true;
                    }
                    // right
                    if (x < w - 1 && engine[(ep.y + 1) * w + (x + 1)] != '.') {
                        cp = engine[(ep.y + 1) * w + (x + 1)];
                        is_part = true;
                    }
                }
            }
        }

        if (is_part) {
//            cout << "is part: " << ep << ": " << cp << endl;
            answer1 += ep.number;
        } else {
//            cout << "is not part: " << ep << endl;
            for (x = ep.p1; x < ep.p2; x++) {
                engine2[ep.y * w + x] = '.';
            }
        }
    }

//    fstream out("/data/tmp/engine.txt", ios_base::out);
//    for (y = 0; y < h; y++) {
//        out.write(engine2 + y * w, w);
//        out << endl;
//    }
//    out.close();

    // part 2

    int answer2 = 0;

    vector<pair<int, int>> to_check;
    set<int> ids_to_check;
    for (auto &gear: gears) {
        to_check.clear();
        ids_to_check.clear();
        to_check.emplace_back(gear.first - 1, gear.second - 1);
        to_check.emplace_back(gear.first, gear.second - 1);
        to_check.emplace_back(gear.first + 1, gear.second - 1);
        to_check.emplace_back(gear.first - 1, gear.second);
        to_check.emplace_back(gear.first + 1, gear.second);
        to_check.emplace_back(gear.first - 1, gear.second + 1);
        to_check.emplace_back(gear.first, gear.second + 1);
        to_check.emplace_back(gear.first + 1, gear.second + 1);
        for (auto &p: to_check) {
            sprintf(buf, "%d:%d", p.first, p.second);
            string k(buf);
            if (ids.contains(k)) {
                ids_to_check.insert(ids[k]);
            }
        }
        cout << "checking " << gear.first << ":" << gear.second << endl;
        int gear_ratio = 1;
        int part_count = 0;
        for (int pid: ids_to_check) {
            cout << " - " << pid << ": " << part_numbers[pid] << endl;
            gear_ratio *= part_numbers[pid];
            part_count++;
        }
        if (part_count == 2) {
            answer2 += gear_ratio;
        }
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    free(engine);
    free(engine2);

    return EXIT_SUCCESS;
}
