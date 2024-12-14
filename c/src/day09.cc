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
    fclose(f);

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
    int *disk2 = new int[blocks];
    memset(disk, 0xff, blocks * sizeof(int));
    memset(disk2, 0xff, blocks * sizeof(int));

    int file_id = 0;
    size_t disk_idx = 0;
    size_t disk_idx2 = 0;

    vector<size_t> gaps;
    vector<size_t> gap_positions;

    bool file = true;
    for (size_t pos = 0; pos < size; pos++) {
        for (size_t i = 0; i < (size_t) (buffer[pos] - '0'); i++) {
            if (file) {
                disk[disk_idx + i] = file_id;
                disk2[disk_idx + i] = file_id;
            }
        }
        if (file) {
            file_id++;
        } else {
            if (pos < size - 1) {
                gaps.push_back(buffer[pos] - '0');
                gap_positions.push_back(disk_idx);
            }
        }
        disk_idx += (size_t) (buffer[pos] - '0');
        disk_idx2 += (size_t) (buffer[pos] - '0');
        file = !file;
    }

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
    }

    for (size_t pos = 0; pos < blocks; pos++) {
        if (disk[pos] != -1) {
            answer1 += (long) pos * disk[pos];
        }
    }

    // part 2

    long answer2 = 0;

    disk_idx2--;
    size_t file_size;

//    cout << "before: " << endl;
//    for (size_t pos = 0; pos < blocks; pos++) {
//        if (disk2[pos] == -1) {
//            cout << ". ";
//        } else {
//            cout << disk2[pos] << " ";
//        }
//    }
//    cout << endl;
//    cout << "gaps" << endl;
//    for (size_t i = 0; i < gaps.size(); i++) {
//        cout << gaps[i] << " (at: " << gap_positions[i] << ") ";
//    }
//    cout << endl;
//    cout << endl;
    while (true) {
        // seek to right-most file
        while (disk2[disk_idx2] == -1) {
            disk_idx2--;
        }
        // check it's size (in blocks)
        file_id = disk2[disk_idx2];
        file_size = 0;
        while (disk_idx2 > 0 && disk2[disk_idx2] == file_id) {
            disk_idx2--;
            file_size++;
        }
        if (disk_idx2 > 0) {
            disk_idx2++;
        }

        // find a gap to move entire file
        empty_block = 0;
//        cout << "id: " << file_id << ", size: " << file_size << ", at: " << disk_idx2 << endl;
        for (size_t gap_id = 0; gap_id < gaps.size(); gap_id++) {
            // find block of this gap
            empty_block = gap_positions[gap_id];
            if (empty_block >= disk_idx2) {
                break;
            }
//            cout << "   checking gap " << gap_id << " with size: " << gaps[gap_id] << endl;
            if (gaps[gap_id] >= file_size) {
                // move file
//                cout << "moving to gap " << gap_id << " of " << gaps[gap_id] << " size (empty block: " << empty_block << ")" << endl;
                for (size_t i = 0; i < file_size; i++) {
                    disk2[empty_block + i] = file_id;
                    disk2[disk_idx2 + i] = -1;
                }
                gaps[gap_id] -= file_size;
                gap_positions[gap_id] += file_size;
//                cout << "gaps" << endl;
//                for (size_t i = 0; i < gaps.size(); i++) {
//                    cout << gaps[i] << " (at: " << gap_positions[i] << ") ";
//                }
//                cout << endl;
//                cout << endl;
                break;
//            } else {
//                // find another and skip this gap
//                while (disk2[empty_block] == -1) {
//                    empty_block++;
//                }
            }
//            for (size_t pos = 0; pos < blocks; pos++) {
//                if (disk2[pos] == -1) {
//                    cout << ". ";
//                } else {
//                    cout << disk2[pos] << " ";
//                }
//            }
//            cout << endl;
//            cout << endl;
//            if (disk_idx2 >= empty_block) {
//                break;
//            }
        }
        if (file_id == 0) {
            break;
        }
        if (disk_idx2 > 0) {
            disk_idx2--;
        }
    }
//    cout << "after: " << endl;
//    for (size_t pos = 0; pos < blocks; pos++) {
//        if (disk2[pos] == -1) {
//            cout << ". ";
//        } else {
//            cout << disk2[pos] << " ";
//        }
//    }
//    cout << endl;

    for (size_t pos = 0; pos < blocks; pos++) {
        if (disk2[pos] != -1) {
            answer2 += (long) pos * disk2[pos];
        }
    }

    delete[] buffer;
    delete[] disk;
    delete[] disk2;

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}
