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

    answer1--;

    // part 2

    int answer2 = 0;

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    free(pipes);
    free(visited);
    free(map);

    return EXIT_SUCCESS;
}

void draw_map(size_t w, size_t h, const u_int8_t *map) {
    for (size_t y = 0; y < h; y++) {
        for (size_t x = 0; x < w; x++) {
            u_int8_t v = map[y * w + x];
            switch (v) {
                case animal:
                    cout << 'S';
                    break;
                case nothing:
                    cout << '.';
                    break;
                case NE:
                    cout << 'L';
                    break;
                case NW:
                    cout << 'J';
                    break;
                case SW:
                    cout << '7';
                    break;
                case SE:
                    cout << 'F';
                    break;
                case NS:
                    cout << '|';
                    break;
                case WE:
                    cout << '-';
                    break;
                default:
                    break;
            }
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
