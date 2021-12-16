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
#include <limits>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;
using namespace aoc2021;

int process(string &line, vector<unsigned long long> &values);
unsigned int take_group(vector<int> &bits, unsigned int pos);
vector<int>::size_type parse_packets(vector<int> &bits, unsigned long from, unsigned long to, int count, int *versions, vector<unsigned long long> &values);

int main(int argc, char *argv[]) {
    Options options("Day 16", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<string> lines;

    while (getline(*input, line)) {
        trim(line);
        lines.push_back(line);
    }

    // part 1 & 2

    for (auto &l: lines) {
        vector<unsigned long long> values;
        cout << "Checking \"" << l << "\"\n";
        cout << "Sum of versions: " << process(l, values) << endl;
        cout << "Value: " << values[0] << endl;
    }

    return EXIT_SUCCESS;
}

int process(string &line, vector<unsigned long long> &values) {
    vector<int> bits;
    for (auto &c: line) {
        if (c >= '0' && c <= '9') {
            int v = c - '0';
            bits.push_back((v & 0x8) >> 3);
            bits.push_back((v & 0x4) >> 2);
            bits.push_back((v & 0x2) >> 1);
            bits.push_back((v & 0x1));
        } else if (c >= 'A' && c <= 'F') {
            int v = 10 + c - 'A';
            bits.push_back((v & 0x8) >> 3);
            bits.push_back((v & 0x4) >> 2);
            bits.push_back((v & 0x2) >> 1);
            bits.push_back((v & 0x1));
        }
    }

    int versions = 0;
    parse_packets(bits, 0, bits.size(), 1, &versions, values);

    return versions;
}

vector<int>::size_type parse_packets(vector<int> &bits, unsigned long from, unsigned long to, int count, int *versions, vector<unsigned long long> &values) {
    vector<int>::size_type pos = from;
    while (pos < to) {
        int v = bits[pos] << 2 | bits[pos + 1] << 1 | bits[pos + 2];
        int type = bits[pos + 3] << 2 | bits[pos + 4] << 1 | bits[pos + 5];
        (*versions) += v;
        pos += 6;

        if (type == 4) {
            // literal value
            unsigned long long value = 0;
            vector<unsigned long long> groups;
            while (true) {
                unsigned int g = take_group(bits, pos);
                pos += 5;
                if (g & 0x10) {
                    // not last group
                    groups.push_back(g & 0xF);
                } else {
                    // last group
                    groups.push_back(g);
                    break;
                }
            }
            int shift = 0;
            for (auto it = groups.crbegin(); it != groups.crend(); it++) {
                value |= *it << shift;
                shift += 4;
            }
            values.push_back(value);
        } else {
            // operator
            int length_type_id = bits[pos++];
            int length = 0;
            vector<unsigned long long> local_values;
            if (length_type_id == 0) {
                // next 15 bits are a number that represents the total length in bits of the sub-packets
                for (int b = 14; b >= 0; b--) {
                    length |= bits[pos++] << b;
                }
                // easy
                parse_packets(bits, pos, pos + length, numeric_limits<int>::max(), versions, local_values);
                pos += length;
            } else {
                // next 11 bits are a number that represents the number of sub-packets immediately contained by this packet
                for (int b = 10; b >= 0; b--) {
                    length |= bits[pos++] << b;
                }
                pos = parse_packets(bits, pos, to, length, versions, local_values);
            }

            switch (type) {
                case 0: { // sum
                    unsigned long sum = 0;
                    for (auto &v: local_values) {
                        sum += v;
                    }
                    values.push_back(sum);
                    break;
                }
                case 1: { // product
                    unsigned long product = 1;
                    for (auto &v: local_values) {
                        product *= v;
                    }
                    values.push_back(product);
                    break;
                }
                case 2: { // min
                    unsigned long min = numeric_limits<unsigned long>::max();
                    for (auto &v: local_values) {
                        if (min > v) {
                            min = v;
                        }
                    }
                    values.push_back(min);
                    break;
                }
                case 3: { // max
                    unsigned long max = 0;
                    for (auto &v: local_values) {
                        if (max < v) {
                            max = v;
                        }
                    }
                    values.push_back(max);
                    break;
                }
                case 5: { // greater than
                    values.push_back(local_values[0] > local_values[1]);
                    break;
                }
                case 6: { // less than
                    values.push_back(local_values[0] < local_values[1]);
                    break;
                }
                case 7: { // equal to
                    values.push_back(local_values[0] == local_values[1]);
                    break;
                }
                default:
                    break;
            }
        }

        if (--count == 0) {
            break;
        }
    }

    return pos;
}

unsigned int take_group(vector<int> &bits, unsigned int pos) {
    unsigned int v = 0;
    v |= bits[pos] << 4;
    v |= bits[pos + 1] << 3;
    v |= bits[pos + 2] << 2;
    v |= bits[pos + 3] << 1;
    v |= bits[pos + 4];
    return v;
}
