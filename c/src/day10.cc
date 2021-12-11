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
#include <deque>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;
using namespace aoc2021;

int make_pool(int *map, int x, int y, int width, int height);

int main(int argc, char *argv[]) {
    Options options("Day 10", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<string> lines;
    vector<string> incomplete_lines;

    while (getline(*input, line)) {
        trim(line);
        lines.push_back(line);
    }

    // part 1

    int answer1 = 0;

    for (string &l : lines) {
        deque<char> stack;
        char check;
        bool ok = true;
        for (char &c : l) {
            if (c == '(' || c == '{' || c == '[' || c == '<') {
                stack.push_back(c);
            } else {
                check = stack.back();
                if (c == ')' && check != '(') {
                    answer1 += 3;
                    ok = false;
                    break;
                }
                if (c == ']' && check != '[') {
                    answer1 += 57;
                    ok = false;
                    break;
                }
                if (c == '}' && check != '{') {
                    answer1 += 1197;
                    ok = false;
                    break;
                }
                if (c == '>' && check != '<') {
                    answer1 += 25137;
                    ok = false;
                    break;
                }
                stack.pop_back();
            }
        }
        if (ok) {
            incomplete_lines.push_back(l);
        }
    }

    // part 2

    long answer2 = 0;
    vector<long> scores;

    for (string &l : incomplete_lines) {
        deque<char> stack;
        for (char &c : l) {
            if (c == '(' || c == '{' || c == '[' || c == '<') {
                stack.push_back(c);
            } else {
                stack.pop_back();
            }
        }

        long score = 0;
        // the stack is non-empty now
        deque<char>::const_iterator it = stack.cend();
        while (it != stack.cbegin()) {
            it--;
            score *= 5;
            if (*it == '(') {
                score += 1;
            }
            if (*it == '[') {
                score += 2;
            }
            if (*it == '{') {
                score += 3;
            }
            if (*it == '<') {
                score += 4;
            }
        }
        scores.push_back(score);
    }

    sort(scores.begin(), scores.end());
    answer2 = scores[scores.size() / 2];

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}
