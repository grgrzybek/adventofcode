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
#include <string>
#include <vector>

#include "png.h"

#include "utils/utils.h"

using namespace std;
using namespace aoc2021;

int make_pool(int *map, int x, int y, int width, int height);

void flash_surrounding(int *map, int x, int y, int width, int height);

void print_map(int *map, int width, int height);

void print_map_png(int *map, int width, int height, int step);

int main(int argc, char *argv[]) {
    Options options("Day 11", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<string> lines;

    int width = 0;
    int height = 0;
    while (getline(*input, line)) {
        trim(line);
        lines.push_back(line);
        if (width == 0) {
            width = (int) line.length();
        }
        height++;
    }

    cout << "Size: " << width << "x" << height << endl;

    int *map1 = new int[width * height];
    int *map2 = new int[width * height];

    int row = 0;
    for (string &l: lines) {
        for (int col = 0; col < width; col++) {
            map1[row * width + col] = l[col] - '0';
        }
        row++;
    }

    // part 1 & 2

    int answer1 = 0;
    int answer2 = 0;

    int i = 0;
    print_map_png(map1, width, height, i);
    for (; i < 1200; i++) {
        for (int c = 0; c < width * height; c++) {
            map2[c] = map1[c] + 1;
        }
//        cout << "========== After step " << i << " ==========" << endl;
//        print_map(map1, width, height);
        while (true) {
            bool flash = false;
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    if (map2[y * width + x] > 9) {
                        flash = true;
                        flash_surrounding(map2, x, y, width, height);
                        if (i < 100) {
                            answer1++;
                        }
                    }
                }
            }
            if (!flash) {
                break;
            }
        }

        bool zero = true;
        for (int j = 0; j < width * height; j++) {
            if (map2[j] != 0) {
                zero = false;
                break;
            }
        }

        print_map_png(map2, width, height, i);
        swap(map1, map2);

        if (zero) {
            answer2 = i + 1;
            break;
        }
    }
    print_map_png(map1, width, height, i);

    // finally, run `ffmpeg -framerate 24 -pattern_type glob -i '*.png' day11.mp4`

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}

void flash_surrounding(int *map, int x, int y, int width, int height) {
    if (x > 0) {
        // left column
        if (y > 0 && map[(y - 1) * width + x - 1] > 0) {
            // top left
            map[(y - 1) * width + x - 1]++;
        }
        // left
        if (map[y * width + x - 1] > 0) {
            map[y * width + x - 1]++;
        }
        if (y < height - 1 && map[(y + 1) * width + x - 1] > 0) {
            // bottom left
            map[(y + 1) * width + x - 1]++;
        }
    }
    // same column
    if (y > 0 && map[(y - 1) * width + x] > 0) {
        // top
        map[(y - 1) * width + x]++;
    }
    // this
    map[y * width + x] = 0;
    if (y < height - 1 && map[(y + 1) * width + x] > 0) {
        // bottom left
        map[(y + 1) * width + x]++;
    }
    if (x < width - 1) {
        // right column
        if (y > 0 && map[(y - 1) * width + x + 1] > 0) {
            // top right
            map[(y - 1) * width + x + 1]++;
        }
        // right
        if (map[y * width + x + 1] > 0) {
            map[y * width + x + 1]++;
        }
        if (y < height - 1 && map[(y + 1) * width + x + 1] > 0) {
            // bottom right
            map[(y + 1) * width + x + 1]++;
        }
    }
}

void print_map(int *map, int width, int height) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            cout << setw(1) << setfill(' ') << map[y * width + x];
        }
        cout << endl;
    }
}

int r[] { 0x20, 0x5f, 0x95, 0xa3, 0xb0, 0xbe, 0xcc, 0xd9, 0xe7, 0xf9 };
int g[] { 0x00, 0x00, 0x06, 0x1d, 0x36, 0x4e, 0x65, 0x89, 0xb3, 0xed };
int b[] { 0x07, 0x14, 0x1e, 0x1a, 0x16, 0x12, 0x0f, 0x0b, 0x07, 0x01 };

void print_map_png(int *map, int width, int height, int step) {
    char fn[64];
    sprintf(fn, "image-%03d.png", step);
    FILE *fp = fopen(fn, "wb");

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
            row[_x * 3] = r[map[y * width + x]];
            row[_x * 3 + 1] = g[map[y * width + x]];
            row[_x * 3 + 2] = b[map[y * width + x]];
        }
        png_write_row(png_ptr, row);
    }

    // End write
    png_write_end(png_ptr, nullptr);

    fclose(fp);
    png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    png_destroy_write_struct(&png_ptr, (png_infopp) nullptr);
}
