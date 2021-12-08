/*
 * Copyright 2021 Grzegorz Grzybek
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
#include <fstream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;
using namespace aoc2021;

int main(int argc, char *argv[]) {
    Options options("Day 08", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<vector<string> *> patterns10;
    vector<vector<string> *> patterns4;

    while (getline(*input, line)) {
        trim(line);
        istringstream iss(line);
        vector<string> *p10 = new vector<string>();
        patterns10.push_back(p10);
        vector<string> *p4 = new vector<string>();
        patterns4.push_back(p4);

        string v;
        for (int i = 0; i < 10; i++) {
            iss >> v;
            sort(v.begin(), v.end());
            p10->push_back(v);
        }
        char pipe;
        iss >> pipe;
        for (int i = 0; i < 4; i++) {
            iss >> v;
            sort(v.begin(), v.end());
            p4->push_back(v);
        }
    }

    // part 1

    int answer1 = 0;

    for (auto *&p4: patterns4) {
        for (auto &s: *p4) {
            if (s.length() == 2 || s.length() == 3 || s.length() == 4 || s.length() == 7) {
                answer1++;
            }
        }
    }

    // part 2

    int answer2 = 0;

    int set = 0;
    for (vector<string> *&p10: patterns10) {
        string s[10];
        vector<string> s235;
        vector<string> s069;

        for (string &_s: *p10) {
            if (_s.length() == 2) {
                s[1] = _s;
            }
            if (_s.length() == 3) {
                s[7] = _s;
            }
            if (_s.length() == 4) {
                s[4] = _s;
            }
            if (_s.length() == 5) {
                // "2", "3", "5"
                s235.push_back(_s);
            }
            if (_s.length() == 6) {
                // "0", "6", "9"
                s069.push_back(_s);
            }
            if (_s.length() == 7) {
                s[8] = _s;
            }
        }

        char a, b = '.', c = '.', d = '.', e = '.', f = '.', g = '.';

        /*
         *  aaa           aaa    aaa           aaa    aaa    aaa    aaa    aaa
         * b   c      c      c      c  b   c  b      b          c  b   c  b   c
         * b   c      c      c      c  b   c  b      b          c  b   c  b   c
         *                ddd    ddd    ddd    ddd    ddd           ddd    ddd
         * e   f      f  e          f      f      f  e   f      f  e   f      f
         * e   f      f  e          f      f      f  e   f      f  e   f      f
         *  ggg           ggg    ggg           ggg    ggg           ggg    ggg
         */

        // for example, segment "a" is the one from number "7" which is not available in number "1"
        string _s7(s[7]);
        remove_if(_s7.begin(), _s7.end(), [s](char c) { // NOLINT(bugprone-unused-return-value)
            return s[1].find(c) != string::npos;
        });
        a = *_s7.begin();

        // we have segment a
        // we have 1, 4, 7, 8

        // 4 and 7 can be used to find 9 and segment g
        for (auto &_s: s069) {
            string s8t(_s);
            string is9 = string(s8t.begin(),
                    remove_if(s8t.begin(), s8t.end(),
                            [s](char c) {
                                return s[4].find(c) != string::npos || s[7].find(c) != string::npos;
                            }));
            if (is9.length() == 1) {
                g = *is9.begin();
                s[9] = _s;
                break;
            }
        }

        // we have segment a, g
        // we have 1, 4, 7, 8, 9

        // 9 and 8 can be used to determine segment e
        string _s8(s[8]);
        e = *string(_s8.begin(),
                remove_if(_s8.begin(), _s8.end(), [s](char c) {
                    return s[9].find(c) != string::npos;
                })).begin();

        // we have segment a, e, g
        // we have 1, 4, 7, 8, 9

        // 8 and segment e can be used to find 2
        for (auto &_s: s235) {
            string s8t(s[8]);
            string is2 = string(s8t.begin(),
                    remove_if(s8t.begin(), s8t.end(), [_s, e](char c) {
                        return _s.find(c) != string::npos || c == e;
                    }));
            if (is2.length() == 2) {
                s[2] = _s;
                break;
            }
        }

        // we have segment a, e, g
        // we have 1, 2, 4, 7, 8, 9

        // 2 and 7 can be used to find segment d
        string _s2(s[2]);
        d = *string(_s2.begin(), remove_if(_s2.begin(), _s2.end(), [s, e, g](char c) {
            return s[7].find(c) != string::npos || c == e || c == g;
        })).begin();

        // we have segment a, d, e, g
        // we have 1, 2, 4, 7, 8, 9

        // 8 and segment d can be used to find 0
        for (auto &_s: s069) {
            string s8t(s[8]);
            string is0 = string(s8t.begin(),
                    remove_if(s8t.begin(), s8t.end(), [_s](char c) {
                        return _s.find(c) != string::npos;
                    }));
            if (is0.length() == 1 && *is0.begin() == d) {
                s[0] = _s;
                break;
            }
        }

        // we have segment a, d, e, g
        // we have 0, 1, 2, 4, 7, 8, 9

        for (auto &_s: s069) {
            if (_s == s[0] || _s == s[9]) {
                continue;
            }
            s[6] = _s;
            break;
        }

        // we have segment a, d, e, g
        // we have 0, 1, 2, 4, 6, 7, 8, 9

        for (auto &_s: s235) {
            if (_s == s[2]) {
                continue;
            }
            string _s6(s[6]);
            string is5 = string(_s6.begin(),
                    remove_if(_s6.begin(), _s6.end(), [_s](char c) {
                        return _s.find(c) != string::npos;
                    }));
            if (is5.length() == 1) {
                s[5] = _s;
                break;
            }
        }

        // we have segment a, d, e, g
        // we have 0, 1, 2, 4, 5, 6, 7, 8, 9

        for (auto &_s: s235) {
            if (_s == s[2] || _s == s[5]) {
                continue;
            }
            s[3] = _s;
            break;
        }

        // we have segment a, d, e, g
        // we have 0, 1, 2, 3, 4, 5, 6, 7, 8, 9

//        cout << " - abcdefg:\n   |||||||\n   " << a << b << c << d << e << f << g << endl;
//        cout << " - abcdefg: " << a << " " << b << " " << c << " " << d << " " << e << " " << f << " " << g << endl;
//        cout << " - 0: " << s[0] << ", 1: " << s[1] << ", 2: " << s[2] << ", 3: " << s[3] << ", 4: " << s[4] << ", 5: "
//             << s[5]
//             << ", 6: " << s[6] << ", 7: " << s[7] << ", 8: " << s[8] << ", 9: " << s[9] << endl;

        int mult = 1000;
        int reading = 0;
        vector<string> *&check = patterns4[set];
        for (auto &v: *check) {
            cout << v << " ";
        }
        cout << ": ";

        for (auto &v: *check) {
            for (int i = 0; i < 10; i++) {
                if (v == s[i + 1]) {
                    reading += (i + 1) * mult;
                }
            }
            mult /= 10;
        }
        cout << reading << endl;
        answer2 += reading;

        set++;
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    for (auto *&ptr: patterns10) {
        delete ptr;
    }
    for (auto *&ptr: patterns4) {
        delete ptr;
    }

    return EXIT_SUCCESS;
}
