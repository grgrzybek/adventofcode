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

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;
using namespace aoc2021;

int main(int argc, char *argv[]) {
    Options options("Day 03", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;

    vector<int> zeros;
    vector<int> ones;

    vector<string *> oxygen_values;
    vector<string *> co2_values;

    int c = 0;
    while (getline(*input, line)) {
        trim(line);
        // for part 2
        oxygen_values.push_back(new string(line));
        co2_values.push_back(new string(line));

        int pos = 0;
        if (c == 0) {
            // init
            for (char ch: line) {
                if (ch == '0') {
                    zeros.push_back(1);
                    ones.push_back(0);
                } else {
                    zeros.push_back(0);
                    ones.push_back(1);
                }
            }
        } else {
            // update
            for (char ch: line) {
                if (ch == '0') {
                    zeros[pos++]++;
                } else {
                    ones[pos++]++;
                }
            }
        }

        c++;
    }

    // part 1

    int gamma = 0;
    int epsilon = 0;

    int shift = 0;
    for (unsigned long i = ones.size(); i > 0; i--) {
        if (ones[i - 1] >= zeros[i - 1]) {
            gamma |= 1 << shift;
        } else {
            epsilon |= 1 << shift;
        }
        shift++;
    }

    int answer1 = gamma * epsilon;

    // part 2

    int oxygen = 0;
    int co2 = 0;

    // the bit starting from left - to search the most common
    int bit = 0;

    // oxygen_values
    while (true) {
        // search for most common
        int zero_count = 0;
        int one_count = 0;
        for (string *&oxygen_value: oxygen_values) {
            if (!oxygen_value) {
                continue;
            }
            if ((*oxygen_value)[bit] == '0') {
                zero_count++;
            } else {
                one_count++;
            }
        }

        // removing not matching
        int values_left = 0;
        string *last_value;
        for (string *&oxygen_value: oxygen_values) {
            if (!oxygen_value) {
                continue;
            }
            if (one_count >= zero_count) {
                // keep value with 1
                if ((*oxygen_value)[bit] != '1') {
                    delete oxygen_value;
                    oxygen_value = nullptr;
                } else {
                    values_left++;
                    last_value = oxygen_value;
                }
            } else {
                // keep value with 0
                if ((*oxygen_value)[bit] != '0') {
                    delete oxygen_value;
                    oxygen_value = nullptr;
                } else {
                    values_left++;
                    last_value = oxygen_value;
                }
            }
        }

        shift = 0;
        if (values_left == 1) {
            for (unsigned long i = last_value->size(); i > 0; i--) {
                oxygen |= ((*last_value)[i - 1] == '0' ? 0 : 1) << shift;
                shift++;
            }
            break;
        }

        bit++;
    }

    bit = 0;

    // co2_values
    while (true) {
        // search for least common
        int zero_count = 0;
        int one_count = 0;
        for (string *&co2_value: co2_values) {
            if (!co2_value) {
                continue;
            }
            if ((*co2_value)[bit] == '0') {
                zero_count++;
            } else {
                one_count++;
            }
        }
        // removing not matching
        int values_left = 0;
        string *last_value;
        for (string *&co2_value: co2_values) {
            if (!co2_value) {
                continue;
            }
            if (zero_count <= one_count) {
                // keep value with 0
                if ((*co2_value)[bit] != '0') {
                    delete co2_value;
                    co2_value = nullptr;
                } else {
                    values_left++;
                    last_value = co2_value;
                }
            } else {
                // keep value with 1
                if ((*co2_value)[bit] != '1') {
                    delete co2_value;
                    co2_value = nullptr;
                } else {
                    values_left++;
                    last_value = co2_value;
                }
            }
        }

        shift = 0;
        if (values_left == 1) {
            for (unsigned long i = last_value->size(); i > 0; i--) {
                co2 |= ((*last_value)[i - 1] == '0' ? 0 : 1) << shift;
                shift++;
            }
            break;
        }

        bit++;
    }

    int answer2 = oxygen * co2;

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}
