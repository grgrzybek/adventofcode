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

struct hand {
    string cards;
    ostringstream cards_sortable1;
    ostringstream cards_sortable2;
    unsigned long bid = 0L;
    // 2, 3, 4, 5, 6, 7, 8, 9, T/10, J, Q, K, A
    u_int8_t counts[13] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    u_int8_t counts2[13] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    unsigned int two = 0;
    unsigned int three = 0;
    unsigned int four = 0;
    unsigned int five = 0;
    unsigned int two_2 = 0;
    unsigned int three_2 = 0;
    unsigned int four_2 = 0;
    unsigned int five_2 = 0;

    bool compare1(const hand *other) const;
    bool compare2(const hand *other) const;

    friend ostream &operator<<(ostream &os, const hand &h);

    void calculate();
};

ostream &operator<<(ostream &os, const hand &h) {
    os << h.cards << ": ";
    os.width(4);
    os << h.bid;
    os << " 2:" << (int) h.counts[0];
    os << " 3:" << (int) h.counts[1];
    os << " 4:" << (int) h.counts[2];
    os << " 5:" << (int) h.counts[3];
    os << " 6:" << (int) h.counts[4];
    os << " 7:" << (int) h.counts[5];
    os << " 8:" << (int) h.counts[6];
    os << " 9:" << (int) h.counts[7];
    os << " T:" << (int) h.counts[8];
    os << " J:" << (int) h.counts[9];
    os << " Q:" << (int) h.counts[10];
    os << " K:" << (int) h.counts[11];
    os << " A:" << (int) h.counts[12];
    return os;
}

bool hand::compare1(const hand *other) const {
    cout << "    comparing:" << endl;
    cout << "     - h1: " << *this << endl;
    cout << "     - h2: " << *other << endl;

    bool same = false;
    if (this->five == 1 && other->five == 1)
        same = true;
    else if (this->four == 1 && other->four == 1)
        same = true;
    else if (this->three == 1 && other->three == 1 && this->two == 1 && other->two == 1)
        same = true;
    else if (this->three == 1 && other->three == 1 && this->two == 0 && other->two == 0)
        same = true;
    else if (this->two == 2 && other->two == 2)
        same = true;
    else if (this->three == 0 && other->three == 0 && this->two == 1 && other->two == 1)
        same = true;
    else if (this->five == 0 && other->five == 0 && this->four == 0 && other->four == 0 && this->three == 0 &&
             other->three == 0 && this->two == 0 && other->two == 0)
        same = true;

    if (same) {
        // lexicographic order
        // -1 - string is before -> return true because we are before (lower rank)
        string s1 = this->cards_sortable1.str();
        string s2 = other->cards_sortable1.str();
        bool ret = s1.compare(s2) < 0;
        cout << "     = (same " << s1 << " vs " << s2 << ") " << ret << endl;
        return ret;
    } else {
        bool ret = false;
        if (this->five != other->five) {
            ret = this->five == 0;
        } else if (this->four != other->four) {
            ret = this->four == 0;
        } else if (this->three == 1 && this->two == 1 && (other->three == 0 || other->two == 0)) {
            ret = false;
        } else if (other->three == 1 && other->two == 1 && (this->three == 0 || this->two == 0)) {
            ret = true;
        } else if (this->three == 1 && this->two == 0 && other->three == 0) {
            ret = false;
        } else if (other->three == 1 && other->two == 0 && this->three == 0) {
            ret = true;
        } else if (this->two == 2 && other->two < 2) {
            ret = false;
        } else if (other->two == 2 && this->two < 2) {
            ret = true;
        } else if (this->two == 1 && other->two == 0) {
            ret = false;
        } else if (other->two == 1 && this->two == 0) {
            ret = true;
        } else {
            cout << "!!" << endl;
        }
        cout << "     = " << ret << endl;
        // should not happen
        return ret;
    }
}

bool hand::compare2(const hand *other) const {
    cout << "    comparing:" << endl;
    cout << "     - h1: " << *this << endl;
    cout << "     - h2: " << *other << endl;

    bool same = false;
    if (this->five_2 == 1 && other->five_2 == 1)
        same = true;
    else if (this->four_2 == 1 && other->four_2 == 1)
        same = true;
    else if (this->three_2 == 1 && other->three_2 == 1 && this->two_2 == 1 && other->two_2 == 1)
        same = true;
    else if (this->three_2 == 1 && other->three_2 == 1 && this->two_2 == 0 && other->two_2 == 0)
        same = true;
    else if (this->two_2 == 2 && other->two_2 == 2)
        same = true;
    else if (this->three_2 == 0 && other->three_2 == 0 && this->two_2 == 1 && other->two_2 == 1)
        same = true;
    else if (this->five_2 == 0 && other->five_2 == 0 && this->four_2 == 0 && other->four_2 == 0 && this->three_2 == 0 &&
             other->three_2 == 0 && this->two_2 == 0 && other->two_2 == 0)
        same = true;

    if (same) {
        // lexicographic order
        // -1 - string is before -> return true because we are before (lower rank)
        string s1 = this->cards_sortable2.str();
        string s2 = other->cards_sortable2.str();
        bool ret = s1.compare(s2) < 0;
        cout << "     = (same " << s1 << " vs " << s2 << ") " << ret << endl;
        return ret;
    } else {
        bool ret = false;
        if (this->five_2 != other->five_2) {
            ret = this->five_2 == 0;
        } else if (this->four_2 != other->four_2) {
            ret = this->four_2 == 0;
        } else if (this->three_2 == 1 && this->two_2 == 1 && (other->three_2 == 0 || other->two_2 == 0)) {
            ret = false;
        } else if (other->three_2 == 1 && other->two_2 == 1 && (this->three_2 == 0 || this->two_2 == 0)) {
            ret = true;
        } else if (this->three_2 == 1 && this->two_2 == 0 && other->three_2 == 0) {
            ret = false;
        } else if (other->three_2 == 1 && other->two_2 == 0 && this->three_2 == 0) {
            ret = true;
        } else if (this->two_2 == 2 && other->two_2 < 2) {
            ret = false;
        } else if (other->two_2 == 2 && this->two_2 < 2) {
            ret = true;
        } else if (this->two_2 == 1 && other->two_2 == 0) {
            ret = false;
        } else if (other->two_2 == 1 && this->two_2 == 0) {
            ret = true;
        } else {
            cout << "!!" << endl;
        }
        cout << "     = " << ret << endl;
        // should not happen
        return ret;
    }
}

void hand::calculate() {
    for (auto &c: this->cards) {
        if (c >= '2' && c <= '9') {
            this->counts[c - '2']++;
            this->counts2[c - '2']++;
            // abcdefgh
            cards_sortable1 << (char)(c - '2' + 'a');
            cards_sortable2 << (char)(c - '2' + 'b');
        }
        if (c == 'T') {
            cards_sortable1 << 'i';
            cards_sortable2 << 'j';
            this->counts[8]++;
            this->counts2[8]++;
        }
        if (c == 'J') {
            cards_sortable1 << 'j';
            cards_sortable2 << 'a';
            this->counts[9]++;
            this->counts2[9]++;
        }
        if (c == 'Q') {
            cards_sortable1 << 'k';
            cards_sortable2 << 'l';
            this->counts[10]++;
            this->counts2[10]++;
        }
        if (c == 'K') {
            cards_sortable1 << 'l';
            cards_sortable2 << 'm';
            this->counts[11]++;
            this->counts2[11]++;
        }
        if (c == 'A') {
            cards_sortable1 << 'm';
            cards_sortable2 << 'n';
            this->counts[12]++;
            this->counts2[12]++;
        }
    }

    u_int8_t jokers = this->counts2[9];
    if (jokers > 0) {
        // we have jokers
        if (jokers == 5) {
            // we could change them to aces, but this won't matter
        } else if (jokers == 4) {
            // just change highest card count to get five of a kind
            for (int p = 12; p >= 0; p--) {
                if (p == 9) {
                    continue;
                }
                if (this->counts2[p] > 0) {
                    this->counts2[p] += 4;
                    this->counts2[9] = 0;
                    break;
                }
            }
        } else if (jokers == 3) {
            bool had_2 = false;
            // JJJaa
            for (int p = 12; p >= 0; p--) {
                if (p == 9) {
                    continue;
                }
                if (this->counts2[p] == 2) {
                    this->counts2[p] += 3;
                    this->counts2[9] = 0;
                    had_2 = true;
                    break;
                }
            }
            // JJJab
            if (!had_2) {
                for (int p = 12; p >= 0; p--) {
                    if (p == 9) {
                        continue;
                    }
                    if (this->counts2[p] > 0) {
                        this->counts2[p] += 3;
                        this->counts2[9] = 0;
                        break;
                    }
                }
            }
        } else if (jokers == 2) {
            bool had_3 = false;
            bool had_2 = false;
            // JJaaa
            for (int p = 12; p >= 0; p--) {
                if (p == 9) {
                    continue;
                }
                if (this->counts2[9] == 2 && this->counts2[p] == 3) {
                    this->counts2[p] += 2;
                    this->counts2[9] = 0;
                    had_3 = true;
                }
                if (this->counts2[p] == 2) {
                    had_2 = true;
                }
            }
            // JJaab
            if (!had_3 && had_2) {
                for (int p = 12; p >= 0; p--) {
                    if (p == 9) {
                        continue;
                    }
                    if (this->counts2[p] == 2) {
                        this->counts2[p] += 2;
                        this->counts2[9] = 0;
                        break;
                    }
                }
            }
            // JJabc
            if (!had_3 && !had_2) {
                for (int p = 12; p >= 0; p--) {
                    if (p == 9) {
                        continue;
                    }
                    if (this->counts2[p] == 1) {
                        this->counts2[p] += 2;
                        this->counts2[9] = 0;
                        break;
                    }
                }
            }
        } else if (jokers == 1) {
            bool had_4 = false;
            bool had_3 = false;
            bool had_2 = false;
            // Jaaaa
            for (int p = 12; p >= 0; p--) {
                if (p == 9) {
                    continue;
                }
                if (this->counts2[9] == 1 && this->counts2[p] == 4) {
                    this->counts2[p] += 1;
                    this->counts2[9] = 0;
                    had_4 = true;
                }
                if (this->counts2[p] == 3) {
                    had_3 = true;
                }
                if (this->counts2[p] == 2) {
                    had_2 = true;
                }
            }
            // Jaaab
            if (!had_4 && had_3) {
                for (int p = 12; p >= 0; p--) {
                    if (p == 9) {
                        continue;
                    }
                    if (this->counts2[p] == 3) {
                        this->counts2[p] += 1;
                        this->counts2[9] = 0;
                        break;
                    }
                }
            }
            // Jaabb and Jaabc
            if (!had_4 && !had_3 && had_2) {
                for (int p = 12; p >= 0; p--) {
                    if (p == 9) {
                        continue;
                    }
                    if (this->counts2[p] == 2) {
                        this->counts2[p] += 1;
                        this->counts2[9] = 0;
                        break;
                    }
                }
            }
            // Jabcd
            if (!had_4 && !had_3 && !had_2) {
                for (int p = 12; p >= 0; p--) {
                    if (p == 9) {
                        continue;
                    }
                    if (this->counts2[p] == 1) {
                        this->counts2[p] += 1;
                        this->counts2[9] = 0;
                        break;
                    }
                }
            }
        }
    }

    for (auto &c: this->counts) {
        if (c == 2) {
            this->two++;
        } else if (c == 3) {
            this->three++;
        } else if (c == 4) {
            this->four++;
        } else if (c == 5) {
            this->five++;
        }
    }
    for (auto &c: this->counts2) {
        if (c == 2) {
            this->two_2++;
        } else if (c == 3) {
            this->three_2++;
        } else if (c == 4) {
            this->four_2++;
        } else if (c == 5) {
            this->five_2++;
        }
    }
}

int main(int argc, char *argv[]) {
    aoc2023::Options options("Day 07", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;

    vector<hand *> hands;

    while (getline(*input, line)) {
        aoc2023::trim(line);
        auto h = new hand;
        hands.emplace_back(h);

        istringstream iss(line);
        iss >> h->cards;
        iss >> h->bid;
        h->calculate();
    }

    for (auto &h: hands) {
        cout << *h << endl;
    }

    // part 1

    unsigned long answer1 = 0;

    sort(hands.begin(), hands.end(), [](const hand *h1, const hand *h2) {
        return h1->compare1(h2);
    });

//    cout << "after sorting" << endl;
    int c = 1;
    for (auto &h: hands) {
//        cout << *h << endl;
        answer1 += c++ * h->bid;
    }

    // part 2

    unsigned long answer2 = 0;

    sort(hands.begin(), hands.end(), [](const hand *h1, const hand *h2) {
        return h1->compare2(h2);
    });

//    cout << "after sorting" << endl;
    c = 1;
    for (auto &h: hands) {
//        cout << *h << endl;
        answer2 += c++ * h->bid;
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    for (auto &h: hands) {
        delete h;
    }

    return EXIT_SUCCESS;
}
