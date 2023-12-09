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
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <numeric>

#include "utils/utils.h"

using namespace std;

[[maybe_unused]] string to_string(const vector<int> &v);

int main(int argc, char *argv[]) {
    aoc2023::Options options("Day 09", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    int n;
    vector<vector<int> *> sequences;

    while (getline(*input, line)) {
        aoc2023::trim(line);
        istringstream iss(line);

        auto s = new vector<int>;
        sequences.emplace_back(s);
        while (!iss.eof()) {
            iss >> n;
            s->emplace_back(n);
        }
    }

    // part 1

    int answer1 = 0;

    vector<int> last_numbers;
    auto tmp1 = new vector<int>;
    vector<int> *tmp2;
    int n1, n2, previous;

    for (auto &s: sequences) {
        last_numbers.clear();
        last_numbers.emplace_back(*(s->end() - 1));

        tmp1 = s;
        tmp2 = new vector<int>;
        while (true) {
//            cout << "Checking sequence:" << to_string(*tmp1) << endl;
            bool all_zeros = true;
            for (auto it = tmp1->begin(); it != tmp1->end() - 1; it++) {
                n1 = *it;
                n2 = *(it + 1);
                tmp2->emplace_back(n2 - n1);
//                cout << " - emplacing " << (n2 - n1) << endl;
                if (n2 - n1 != 0) {
                    all_zeros = false;
                }
            }
            last_numbers.emplace_back(*(tmp2->end() - 1));
            if (tmp1 != s) {
                delete tmp1;
            }
            tmp1 = tmp2;
            tmp2 = new vector<int>;
            if (all_zeros) {
                break;
            }
        }
        delete tmp2;

        previous = 0;
//        cout << "last numbers:" << to_string(last_numbers) << endl;
        for (auto rit = last_numbers.crbegin() + 1; rit != last_numbers.crend(); rit++) {
//            cout << "    " << previous << " + " << *rit << endl;
            previous += *rit;
        }
//        cout << "    previous total: " << previous << endl;

        answer1 += previous;
    }

    // part 2

    int answer2 = 0;

    vector<int> first_numbers;
    tmp1 = new vector<int>;

    for (auto &s: sequences) {
        first_numbers.clear();
        first_numbers.emplace_back(*(s->begin()));

        tmp1 = s;
        tmp2 = new vector<int>;
        while (true) {
//            cout << "Checking sequence:" << to_string(*tmp1) << endl;
            bool all_zeros = true;
            for (auto it = tmp1->begin(); it != tmp1->end() - 1; it++) {
                n1 = *it;
                n2 = *(it + 1);
                tmp2->emplace_back(n2 - n1);
//                cout << " - emplacing " << (n2 - n1) << endl;
                if (n2 - n1 != 0) {
                    all_zeros = false;
                }
            }
            first_numbers.emplace_back(*(tmp2->begin()));
            if (tmp1 != s) {
                delete tmp1;
            }
            tmp1 = tmp2;
            tmp2 = new vector<int>;
            if (all_zeros) {
                break;
            }
        }
        delete tmp2;

        previous = 0;
        cout << "first numbers:" << to_string(first_numbers) << endl;
        for (auto rit = first_numbers.crbegin() + 1; rit != first_numbers.crend(); rit++) {
            cout << "    " << *rit << " - " << previous << endl;
            previous = *rit - previous;
        }
        cout << "    previous total: " << previous << endl;

        answer2 += previous;
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    for (auto &s: sequences) {
        delete s;
    }

    return EXIT_SUCCESS;
}

[[maybe_unused]] string to_string(const vector<int> &v) {
    ostringstream os;
    for (auto &n: v) {
        os << " " << n;
    }
    return os.str();
}
