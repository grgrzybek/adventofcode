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
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>

#include "utils/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    aoc2024::Options options("Day 11", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;


    getline(*input, line);
    istringstream iss(line);
    list<long> values;
    list<long> values2;

    long v, c;
    while (!iss.eof()) {
        iss >> v;
        values.push_back(v);
        values2.push_back(v);
    }

    // part 1

    size_t answer1 = 0;

    ostringstream oss;
//    for (int i = 0; i < 25; i++) {
////        cout << "size: " << values.size() << ": ";
////        for (auto it = values.begin(); it != values.end(); it++) {
////            cout << *it << " ";
////        }
////        cout << endl << endl;
//        for (auto it = values.begin(); it != values.end(); it++) {
//            v = *it;
//            if (v == 0) {
//
//                *it = 1;
//            } else {
//                oss.str("");
//                oss << *it;
//                unsigned long size = oss.str().length();
//                if (size % 2 == 0) {
//                    int v1 = stoi(oss.str().substr(0, size / 2));
//                    int v2 = stoi(oss.str().substr(size / 2));
//                    *it = v1;
//                    it++;
//                    it = values.insert(it, v2);
//                } else {
//                    *it *= 2024;
//                }
//            }
//        }
//    }
//
//    answer1 = values.size();

    // part 2

    map<long, long> m;
//    map<long, map<long, long>*> trees;
//    map<long, list<long>*> sizes;

    for (auto it = values2.begin(); it != values2.end(); it++) {
        m[*it]++;
    }

//    cout << "start:\n";
//    for (auto &e: m) {
//        cout << "[" << e.first << "]: " << e.second << endl;
//    }
//    cout << endl;

    long answer2 = 0;
    for (int i = 0; i < 75; i++) {
        if (i == 25) {
            for (auto &e: m) {
                answer1 += e.second;
            }
        }
//        cout << "iteration " << (i + 1) << endl;
        map<long, long> copy;
        for (auto &e: m) {
            copy[e.first] = e.second;
            m[e.first] = 0;
        }
        for (auto &e: copy) {
            if (e.second == 0) {
                continue;
            }
            v = e.first;
            c = copy[v];
//            cout << "checking " << v << " (" << c << ")" << endl;
            if (v == 0) {
//                cout << "   m[0] = 0" << endl;
                if (!m.contains(1) || m[1] == 0) {
//                    cout << "   m[1] = " << c << endl;
                    m[1] = c;
                } else {
//                    cout << "   m[1] += " << c << endl;
                    m[1] += c;
                }
            } else {
                oss.str("");
                oss << v;
                unsigned long size = oss.str().length();
                if (size % 2 == 0) {
                    int v1 = stoi(oss.str().substr(0, size / 2));
                    int v2 = stoi(oss.str().substr(size / 2));
//                    cout << "   m[" << v << "] = 0" << endl;
                    if (!m.contains(v1) || m[v1] == 0) {
//                        cout << "   m[" << v1 << "] = " << c << endl;
                        m[v1] = c;
                    } else {
//                        cout << "   m[" << v1 << "] += " << c << endl;
                        m[v1] += c;
                    }
                    if (!m.contains(v2) || m[v2] == 0) {
//                        cout << "   m[" << v2 << "] = " << c << endl;
                        m[v2] = c;
                    } else {
//                        cout << "   m[" << v2 << "] += " << c << endl;
                        m[v2] += c;
                    }
                } else {
//                    cout << "   m[" << v << "] = 0" << endl;
                    if (!m.contains(v * 2024) || m[v * 2024] == 0) {
//                        cout << "   m[" << (v * 2024) << "] = " << c << endl;
                        m[v * 2024] = c;
                    } else {
//                        cout << "   m[" << (v * 2024) << "] += " << c << endl;
                        m[v * 2024] += c;
                    }
                }
            }
        }
//        long s = 0;
//        for (auto &e: m) {
//            cout << "[" << e.first << "]: " << e.second << endl;
//            s += e.second;
//        }
//        cout << "size: " << s << endl;
//        cout << endl;
    }
    for (auto &e: m) {
        answer2 += e.second;
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}
