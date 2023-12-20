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

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>
#include <numeric>

#include "utils/utils.h"

using namespace std;

struct slot {
    string label;
    int fl; // focal length

    slot(string &label, int focal_length) : label(label), fl(focal_length) {}
};

int hash_alg(string &v);

int main(int argc, char *argv[]) {
    aoc2023::Options options("Day 15", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    vector<string> strings;
    string line;
    string token;
    getline(*input, line);
    string::size_type pos1 = 0;
    string::size_type pos2 = 0;
    while (true) {
        pos2 = line.find(',', pos1);
        if (pos2 == string::npos) {
            pos2 = line.size();
        }
        token = line.substr(pos1, pos2 - pos1);
        pos1 = pos2;
        strings.emplace_back(token);
        if (pos1 == line.size()) {
            break;
        }
        pos1++;
    }

    // part 1

    unsigned long answer1 = 0;

    for (auto &s: strings) {
        answer1 += hash_alg(s);
    }

    // part 2

    unsigned long answer2 = 0;

    // rn=1,cm-,qp=3,cm=2,qp-,pc=4,ot=9,ab=5,pc-,pc=6,ot=7

    vector<list<slot *> *> boxes(256);

    for (size_t pos = 0; pos < 256; pos++) {
        boxes[pos] = new list<slot *>;
    }

    string label;
    int focal_length;
    string::size_type pos;
    for (auto &str: strings) {
        if (str.ends_with('-')) {
            label = str.substr(0, str.length() - 1);
            cout << " - " << label << endl;

            auto hash = hash_alg(label);
            boxes[hash]->remove_if([label](const slot *s) { return s->label == label; });
        } else {
            pos = str.find('=');
            label = str.substr(0, pos);
            focal_length = (int) strtol(str.substr(pos + 1, str.length() - pos - 1).c_str(), nullptr, 10);
            cout << " = " << label << " | " << focal_length << endl;
            auto hash = hash_alg(label);
            bool found = false;
            for (auto &s: *boxes[hash]) {
                if (s->label == label) {
                    s->fl = focal_length;
                    found = true;
                    break;
                }
            }
            if (!found) {
                boxes[hash]->emplace_back(new slot(label, focal_length));
            }
        }
    }

    pos = 1;
    for (auto &b: boxes) {
        pos2 = 1;
        for (auto &s: *b) {
            answer2 += (pos * pos2 * s->fl);
            pos2++;
        }
        pos++;
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    for (auto &b: boxes) {
        for (auto &s: *b) {
            delete s;
        }
        delete b;
    }

    return EXIT_SUCCESS;
}

int hash_alg(string &s) {
    int v = 0;
    for (auto &c: s) {
        v += (int) c;
        v *= 17;
        v %= 256;
    }

    return v;
}
