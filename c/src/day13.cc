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

#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>

#include "utils/utils.h"

using namespace std;

struct packet {
    bool divider = false;
    packet *parent = nullptr;

    vector<int> values;
    vector<packet *> packets;

    ~packet() {
        for (auto &p: packets) {
            delete p;
        }
    }

    void to_string(size_t indent) const { // NOLINT(misc-no-recursion)
        string ind;
        for (size_t i = 0; i < indent; i++) {
            ind += " ";
        }
        for (size_t i = 0; i < values.size(); i++) {
            if (values[i] >= 0) {
                cout << ind << values[i] << "\n";
            } else {
                cout << ind << "{\n";
                packets[i]->to_string(indent + 2);
                cout << ind << "}\n";
            }
        }
    }

    int right_order(packet *p2) { // NOLINT(misc-no-recursion)
        for (size_t i = 0; i < ::min(this->values.size(), p2->values.size()); i++) {
            if (this->values[i] >= 0 && p2->values[i] >= 0) {
                // two values
                if (this->values[i] == p2->values[i]) {
                    // the same values
                    continue;
                }
                return this->values[i] < p2->values[i] ? -1 : 1;
            } else if (this->values[i] == -1 && p2->values[i] == -1) {
                // two lists
                int r = this->packets[i]->right_order(p2->packets[i]);
                if (r != 0) {
                    return r;
                }
            } else {
                // a list and a value
                if (this->values[i] >= 0) {
                    this->packets[i] = new packet;
                    this->packets[i]->values.push_back(this->values[i]);
                    this->packets[i]->packets.push_back(nullptr);
                    this->values[i] = -1;
                }
                if (p2->values[i] >= 0) {
                    p2->packets[i] = new packet;
                    p2->packets[i]->values.push_back(p2->values[i]);
                    p2->packets[i]->packets.push_back(nullptr);
                    p2->values[i] = -1;
                }
                int r = this->packets[i]->right_order(p2->packets[i]);
                if (r != 0) {
                    return r;
                }
            }
        }
        return this->values.size() < p2->values.size() ? -1 : (this->values.size() > p2->values.size() ? 1 : 0);
    }

    friend bool operator<(packet &a, packet &b) {
        return a.right_order(&b) == -1;
    }
};

packet *parse(string &line);

int main(int argc, char *argv[]) {
    aoc2022::Options options("Day 13", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<packet *> packets1;
    vector<packet *> packets2;

    while (getline(*input, line)) {
        if (!aoc2022::has_text(line)) {
            continue;
        }
        packets1.emplace_back(parse(line));
        getline(*input, line);
        packets2.emplace_back(parse(line));
    }

//    cout << "=== packets1\n";
//    for (auto &p: packets1) {
//        cout << "===\n";
//        p->to_string(0);
//    }
//    cout << "=== packets2\n";
//    for (auto &p: packets2) {
//        cout << "===\n";
//        p->to_string(0);
//    }

    // part 1

    int answer1 = 0;

    for (size_t i = 0; i < packets1.size(); i++) {
        packet *p1 = packets1[i];
        packet *p2 = packets2[i];

        cout << "compare: \n";
        cout << "1: \n";
        p1->to_string(0);
        cout << "2: \n";
        p2->to_string(0);
        if (p1->right_order(p2) == -1) {
            cout << "right order\n";
            answer1 += (int) i + 1;
        }
    }

    // part 2

    int answer2 = 1;
    vector<packet *> packets;
    packets.insert(packets.end(), packets1.begin(), packets1.end());
    packets.insert(packets.end(), packets2.begin(), packets2.end());


    string dp2 = "[[2]]";
    packet *dp = parse(dp2);
    dp->divider = true;
    packets.push_back(dp);
    string dp6 = "[[6]]";
    dp = new packet;
    dp = parse(dp6);
    dp->divider = true;
    packets.push_back(dp);

    sort(packets.begin(), packets.end(), [](packet *p1, packet *p2) {
        return *p1 < *p2;
    });

    int pos = 1;
    for (auto &p: packets) {
        if (p->divider) {
            answer2 *= pos;
        }
        pos++;
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    for (auto &p: packets) {
        delete p;
    }

    return EXIT_SUCCESS;
}

packet *parse(string &line) {
    cout << "parse: " << line << "\n";
    char l[line.length()];
    ::strcpy(l, line.c_str());
    packet *root = nullptr;
    packet *current = nullptr;
    packet *p = nullptr;
    int pow = 0;
    int value = 0;
    bool in_value = false;
    for (char c : line) {
        if (c == '[') {
            if (current != nullptr) {
                p = new packet;
                p->parent = current;
                current->values.push_back(-1);
                current->packets.push_back(p);
                current = p;
            } else {
                current = new packet;
            }
            if (root == nullptr) {
                root = current;
            }
        } else if (c == ']' || c == ',') {
            pow = 0;
            if (in_value) {
                current->values.push_back(value);
                current->packets.push_back(nullptr);
            }
            if (c == ']') {
                current = current->parent;
            }
            in_value = false;
            value = 0;
        } else {
            in_value = true;
            value *= ((int) ::pow(10, pow++));
            value += c - '0';
        }
    }
    return root;
}
