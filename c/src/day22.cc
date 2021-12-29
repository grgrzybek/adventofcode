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

#include <cstdio>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;
using namespace aoc2021;

struct cuboid {
    long long x1, x2, y1, y2, z1, z2;

    void slice(long long _x1, long long _x2,
               long long _y1, long long _y2,
               long long _z1, long long _z2,
               list<cuboid *> &cuboids, bool on) {
        if (_x1 <= x1 && x2 <= _x2 && _y1 <= y1 && y2 <= _y2 && _z1 <= z1 && z2 <= _z2) {
            // special case - full inclusion - this cuboid is swallowed
            return;
        }
        if (x1 > _x2 || x2 < _x1 || y1 > _y2 || y2 < _y1 || z1 > _z2 || z2 < _z1) {
            // special case - no intersection
            // no intersection. this cuboid stays unsliced
            cuboids.push_back(this);
            return;
        }

        /*
         * this:                       (x1)**********************(x2)
         * new (a):             (_x1)---------------------(_x2)
         * new (b):             (_x1)----------------------------------(_x2)
         * new (c):                           (_x1)--------------------(_x2)
         * new (d):                           (_x1)-------(_x2)
         * new (E): (_x1)-------(_x2)
         * new (F):                                                             (_x1)-------(_x2)
         *
         * case a: (x1-_x2), [_x2-x2] - the first one only when NOT intersecting with the new one in other dimensions
         * case b: (x1-x2)
         * case c: [x1-_x1], (_x1-x2)
         * case d: [x1-_x1], (_x1-_x2), [_x2-x2]
         *
         *                  xxxxxxxxxxxxxxxxxxxxxxxxxx                      |
         *       ......                                                     | no intersection
         *       ...........                                                | no intersection
         *       ............                                               | x1:_x2, _x2+1:x2 (x1 == _x2)
         *       ........................                                   | x1:_x2, _x2+1:x2
         *                  .                                               | x1:_x2, _x2+1:x2
         *                  ......................                          | x1:_x2, _x2+1:x2
         *                  ..........................                      | full inclusion
         *                  .....................................           | full inclusion
         *                        ................                          | x1:_x1-1, _x1:_x2, _x2+1:x2
         *             ...............................                      | full inclusion
         *                  ..........................                      | full inclusion
         *                      ......................                      | x1:_x1-1, _x1:x2
         *                                           .                      | x1:_x1-1, _x1:x2
         *                               .................................. | x1:_x1-1, _x1:x2
         *                                           ............           | x1:_x1-1, _x1:x2 (_x1 == x2)
         *                                            ...........           | no intersection
         *                                                 ......           | no intersection
         */

        // there's an intersection - slice "this" into various number of smaller cuboids
        long long xpoint = x1, ypoint = y1, zpoint = z1;

        vector<pair<long long, long long>> dimx;
        if (_x1 > x1/* && _x1 <= x2*/) {
            dimx.emplace_back(make_pair(x1, _x1 - 1));
            xpoint = _x1;
        }
        if (/*_x2 >= xpoint && */_x2 < x2) {
            dimx.emplace_back(make_pair(xpoint, _x2));
            xpoint = _x2 + 1;
        }
        dimx.emplace_back(make_pair(xpoint, x2));

        vector<pair<long long, long long>> dimy;
        if (_y1 > y1/* && _y1 <= y2*/) {
            dimy.emplace_back(make_pair(y1, _y1 - 1));
            ypoint = _y1;
        }
        if (/*_y2 >= ypoint && */_y2 < y2) {
            dimy.emplace_back(make_pair(ypoint, _y2));
            ypoint = _y2 + 1;
        }
        dimy.emplace_back(make_pair(ypoint, y2));

        vector<pair<long long, long long>> dimz;
        if (_z1 > z1/* && _z1 <= z2*/) {
            dimz.emplace_back(make_pair(z1, _z1 - 1));
            zpoint = _z1;
        }
        if (/*_z2 >= zpoint && */_z2 < z2) {
            dimz.emplace_back(make_pair(zpoint, _z2));
            zpoint = _z2 + 1;
        }
        dimz.emplace_back(make_pair(zpoint, z2));

        for (auto &pz: dimz) {
            for (auto &py: dimy) {
                for (auto &px: dimx) {
                    // add it if it's NOT intersecting at all with the new one
                    if (px.first > _x2 || px.second < _x1 || py.first > _y2 || py.second < _y1 || pz.first > _z2 || pz.second < _z1) {
                        cuboid *c = new cuboid{
                                .x1=px.first, .x2=px.second,
                                .y1=py.first, .y2=py.second,
                                .z1=pz.first, .z2=pz.second
                        };
                        cuboids.emplace_back(c);
                    }
                }
            }
        }
    }

    explicit operator string() const {
        ostringstream oss;
        oss << x1 << ":" << x2 << ", " << y1 << ":" << y2 << ", " << z1 << ":" << z2;
        return oss.str();
    }
};

int main(int argc, char *argv[]) {
    Options options("Day 22", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<string> lines;
    vector<pair<bool, long long *>> instructions;

    long long x1, x2, y1, y2, z1, z2;
    char onoff[3];
    while (getline(*input, line)) {
        trim(line);
        sscanf(line.c_str(), "%s x=%lld..%lld,y=%lld..%lld,z=%lld..%lld", onoff, &x1, &x2, &y1, &y2, &z1, &z2);
        bool on = onoff[1] == 'n';
        instructions.emplace_back(make_pair(on, new long long[]{x1, x2, y1, y2, z1, z2}));
    }

    // part 1

    map<string, bool> space;
    for (auto &p: instructions) {
        x1 = p.second[0];
        x2 = p.second[1];
        y1 = p.second[2];
        y2 = p.second[3];
        z1 = p.second[4];
        z2 = p.second[5];
        if (x1 < -50) {
            x1 = -50;
        }
        if (y1 < -50) {
            y1 = -50;
        }
        if (z1 < -50) {
            z1 = -50;
        }
        if (x2 > 50) {
            x2 = 50;
        }
        if (y2 > 50) {
            y2 = 50;
        }
        if (z2 > 50) {
            z2 = 50;
        }
        char ckey[12];
        for (int _z = z1; _z <= z2; _z++) {
            for (int _y = y1; _y <= y2; _y++) {
                for (int _x = x1; _x <= x2; _x++) {
                    sprintf(ckey, "%03d:%03d:%03d", _x, _y, _z);
                    if (p.first) {
                        space[string(ckey)] = true;
                    } else {
                        space.erase(string(ckey));
                    }
                }
            }
        }
    }

    int answer1 = (int) space.size();

    // part 2

    list<cuboid *> cuboids;
    list<cuboid *> new_cuboids;

    for (auto &p: instructions) {
        if (p.first) {
            // "on"
            // at worst case we may need to add 7 cuboids replacing each cuboid intersecting with the new one
            // but to make life easier, a little cuboid fully inside a larger one makes 26 cuboids, although it
            // could be optimised to (Rubik's cuboid) 2x 1:3:3 cuboids, 2x 1:1:3 cuboids and 2x 1:1:1 cuboids
            for (auto *&c: cuboids) {
                c->slice(p.second[0], p.second[1], p.second[2], p.second[3], p.second[4], p.second[5], new_cuboids, true);
            }
            // slicing all existing cuboids either produces new (smaller) cuboids, or returns the cuboid being slices
            // (if there's no intersection at all)
            cuboids.clear();
            // add_all()
            cuboids.splice(cuboids.end(), new_cuboids);
            // add newly created cuboid - it's ON
            cuboids.push_back(new cuboid{
                    .x1 = p.second[0], .x2 = p.second[1],
                    .y1 = p.second[2], .y2 = p.second[3],
                    .z1 = p.second[4], .z2 = p.second[5]
            });
        } else {
            // "off"
            for (auto *&c: cuboids) {
                c->slice(p.second[0], p.second[1], p.second[2], p.second[3], p.second[4], p.second[5], new_cuboids, false);
            }
            cuboids.clear();
            // add_all()
            cuboids.splice(cuboids.end(), new_cuboids);
            // simply not add anything
        }
    }

    long long answer2 = 0;

    for (auto *&c: cuboids) {
//        cout << "c: " << (string) *c << endl;
        answer2 += (c->x2 - c->x1 + 1) * (c->y2 - c->y1 + 1) * (c->z2 - c->z1 + 1);
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    for (auto &p: instructions) {
        delete[] p.second;
    }
    for (auto &c: cuboids) {
        delete c;
    }
    cuboids.clear();

    return EXIT_SUCCESS;
}
