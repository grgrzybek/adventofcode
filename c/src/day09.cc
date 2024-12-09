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

int main(int argc, char *argv[]) {
    aoc2024::Options options("Day 09", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    fs::path p = options.file_name();
    FILE *f = fopen(options.file_name().c_str(), "r");

    uintmax_t size = fs::file_size(p);
    char *buffer = new char[size];

    fread((void *) buffer, 1, size, f);
    while (buffer[size - 1] < '0' || buffer[size - 1] > '9') {
        size--;
    }
    buffer[size++] = '0'; // last free space, so we have Nx file|space
    cout << "real size: " << size << endl;

    // part 1

    long answer1 = 0;

    size_t blocks = 0;
    for (size_t pos = 0; pos < size; pos++) {
        blocks += (buffer[pos] - '0');
    }

    int *disk = new int[blocks];
    memset(disk, 0xff, blocks * sizeof(int));

    int file_id = 0;
    size_t disk_idx = 0;

    bool file = true;
    for (size_t pos = 0; pos < size; pos++) {
        for (size_t i = 0; i < (size_t) (buffer[pos] - '0'); i++) {
            if (file) {
                disk[disk_idx + i] = file_id;
            }
        }
        if (file) {
            file_id++;
        }
        disk_idx += (size_t) (buffer[pos] - '0');
        file = !file;
    }
//    cout << "before: " << endl;
//    for (size_t pos = 0; pos < blocks; pos++) {
//        if (disk[pos] == -1) {
//            cout << ". ";
//        } else {
//            cout << disk[pos] << " ";
//        }
//    }
//    cout << endl;

    size_t empty_block = 0;
    disk_idx--;

    while (empty_block < disk_idx) {
        while (disk[empty_block] != -1) {
            empty_block++;
        }
        while (disk[disk_idx] == -1) {
            disk_idx--;
        }
        if (empty_block >= disk_idx) {
            break;
        }
        disk[empty_block] = disk[disk_idx];
        disk[disk_idx--] = -1;
//        for (size_t pos = 0; pos < blocks; pos++) {
//            if (disk[pos] == -1) {
//                cout << ".";
//            } else {
//                cout << disk[pos] << "";
//            }
//        }
//        cout << endl;
    }
//    cout << "after: " << endl;
//    for (size_t pos = 0; pos < blocks; pos++) {
//        if (disk[pos] == -1) {
//            cout << ". ";
//        } else {
//            cout << disk[pos] << " ";
//        }
//    }
//    cout << endl;
//    cout << "empty: " << empty_block << ", disk idx: " << disk_idx << endl;

    for (size_t pos = 0; pos < blocks; pos++) {
        if (disk[pos] != -1) {
            answer1 += (pos * disk[pos]);
        }
    }

    // part 2

    int answer2 = 0;

    delete[] buffer;
    delete[] disk;

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}
