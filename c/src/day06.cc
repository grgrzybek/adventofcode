/*
 * Copyright 2025 Grzegorz Grzybek
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
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    aoc2025::Options options("Day 06", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line, sym;
    vector<vector<long> *> table;
    vector<string> lines;
    string operations_line;
    vector<char> operations;

    while (getline(*input, line)) {
        string to_trim = line;
        aoc2025::trim(to_trim);
        istringstream iss(to_trim);
        auto row = new vector<long>;
        bool symbols = false;
        while (!iss.eof()) {
            iss >> sym;
            if (sym.length() == 1 && (sym[0] == '*' || sym[0] == '+')) {
                symbols = true;
                operations.push_back(sym[0]);
            } else {
                row->emplace_back(stol(sym));
            }
        }
        if (!symbols) {
            lines.emplace_back(line);
            table.emplace_back(row);
        } else {
            operations_line = line;
        }
    }

    // part 1

    long answer1 = 0;
    for (int i = 0; i < (int) operations.size(); i++) {
        char op = operations[i];
        long v = 0L;
        if (op == '+') {
            for (auto &r: table) {
                v += r->at(i);
            }
        } else {
            v = 1;
            for (auto &r: table) {
                v *= r->at(i);
            }
        }
        answer1 += v;
    }

    // part 2

    long answer2 = 0;
    int p1 = 0;
    int p2 = 0;
    int op = 0;
    for (p1 = 0; p1 < (int) operations_line.length();) {
        p2 = p1 + 1;
        while (p2 < (int) operations_line.length() && operations_line[p2] == ' ') {
            p2++;
        }
        if (p2 == (int) operations_line.length()) {
            p2++;
        }
        char oper = operations[op++];
        cout << "checking " << p1 << " - " << p2 << " [" << oper << "]" << endl;
        long v = 0;
        if (oper == '*') {
            v = 1;
        }
        int p = (int) p2 - 2;
        while (p >= p1) {
            int number = 0;
            cout << " - " << p << endl;
            int exp = 1;
            for (int row = (int) lines.size() - 1; row >= 0; row--) {
                cout << "   " << lines[row] << endl;
                char c = lines[row][p];
                if (c != ' ') {
                    number += (c - '0') * exp;
                    exp *= 10;
                }
            }
            cout << " --- " << number << endl;
            p--;
            if (oper == '*') {
                v *= number;
            } else {
                v += number;
            }
        }
        answer2 += v;
        p1 = p2;
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    for (auto &l: table) {
        delete l;
    }

    return EXIT_SUCCESS;
}
