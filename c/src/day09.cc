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

#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "png.h"

#include "utils/utils.h"

using namespace std;

void print_map_png_part1(map<pair<int, int>, int> &map, int width, int height, int dx, int dy, int max, int step, int tx, int ty);
void print_map_png_part2(map<pair<int, int>, int> &map, int width, int height, int dx, int dy, int max, int step, pair<int, int> *rope);

int main(int argc, char *argv[]) {
    aoc2022::Options options("Day 09", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<pair<int, int>> moves;

    int move;
    string direction;
    while (getline(*input, line)) {
        istringstream iss(line);
        iss >> direction >> move;
        for (int i = 0; i < move; i++) {
            if (direction[0] == 'U') {
                moves.emplace_back(0, 1);
            } else if (direction[0] == 'D') {
                moves.emplace_back(0, -1);
            } else if (direction[0] == 'R') {
                moves.emplace_back(1, 0);
            } else if (direction[0] == 'L') {
                moves.emplace_back(-1, 0);
            }
        }
    }

    // part 1

    // Initial state:
    //  ..... ..... ..... ..... ..... ..... ..... .....
    //  .T... .T... ...T. ...T. ..... ..... ..... .....
    //  ..H.. ..H.. ..H.. ..H.. ..H.. ..H.. ..H.. ..H..
    //  ..... ..... ..... ..... ...T. ...T. .T... .T...
    //  ..... ..... ..... ..... ..... ..... ..... .....
    // Head moves:
    //  1R    1D    1L    1D    1U    1L    1U    1R
    //  ..... ..... ..... ..... ..... ..... ..... .....
    //  .T... .T... ...T. ...T. ..H.. ..... ..H.. .....
    //  ...H. ..... .H... ..... ..... .H... ..... ...H.
    //  ..... ..H.. ..... ..H.. ...T. ...T. .T... .T...
    //  ..... ..... ..... ..... ..... ..... ..... .....
    // Tail catches up diagonally:
    //  x+ y+ x+ y+ x- y+ x- y+ x- y- x- y- x+ y- x+ y-
    //  ..... ..... ..... ..... ..... ..... ..... .....
    //  ..... ..... ..... ..... ..H.. ..... ..H.. .....
    //  ..TH. ..T.. .HT.. ..T.. ..T.. .HT.. ..T.. ..TH.
    //  ..... ..H.. ..... ..H.. ..... ..... ..... .....
    //  ..... ..... ..... ..... ..... ..... ..... .....

    map<pair<int, int>, int> map;
    int min_x = 0;
    int min_y = 0;
    int max_x = 0;
    int max_y = 0;

    int head_x = 0, head_y = 0, tail_x = 0, tail_y = 0;
    map[make_pair(0, 0)] = 1;
    for (auto &xy: moves) {
//        cout << "head moves from [" << head_x << "," << head_y << "] to ";
        head_x += xy.first;
        head_y += xy.second;
//        cout << "[" << head_x << "," << head_y << "]\n";

//        if (min_x > head_x) {
//            min_x = head_x;
//        }
//        if (max_x < head_x) {
//            max_x = head_x;
//        }
//        if (min_y > head_y) {
//            min_y = head_y;
//        }
//        if (max_y < head_y) {
//            max_y = head_y;
//        }

        // adjust tail
        if (abs(head_x - tail_x) <= 1 && abs(head_y - tail_y) <= 1) {
            // no need to move the tail
//            cout << "tail doesn't move\n\n";
            continue;
        }
//        cout << "tail catches up from [" << tail_x << "," << tail_y << "] to ";
        if (abs(head_x - tail_x) > 1 && head_y == tail_y) {
            // only move horizontally
            // ..H.T.. -> tail_x--
            // ..T.H.. -> tail_x++
            tail_x += (head_x < tail_x ? -1 : 1);
        } else if (abs(head_y - tail_y) > 1 && head_x == tail_x) {
            // only move vertically
            // .   .
            // H   T
            // .   .
            // T   H
            // .   .
            tail_y += (head_y < tail_y ? -1 : 1);
        } else {
            // move diagonally
            tail_x += (head_x < tail_x ? -1 : 1);
            tail_y += (head_y < tail_y ? -1 : 1);
        }
//        cout << "[" << tail_x << "," << tail_y << "]\n";

        if (min_x > tail_x) {
            min_x = tail_x;
        }
        if (max_x < tail_x) {
            max_x = tail_x;
        }
        if (min_y > tail_y) {
            min_y = tail_y;
        }
        if (max_y < tail_y) {
            max_y = tail_y;
        }

        pair<int, int> new_tail = make_pair(tail_x, tail_y);
        if (!map.contains(new_tail)) {
            map[new_tail] = 1;
        } else {
            map[new_tail]++;
        }
//        cout << "\n";
    }

    unsigned long answer1 = map.size();

    int dx = -min_x;
    int dy = -min_y;
    int w = max_x - min_x + 1;
    int h = max_y - min_y + 1;
    int max_visit = 0;
    for (auto &p: map) {
//        cout << "[" << p.first.first << "," << p.first.second << "]=" << p.second << "\n";
        if (p.second > max_visit) {
            max_visit = p.second;
        }
    }
    cout << "min_x: " << min_x << ", min_y: " << min_y << ", ";
    cout << "max_x: " << max_x << ", max_y: " << max_y << "\n";
    cout << "dx: " << dx << ", dy: " << dy << "\n";
    cout << "width: " << w << ", height: " << h << "\n";
    cout << "max_visit: " << max_visit << "\n";
    cout << "steps: " << moves.size() << "\n";

    int step = 0;
    print_map_png_part1(map, w, h, dx, dy, max_visit, step, numeric_limits<int>::max(), numeric_limits<int>::max());

    // visualization

    head_x = 0, head_y = 0, tail_x = 0, tail_y = 0;
    map.clear();
    map[make_pair(0, 0)] = 1;
    for (auto &xy: moves) {
        head_x += xy.first;
        head_y += xy.second;
        if (step % 50 == 0) {
            cout << "step: " << step << "\n";
            print_map_png_part1(map, w, h, dx, dy, max_visit, step, tail_x, tail_y);
        }
        step++;
        if (abs(head_x - tail_x) <= 1 && abs(head_y - tail_y) <= 1) {
            continue;
        }
        if (abs(head_x - tail_x) > 1 && head_y == tail_y) {
            tail_x += (head_x < tail_x ? -1 : 1);
        } else if (abs(head_y - tail_y) > 1 && head_x == tail_x) {
            tail_y += (head_y < tail_y ? -1 : 1);
        } else {
            tail_x += (head_x < tail_x ? -1 : 1);
            tail_y += (head_y < tail_y ? -1 : 1);
        }

        pair<int, int> new_tail = make_pair(tail_x, tail_y);
        if (!map.contains(new_tail)) {
            map[new_tail] = 1;
        } else {
            map[new_tail]++;
        }
    }

//    for (int y = max_y + dy; y >= 0; y--) {
//        for (int x = 0; x <= max_x + dx; x++) {
//            auto p = make_pair(x - dx, y - dy);
//            if (map.contains(p)) {
//                cout << map[p];
//            } else {
//                cout << ".";
//            }
//        }
//        cout << "\n";
//    }

    // part 2

    pair<int, int> rope[10];
    for (auto &p: rope) {
        p.first = 0;
        p.second = 0;
    }
    map.clear();
    map[rope[9]] = 1;

    step = 0;
    for (auto &xy: moves) {
        if (step % 20 == 0) {
            cout << "step: " << step << "\n";
            print_map_png_part2(map, w, h, dx, dy, max_visit, step, rope);
        }
        step++;

        rope[0].first += xy.first;
        rope[0].second += xy.second;

        bool no_more_moves = false;
        for (int tail = 1; tail <= 9; tail++) {
            if (abs(rope[tail - 1].first - rope[tail].first) <= 1 && abs(rope[tail - 1].second - rope[tail].second) <= 1) {
                // this tail doesn't move, so we can proceed directly to next instruction, because no other tail will move
                no_more_moves = true;
                break;
            }
            if (abs(rope[tail - 1].first - rope[tail].first) > 1 && rope[tail - 1].second == rope[tail].second) {
                rope[tail].first += (rope[tail - 1].first < rope[tail].first ? -1 : 1);
            } else if (abs(rope[tail - 1].second - rope[tail].second) > 1 && rope[tail - 1].first == rope[tail].first) {
                rope[tail].second += (rope[tail - 1].second < rope[tail].second ? -1 : 1);
            } else {
                rope[tail].first += (rope[tail - 1].first < rope[tail].first ? -1 : 1);
                rope[tail].second += (rope[tail - 1].second < rope[tail].second ? -1 : 1);
            }
        }
        if (no_more_moves) {
            continue;
        }

        pair<int, int> new_tail = make_pair(rope[9].first, rope[9].second);
        if (!map.contains(new_tail)) {
            map[new_tail] = 1;
        } else {
            map[new_tail]++;
        }
    }

    unsigned long answer2 = map.size();

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}

void print_map_png_part1(map<pair<int, int>, int> &map, int width, int height, int dx, int dy, int max, int step, int tx, int ty) {
    char fn[64];
    sprintf(fn, "image-day-09a-%05d.png", step);
    FILE *fp = fopen(fn, "wb");

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    png_infop info_ptr = png_create_info_struct(png_ptr);

    setjmp(png_jmpbuf(png_ptr));

    png_init_io(png_ptr, fp);

    const int SCALE = 3;

    png_set_IHDR(png_ptr, info_ptr, width * SCALE, height * SCALE,
            8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    // Allocate memory for one row (3 bytes per pixel - RGB)
    png_bytep row = (png_bytep) malloc(3 * width * SCALE * sizeof(png_byte));

    // Write image data
    int _x, _y;
    for (_y = height * SCALE - 1; _y >= 0; _y--) {
        int y = ((_y) / SCALE);
        for (_x = 0; _x < width * SCALE; _x++) {
            int x = ((_x) / SCALE);
            auto p = make_pair(x - dx, y - dy);
            if (tx != numeric_limits<int>::max() && tx == x - dx && ty == y - dy) {
                row[_x * 3] = 0xF0;
                row[_x * 3 + 1] = 0x00;
                row[_x * 3 + 2] = 0x00;
            } else if (!map.contains(p)) {
                row[_x * 3] = 0x00;
                row[_x * 3 + 1] = 0x00;
                row[_x * 3 + 2] = 0x00;
            } else {
                row[_x * 3] = 0xFF / max * map[p];
                row[_x * 3 + 1] = 0xFF / max * map[p];
                row[_x * 3 + 2] = 0x70;
            }
        }
        png_write_row(png_ptr, row);
    }

    // End write
    png_write_end(png_ptr, nullptr);

    fclose(fp);
    png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    png_destroy_write_struct(&png_ptr, (png_infopp) nullptr);
}

void print_map_png_part2(map<pair<int, int>, int> &map, int width, int height, int dx, int dy, int max, int step, pair<int, int> *rope) {
    char fn[64];
    sprintf(fn, "image-day-09b-%05d.png", step);
    FILE *fp = fopen(fn, "wb");

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    png_infop info_ptr = png_create_info_struct(png_ptr);

    setjmp(png_jmpbuf(png_ptr));

    png_init_io(png_ptr, fp);

    const int SCALE = 3;

    png_set_IHDR(png_ptr, info_ptr, width * SCALE, height * SCALE,
            8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    // Allocate memory for one row (3 bytes per pixel - RGB)
    png_bytep row = (png_bytep) malloc(3 * width * SCALE * sizeof(png_byte));

    // Write image data
    int _x, _y;
    for (_y = height * SCALE - 1; _y >= 0; _y--) {
        int y = ((_y) / SCALE);
        for (_x = 0; _x < width * SCALE; _x++) {
            int x = ((_x) / SCALE);
            auto p = make_pair(x - dx, y - dy);
            bool is_rope = false;
            for (int knot = 0; knot < 10; knot++) {
                auto p2 = rope[knot];
                is_rope = p == p2;
                if (is_rope) {
                    break;
                }
            }
            if (is_rope) {
                row[_x * 3] = 0xF0;
                row[_x * 3 + 1] = 0x00;
                row[_x * 3 + 2] = 0x00;
            } else if (!map.contains(p)) {
                row[_x * 3] = 0x00;
                row[_x * 3 + 1] = 0x00;
                row[_x * 3 + 2] = 0x00;
            } else {
                row[_x * 3] = 0xFF / max * map[p];
                row[_x * 3 + 1] = 0xFF / max * map[p];
                row[_x * 3 + 2] = 0x70;
            }
        }
        png_write_row(png_ptr, row);
    }

    // End write
    png_write_end(png_ptr, nullptr);

    fclose(fp);
    png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    png_destroy_write_struct(&png_ptr, (png_infopp) nullptr);
}
