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

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "png.h"

#include "utils/utils.h"

using namespace std;
using namespace aoc2021;

void print_map_png(const byte *map, int width, int height, int step);

int main(int argc, char *argv[]) {
    Options options("Day 20", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;

    getline(*input, line);
    trim(line);

    int image_enhancement_algorithm[512];

    int idx = 0;
    for (auto c: line) {
        image_enhancement_algorithm[idx++] = (c == '#' ? 1 : 0);
    }

    getline(*input, line);
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

    // part 1 & 2

    int width1 = width + 102;
    int height1 = height + 102;

    byte *infinite_image1 = new byte[width1 * height1];
    byte *infinite_image2 = new byte[width1 * height1];
    memset(infinite_image1, 0, width1 * height1 * sizeof(byte));

    int y = 51;
    for (auto &l: lines) {
        for (int x = 0; x < width; x++) {
            infinite_image1[y * width1 + x + 51] = l[x] == '#' ? (byte) 1 : (byte) 0;
        }
        y++;
    }

    print_map_png(infinite_image1, width1, height1, 0);

    int answer1 = 0;
    int answer2 = 0;

    int infinite_odd = image_enhancement_algorithm[0];
    int infinite_even = infinite_odd == 1 ? image_enhancement_algorithm[511] : 0;
    for (int step = 0; step < 50; step++) {
        memset(infinite_image2, 0, width1 * height1 * sizeof(byte));
        // process - but without extra pixels (though the image will be expanding)
        for (y = 0; y < height1; y++) {
            for (int x = 0; x < width1; x++) {
                idx = 0;
                if (y < height1 - 1) {
                    if (x < width1 - 1) {
                        idx |= (int) infinite_image1[(y + 1) * width1 + x + 1];
                    } else {
                        idx |= step % 2 == 0 ? infinite_even * 0x1 : infinite_odd * 0x1;
                    }
                    idx |= (int) infinite_image1[(y + 1) * width1 + x] << 1;
                    if (x > 0) {
                        idx |= (int) infinite_image1[(y + 1) * width1 + x - 1] << 2;
                    } else {
                        idx |= step % 2 == 0 ? infinite_even * 0x4 : infinite_odd * 0x4;
                    }
                } else {
                    idx |= step % 2 == 0 ? infinite_even * 0x7 : infinite_odd * 0x7;
                }
                if (x < width1 - 1) {
                    idx |= (int) infinite_image1[y * width1 + x + 1] << 3;
                } else {
                    idx |= step % 2 == 0 ? infinite_even * 0x8 : infinite_odd * 0x8;
                }
                idx |= (int) infinite_image1[y * width1 + x] << 4;
                if (x > 0) {
                    idx |= (int) infinite_image1[y * width1 + x - 1] << 5;
                } else {
                    idx |= step % 2 == 0 ? infinite_even * 0x20 : infinite_odd * 0x20;
                }
                if (y > 0) {
                    if (x < width1 - 1) {
                        idx |= (int) infinite_image1[(y - 1) * width1 + x + 1] << 6;
                    } else {
                        idx |= step % 2 == 0 ? infinite_even * 0x40 : infinite_odd * 0x40;
                    }
                    idx |= (int) infinite_image1[(y - 1) * width1 + x] << 7;
                    if (x > 0) {
                        idx |= (int) infinite_image1[(y - 1) * width1 + x - 1] << 8;
                    } else {
                        idx |= step % 2 == 0 ? infinite_even * 0x100 : infinite_odd * 0x100;
                    }
                } else {
                    idx |= step % 2 == 0 ? infinite_even * 0x7 : infinite_odd * 0x7;
                }
                infinite_image2[y * width1 + x] = (byte) image_enhancement_algorithm[idx];
            }
        }
        swap(infinite_image1, infinite_image2);

        print_map_png(infinite_image1, width1, height1, step + 1);

        if (step == 1) {
            for (int i = 0; i < width1 * height1; i++) {
                answer1 += (int) infinite_image1[i];
            }
        }
    }

    for (int i = 0; i < width1 * height1; i++) {
        answer2 += (int) infinite_image1[i];
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}

void print_map_png(const byte *map, int width, int height, int step) {
    char fn[64];
    sprintf(fn, "image-%03d.png", step);
    FILE *fp = fopen(fn, "wb");

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    png_infop info_ptr = png_create_info_struct(png_ptr);

    setjmp(png_jmpbuf(png_ptr));

    png_init_io(png_ptr, fp);

    const int SCALE = 6;

    png_set_IHDR(png_ptr, info_ptr, width * SCALE, height * SCALE,
            8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    // Allocate memory for one row (3 bytes per pixel - RGB)
    auto row = (png_bytep) malloc(3 * width * SCALE * sizeof(png_byte));

    // Write image data
    int _x, _y;
    for (_y = 0; _y < height * SCALE; _y++) {
        int y = ((_y) / SCALE) ;
        for (_x = 0; _x < width * SCALE; _x++) {
            int x = ((_x) / SCALE);
            row[_x * 3] = map[y * width + x] == (byte) 1 ? 0xff : 0x00;
            row[_x * 3 + 1] = row[_x * 3];
            row[_x * 3 + 2] = row[_x * 3];
        }
        png_write_row(png_ptr, row);
    }

    // End write
    png_write_end(png_ptr, nullptr);

    fclose(fp);
    png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    png_destroy_write_struct(&png_ptr, (png_infopp) nullptr);
}
