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

#include <iostream>
#include <set>
#include <string>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    aoc2022::Options options("Day 06", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    // bvwbjplbgvbhsrlpgdmjqwftvncz: first marker after character 5
    // nppdvjthqldpwncqszvftbrmjlhg: first marker after character 6
    // nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg: first marker after character 10
    // zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw: first marker after character 11

    ifstream *input = options.file();

    string line;
    set<char> header;

    getline(*input, line);

    // part 1

    int answer1 = 0;
    for (iter_difference_t<char> pos = 4; pos < static_cast<iter_difference_t<char>>(line.length()); pos++) {
        header.clear();
        header.insert(line.begin() + pos - 4, line.begin() + pos);
        if (header.size() == 4) {
            answer1 = pos;
            break;
        }
    }

    // part 2

    int answer2 = 0;
    for (iter_difference_t<char> pos = 14; pos < static_cast<iter_difference_t<char>>(line.length()); pos++) {
        header.clear();
        header.insert(line.begin() + pos - 14, line.begin() + pos);
        if (header.size() == 14) {
            answer2 = pos;
            break;
        }
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}