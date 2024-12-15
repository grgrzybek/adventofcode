/*
 * Copyright 2024 Grzegorz Grzybek
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

#include <cstdlib>
#include <cstring>
#include <deque>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

struct robot {
    int x, y, vx, vy;

    friend ostream &operator<<(ostream &os, const robot &r);
};

ostream &operator<<(ostream &os, const robot &r) {
    os << "robot: [" << r.x << "," << r.y << "], v: [" << r.vx << "," << r.vy << "]";
    return os;
}

int main(int argc, char *argv[]) {
    aoc2024::Options options("Day 14", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    string tok1;
    string tok2;
    istringstream iss;

    vector<robot *> robots;

    robot *r;
    while (getline(*input, line)) {
        aoc2024::trim(line);
        if (line.empty()) {
            continue;
        }
        r = new robot;
        robots.push_back(r);
        sscanf(line.c_str(), "p=%d,%d v=%d,%d", &r->x, &r->y, &r->vx, &r->vy);
    }

//    for (auto &_r: robots) {
//        cout << *_r << endl;
//    }

    // part 1

    long answer1 = 0;


    // 11 tiles wide and 7 tiles tall - in example
    // 101 tiles wide and 103 tiles tall - real data
    const int W = 101;
    const int H = 103;
    const int MID_W = W / 2;
    const int MID_H = H / 2;

    char *room = (char *) malloc(sizeof(char) * W * H);
    memset(room, '.', W * H);

    for (int i = 0; i < 100; i++) {
        for (auto &_r: robots) {
    //        cout << *_r;
            _r->x += (1 * _r->vx);
            _r->y += (1 * _r->vy);
            _r->x %= W;
            _r->y %= H;
            if (_r->x < 0) {
                _r->x += W;
            }
            if (_r->y < 0) {
                _r->y += H;
            }
    //        cout << " -> " << *_r << endl;
//            room[_r->y * W + _r->x] = '*';
        }

//        cout << "\n--------------------------- after " << (i + 0) << "\n";
//        for (int y = 0; y < H; y++) {
//            for (int x = 0; x < W; x++) {
//                if (room[y * W + x] == '0') {
//                    cout << '.';
//                } else {
//                    cout << room[y * W + x];
//                }
//            }
//            cout << endl;
//        }
    }

    // TL, TR, BL, BR
    int counts[] = {0, 0, 0, 0};
    for (auto &_r: robots) {
        if (_r->x < MID_W) {
            // left half
            if (_r->y < MID_H) {
                counts[0]++;
            }
            if (_r->y > MID_H) {
                counts[2]++;
            }
        } else if (_r->x > MID_W) {
            // right half
            if (_r->y < MID_H) {
                counts[1]++;
            }
            if (_r->y > MID_H) {
                counts[3]++;
            }
        }
    }

    answer1 = counts[0] * counts[1] * counts[2] * counts[3];

    // part 2

    long answer2 = 0;

    int i = 100;
    int min_i = 100;
    int max_symetrical = 0;
    while (i < 100000) {
        memset(room, '.', W * H);
        for (auto &_r: robots) {
    //        cout << *_r;
            _r->x += (1 * _r->vx);
            _r->y += (1 * _r->vy);
            _r->x %= W;
            _r->y %= H;
            if (_r->x < 0) {
                _r->x += W;
            }
            if (_r->y < 0) {
                _r->y += H;
            }
    //        cout << " -> " << *_r << endl;
            room[_r->y * W + _r->x] = '*';
        }
        i++;

        int symmetrical = 0;
        // left-right symmetry
//        for (int y = 0; y < H; y++) {
//            for (int x = 0; x < MID_W; x++) {
//                // 11, mid_x = 5: 01234|6789A
//                if (room[y * W + x] == room[y * W + W - 1 - x]) {
//                    symmetrical++;
//                }
//            }
//        }
        // top-down symmetry
        for (int x = 0; x < W; x++) {
            for (int y = 0; y < MID_H; y++) {
                // 11, mid_x = 5: 01234|6789A
                if (room[y * W + x] == room[(H - 1 - y) * W + x]) {
                    symmetrical++;
                }
            }
        }
        if (symmetrical > max_symetrical) {
            max_symetrical = symmetrical;
            min_i = i;
            cout << max_symetrical << endl;
            cout << "\n--------------------------- after " << (i + 0) << "\n";
            for (int y = 0; y < H; y++) {
                for (int x = 0; x < W; x++) {
                    if (room[y * W + x] == '0') {
                        cout << '.';
                    } else {
                        cout << room[y * W + x];
                    }
                }
                cout << endl;
            }
        }
    }
    cout << max_symetrical << endl;
    cout << min_i << endl;

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    for (auto &_r: robots) {
        delete _r;
    }

    free(room);

    return EXIT_SUCCESS;
}
