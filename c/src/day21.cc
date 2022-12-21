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
#include <iostream>
#include <list>
#include <map>
#include <string>

#include "utils/utils.h"

using namespace std;

struct monkey {
    string name;
    bool has_value = false;
    long value;
    char operation;
    string parent;
    string monkey1;
    string monkey2;

    int where_is_human = 0; // 1: left, 2: right

    void print() const {
        cout << name << ": ";
        if (has_value) {
            cout << value;
        } else {
            cout << monkey1 << operation << monkey2;
        }
    }
};

void evaluate(monkey *m, map<string, monkey *> &herd) { // NOLINT(misc-no-recursion)
    if (m->monkey1.empty()) {
        return;
    }
    monkey *m1 = herd[m->monkey1];
    monkey *m2 = herd[m->monkey2];
    evaluate(m1, herd);
    evaluate(m2, herd);

    switch (m->operation) {
        case '+':
            m->value = m1->value + m2->value;
//            cout << m1->value << "+" << m2->value << " = " << m->value << "\n";
            break;
        case '-':
            m->value = m1->value - m2->value;
//            cout << m1->value << "-" << m2->value << " = " << m->value << "\n";
            break;
        case '*':
            m->value = m1->value * m2->value;
//            cout << m1->value << "*" << m2->value << " = " << m->value << "\n";
            break;
        case '/':
            if (m1->value % m2->value != 0) {
                cout << "!!!\n";
            }
            m->value = m1->value / m2->value;
//            cout << m1->value << "/" << m2->value << " = " << m->value << "\n";
            break;
        default:
            break;
    }
}

void set_parent(monkey *m, map<string, monkey *> &herd) { // NOLINT(misc-no-recursion)
    if (m->has_value) {
        return;
    }
    monkey *m1 = herd[m->monkey1];
    monkey *m2 = herd[m->monkey2];
    m1->parent = m->name;
    m2->parent = m->name;
    set_parent(m1, herd);
    set_parent(m2, herd);
}

int main(int argc, char *argv[]) {
    aoc2022::Options options("Day 21", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    list<monkey> monkeys;

    while (getline(*input, line)) {
        aoc2022::trim(line);
        if (!aoc2022::has_text(line)) {
            continue;
        }

        monkey m;
        string::size_type pos = line.find(':');
        m.name = line.substr(0, pos);
        pos += 2;
        if (line.find(' ', pos) == string::npos) {
            // a number
            m.value = stol(line.substr(pos));
            m.has_value = true;
        } else {
            string::size_type pos2 = line.find(' ', pos);
            m.monkey1 = line.substr(pos, pos2 - pos);
            pos += m.monkey1.length() + 1;
            m.operation = line[pos];
            pos += 2;
            m.monkey2 = line.substr(pos);
        }

        monkeys.emplace_back(std::move(m));
    }

    map<string, monkey *> herd;
    for (auto &m: monkeys) {
        herd[m.name] = &m;
    }

    // part 1

    auto m = herd["root"];
    evaluate(m, herd);

    long answer1 = m->value;

    // part 2

    set_parent(m, herd);

    auto h = herd["humn"];
    while (h->name != "root") {
        auto p = herd[h->parent];
        if (p->monkey1 == h->name) {
            p->where_is_human = 1;
        } else {
            p->where_is_human = 2;
        }
        h = p;
    }

    long answer2 = m->value;

    m = herd["root"];
    evaluate(m, herd);
    long changed_value = 0L;
    while (true) {
        if (m->name == "humn") {
            m->value = changed_value;
            answer2 = changed_value;
            break;
        }
        long v1 = herd[m->monkey1]->value;
        long v2 = herd[m->monkey2]->value;
        if (m->name == "root") {
            // easy
            changed_value = m->where_is_human == 1 ? v2 : v1;
            cout << "the expression for " << m->name << ": " << v1 << " " << m->operation << " " << v2 << "\n";
            cout << "changing value of " << (m->where_is_human == 1 ? "v1" : "v2")
                 << " (" << (m->where_is_human == 1 ? herd[m->monkey1]->name : herd[m->monkey2]->name) << ") from "
                 << (m->where_is_human == 1 ? v1 : v2) << " to " << changed_value << "\n\n";
        } else {
            // recalculate changed_value based on the operation
            switch (m->operation) {
                case '+':
                    changed_value = m->where_is_human == 1 ? changed_value - v2 : changed_value - v1;
                    break;
                case '-':
                    changed_value = m->where_is_human == 1 ? v2 + changed_value : v1 - changed_value;
                    break;
                case '*':
                    changed_value = m->where_is_human == 1 ? changed_value / v2 : changed_value / v1;
                    break;
                case '/':
                    changed_value = m->where_is_human == 1 ? changed_value * v2 : changed_value * v1;
                    break;
                default:
                    break;
            }
            cout << "the expression for " << m->name << ": " << v1 << " " << m->operation << " " << v2 << "\n";
            cout << "changing value of " << (m->where_is_human == 1 ? "v1" : "v2")
                 << " (" << (m->where_is_human == 1 ? herd[m->monkey1]->name : herd[m->monkey2]->name) << ") from "
                 << (m->where_is_human == 1 ? v1 : v2) << " to " << changed_value << "\n\n";
        }
        if (m->where_is_human == 1) {
            // left
            m = herd[m->monkey1];
        } else {
            // right
            m = herd[m->monkey2];
        }
    }

    m = herd["root"];
    evaluate(m, herd);

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}
