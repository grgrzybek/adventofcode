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

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "png.h"

#include "utils/utils.h"

using namespace std;

unsigned long calculate_scenic_score(int const *forest, int row, int column, int w, int h);
void print_map_png(int const *map, int width, int height, int _c, int _r);

int main(int argc, char *argv[]) {
    aoc2022::Options options("Day 08", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<string> forest_map;
    size_t w = 0, h = 0;

    while (getline(*input, line)) {
        forest_map.push_back(line);
        w = line.length();
        h++;
    }
    int forest[w * h];
    bool hidden[w][h];
    std::memset(forest, 0, sizeof(int) * w * h);
    std::memset(hidden, 1, sizeof(bool) * w * h);

    for (size_t r = 0; r < h; r++) {
        for (size_t c = 0; c < w; c++) {
            forest[r * w + c] = forest_map[r][c] - '0';
        }
    }

    // part 1

    unsigned long answer1 = 0;
    int ht = 0;

    for (size_t r = 1; r < h - 1; r++) {
        ht = forest[r * w];
        for (size_t c = 1; c < w - 1; c++) {
            if (forest[r * w + c] > ht) {
                hidden[c][r] = false;
                ht = forest[r * w + c];
            }
        }
        ht = forest[r * w + w - 1];
        for (size_t c = w - 2; c > 0; c--) {
            if (forest[r * w + c] > ht) {
                hidden[c][r] = false;
                ht = forest[r * w + c];
            }
        }
    }
    for (size_t c = 1; c < w - 1; c++) {
        ht = forest[c];
        for (size_t r = 1; r < h - 1; r++) {
            if (forest[r * w + c] > ht) {
                hidden[c][r] = false;
                ht = forest[r * w + c];
            }
        }
        ht = forest[(h - 1) * w + c];
        for (size_t r = h - 2; r > 0; r--) {
            if (forest[r * w + c] > ht) {
                hidden[c][r] = false;
                ht = forest[r * w + c];
            }
        }
    }
    for (size_t r = 1; r < h - 1; r++) {
        for (size_t c = 1; c < w - 1; c++) {
            if (!hidden[c][r]) {
                answer1++;
            }
        }
    }
    answer1 += 2 * w + 2 * h - 4;

    // part 2

    unsigned long answer2 = 0;
    int x, y;

    for (size_t r = 0; r < h; r++) {
        for (size_t c = 0; c < w; c++) {
            unsigned long distance = calculate_scenic_score(forest, (int) r, (int) c, (int) w, (int) h);
            if (distance > answer2) {
                answer2 = distance;
                x = (int) c;
                y = (int) r;
            }
        }
    }
    print_map_png(forest, (int) w, (int) h, x, y);

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}

unsigned long calculate_scenic_score(int const *forest, int row, int column, int w, int h) {
    int score = 1;
    int dir_score = 0;
    int tc = 0;
    int tree = forest[row * w + column];
    for (int left = column - 1; left >= 0; left--) {
        tc++;
        dir_score++;
        if (forest[row * w + left] >= tree) {
            break;
        }
    }
    score *= dir_score;
    dir_score = 0;
    for (int right = column + 1; right < w; right++) {
        tc++;
        dir_score++;
        if (forest[row * w + right] >= tree) {
            break;
        }
    }
    score *= dir_score;
    dir_score = 0;
    for (int  up = row - 1; up >= 0; up--) {
        tc++;
        dir_score++;
        if (forest[up * w + column] >= tree) {
            break;
        }
    }
    score *= dir_score;
    dir_score = 0;
    for (int down = row + 1; down < h; down++) {
        tc++;
        dir_score++;
        if (forest[down * w + column] >= tree) {
            break;
        }
    }
    score *= dir_score;

    return tc == 0 ? 0 : score;
}

int r[] { 0x07, 0x14, 0x1e, 0x1a, 0x16, 0x12, 0x0f, 0x0b, 0x07, 0x01 };
int g[] { 0x20, 0x5f, 0x95, 0xa3, 0xb0, 0xbe, 0xcc, 0xd9, 0xe7, 0xf9 };
int b[] { 0x07, 0x14, 0x1e, 0x1a, 0x16, 0x12, 0x0f, 0x0b, 0x07, 0x01 };

void print_map_png(int const *map, int width, int height, int _c, int _r) {
    FILE *fp = fopen("image.png", "wb");

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    png_infop info_ptr = png_create_info_struct(png_ptr);

    setjmp(png_jmpbuf(png_ptr));

    png_init_io(png_ptr, fp);

    const int SCALE = 20;

    png_set_IHDR(png_ptr, info_ptr, width * SCALE, height * SCALE,
            8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    // Allocate memory for one row (3 bytes per pixel - RGB)
    png_bytep row = (png_bytep) malloc(3 * width * SCALE * sizeof(png_byte));

    // Write image data
    int _x, _y;
    for (_y = 0; _y < height * SCALE; _y++) {
        int y = ((_y) / SCALE) ;
        for (_x = 0; _x < width * SCALE; _x++) {
            int x = ((_x) / SCALE);
            if (x == _c && y == _r) {
                row[_x * 3] = 0x00;
                row[_x * 3 + 1] = 0x00;
                row[_x * 3 + 2] = 0xff;
            } else {
                row[_x * 3] = r[map[y * width + x]];
                row[_x * 3 + 1] = g[map[y * width + x]];
                row[_x * 3 + 2] = b[map[y * width + x]];
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
