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

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <vector>

#include "utils/utils.h"

using namespace std;
namespace fs = std::filesystem;

int evaluate(const char *buffer, size_t pos, size_t limit);

int main(int argc, char *argv[]) {
    aoc2024::Options options("Day 03", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    fs::path p = options.file_name();
    FILE *f = fopen(options.file_name().c_str(), "r");

    uintmax_t size = fs::file_size(p);
    char *buffer = new char[size + 1];
    buffer[size] = '\0';

    fread((void *) buffer, size, 1, f);
//    cout << string(buffer) << endl;

    // part 1

    int answer1 = 0;

    size_t pos = 0;
    while (pos < size - 3) {
        if (strncmp(buffer + pos, "mul(", 4) == 0) {
            answer1 += evaluate(buffer, pos, size);
        }
        pos++;
    }

    // part 2

    int answer2 = 0;

    pos = 0;
    bool enabled = true;
    while (pos < size - 3) {
        if (strncmp(buffer + pos, "do()", 4) == 0) {
//            cout << "enabled at " << pos << endl;
            enabled = true;
            pos += 4;
        } else if (strncmp(buffer + pos, "don't()", 7) == 0) {
//            cout << "disabled at " << pos << endl;
            enabled = false;
            pos += 7;
        } else if (enabled && strncmp(buffer + pos, "mul(", 4) == 0) {
            answer2 += evaluate(buffer, pos, size);
            pos++;
        } else {
            pos++;
        }
    }

    delete[] buffer;

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}

int evaluate(const char *buffer, size_t pos, size_t limit) {
    int a, b;
//    cout << "scanning: [" << string(buffer + pos) << "]" << endl;
    size_t p = pos + 4;
    size_t comma = -1;
    int state = 1; // 1 - a, 2 - ",", 3 - b, 4 - ")"
    bool ok = true;
    while (p < limit) {
        switch (state) {
            case 1:
                if (!(buffer[p] >= '0' && buffer[p] <= '9')) {
                    if (p > pos && buffer[p] == ',') {
                        state = 3;
                        comma = pos;
                    } else {
                        ok = false;
                    }
                }
                break;
            case 3:
                if (!(buffer[p] >= '0' && buffer[p] <= '9')) {
                    if (p > comma && buffer[p] == ')') {
                        state = 4;
                    } else {
                        ok = false;
                    }
                }
                break;
            case 4:
                break;
        }
        if (!ok || state == 4) {
            break;
        }
        p++;
    }
    if (ok) {
        int scanned = sscanf(buffer + pos, "mul(%d,%d)", &a, &b);
//        cout << "    scanned: " << a << ", " << b << " (" << scanned << ")" << endl;
        return scanned == 2 ? a * b : 0;
    } else {
        return 0;
    }
}
