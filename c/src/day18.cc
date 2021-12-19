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

#include <deque>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;
using namespace aoc2021;

class sfnumber {
private:
    int lvalue = -1, rvalue = -1;
    int depth = 0;
    sfnumber *parent = nullptr;
    sfnumber *lpair = nullptr, *rpair = nullptr;
public:
    explicit sfnumber(sfnumber *parent) : parent(parent) { }
    sfnumber(sfnumber &other);

    ~sfnumber();

    static sfnumber *parse(string &number);

    explicit operator string() const;

    sfnumber *operator+(sfnumber *rarg);
    sfnumber &operator=(sfnumber *other);

    void increase_depth();

    bool explode();

    bool split();

    int magnitude();

    void add_left(int value, sfnumber *number);

    void add_right(int value, sfnumber *number);
};

int main(int argc, char *argv[]) {
    Options options("Day 18", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<sfnumber *> snailfish_numbers;

    while (getline(*input, line)) {
        trim(line);

        sfnumber *x = sfnumber::parse(line);
        snailfish_numbers.push_back(x);
    }

    // part 1

    sfnumber *sum = nullptr;
    for (auto *&sfn: snailfish_numbers) {
        if (sum == nullptr) {
            sum = new sfnumber(*sfn);
        } else {
            sfnumber *tmp = sum;
            sum = *sum + sfn;
            delete tmp;
        }
    }

    int answer1 = sum->magnitude();

    // part 2

    int answer2 = 0;

    for (auto *&sfn1: snailfish_numbers) {
        for (auto *&sfn2: snailfish_numbers) {
            if (sfn1 != sfn2) {
                auto sfn = *sfn1 + sfn2;
                int m = sfn->magnitude();
                if (m > answer2) {
                    answer2 = m;
                }
                delete sfn;
            }
        }
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    return EXIT_SUCCESS;
}

typedef enum {
    LEFT = 1,
    RIGHT = 2
} TSide;

sfnumber *sfnumber::parse(string &number) {
    sfnumber *n = nullptr;
    sfnumber *current = nullptr;
    deque<TSide> sides;

    for (char c: number) {
        switch (c) {
            case '[': {
                auto *sfn = new sfnumber(current);
                if (current) {
                    if (sides.back() == LEFT) {
                        current->lpair = sfn;
                    } else if (sides.back() == RIGHT) {
                        current->rpair = sfn;
                    }
                } else {
                    // top number
                    n = sfn;
                }
                current = sfn;
                sides.push_back(LEFT);
                sfn->depth = (int) sides.size();
                break;
            }
            case ']': {
                sides.pop_back();
                current = current->parent;
                break;
            }
            case ',':
                sides.back() = RIGHT;
                break;
            default:
                // number 0-9
                if (sides.back() == LEFT) {
                    current->lvalue = c - '0';
                } else if (sides.back() == RIGHT) {
                    current->rvalue = c - '0';
                }
                break;
        }
    }
    return n;
}

sfnumber::operator string() const { // NOLINT(misc-no-recursion)
    ostringstream oss;
    oss << "[";
    if (lpair) {
        oss << (string) *lpair;
    } else {
        oss << lvalue;
    }
    oss << ",";
    if (rpair) {
        oss << (string) *rpair;
    } else {
        oss << rvalue;
    }
//    oss << "(d:" << depth << ")";
    oss << "]";
    return oss.str();
}

sfnumber::~sfnumber() {
    delete lpair;
    delete rpair;
}

int sfnumber::magnitude() { // NOLINT(misc-no-recursion)
    int lv = 3 * (lpair ? lpair->magnitude() : lvalue);
    int rv = 2 * (rpair ? rpair->magnitude() : rvalue);
    return lv + rv;
}

sfnumber *sfnumber::operator+(sfnumber *rarg) {
    auto *result = new sfnumber(nullptr);
    result->lpair = new sfnumber(*this);
    result->lpair->increase_depth();
    result->lpair->parent = result;
    result->rpair = new sfnumber(*rarg);
    result->rpair->increase_depth();
    result->rpair->parent = result;
    result->depth = 1;

    bool change = false;
    while (true) {
        if (result->explode()) {
            change = true;
        } else if (result->split()) {
            change = true;
        }
        if (!change) {
            break;
        }
        change = false;
    }

    return result;
}

void sfnumber::increase_depth() { // NOLINT(misc-no-recursion)
    depth++;
    if (lpair) {
        lpair->increase_depth();
    }
    if (rpair) {
        rpair->increase_depth();
    }
}

/**
 * Find left-most pair, that has depth 5 or higher and exploder it.
 * @return
 */
bool sfnumber::explode() { // NOLINT(misc-no-recursion)
    if (depth >= 5) {
        // explode itself

        // our left value should be added to first number "on the left"
        parent->add_left(lvalue, this);
        // our right value should be added to first number "on the right"
        parent->add_right(rvalue, this);

        if (parent->lpair == this) {
            parent->lvalue = 0;
            parent->lpair = nullptr;
        } else if (parent->rpair == this) {
            parent->rvalue = 0;
            parent->rpair = nullptr;
        }

        delete this;
        return true;
    } else {
        if (lpair && lpair->explode()) {
            return true;
        }
        if (rpair && rpair->explode()) {
            return true;
        }
        return false;
    }
}

/**
 * adds a value to the first value on the left of a number
 * @param value
 * @param number
 */
void sfnumber::add_left(int value, sfnumber *number) { // NOLINT(misc-no-recursion)
    if (number == nullptr) {
        // we're adding "down" and "left"
        if (!lpair) {
            lvalue += value;
        } else {
            lpair->add_left(value, nullptr);
        }
    } else if (number == rpair) {
        // we have either a value or another sfnumber left of the pair being exploded
        if (!lpair) {
            lvalue += value;
        } else {
            lpair->add_right(value, nullptr);
        }
    } else if (number == lpair) {
        // our left pair was exploded - its left value should be added through parent, as we have nothing left on the left.
        if (parent != nullptr) {
            parent->add_left(value, this);
        }
    }
}

/**
 * adds a value to the first value on the right of a number
 * @param value
 * @param number
 */
void sfnumber::add_right(int value, sfnumber *number) { // NOLINT(misc-no-recursion)
    if (number == nullptr) {
        // we're adding "down" and "right"
        if (!rpair) {
            rvalue += value;
        } else {
            rpair->add_right(value, nullptr);
        }
    } else if (number == lpair) {
        // we have either a value or another sfnumber left of the pair being exploded
        if (!rpair) {
            rvalue += value;
        } else {
            rpair->add_left(value, nullptr);
        }
    } else if (number == rpair) {
        // our right pair was exploded - its left value should be added through parent, as we have nothing left on the right.
        if (parent != nullptr) {
            parent->add_right(value, this);
        }
    }
}

/**
 * Find left-most number that's >= 10 and split it.
 * We never have to go up, because we're always starting from the top.
 * @return
 */
bool sfnumber::split() { // NOLINT(misc-no-recursion)
    if (!lpair && lvalue >= 10) {
        lpair = new sfnumber(this);
        lpair->depth = depth + 1;
        lpair->lvalue = lvalue / 2;
        lpair->rvalue = lvalue / 2;
        if (lvalue % 2 == 1) {
            lpair->rvalue++;
        }
        return true;
    } else if (lpair && lpair->split()) {
        return true;
    } else if (!rpair && rvalue >= 10) {
        rpair = new sfnumber(this);
        rpair->depth = depth + 1;
        rpair->lvalue = rvalue / 2;
        rpair->rvalue = rvalue / 2;
        if (rvalue % 2 == 1) {
            rpair->rvalue++;
        }
        return true;
    } else if (rpair && rpair->split()) {
        return true;
    }

    return false;
}

sfnumber::sfnumber(sfnumber &other) { // NOLINT(misc-no-recursion)
    this->lvalue = other.lvalue;
    this->rvalue = other.rvalue;
    this->depth = other.depth;
    if (other.lpair) {
        this->lpair = new sfnumber(*other.lpair);
        this->lpair->parent = this;
    }
    if (other.rpair) {
        this->rpair = new sfnumber(*other.rpair);
        this->rpair->parent = this;
    }
}

sfnumber &sfnumber::operator=(sfnumber *other) {
    this->lvalue = other->lvalue;
    this->rvalue = other->rvalue;
    this->depth = other->depth;
    if (other->lpair) {
        this->lpair = new sfnumber(*other->lpair);
        this->lpair->parent = this;
    }
    if (other->rpair) {
        this->rpair = new sfnumber(*other->rpair);
        this->rpair->parent = this;
    }

    return *this;
}
