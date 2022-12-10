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
#include <deque>
#include <iostream>
#include <vector>

#include "utils/utils.h"

using namespace std;

struct monkey {
    int n;
    long long value;
    long long modulo;
    int m_true;
    int m_false;
    deque<long long> interests;
    int inspection_count = 0;

    long long (*op)(long long a, long long b);

    monkey(int n, long long value, long long modulo, int m_true, int m_false) : n(n), value(value), modulo(modulo),
                                                                    m_true(m_true), m_false(m_false) {}
};

long long imul(long long item, long long value) {
    return item * value;
}

long long iadd(long long item, long long value) {
    return item + value;
}

long long mul(long long item, long long _) {
    return item * item;
}

int main(int argc, char *argv[]) {
    aoc2022::Options options("Day 11", argc, argv);

    cout << "Starting " << options.program_name << endl;

    // Monkey 3:
    //   Starting items: 74
    //   Operation: new = old + 3
    //   Test: divisible by 17
    //     If true: throw to monkey 0
    //     If false: throw to monkey 1

    vector<monkey *> monkeys;

    // test data
//    auto m = new monkey(0, 19, 23, 2, 3);
//    m->interests.push_back(79);
//    m->interests.push_back(98);
//    m->op = imul;
//    monkeys.push_back(m);
//    m = new monkey(1, 6, 19, 2, 0);
//    m->interests.push_back(54);
//    m->interests.push_back(65);
//    m->interests.push_back(75);
//    m->interests.push_back(74);
//    m->op = iadd;
//    monkeys.push_back(m);
//    m = new monkey(2, 0, 13, 1, 3);
//    m->interests.push_back(79);
//    m->interests.push_back(60);
//    m->interests.push_back(97);
//    m->op = mul;
//    monkeys.push_back(m);
//    m = new monkey(3, 3, 17, 0, 1);
//    m->interests.push_back(74);
//    m->op = iadd;
//    monkeys.push_back(m);

    // real data
    auto m = new monkey(0, 3, 5, 2, 3);
    m->interests.push_back(65);
    m->interests.push_back(78);
    m->op = imul;
    monkeys.push_back(m);

    m = new monkey(1, 8, 11, 4, 7);
    m->interests.push_back(54);
    m->interests.push_back(78);
    m->interests.push_back(86);
    m->interests.push_back(79);
    m->interests.push_back(73);
    m->interests.push_back(64);
    m->interests.push_back(85);
    m->interests.push_back(88);
    m->op = iadd;
    monkeys.push_back(m);

    m = new monkey(2, 2, 2, 5, 3);
    m->interests.push_back(69);
    m->interests.push_back(97);
    m->interests.push_back(77);
    m->interests.push_back(88);
    m->interests.push_back(87);
    m->op = iadd;
    monkeys.push_back(m);

    m = new monkey(3, 4, 13, 1, 5);
    m->interests.push_back(99);
    m->op = iadd;
    monkeys.push_back(m);

    m = new monkey(4, 19, 7, 7, 6);
    m->interests.push_back(60);
    m->interests.push_back(57);
    m->interests.push_back(52);
    m->op = imul;
    monkeys.push_back(m);

    m = new monkey(5, 5, 3, 4, 1);
    m->interests.push_back(91);
    m->interests.push_back(82);
    m->interests.push_back(85);
    m->interests.push_back(73);
    m->interests.push_back(84);
    m->interests.push_back(53);
    m->op = iadd;
    monkeys.push_back(m);

    m = new monkey(6, 0, 17, 0, 2);
    m->interests.push_back(88);
    m->interests.push_back(74);
    m->interests.push_back(68);
    m->interests.push_back(56);
    m->op = mul;
    monkeys.push_back(m);

    m = new monkey(7, 1, 19, 6, 0);
    m->interests.push_back(54);
    m->interests.push_back(82);
    m->interests.push_back(72);
    m->interests.push_back(71);
    m->interests.push_back(53);
    m->interests.push_back(99);
    m->interests.push_back(67);
    m->op = iadd;
    monkeys.push_back(m);

    // part 1

    long answer1 = 0;

    long long v;
    for (int round = 0; round < 20; round++) {
        for (auto &monkey: monkeys) {
            while (!monkey->interests.empty()) {
                monkey->inspection_count++;
                v = monkey->interests.front();
                monkey->interests.pop_front();
                v = monkey->op(v, monkey->value);
                v = v / 3;
                if (v % monkey->modulo == 0) {
                    // divisible
                    monkeys[monkey->m_true]->interests.push_back(v);
                } else {
                    // not divisible
                    monkeys[monkey->m_false]->interests.push_back(v);
                }
            }
        }
    }
    vector<long long> inspections;
    for (auto &_m : monkeys) {
        inspections.push_back(_m->inspection_count);
    }
    sort(inspections.begin(), inspections.end(), [](long long i1, long long i2) { return i1 > i2; });
    answer1 = inspections[0] * inspections[1];

    // part 2

    // test data
//    monkeys[0]->interests.clear();
//    monkeys[0]->interests.push_back(79);
//    monkeys[0]->interests.push_back(98);
//    monkeys[0]->inspection_count = 0;
//    monkeys[1]->interests.clear();
//    monkeys[1]->interests.push_back(54);
//    monkeys[1]->interests.push_back(65);
//    monkeys[1]->interests.push_back(75);
//    monkeys[1]->interests.push_back(74);
//    monkeys[1]->inspection_count = 0;
//    monkeys[2]->interests.clear();
//    monkeys[2]->interests.push_back(79);
//    monkeys[2]->interests.push_back(60);
//    monkeys[2]->interests.push_back(97);
//    monkeys[2]->inspection_count = 0;
//    monkeys[3]->interests.clear();
//    monkeys[3]->interests.push_back(74);
//    monkeys[3]->inspection_count = 0;

    // real data
    for (auto &monkey: monkeys) {
        monkey->interests.clear();
        monkey->inspection_count = 0;
    }
    m = monkeys[0];
    m->interests.push_back(65);
    m->interests.push_back(78);
    m = monkeys[1];
    m->interests.push_back(54);
    m->interests.push_back(78);
    m->interests.push_back(86);
    m->interests.push_back(79);
    m->interests.push_back(73);
    m->interests.push_back(64);
    m->interests.push_back(85);
    m->interests.push_back(88);
    m = monkeys[2];
    m->interests.push_back(69);
    m->interests.push_back(97);
    m->interests.push_back(77);
    m->interests.push_back(88);
    m->interests.push_back(87);
    m = monkeys[3];
    m->interests.push_back(99);
    m = monkeys[4];
    m->interests.push_back(60);
    m->interests.push_back(57);
    m->interests.push_back(52);
    m = monkeys[5];
    m->interests.push_back(91);
    m->interests.push_back(82);
    m->interests.push_back(85);
    m->interests.push_back(73);
    m->interests.push_back(84);
    m->interests.push_back(53);
    m = monkeys[6];
    m->interests.push_back(88);
    m->interests.push_back(74);
    m->interests.push_back(68);
    m->interests.push_back(56);
    m = monkeys[7];
    m->interests.push_back(54);
    m->interests.push_back(82);
    m->interests.push_back(72);
    m->interests.push_back(71);
    m->interests.push_back(53);
    m->interests.push_back(99);
    m->interests.push_back(67);

    long long answer2 = 0;

    long long lcm = 1;
    for (auto &monkey: monkeys) {
        lcm *= monkey->modulo;
    }
    for (int round = 0; round < 10000; round++) {
        for (auto &monkey: monkeys) {
            while (!monkey->interests.empty()) {
                monkey->inspection_count++;
                v = monkey->interests.front();
                monkey->interests.pop_front();
                v = monkey->op(v, monkey->value);
                v = v % lcm;
                if (v % monkey->modulo == 0) {
                    // divisible
                    monkeys[monkey->m_true]->interests.push_back(v);
                } else {
                    // not divisible
                    monkeys[monkey->m_false]->interests.push_back(v);
                }
            }
        }
    }

    inspections.clear();
    for (auto &_m : monkeys) {
        inspections.push_back(_m->inspection_count);
    }
    sort(inspections.begin(), inspections.end(), [](long long i1, long long i2) { return i1 > i2; });
    answer2 = inspections[0] * inspections[1];

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    for (auto &_m: monkeys) {
        delete _m;
    }

    return EXIT_SUCCESS;
}
