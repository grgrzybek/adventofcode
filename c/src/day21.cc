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
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

#include "utils/utils.h"

using namespace std;

string find_num_instructions(string &pattern, map<char, pair<int, int>> &pad);

void transform(map<string, string> &cache, map<string, long> &counts, map<char, pair<int, int>> &pad);
string find_dir_instructions(string &pattern, map<char, pair<int, int>> &pad);

int main(int argc, char *argv[]) {
    aoc2024::Options options("Day 21", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;

    vector<string> codes;

    while (getline(*input, line)) {
        aoc2024::trim(line);
        if (line.empty()) {
            break;
        }
        codes.push_back(line);
    }

//    for (auto &code: codes) {
//        cout << code << endl;
//    }

    map<char, pair<int, int>> numpad;
    numpad['7'] = make_pair(0, 0);
    numpad['8'] = make_pair(1, 0);
    numpad['9'] = make_pair(2, 0);
    numpad['4'] = make_pair(0, 1);
    numpad['5'] = make_pair(1, 1);
    numpad['6'] = make_pair(2, 1);
    numpad['1'] = make_pair(0, 2);
    numpad['2'] = make_pair(1, 2);
    numpad['3'] = make_pair(2, 2);
    numpad['0'] = make_pair(1, 3);
    numpad['A'] = make_pair(2, 3);

    map<char, pair<int, int>> dirpad;
    dirpad['^'] = make_pair(1, 0);
    dirpad['A'] = make_pair(2, 0);
    dirpad['<'] = make_pair(0, 1);
    dirpad['v'] = make_pair(1, 1);
    dirpad['>'] = make_pair(2, 1);

    // part 1

    long answer1 = 0;

    for (auto &code: codes) {
        string pattern;
        cout << code << ":\n";
        pattern = find_num_instructions(code, numpad);
        cout << "  num1: " << pattern << "\n";
        pattern = find_dir_instructions(pattern, dirpad);
        cout << "  dir1: " << pattern << "\n";
        pattern = find_dir_instructions(pattern, dirpad);
        cout << "  dir2: " << pattern << "\n";

        cout << "    code: " << stoi(code.substr(0, 3)) << endl;
        cout << "    length: " << pattern.length() << endl;
        answer1 += ((long) pattern.length() * stoi(code.substr(0, 3)));
    }

    // part 2

    long answer2 = 0;

    for (auto &code: codes) {
        cout << "checking code: " << code << endl;
        string pattern;
        pattern = find_num_instructions(code, numpad);
        map<string, string> cache;
        map<string, long> counts;
        counts[find_dir_instructions(pattern, dirpad)] = 1L;

        for (int i = 0; i < 24; i++) {
            transform(cache, counts, dirpad);
        }
        long tc = 0;
        for (auto &p: counts) {
            cout << "  " << p.first << ": " << p.second << endl;
            cout << "    " << p.first.length() << " x " << p.second << endl;
            tc += (p.second * (long) p.first.length());
        }
//        for (auto &p: cache) {
//            cout << "  " << p.first << " -> " << p.second << endl;
//        }
        answer2 += ((long) tc * stoi(code.substr(0, 3)));
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}

string find_num_instructions(string &pattern, map<char, pair<int, int>> &pad) {
    int x = 2;
    int y = 3;

    string result;

    for (char c: pattern) {
        auto p = pad[c];
        if (y < 3 || (y == 3 && p.first > 0)) {
            // safely move left
            while (x > p.first) {
                result += '<';
                x--;
            }
        }
        if (x > 0 || (x == 0 && p.second < 3)) {
            // safely move down
            while (y < p.second) {
                result += 'v';
                y++;
            }
        }
        while (y > p.second) {
            result += '^';
            y--;
        }
        while (x > p.first) {
            result += '<';
            x--;
        }
        while (x < p.first) {
            result += '>';
            x++;
        }
        while (y < p.second) {
            result += 'v';
            y++;
        }
        result += 'A';
    }

    return result;
}

string find_dir_instructions(string &pattern, map<char, pair<int, int>> &pad) {
    int x = 2;
    int y = 0;

    string result;

//    cout << "  checkking \"" << pattern << "\"\n";
    for (char c: pattern) {
        auto p = pad[c];
        if (y == 1 || p.first > 0) {
            while (x > p.first) {
                result += '<';
                x--;
            }
        }
        if (x > 0) {
            while (y > p.second) {
                result += '^';
                y--;
            }
        }
        while (y < p.second) {
            result += 'v';
            y++;
        }
        while (x > p.first) {
            result += '<';
            x--;
        }
        while (x < p.first) {
            result += '>';
            x++;
        }
        while (y > p.second) {
            result += '^';
            y--;
        }
        result += 'A';
////        cout << "    checking '" << c << "': ";
//        if (p.second == y) {
//            // the same row
//            while (x > p.first) {
////                cout << '<';
//                result += '<';
//                x--;
//            }
//            while (x < p.first) {
////                cout << '>';
//                result += '>';
//                x++;
//            }
////            cout << 'A';
//            result += 'A';
//        } else if (p.first == x) {
//            // the same column
//            while (y < p.second) {
////                cout << 'v';
//                result += 'v';
//                y++;
//            }
//            while (y > p.second) {
////                cout << '^';
//                result += '^';
//                y--;
//            }
////            cout << 'A';
//            result += 'A';
//        } else {
//            // optimize for next robot
//            if (x == 0 && y == 1) {
//                // we're over '<' and we have to move right first
//                while (x < p.first) {
////                    cout << '>';
//                    result += '>';
//                    x++;
//                }
//                while (y < p.second) {
////                    cout << 'v';
//                    result += 'v';
//                    y++;
//                }
//                while (y > p.second) {
////                    cout << '^';
//                    result += '^';
//                    y--;
//                }
////                cout << 'A';
//                result += 'A';
//            } else if (p.first == 0 && p.second == 1) {
//                // we're moving to '<', so we have to move down first
//                while (y < p.second) {
////                    cout << 'v';
//                    result += 'v';
//                    y++;
//                }
//                while (x > p.first) {
////                    cout << '<';
//                    result += '<';
//                    x--;
//                }
//                while (x < p.first) {
////                    cout << '>';
//                    result += '>';
//                    x++;
//                }
////                cout << 'A';
//                result += 'A';
//            } else {
//                // we're moving within ^, V, > and A
//                while (y < p.second) {
////                    cout << 'v';
//                    result += 'v';
//                    y++;
//                }
//                while (y > p.second) {
////                    cout << '^';
//                    result += '^';
//                    y--;
//                }
//                while (x > p.first) {
////                    cout << '<';
//                    result += '<';
//                    x--;
//                }
//                while (x < p.first) {
////                    cout << '>';
//                    result += '>';
//                    x++;
//                }
//                result += 'A';
//            }
//        }
////        cout << endl;
    }

    return result;
}

void transform(map<string, string> &cache, map<string, long> &counts, map<char, pair<int, int>> &pad) {
    map<string, long> new_counts;
    for (auto &p: counts) {
        vector<string> subpatterns;
        string::size_type p1 = 0;
        string::size_type p2;
        cout << "  splitting \"" << p.first << "\" into:\n";
        while (p1 < p.first.length()) {
            p2 = p.first.find('A', p1) + 1;
            subpatterns.push_back(p.first.substr(p1, p2 - p1));
            p1 = p2;
        }
        for (auto &sp: subpatterns) {
            cout << "    " << sp << " -> ";
            if (!cache.contains(sp)) {
                string t = find_dir_instructions(sp, pad);
                cache[sp] = t;
            }
            cout << cache[sp] << endl;
            new_counts[cache[sp]] += p.second;
        }
    }
    counts.clear();
    counts.insert(new_counts.begin(), new_counts.end());
}
