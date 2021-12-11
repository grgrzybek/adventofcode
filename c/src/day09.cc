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

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_events.h>

#include "utils/utils.h"

using namespace std;
using namespace aoc2021;

int make_pool(int *map, int x, int y, int width, int height);

int main(int argc, char *argv[]) {
    Options options("Day 09", argc, argv);
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

    int *map = new int[width * height];

    int row = 0;
    for (string &l: lines) {
        for (int col = 0; col < width; col++) {
            map[row * width + col] = l[col] - '0';
        }
        row++;
    }

    // part 1

    int answer1 = 0;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            bool low = true;
            int v0 = map[width * y + x];
            if (x > 0) {
                low &= map[width * y + x - 1] > v0;
            }
            if (x < width - 1) {
                low &= map[width * y + x + 1] > v0;
            }
            if (y > 0) {
                low &= map[width * (y - 1) + x] > v0;
            }
            if (y < height - 1) {
                low &= map[width * (y + 1) + x] > v0;
            }
            if (low) {
                answer1 += 1 + v0;
            }
        }
    }

    // SDL

    SDL_Window *window;
    SDL_Surface *surface;

    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("SDL Tutorial", 100, 100, width * 4, height * 4, SDL_WINDOW_SHOWN);
    surface = SDL_GetWindowSurface(window);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int v0 = map[width * y + x];
            SDL_Rect rect { .x = x*4, .y = y*4, .w = 4, .h = 4 };
            SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0xF * v0, 0xF * v0, 0x00));
        }
    }

    SDL_UpdateWindowSurface(window);

    SDL_Event event;
    bool run = true;
    while (run) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYUP) {
                run = false;
            }
        }
    }

    // part 2

    vector<int> pool_sizes;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int v0 = map[width * y + x];
            if (v0 != -1 && v0 < 9) {
                pool_sizes.push_back(make_pool(map, x, y, width, height));
            }
        }
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int v0 = map[width * y + x];
            SDL_Rect rect { .x = x*4, .y = y*4, .w = 4, .h = 4 };
            if (v0 == -1) {
                SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0x90, 0x90, 0x88));
            } else {
                SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0x90, 0x00, 0x00));
            }
        }
    }

    SDL_UpdateWindowSurface(window);

    run = true;
    while (run) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYUP) {
                SDL_DestroyWindow(window);
                SDL_Quit();
                run = false;
            }
        }
    }

    sort(pool_sizes.begin(), pool_sizes.end());

    int answer2 = pool_sizes[pool_sizes.size() - 1] * pool_sizes[pool_sizes.size() - 2] * pool_sizes[pool_sizes.size() - 3];

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    delete[] map;

    return EXIT_SUCCESS;
}

int make_pool(int *map, int x, int y, int width, int height) {
    int size = 0;
    int v = map[width * y + x];
    if (v != -1 && v < 9) {
        map[width * y + x] = -1;
        size++;
        if (x > 0 && map[width * y + x - 1] != 9 && map[width * y + x - 1] != -1) {
            size += make_pool(map, x - 1, y, width, height);
        }
        if (x < width - 1 && map[width * y + x + 1] != 9 && map[width * y + x + 1] != -1) {
            size += make_pool(map, x + 1, y, width, height);
        }
        if (y > 0 && map[width * (y - 1) + x] != 9 && map[width * (y - 1) + x] != -1) {
            size += make_pool(map, x, y - 1, width, height);
        }
        if (y < height - 1 && width * (y + 1) + x != 9 && width * (y + 1) + x != -1) {
            size += make_pool(map, x, y + 1, width, height);
        }
    }

    return size;
}
