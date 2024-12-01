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
#include <cstring>
#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <set>
#include <string>
#include <vector>
#include <numeric>

#include "utils/utils.h"

using namespace std;

struct instruction {
    long dx, dy;

    instruction(const long _dx, const long _dy) : dx(_dx), dy(_dy) {}
};

struct edge {
    long x1, y1, x2, y2;

    edge(const long _x1, const long _y1, const long _x2, const long _y2) : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {}

    friend ostream &operator<<(ostream &os, const edge &line);
};

struct rect {
    long x1, x2, h;

    rect(const long _x1, const long _x2, const long _h) : x1(_x1), x2(_x2), h(_h) {}

    friend ostream &operator<<(ostream &os, const rect &r);
};

ostream &operator<<(ostream &os, const edge &line) {
    if (line.x1 == line.x2) {
        os << " | (";
    } else {
        os << " - (";
    }
    os << line.x1 << ":" << line.y1 << " - " << line.x2 << ":" << line.y2 << ")";
    return os;
}

ostream &operator<<(ostream &os, const rect &r) {
    os << r.x1 << " -- " << r.x2;
    return os;
}

bool rcmp(const rect &r1, const rect &r2);

int main(int argc, char *argv[]) {
    aoc2023::Options options("Day 18", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    // part 1
    vector<instruction> lines1;
    // part 2
    vector<instruction> lines2;

    long w1, h1, w2, h2;
    long x, y, dx, dy;
    long x1_1 = numeric_limits<long>::max();
    long x2_1 = numeric_limits<long>::min();
    long y1_1 = numeric_limits<long>::max();
    long y2_1 = numeric_limits<long>::min();

    char dir1;
    char dir2;
    long v;
    long v2;
    string hex;
    while (getline(*input, line)) {
        istringstream iss(line);
        if (!aoc2023::has_text(line)) {
            continue;
        }
        iss >> dir1;
        iss >> v;
        iss >> hex;
        dir2 = line[line.length() - 2];
        hex = hex.substr(2, 5);
        v2 = strtol(hex.c_str(), nullptr, 16);
        // R 6 (#70c710)
        // D 5 (#0dc571)
        // L 2 (#5713f0)
        // D 2 (#d2c081)
        if (dir1 == 'U') {
            lines1.emplace_back(0, -v);
        } else if (dir1 == 'D') {
            lines1.emplace_back(0, v);
        } else if (dir1 == 'R') {
            lines1.emplace_back(v, 0);
        } else if (dir1 == 'L') {
            lines1.emplace_back(-v, 0);
        }
        // 0 means R, 1 means D, 2 means L, and 3 means U
        if (dir2 == '3') {
            lines2.emplace_back(0, -v2);
        } else if (dir2 == '1') {
            lines2.emplace_back(0, v2);
        } else if (dir2 == '0') {
            lines2.emplace_back(v2, 0);
        } else if (dir2 == '2') {
            lines2.emplace_back(-v2, 0);
        }
    }

    x = 0;
    y = 0;
    for (auto &in: lines1) {
        // cout << "1) dx: " << in.dx << ", dy: " << in.dy << endl;
        x += in.dx;
        y += in.dy;

        if (x < x1_1) {
            x1_1 = x;
        }
        if (x > x2_1) {
            x2_1 = x;
        }
        if (y < y1_1) {
            y1_1 = y;
        }
        if (y > y2_1) {
            y2_1 = y;
        }
    }
    w1 = x2_1 - x1_1 + 1 + 2;
    h1 = y2_1 - y1_1 + 1 + 2;

    cout << x1_1 << ":" << y1_1 << " - " << x2_1 << ":" << y2_1 << endl;
    cout << "w1 = " << w1 << ", h1 = " << h1 << "\n\n";

    x = 0;
    y = 0;
    long x1_2 = numeric_limits<long>::max();
    long x2_2 = numeric_limits<long>::min();
    long y1_2 = numeric_limits<long>::max();
    long y2_2 = numeric_limits<long>::min();
    for (auto &in: lines2) {
        // cout << "2) dx: " << in.dx << ", dy: " << in.dy << endl;
        x += in.dx;
        y += in.dy;

        if (x < x1_2) {
            x1_2 = x;
        }
        if (x > x2_2) {
            x2_2 = x;
        }
        if (y < y1_2) {
            y1_2 = y;
        }
        if (y > y2_2) {
            y2_2 = y;
        }
    }
    w2 = x2_2 - x1_2 + 1;
    h2 = y2_2 - y1_2 + 1;

    cout << x1_2 << ":" << y1_2 << " - " << x2_2 << ":" << y2_2 << endl;
    cout << "w2 = " << w2 << ", h2 = " << h2 << "\n\n";

    auto map = static_cast<char *>(malloc(sizeof(char) * w1 * h1));
    memset(map, '.', sizeof(char) * w1 * h1);

    // part 1

    long answer1 = 0;

    x = -x1_1 + 1;
    y = -y1_1 + 1;

    map[y * w1 + x] = '#';
    for (auto &in: lines1) {
        if (in.dx != 0) {
            dx = in.dx > 0 ? 1 : -1;
            x1_1 = x;
            x2_1 = x + in.dx + dx;
            for (x = x1_1; x != x2_1; x += dx) {
                map[y * w1 + x] = '#';
            }
            x -= dx;
        } else {
            dy = in.dy > 0 ? 1 : -1;
            y1_1 = y;
            y2_1 = y + in.dy + dy;
            for (y = y1_1; y != y2_1; y += dy) {
                map[y * w1 + x] = '#';
            }
            y -= dy;
        }
    }

    deque<pair<int, int>> q;
    q.emplace_back(0, 0);
    while (!q.empty()) {
        auto p = q.back();
        q.pop_back();

        x = p.first;
        y = p.second;
        map[y * w1 + x] = '~';

        if (x > 0 && map[y * w1 + x - 1] == '.') {
            q.emplace_back(x - 1, y);
        }
        if (y > 0 && map[(y - 1) * w1 + x] == '.') {
            q.emplace_back(x, y - 1);
        }
        if (x < w1 - 1 && map[y * w1 + x + 1] == '.') {
            q.emplace_back(x + 1, y);
        }
        if (y < h1 - 1 && map[(y + 1) * w1 + x] == '.') {
            q.emplace_back(x, y + 1);
        }
    }

    for (y = 0; y < h1; y++) {
        for (x = 0; x < w1; x++) {
            if (map[y * w1 + x] != '~') {
                answer1++;
            }
        }
    }

    // part 2

    long answer2 = 0;

    vector<edge> v_edges;
    vector<edge> v_edges_tmp;
    vector<edge> h_edges;
    vector<edge> h_edges_tmp;

    x = 0;
    y = 0;
    for (auto &l: lines2) {
        if (l.dx == 0) {
            // vertical edge
            if (l.dy < 0) {
                // bottom -> top
                v_edges_tmp.emplace_back(x, y + l.dy, x, y);
            } else {
                // top -> bottom
                v_edges_tmp.emplace_back(x, y, x, y + l.dy);
            }
        } else {
            // horizontal edge
            if (l.dx < 0) {
                // right -> left
                h_edges_tmp.emplace_back(x + l.dx, y, x, y);
            } else {
                // left -> right
                h_edges_tmp.emplace_back(x, y, x + l.dx, y);
            }
        }
        x += l.dx;
        y += l.dy;
    }

    set<long> x_steps;
    set<long> y_steps;

    // split v-edges when there's any h-edge for it
    for (auto &ve: v_edges_tmp) {
        cout << "Checking VE: " << ve << endl;
        set<long> ys;
        ys.insert(ve.y1);
        y_steps.insert(ve.y1);
        for (auto &he: h_edges_tmp) {
            cout << "Cutting by HE: " << he << endl;
            if (ve.y1 <= he.y1 && ve.y2 >= he.y1) {
                cout << " + " << he.y1 << endl;
                ys.insert(he.y1);
                y_steps.insert(ve.y1);
            }
        }
        ys.insert(ve.y2);
        y_steps.insert(ve.y2);
        // already sorted
        vector ysv(ys.begin(), ys.end());
        for (int i = 1; i < static_cast<int>(ysv.size()); i++) {
            v_edges.emplace_back(ve.x1, ysv[i - 1], ve.x1, ysv[i]);
        }
    }
    // split h-edges when there's any v-edge for it
    for (auto &he: h_edges_tmp) {
        cout << "Checking HE: " << he << endl;
        set<long> xs;
        xs.insert(he.x1);
        x_steps.insert(he.x1);
        for (auto &ve: v_edges_tmp) {
            cout << "Cutting by VE: " << he << endl;
            if (he.x1 <= ve.x1 && he.x2 >= ve.x1) {
                cout << " + " << ve.x1 << endl;
                xs.insert(ve.x1);
                x_steps.insert(ve.x1);
            }
        }
        xs.insert(he.x2);
        x_steps.insert(he.x2);
        // already sorted
        vector xsv(xs.begin(), xs.end());
        for (int i = 1; i < static_cast<int>(xsv.size()); i++) {
            h_edges.emplace_back(xsv[i - 1], he.y1, xsv[i], he.y1);
        }
    }

    ranges::sort(v_edges, [](const edge &e1, const edge &e2) {
        if (e1.y1 == e2.y1) {
            return e1.x1 < e2.x1;
        }
        return e1.y1 < e2.y1;
    });
    ranges::sort(h_edges, [](const edge &e1, const edge &e2) {
        if (e1.y1 == e2.y1) {
            return e1.x1 < e2.x1;
        }
        return e1.y1 < e2.y1;
    });

    cout << "H edges:\n";
    for (auto &he: h_edges) {
        cout << he << endl;
    }
    cout << "V edges:\n";
    for (auto &ve: v_edges) {
        cout << ve << endl;
    }
    cout << "Y Steps:\n";
    for (auto &s: y_steps) {
        cout << s << endl;
    }
    cout << "X Steps:\n";
    for (auto &s: x_steps) {
        cout << s << endl;
    }

    long previous = *y_steps.begin();
    vector<rect> *upper_lines = nullptr;
    auto *lower_lines = new vector<rect>;
    auto *tmp_lines = new vector<rect>;

    for (auto &s: y_steps) {
        cout << "At level " << s << endl;
        for (auto &he: h_edges) {
            if (s == he.y1) {
                // add rectangle in the making
                lower_lines->emplace_back(he.x1, he.x2, 0);
            }
        }
        ranges::sort(*lower_lines, rcmp);

        if (upper_lines != nullptr) {
            // collapsing and calculating areas
            // each rectangle in the making (from upper_lines) is ALWAYS completed, but may:
            //  - start a new rectangle of the same width
            //    +---+
            //    |   |
            //    +---+
            //  - start a new wider rectangle with one x from old one
            //    +---+
            //    |   +--+
            //    +------+
            //  - start a new narrower rectangle with one x from old one
            //    +---+
            //    |  ++
            //    +--+
            //  - start a new rectangle
            //      +---+
            //    +-+  ++
            //    +----+
            //  - or even
            //    +------+
            //    | +-+  |
            //    +-+ +--+

            // immediately add areas of started rects
            for (auto &r1: *upper_lines) {
                cout << "Add area: " << ((r1.x2 - r1.x1 + 1) * (s - previous)) << endl;
                answer2 += (r1.x2 - r1.x1 + 1) * (s - previous);
            }

            tmp_lines->clear();
            x_steps.clear();
            for (auto &r1: *upper_lines) {
                x_steps.insert(r1.x1);
                x_steps.insert(r1.x2);
            }
            for (auto &r2: *lower_lines) {
                x_steps.insert(r2.x1);
                x_steps.insert(r2.x2);
            }

            cout << "X-steps: ";
            for (auto &xs: x_steps) {
                cout << xs << " ";
                // there should be at most one upper and one lower h-line for given x step
                auto urp = ranges::find_if(*upper_lines, [xs](const rect &r) {
                    return r.x1 <= xs && r.x2 >= xs;
                });
                auto lrp = ranges::find_if(*lower_lines, [xs](const rect &r) {
                    return r.x1 <= xs && r.x2 >= xs;
                });
                if (lrp == lower_lines->end()) {

                }
            }
            cout << endl;


//            for (auto &r1: *upper_lines) {
//                cout << "Checking upper rect: " << r1 << endl;
//                for (auto &r2: *lower_lines) {
//                    cout << "   With lower rect: " << r2 << endl;
//                    if (r1.x2 < r2.x1) {
//                        continue;
//                    }
//                    if (r1.x2 == r2.x1) {
//                        tmp_lines->emplace_back(r1.x1, r2.x2, 0);
//                    }
//                }
//            }

            // upper_lines no longer needed
            delete upper_lines;
        }
        // in both cases
        upper_lines = lower_lines;
        lower_lines = new vector<rect>;

        cout << "At step " << s << " we have rectangles:\n";
        for (auto &r: *upper_lines) {
            cout << " - " << r << endl;
        }

        previous = s;
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    free(map);
    delete upper_lines;
    delete lower_lines;
    delete tmp_lines;

    return EXIT_SUCCESS;
}

bool rcmp(const rect &r1, const rect &r2) {
    return r1.x1 <= r2.x1;
}
