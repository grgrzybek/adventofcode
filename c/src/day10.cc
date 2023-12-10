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
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "utils/utils.h"

void draw_map(size_t w, size_t h, const u_int8_t *map);

void draw_visited(size_t w, size_t h, const int *visited);

char to_str(u_int8_t current);

using namespace std;

// VIM: hjkl = left | top | bottom | right
// AoC: LJ7F = ╰ ╯ ╮ ╭ = NE | NW | SW | SE
// bits: 76 54 32 10 = N E S W
// NE & N == true
// NW & N == true
// SW & N == false
// ...
const u_int8_t animal = 0b11111111;
const u_int8_t nothing = 0b00000000;
const u_int8_t water = 0b01010101;
const u_int8_t pipe = 0b10101010;
const u_int8_t NE = 0b11110000;
const u_int8_t NW = 0b11000011;
const u_int8_t SW = 0b00001111;
const u_int8_t SE = 0b00111100;
const u_int8_t NS = 0b11001100;
const u_int8_t WE = 0b00110011;
const u_int8_t N = 0b11000000;
const u_int8_t E = 0b00110000;
const u_int8_t S = 0b00001100;
const u_int8_t W = 0b00000011;

int main(int argc, char *argv[]) {
    aoc2023::Options options("Day 10", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<string> lines;
    size_t w = 0, h = 0;
    size_t sx = 0, sy = 0;
    size_t x, y;

    char *pipes;
    int *visited;
    u_int8_t *map;
    u_int8_t *map2;

    while (getline(*input, line)) {
        aoc2023::trim(line);
        w = line.length();
        h++;
        lines.emplace_back(line);

        x = line.find('S');
        if (x != string::npos) {
            sx = x;
            sy = h - 1;
        }
    }
    pipes = static_cast<char *>(malloc(sizeof(char) * w * h));
    visited = static_cast<int *>(malloc(sizeof(int) * w * h));
    map = static_cast<u_int8_t *>(malloc(sizeof(u_int8_t) * w * h));
    map2 = static_cast<u_int8_t *>(malloc(sizeof(u_int8_t) * (w * 2) * (h * 2)));

    cout << "lines: " << lines.size() << ", w: " << w << ", h: " << h << endl;
    cout << "S: " << sx << " : " << sy << endl;

    y = 0;
    for (auto &l: lines) {
        for (x = 0; x < w; x++) {
            pipes[y * w + x] = l[x];
            switch (l[x]) {
                case 'S':
                    map[y * w + x] = animal;
                    visited[y * w + x] = 0;
                    break;
                case '.':
                    map[y * w + x] = nothing;
                    visited[y * w + x] = 0;
                    break;
                case 'L':
                    map[y * w + x] = NE;
                    visited[y * w + x] = 0;
                    break;
                case 'J':
                    map[y * w + x] = NW;
                    visited[y * w + x] = 0;
                    break;
                case '7':
                    map[y * w + x] = SW;
                    visited[y * w + x] = 0;
                    break;
                case 'F':
                    map[y * w + x] = SE;
                    visited[y * w + x] = 0;
                    break;
                case '-':
                    map[y * w + x] = WE;
                    visited[y * w + x] = 0;
                    break;
                case '|':
                    map[y * w + x] = NS;
                    visited[y * w + x] = 0;
                    break;
            }
        }
        y++;
    }

    // part 1

    int answer1 = 0;

//    draw_map(w, h, map);

    deque<pair<size_t, size_t>> q;
    q.emplace_back(sx, sy);
    visited[sy * w + sx] = 1;
    answer1 = 1;
    while (!q.empty()) {
        auto xy = q.front();
        x = xy.first;
        y = xy.second;
//        cout << "Checking " << x+1 << ":" << y+1 << endl;
        q.pop_front();
        u_int8_t current = map[y * w + x];
        if (answer1 < visited[y * w + x]) {
            answer1 = visited[y * w + x];
//            cout << "    answer1: " << answer1 << endl;
        }
        // animal && any direction will always work
        if (x > 0 && (current & W) && (map[y * w + x - 1] & E) && visited[y * w + x - 1] == 0) {
            visited[y * w + x - 1] = visited[y * w + x] + 1;
            q.emplace_back(x - 1, y);
        }
        if (x < w - 1 && (current & E) && (map[y * w + x + 1] & W) && visited[y * w + x + 1] == 0) {
            visited[y * w + x + 1] = visited[y * w + x] + 1;
            q.emplace_back(x + 1, y);
        }
        if (y > 0 && (current & N) && (map[(y - 1) * w + x] & S) && visited[(y - 1) * w + x] == 0) {
            visited[(y - 1) * w + x] = visited[y * w + x] + 1;
            q.emplace_back(x, y - 1);
        }
        if (y < h - 1 && (current & S) && (map[(y + 1) * w + x] & N) && visited[(y + 1) * w + x] == 0) {
            visited[(y + 1) * w + x] = visited[y * w + x] + 1;
            q.emplace_back(x, y + 1);
        }
    }

//    draw_visited(w, h, visited);

    cout << "=== Map 1 " << endl;
    draw_map(w, h, map);

    answer1--;

    // part 2

    int answer2 = 0;

    size_t mx, my;
    size_t w2 = w * 2;
    size_t h2 = h * 2;
    for (my = 0, y = 0; my < h; my++, y += 2) {
        for (mx = 0, x = 0; mx < w; mx++, x += 2) {
            u_int8_t v = map[my * w + mx];
//            cout << (int)v << " x=" << x << ", y=" << y << "|";
            switch (v) {
                case animal:
                    map2[y * w2 + x] = animal;
                    map2[(y + 1) * w2 + x] = NS;
                    map2[y * w2 + x + 1] = WE;
                    map2[(y + 1) * w2 + x + 1] = nothing;
                    break;
                case nothing:
                    map2[y * w2 + x] = nothing;
                    map2[(y + 1) * w2 + x] = NS;
                    map2[y * w2 + x + 1] = WE;
                    map2[(y + 1) * w2 + x + 1] = nothing;
                    break;
                case NE:
                    // |.
                    // +-
                    map2[y * w2 + x] = NE;
                    map2[(y + 1) * w2 + x] = NS;
                    map2[y * w2 + x + 1] = WE;
                    map2[(y + 1) * w2 + x + 1] = nothing;
                    break;
                case NW:
                    // |.
                    // J.
                    map2[y * w2 + x] = NW;
                    map2[(y + 1) * w2 + x] = NS;
                    map2[y * w2 + x + 1] = WE;
                    map2[(y + 1) * w2 + x + 1] = nothing;
                    break;
                case SW:
                    // ..
                    // 7.
                    map2[y * w2 + x] = SW;
                    map2[(y + 1) * w2 + x] = NS;
                    map2[y * w2 + x + 1] = WE;
                    map2[(y + 1) * w2 + x + 1] = nothing;
                    break;
                case SE:
                    // ..
                    // F-
                    map2[y * w2 + x] = SE;
                    map2[(y + 1) * w2 + x] = NS;
                    map2[y * w2 + x + 1] = WE;
                    map2[(y + 1) * w2 + x + 1] = nothing;
                    break;
                case NS:
                    // |.
                    // |.
                    map2[y * w2 + x] = NS;
                    map2[(y + 1) * w2 + x] = NS;
                    map2[y * w2 + x + 1] = WE;
                    map2[(y + 1) * w2 + x + 1] = nothing;
                    break;
                case WE:
                    // ..
                    // __
                    map2[y * w2 + x] = WE;
                    map2[(y + 1) * w2 + x] = NS;
                    map2[y * w2 + x + 1] = WE;
                    map2[(y + 1) * w2 + x + 1] = nothing;
                    break;
                default:
                    break;
            }
        }
//        cout << endl;
    }

    cout << "=== Map 2 " << endl;
    draw_map(w2, h2, map2);

    size_t w3 = w2 + 2;
    size_t h3 = h2 + 2;

    auto map3 = static_cast<u_int8_t *>(malloc(sizeof(u_int8_t) * w3 * h3));
    for (y = 0; y < h3; y++) {
        if (y == 0 || y == h3 - 1) {
            for (x = 0; x < w3; x++) {
                map3[y * w3 + x] = nothing;
            }
        } else {
            map3[y * w3] = nothing;
            for (x = 1; x < w3 - 1; x++) {
                map3[y * w3 + x] = map2[(y - 1) * w2 + x - 1];
            }
            map3[y * w3 + w3 - 1] = nothing;
        }
    }

    cout << "=== Map 3 " << endl;
    draw_map(w3, h3, map3);

    // replace the pipe with '#'

    q.clear();
    q.emplace_back(sx * 2 + 1, sy * 2 + 1);
    q.emplace_back(sx * 2 + 2, sy * 2 + 1);
    q.emplace_back(sx * 2 + 1, sy * 2 + 2);
    q.emplace_back(sx * 2 + 2, sy * 2 + 2);
    while (!q.empty()) {
        auto xy = q.front();
        x = xy.first;
        y = xy.second;
//        cout << "Checking " << x+1 << ":" << y+1 << ": " << q.size() << endl;
        q.pop_front();
        u_int8_t current = map3[y * w3 + x];
//        cout << to_str(current) << endl;
        if (current == pipe) {
            continue;
        }
        if ((current & W) && (map3[y * w3 + x - 1] & E) && map3[y * w3 + x - 1] != pipe && map3[y * w3 + x - 1] != water && map3[y * w3 + x - 1] != animal) {
            q.emplace_back(x - 1, y);
        }
        if ((current & E) && (map3[y * w3 + x + 1] & W) && map3[y * w3 + x + 1] != pipe && map3[y * w3 + x + 1] != water && map3[y * w3 + x + 1] != animal) {
            q.emplace_back(x + 1, y);
        }
        if ((current & N) && (map3[(y - 1) * w3 + x] & S) && map3[(y - 1) * w3 + x] != pipe && map3[(y - 1) * w3 + x] != water && map3[(y - 1) * w3 + x] != animal) {
            q.emplace_back(x, y - 1);
        }
        if ((current & S) && (map3[(y + 1) * w3 + x] & N) && map3[(y + 1) * w3 + x] != pipe && map3[(y + 1) * w3 + x] != water && map3[(y + 1) * w3 + x] != animal) {
            q.emplace_back(x, y + 1);
        }
        map3[y * w3 + x] = pipe;
    }

    cout << "=== Map 4 " << endl;
    draw_map(w3, h3, map3);

    // flood with water
    q.clear();
    q.emplace_back(0, 0);
    map3[0] = water;
    while (!q.empty()) {
        auto xy = q.front();
        q.pop_front();
        x = xy.first;
        y = xy.second;

        if (x > 0 && (map3[y * w3 + x - 1] != water && map3[y * w3 + x - 1] != pipe)) {
            map3[y * w3 + x - 1] = water;
            q.emplace_back(x - 1, y);
        }
        if (x < w3 - 1 && (map3[y * w3 + x + 1] != water && map3[y * w3 + x + 1] != pipe)) {
            map3[y * w3 + x + 1] = water;
            q.emplace_back(x + 1, y);
        }
        if (y > 0 && (map3[(y - 1) * w3 + x] != water && map3[(y - 1) * w3 + x] != pipe)) {
            map3[(y - 1) * w3 + x] = water;
            q.emplace_back(x, y - 1);
        }
        if (y < h3 - 1 && (map3[(y + 1) * w3 + x] != water && map3[(y + 1) * w3 + x] != pipe)) {
            map3[(y + 1) * w3 + x] = water;
            q.emplace_back(x, y + 1);
        }
    }

    cout << "=== Map 5 " << endl;
    draw_map(w3, h3, map3);

    for (y = 1; y < h3 - 1; y += 2) {
        for (x = 1; x < w3 - 1; x += 2) {
            // copy top left of the 2x2 square
//            cout << "x: " << (x - 1) / 2 << ", y: " << (y - 1) / 2 << endl;
            map[((y - 1) / 2) * w + ((x - 1) / 2)] = map3[y * w3 + x];
        }
    }

    cout << "=== Map 6 " << endl;
    draw_map(w, h, map);

    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            bool inside = map[y * w + x] != pipe && map[y * w + x] != water;
            if (inside) {
                answer2++;
            }
        }
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    free(pipes);
    free(visited);
    free(map);
    free(map2);
    free(map3);

    return EXIT_SUCCESS;
}

char to_str(u_int8_t v) {
    switch (v) {
        case animal:
            return 'S';
        case nothing:
            return '.';
        case NE:
            return 'L';
        case NW:
            return 'J';
        case SW:
            return '7';
        case SE:
            return 'F';
        case NS:
            return '|';
        case WE:
            return '-';
        case water:
            return '~';
        case pipe:
            return '#';
        default:
            return '?';
    }
}

void draw_map(size_t w, size_t h, const u_int8_t *map) {
    for (size_t y = 0; y < h; y++) {
        for (size_t x = 0; x < w; x++) {
            u_int8_t v = map[y * w + x];
            cout << to_str(v);
        }
        cout << endl;
    }
}

void draw_visited(size_t w, size_t h, const int *map) {
    for (size_t y = 0; y < h; y++) {
        for (size_t x = 0; x < w; x++) {
            const int v = map[y * w + x];
            cout << v;
        }
        cout << endl;
    }
}
