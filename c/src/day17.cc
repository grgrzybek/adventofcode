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
#include <iomanip>

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
    adv = 0, // A >> combo -> A
    bxl = 1, // B XOR literal -> B
    bst = 2, // combo % 8 -> B
    jnz = 3, // [A] = 0: nothing, else: jump to literal
    bxc = 4, // B XOR C -> B (ignores the argument)
    out = 5, // output "combo % 8"
    bdv = 6, // A >> combo -> B
    cdv = 7  // A >> combo -> C
};
// start:
//   bst 2,4, | b = a % 8 - take last base(8) digit
//   bxl 1,1, | b = b ^ 1 - 000->001, 001->000, 010->011, 011->010
//                          100->101, 101->100, 110->111, 111->110
//   cdv 7,5, | c = a >> b - 0>>1, 1>>0, 2>>3, 3>>2
//                           4>>5, 5>>4, 6>>7, 7>>6
//   bxl 1,4, | b = b ^ 4
//   adv 0,3, | a = a >> 3
//   bxc 4,5, | b = b ^ c
//   out 5,5, | out(b % 8)
//   jnz 3,0, | if (a != 0) goto start

// start:
//   0,1, | a = a >> 1
//   5,4, | out(a % 8)
//   3,0  | if (a != 0) goto start

// start:
//   0,3, | a = a >> 3
//   5,4, | out(a % 8)
//   3,0  | if (a != 0) goto start

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
    vector<long> prog_linear;

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
                prog_linear.push_back(l1);
                prog_linear.push_back(l2);
                if (p2 == string::npos) {
                    break;
                }
                p1 = p2 + 1;
            }
        }
    }

    // part 1

    string answer1;

    int ip = 0;
    vector<long> output;

//    cout << "--- running forward ---\n";
    while (ip < (int) prog.size()) {
//        cout << setbase(8) << "A: " << *registers[4] << ", B: " << *registers[5] << ", C: " << *registers[6] << endl;
        auto &ins = prog[ip];
        long arg = ins.second;
//        cout << "checking " << ins.first << " (" << arg << ") [ip: " << ip << "]\n";
        switch (ins.first) {
            case adv:
//                cout << " [0:adv]   a=" << a << " >> " << (arg < 4 ? arg : *registers[arg]) << " -> a=";
                a = a >> (arg < 4 ? arg : *registers[arg]);
//                cout << a << endl;
                break;
            case bxl:
//                cout << " [1:bxl]   b=" << b << " ^ " << arg << " -> b=";
                b = b ^ arg;
//                cout << b << endl;
                break;
            case bst:
//                if (arg < 4) {
//                    cout << " [2:bst]   '" << arg << "' % 8 = " << (arg % 8) << " -> b=";
//                } else {
//                    cout << " [2:bst]   " << (char)('A' + arg - 4) << "=" << *registers[arg] << " % 8 = " << *registers[arg] % 8 << " -> b=";
//                }
                b = (arg < 4 ? arg : *registers[arg]) % 8;
//                cout << b << endl;
                break;
            case jnz:
//                cout << " [3:jnz]   jnz=" << (a != 0) << endl;
                ip = (a == 0 ? ip+1 : (int) arg);
//                if (a != 0) {
//                    cout << endl;
//                }
                break;
            case bxc:
//                cout << " [4:bxc]   b=" << b << " ^ c=" << *registers[6] << " -> b=";
                b = b ^ c;
//                cout << b << endl;
                break;
            case out:
//                cout << " [5:out]   out(" << (arg < 4 ? arg : *registers[arg]) % 8 << ")" << endl;
                output.push_back((arg < 4 ? arg : *registers[arg]) % 8);
                break;
            case bdv:
//                cout << " [6:bdv]   a=" << a << " >> " << (arg < 4 ? arg : *registers[arg]) << " -> b=";
                b = a >> (arg < 4 ? arg : *registers[arg]);
//                cout << b << endl;
                break;
            case cdv:
//                cout << " [7:cdv]   a=" << a << " >> " << (arg < 4 ? arg : *registers[arg]) << " -> c=";
                c = a >> (arg < 4 ? arg : *registers[arg]);
//                cout << c << endl;
                break;
            default:
                break;
        }
        if (ins.first != jnz) {
            ip++;
        }
    }

    // part 2

    ip = -1;
    int outp = (int) prog_linear.size() - 1;

    auto answers2 = new vector<long>;
    answers2->push_back(0L);

    // checking expected output backwards. the "a" value that produces the last element
    // will be shifted (by 3 bits) left, because it doesn't have anything on its left, but impacts
    // the next (right) 3 bits
    while (outp >= 0) {
        cout << "finding input to match prog[" << outp << "] = " << prog_linear[outp] << endl;

        auto answers2_verified = new vector<long>;
        int an = 1;
        for (auto &answer2: *answers2) {
            cout << "--- starting with a=" << answer2 << " (" << (an++) << " of " << answers2->size() << " potential \"a\"s)\n";
            for (long i = 0; i < 8; i++) {
                *registers[4] = (answer2 << 3) + i;
                cout << "    checking with A=" << *registers[4] << endl;
                *registers[5] = 0;
                *registers[6] = 0;

                vector<int> outs;
                ip = 0;
                while (ip < (int) prog.size()) {
    //                cout << setbase(8) << "A: " << *registers[4] << ", B: " << *registers[5] << ", C: " << *registers[6] << endl;
                    auto &ins = prog[ip];
                    long arg = ins.second;
            //        cout << "checking " << ins.first << " (" << arg << ") [ip: " << ip << "]\n";
                    switch (ins.first) {
                        case adv:
    //                        cout << " [0:adv]   a=" << a << " >> " << (arg < 4 ? arg : *registers[arg]) << " -> a=";
                            a = a >> (arg < 4 ? arg : *registers[arg]);
    //                        cout << a << endl;
                            break;
                        case bxl:
    //                        cout << " [1:bxl]   b=" << b << " ^ " << arg << " -> b=";
                            b = b ^ arg;
    //                        cout << b << endl;
                            break;
                        case bst:
    //                        if (arg < 4) {
    //                            cout << " [2:bst]   '" << arg << "' % 8 = " << (arg % 8) << " -> b=";
    //                        } else {
    //                            cout << " [2:bst]   " << (char)('A' + arg - 4) << "=" << *registers[arg] << " % 8 = " << *registers[arg] % 8 << " -> b=";
    //                        }
                            b = (arg < 4 ? arg : *registers[arg]) % 8;
    //                        cout << b << endl;
                            break;
                        case jnz:
    //                        cout << " [3:jnz]   jnz=" << (a != 0) << endl;
                            ip = (a == 0 ? ip+1 : (int) arg);
    //                        if (a != 0) {
    //                            cout << endl;
    //                        }
                            break;
                        case bxc:
    //                        cout << " [4:bxc]   b=" << b << " ^ c=" << *registers[6] << " -> b=";
                            b = b ^ c;
    //                        cout << b << endl;
                            break;
                        case out:
//                            cout << " [5:out]   out(" << (arg < 4 ? arg : *registers[arg]) % 8 << ")" << endl;
                            outs.push_back((long) (arg < 4 ? arg : *registers[arg]) % 8);
                            break;
                        case bdv:
    //                        cout << " [6:bdv]   a=" << a << " >> " << (arg < 4 ? arg : *registers[arg]) << " -> b=";
                            b = a >> (arg < 4 ? arg : *registers[arg]);
    //                        cout << b << endl;
                            break;
                        case cdv:
    //                        cout << " [7:cdv]   a=" << a << " >> " << (arg < 4 ? arg : *registers[arg]) << " -> c=";
                            c = a >> (arg < 4 ? arg : *registers[arg]);
    //                        cout << c << endl;
                            break;
                        default:
                            break;
                    }
                    if (ins.first != jnz) {
                        ip++;
                    }
                }

                // program completed for given "a", produced output - let's check it

                bool match = true;
                cout << "        output: ";
                for (int j = 0; j < (int) outs.size(); j++) {
                    if (j > 0) {
                        cout << ",";
                    }
                    cout << outs[j];
                    if (outs[j] != prog_linear[outp + j]) {
                        match = false;
                    }
                }
                cout << endl;
                if (match) {
                    cout << "for expected digit '" << prog_linear[outp] << "', the digit is " << i << " (answer2: " << answer2 << ")" <<endl;
                    // this will become new answer2 to start with.
                    if ((answer2 << 3) + i < 0) {
                        cout << "???\n";
                    }
                    answers2_verified->push_back((answer2 << 3) + i);
                    cout << "    now answer2: " << setbase(10) << ((answer2 << 3) + i) << " = octal " << setbase(8) << ((answer2 << 3) + i) << setbase(10) << endl;
                }
            }
        }

        // all "a" checked, let's swap answers2 and answers2_verified
        answers2->clear();
        answers2->assign(answers2_verified->begin(), answers2_verified->end());
        delete answers2_verified;

        outp--;
    }

//    cout << endl;
//    cout << "linear program: ";
//    for (auto &e: prog_linear) {
//        cout << " " << e;
//    }
//    cout << endl;
//    cout << endl;
//    while (true) {
//        if (ip == -1) {
//            ip = (int) prog.size() - 2;
//            outp--;
//            cout << endl;
//        }
//        if (outp == -1) {
//            break;
//        }
//        auto &ins = prog[ip];
//        long current_output = prog_linear[outp];
//
////        cout << "checking " << ins.first << " (" << arg << ") [ip: " << ip << "]\n";
//        cout << "leading to " << setbase(10) << current_output << " [ip: " << ip << ", outp: " << outp << "]\n";
//        cout << "    A: " << setbase(8) << *registers[4] << ", B: " << *registers[5] << ", C: " << *registers[6] << endl;
//
//        long op = ins.first;
//        long arg = ins.second;
//
//        switch (op) {
//            case adv:
//                // multiply instead of divide (shift left instead of shift right)
//                cout << " [0:adv]   a=" << a << " << " << (arg < 4 ? arg : *registers[arg]) << " -> a=";
//                a <<= (arg < 4 ? arg : *registers[arg]);
//                cout << a << endl;
//                break;
//            case bxl:
//                // xor is reversible
//                cout << " [1:bxl]   b=" << b << " ^ " << arg << " -> b=";
//                b = b ^ arg;
//                cout << b << endl;
//                break;
//            case bst:
//                // noop?
//                if (arg < 4) {
//                    cout << " [2:bst]   '" << arg << "' % 8 = " << (arg % 8) << " -> b=";
//                } else {
//                    cout << " [2:bst]   " << (char)('A' + arg - 4) << "=" << *registers[arg] << " % 8 = " << *registers[arg] % 8 << " -> b=";
//                }
//                b = (arg < 4 ? arg : *registers[arg]) % 8;
//                cout << b << endl;
//                break;
//            case jnz:
//                // noop in part 2
//                break;
//            case bxc:
//                // xor is reversible
//                cout << " [4:bxc]   b=" << b << " ^ c=" << *registers[6] << " -> b=";
//                b = b ^ c;
//                cout << b << endl;
//                break;
//            case out:
//                // we should lead to 3 bits of combo containing current output - without touching other bits!
//                if (arg >= 4) {
//                    *registers[arg] |= current_output;
//                }
//                cout << " [5:out]   out(" << (arg < 4 ? arg : *registers[arg]) % 8 << ")" << endl;
//                break;
//            case bdv:
//                // multiply instead of divide (shift left instead of shift right)
//                cout << " [6:bdv]   a=" << a << " << " << (arg < 4 ? arg : *registers[arg]) << " -> b=";
//                b = a << (arg < 4 ? arg : *registers[arg]);
//                cout << b << endl;
//                break;
//            case cdv:
//                // multiply instead of divide (shift left instead of shift right)
//                cout << " [7:cdv]   a=" << a << " << " << (arg < 4 ? arg : *registers[arg]) << " -> c=";
//                c = a << (arg < 4 ? arg : *registers[arg]);
//                cout << c << endl;
//                break;
//            default:
//                break;
//        }
//
//        if (op != jnz) {
//            ip--;
//        }
//    }
//
//    long answer2 = *registers[4];

    ostringstream oss;
    for (auto &o: output) {
        oss << o << ",";
    }
    answer1 = oss.str().substr(0, oss.str().length() - 1);

    sort(answers2->begin(), answers2->end());
    long answer2 = *answers2->begin();

    cout << setbase(10);
    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    delete answers2;

    return EXIT_SUCCESS;
}
