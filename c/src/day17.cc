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
#include <deque>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

/*
 * Literal operand - literal value
 *
 * Combo operand 0 through 3 represent literal values 0 through 3.
 * Combo operand 4 represents the value of register A.
 * Combo operand 5 represents the value of register B.
 * Combo operand 6 represents the value of register C.
 * Combo operand 7 is reserved and will not appear in valid programs.
 */
enum inst {
    adv = 0, // A / 2^combo -> A
    bxl = 1, // B XOR literal -> B
    bst = 2, // B % 8 -> B
    jnz = 3, // [A] = 0: nothing, else: jump to literal
    bxc = 4, // B XOR C -> B (ignores the argument)
    out = 5, // combo % 8 and output
    bdv = 6, // A / 2^combo -> B
    cdv = 7  // A / 2^combo -> C
};

int main(int argc, char *argv[]) {
    aoc2024::Options options("Day 17", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    string tok;

    long a = 0L, b = 0L, c = 0L;
    long *registers[] = { nullptr, nullptr, nullptr, nullptr, &a, &b, &c };
    vector<pair<long, long>> prog;

    while (getline(*input, line)) {
        aoc2024::trim(line);
        if (line.empty()) {
            continue;
        }
        if (line.starts_with("Register A: ")) {
            a = stol(line.substr(12));
        } else if (line.starts_with("Register B: ")) {
            b = stol(line.substr(12));
        } else if (line.starts_with("Register C: ")) {
            c = stol(line.substr(12));
        } else if (line.starts_with("Program: ")) {
            string l = line.substr(9);
            string::size_type p1 = 0;
            string::size_type p2;
            while (true) {
                p2 = l.find(',', p1);
                long l1 = stol(l.substr(p1, p2 - p1));
                p1 = p2;
                p2 = l.find(',', p2 + 1);
                long l2;
                if (p2 == string::npos) {
                    l2 = stol(l.substr(p1 + 1));
                } else {
                    l2 = stol(l.substr(p1 + 1, p2 - p1));
                }
                prog.emplace_back(l1, l2);
                cout << "l2: " << l2 << endl;
                if (p2 == string::npos) {
                    break;
                }
                p1 = p2 + 1;
            }
        }
    }

//    for (auto &p: prog) {
//        cout << p.first << "(" << p.second << ")" << endl;
//    }

    // part 1

    long answer1 = 0;

    size_t ip = 0;
    vector<long> output;

    while (ip < prog.size()) {
        auto &ins = prog[ip];
        long arg = ins.second;
        cout << "checking " << ins.first << " (" << arg << ") [ip: " << ip << "]\n";
        switch (ins.first) {
            case adv:
                a = a / (1 << (arg < 4 ? arg : *registers[arg]));
                break;
            case bxl:
                b = b ^ arg;
                break;
            case bst:
                b = (arg < 4 ? arg : *registers[arg]) % 8;
                break;
            case jnz:
                ip = a == 0 ? ip+1 : arg;
                break;
            case bxc:
                b = b ^ c;
                break;
            case out:
                output.push_back((arg < 4 ? arg : *registers[arg]) % 8);
                break;
            case bdv:
                b = a / (1 << (arg < 4 ? arg : *registers[arg]));
                break;
            case cdv:
                c = a / (1 << (arg < 4 ? arg : *registers[arg]));
                break;
            default:
                break;
        }
        if (ins.first != jnz) {
            ip++;
        }
    }

    for (auto &o: output) {
        cout << o << ",";
    }
    cout << endl;

    // part 2

    long answer2 = 0;

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}
