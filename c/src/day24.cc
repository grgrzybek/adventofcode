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
#include <cstdio>
#include <deque>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

#include "utils/utils.h"

using namespace std;

enum op_e {
    INIT, AND, OR, XOR
};

struct logic {
    string name;
    string func_name;
    int arg = -1;
    string s1, s2;
    int value = -1;
    op_e op;
    friend ostream &operator<<(ostream &os, logic &l);
};

void show(map<string, logic> *pMap, string &key, int depth);

ostream &operator<<(ostream &os, logic &l) {
    if (l.op == INIT) {
        os << l.name << ": " << l.value;
        if (!l.func_name.empty()) {
            os << ", f: " << l.func_name;
        }
    } else {
        if (l.func_name.empty()) {
            os << l.name << ": " << l.s1 << (l.op == AND ? " AND " : (l.op == OR ? " OR " : " XOR ")) << l.s2;
            if (l.value != -1) {
                os << ": " << l.value;
            }
        } else {
            os << l.name << " f: " << l.func_name << "(" << l.s1 << ", " << l.s2 << "), arg: " << l.arg;
        }
    }
    return os;
}

int main(int argc, char *argv[]) {
    aoc2024::Options options("Day 24", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    string tok;

    int max_z = 0;

    set<string> signals;
    auto system = new map<string, logic>;

    bool reading_signals = true;
    while (getline(*input, line)) {
        aoc2024::trim(line);
        if (line.empty()) {
            reading_signals = false;
            continue;
        }
        if (reading_signals) {
            string::size_type p1 = line.find(':');
            string name = line.substr(0, p1);
            signals.insert(name);
            (*system)[name] = {
                        .name = name, .s1 = "", .s2 = "", .value = line[line.length() - 1] - '0', .op = INIT
            };
        } else {
            istringstream iss(line);
            logic l;
            iss >> l.s1;
            iss >> tok;
            if (tok == "AND") {
                l.op = AND;
            } else if (tok == "OR") {
                l.op = OR;
            } else if (tok == "XOR") {
                l.op = XOR;
            }
            iss >> l.s2;
            iss >> tok;
            iss >> l.name;
            signals.insert(l.s1);
            signals.insert(l.s2);
            signals.insert(l.name);
            (*system)[l.name] = l;
        }
    }

//    cout << "--- initial state\n";
    for (auto &p: *system) {
//        cout << p.second << endl;
        if (p.second.name[0] == 'z') {
            int z = 0;
            sscanf(p.second.name.c_str(), "z%d", &z);
            if (max_z < z) {
                max_z = z;
            }
        }
    }

    // part 1

    unsigned long answer1 = 0;

    while (true) {
        bool change = false;

        for (auto &p: *system) {
            if (p.second.value == -1) {
                auto &v1 = (*system)[p.second.s1];
                auto &v2 = (*system)[p.second.s2];
                if (v1.value >= 0 && v2.value >= 0) {
                    switch (p.second.op) {
                        case AND:
                            p.second.value = v1.value & v2.value;
                            break;
                        case OR:
                            p.second.value = v1.value | v2.value;
                            break;
                        case XOR:
                            p.second.value = v1.value ^ v2.value;
                            break;
                        default:
                            break;
                    }
                    change = true;
                }
            }
        }

        if (!change) {
            break;
        }
    }

//    cout << "--- part1 state\n";
    for (auto &p: *system) {
//        cout << p.second << endl;
        if (p.first.starts_with("z")) {
            int bit = stoi(p.first.substr(1));
//            cout << p.first << ": " << bit << ", value: " << p.second.value << " -> " << ((long) p.second.value << (long) bit) << endl;
            answer1 |= ((long) p.second.value << (long) bit);
        }
    }

    // part 2

    // bit 0 is easy:
    // 0+0 = 0
    // 0+1 = 1
    // 1+0 = 1
    // 1+1 = 0, carry 1
    //     z00 = x00 XOR y00
    // carry00 = x00 AND y00
    //
    // bit 1:
    // 00+00 = 000
    // 00+01 = 001
    // 00+10 = 010
    // 00+11 = 011
    // 01+00 = 001
    // 01+01 = 010, xor + carry
    // 01+10 = 011
    // 10+00 = 010
    // 10+01 = 011
    // 11+00 = 011
    // ---
    // 01+11 = 100
    // 10+10 = 100
    // 10+11 = 101
    // 11+01 = 100
    // 11+10 = 101
    // 11+11 = 110
    //
    // z00: x00 XOR y00: 0
    //   x00: 1
    //   y00: 1
    //
    // z(N) = aaa XOR bbb
    //   aaa = x(N) XOR y(N)
    //     XOR
    //   bbb = ccc OR ddd = 0
    //     ccc = x(N-1) AND y(N-1) = 0
    //       OR
    //     ddd = eee AND fff = 0
    //       eee = x(N-1) XOR y(N-1) = 0
    //         AND
    //       fff = x(N-2) AND y(N-2) = 0

    /*
    // z01: nmk XOR dsr: 0
    //   nmk: x01 XOR y01: 1
    //     x01: 0
    //     y01: 1
    //   dsr: x00 AND y00: 1
    //     x00: 1
    //     y00: 1
    */

    // z(N) = nmk XOR dsr = "xor(N)" XOR "carry(N-1)"
    //   nmk = x(N) XOR y(N) = "xor(N)"
    //     XOR
    //   dsr = aaa OR bbb = "carry(N-1)" = "and(N-1)" OR "carry_x(N-1)"
    //     aaa = x(N-1) AND y(N-1) = "and(N-1)"
    //       OR
    //     bbb = ccc AND ddd = "carry_x(N-1)"
    //       ccc = x(N-1) XOR y(N-1) = "xor(N-1)"
    //         AND
    //       ddd = x(N-2) AND y(N-2) = "and(N-2)"

    /*
    // z02: skd XOR ssq: 0
    //   skd: hqh OR qrt: 1
    //     hqh: x01 AND y01: 0
    //       x01: 0
    //       y01: 1
    //     qrt: nmk AND dsr: 1
    //       nmk: x01 XOR y01: 1
    //         x01: 0
    //         y01: 1
    //       dsr: x00 AND y00: 1
    //         x00: 1
    //         y00: 1
    //   ssq: x02 XOR y02: 1
    //     x02: 0
    //     y02: 1
    */

    // z(N) = ssq XOR skd = "xor(N)" XOR "carry(N-1)"
    //   ssq = x(N) XOR y(N) = "xor(N)"
    //     XOR
    //   skd = hqh OR qrt = "carry(N-1)" = "and(N-1)" OR "carry_x(N-1)"
    //     hqh = x(N-1) AND y(N-1) = "and(N-1)"
    //       OR
    //     qrt = nmk AND dsr = "carry_x(N-1)"
    //       nmk = x(N-1) XOR y(N-1) = "xor(N-1)"
    //         AND
    //       dsr = x(N-2) AND y(N-2) = "and(N-2)"

    string answer2;

    auto p11 = (*system)["z09"];
    auto p12 = (*system)["nnf"];
    p11.name = "nnf";
    p12.name = "z09";
    (*system)["z09"] = p12;
    (*system)["nnf"] = p11;

    p11 = (*system)["z20"];
    p12 = (*system)["nhs"];
    p11.name = "nhs";
    p12.name = "z20";
    (*system)["z20"] = p12;
    (*system)["nhs"] = p11;

    p11 = (*system)["kqh"];
    p12 = (*system)["ddn"];
    p11.name = "ddn";
    p12.name = "kqh";
    (*system)["kqh"] = p12;
    (*system)["ddn"] = p11;

    p11 = (*system)["wrc"];
    p12 = (*system)["z34"];
    p11.name = "z34";
    p12.name = "wrc";
    (*system)["wrc"] = p12;
    (*system)["z34"] = p11;

    set<string> changes = {
                "z09", "nnf", "z20", "nhs", "kqh", "ddn", "wrc", "z34"
    };
    ostringstream oss;
    for (auto &c: changes) {
        oss << "," << c;
    }
    answer2 = oss.str().substr(1);

    while (true) {
        bool change = false;
        for (auto &p: *system) {
            if (p.second.func_name.empty()) {
//                cout << p.second << " is empty\n";
                // check its function
                if (p.second.name[0] == 'x') {
                    p.second.func_name = "x";
                    sscanf(p.second.name.c_str(), "x%02d", &p.second.arg);
                    change = true;
                }
                if (p.second.name[0] == 'y') {
                    p.second.func_name = "y";
                    sscanf(p.second.name.c_str(), "y%02d", &p.second.arg);
                    change = true;
                }
                if (!p.second.s1.empty() && !p.second.s2.empty()) {
                    if (p.second.s1[0] == 'y' && p.second.s2[0] == 'x') {
                        swap(p.second.s1, p.second.s2);
                    }
                    if (p.second.s1[0] == 'x' && p.second.s2[0] == 'y') {
                        sscanf(p.second.s1.c_str(), "x%02d", &p.second.arg);
                        if (p.second.op == XOR) {
                            p.second.func_name = "xor";
//                            cout << p.second << endl;
                            change = true;
                        }
                        if (p.second.op == AND) {
                            p.second.func_name = "and";
//                            cout << p.second << endl;
                            change = true;
                        }
                    } else {
                        auto a1 = (*system)[p.second.s1];
                        auto a2 = (*system)[p.second.s2];
                        if (!a1.func_name.empty() && !a2.func_name.empty()) {
                            if (p.second.op == XOR) {
                                // one of the args must be xorN, the other is carry(N-1) and we're in z(N)
                                if (a1.func_name == "xor" && a2.func_name == "carry") {
                                    p.second.func_name = "z";
                                    p.second.arg = a1.arg;
                                    change = true;
                                } else if (a2.func_name == "xor" && a1.func_name == "carry") {
                                    p.second.func_name = "z";
                                    p.second.arg = a2.arg;
                                    change = true;
                                }
                            } else if (p.second.op == AND) {
                                // carry_x
                                if (a1.func_name == "xor" && a2.func_name == "and") {
                                    p.second.func_name = "carry_x";
                                    p.second.arg = a1.arg;
                                    change = true;
                                } else if (a2.func_name == "xor" && a1.func_name == "and") {
                                    p.second.func_name = "carry_x";
                                    p.second.arg = a2.arg;
                                    change = true;
                                }
                                if (a1.func_name == "xor" && a2.func_name == "carry") {
                                    p.second.func_name = "carry_x";
                                    p.second.arg = a1.arg;
                                    change = true;
                                } else if (a2.func_name == "xor" && a1.func_name == "carry") {
                                    p.second.func_name = "carry_x";
                                    p.second.arg = a2.arg;
                                    change = true;
                                }
                            } else if (p.second.op == OR) {
                                // carry
                                if (a1.func_name == "and" && a2.func_name == "carry_x") {
                                    p.second.func_name = "carry";
                                    p.second.arg = a1.arg;
                                    change = true;
                                } else if (a2.func_name == "and" && a1.func_name == "carry_x") {
                                    p.second.func_name = "carry";
                                    p.second.arg = a2.arg;
                                    change = true;
                                }
                            }
                        }
                    }
                }
            }
        }

        if (!change) {
            break;
        }
    }

    int processed = 0;
    for (auto &p: *system) {
        if (!p.second.func_name.empty()) {
            processed++;
        }
        cout << p.second << endl;
    }
    cout << "all: " << system->size() << ", processed: " << processed << endl;

    for (int z = 0; z <= max_z; z++) {
        char key[4] = { 'z', 0, 0, 0 };
        sprintf(key + 1, "%02d", z);
        string k = key;
        show(system, k, 0);
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    delete system;

    return EXIT_SUCCESS;
}

void show(map<string, logic> *system, string &key, int depth) {
    for (int i = 0; i < depth; i++) {
        cout << " ";
    }
    cout << (*system)[key] << endl;
    if (!(*system)[key].s1.empty()) {
        show(system, (*system)[key].s1, depth + 2);
    }
    if (!(*system)[key].s2.empty()) {
        show(system, (*system)[key].s2, depth + 2);
    }
}
