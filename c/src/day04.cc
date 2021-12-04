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

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;
using namespace aoc2021;

void print_board(int *&board);

int main(int argc, char *argv[]) {
    Options options("Day 03", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;

    vector<int> numbers;
    vector<int *> original_boards;
    vector<int *> boards1;
    vector<bool> boards_finished;
    int board_count = 0;

    int c = 0;
    while (getline(*input, line)) {
        trim(line);
        if (line.empty()) {
            if (c >= 1) {
                original_boards.push_back(new int[25]{});
                boards1.push_back(new int[25]{});
                boards_finished.push_back(false);
                board_count++;
            }
            continue;
        }

        if (c == 0) {
            // numbers
            istringstream iss(line);
            int v;
            char comma;
            while (!iss.eof()) {
                iss >> v;
                numbers.push_back(v);
                iss >> comma;
            }
        } else {
            // row for current bingo card
            istringstream iss(line);
            int v;
            int n = 0;
            while (!iss.eof()) {
                iss >> v;
                original_boards[board_count - 1][n] = v;
                boards1[board_count - 1][n] = -1;
                n++;
            }
            for (int i = 0; i < 4; i++) {
                // next 4 lines in bingo card
                getline(*input, line);
                trim(line);
                istringstream iss2(line);
                while (!iss2.eof()) {
                    iss2 >> v;
                    original_boards[board_count - 1][n] = v;
                    boards1[board_count - 1][n] = -1;
                    n++;
                }
            }
        }

        c++;
    }

//    cout << "Numbers: ";
//    for (auto &n: numbers) {
//        cout << n << " ";
//    }
//    cout << endl;
//
//    cout << "Boards: " << board_count << endl;
//    int bc = 1;
//    for (int *&board: original_boards) {
//        cout << "Board " << bc++;
//        print_board(board);
//    }

    // part 1 & 2

    int answer1 = 0;
    int answer2 = 0;

    for (int &n: numbers) {
        int board = 0;
        // marking current number in all boards in boards1 - changing -1 to number
        for (int *&ob: original_boards) {
            for (int i = 0; i < 25; i++) {
                if (ob[i] == n) {
                    boards1[board][i] = n;
                }
            }
            board++;
        }

        vector<int> winning_boards;
        board = 0;

        // checking rows and columns on every board
        for (int *&ob: boards1) {
            if (boards_finished[board]) {
                board++;
                continue;
            }
            // checking rows
            int full_row = -1;
            for (int row = 0; row < 5; row++) {
                int full = 0;
                for (int cell = 0; cell < 5; cell++) {
                    if (ob[row * 5 + cell] != -1) {
                        full++;
                    }
                }
                if (full == 5) {
                    full_row = row / 5;
                    break;
                }
            }

            // checking columns
            int full_column = -1;
            for (int i = 0; i < 5; i++) {
                int full = 0;
                for (int row = 0; row < 5; row++) {
                    if (ob[row * 5 + i] != -1) {
                        full++;
                    }
                }
                if (full == 5) {
                    full_column = i;
                    break;
                }
            }

            if (full_row >= 0 || full_column >= 0) {
                winning_boards.push_back(board);
                boards_finished[board] = true;
            }

            board++;
        }

        int sum_unmarked = 0;
        if (!winning_boards.empty()) {
            for (int &winning_board: winning_boards) {
                for (int i = 0; i < 25; i++) {
                    if (boards1[winning_board][i] == -1) {
                        sum_unmarked += original_boards[winning_board][i];
                    }
                }
//                cout << "Winning board: " << winning_board + 1 << ", n: " << n << endl;
//                print_board(boards1[winning_board]);
                if (answer1 == 0) {
                    answer1 = sum_unmarked * n;
                }
                answer2 = sum_unmarked * n;
            }
        }
    }

    // part 2

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    for (int *&ptr: original_boards) {
        delete[] ptr;
    }
    for (int *&ptr: boards1) {
        delete[] ptr;
    }

    return EXIT_SUCCESS;
}

void print_board(int *&board) {
    for (int i = 0; i < 25; i++) {
        if (i % 5 == 0) {
            cout << endl;
        }
        cout << setw(3) << setfill(' ') << board[i];
    }
    cout << endl << endl;
}
