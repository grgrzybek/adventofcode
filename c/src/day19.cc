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

#include <cstdio>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <iomanip>

#include "utils/utils.h"

using namespace std;
using namespace aoc2021;

struct relative_coords {
    int x, y, z;

    bool operator==(const relative_coords &other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const relative_coords &other) const {
        return x != other.x || y != other.y || z != other.z;
    }

    explicit operator string() const {
        ostringstream oss;
        oss << "[" << x << "," << y << "," << z << "]";
        return oss.str();
    }
};

struct rc_less {
    bool operator()(relative_coords *const &c1, relative_coords *const &c2) const {
        // https://en.cppreference.com/w/cpp/named_req/Compare
        if (c1->z != c2->z) {
            return c1->z < c2->z;
        }
        if (c1->y != c2->y) {
            return c1->y < c2->y;
        }
        if (c1->x != c2->x) {
            return c1->x < c2->x;
        }
        return false;
    }
};

struct single_scanner {
    int id;

    bool resolved;
    int resolved_view;
    relative_coords *position;

    // initial relative beacon positions for given scanner (each scanner may see different number of beacons)
    vector<relative_coords *> *beacons;

    // each beacon has a 24-element array of sorted (by z, y, x) lists of the beacons it sees
    /*
     * 6|........
     * 5|1.......
     * 4|........
     * 3|........
     * 2|..2.....
     * 1|.S......
     * 0|....3...
     * -+--------
     *  |01234567
     *
     * Scanner is at position 1,1, sees 3 beacons (relatively):
     *  - b1: -1,4
     *  - b2: 1,1
     *  - b3: 3,-1
     *
     * Beacons see each other:
     *  - b1: b2=2,-3, b3=4,-5
     *  - b2: b1=-2,3, b3=2,-2
     *  - b3: b1=-4,5, b2=-2,2
     *
     * However - that's only if scanner (and beacons) "look" up (know where the up/down, left/right is).
     * In 2D, a scanner (and beacons too) may be oriented in 4 ways. So beacon 1 may see beacon 2 in 4 ways:
     *  - 2, -3
     *  - 3, 2
     *  - -2, 3
     *  - -3, -2
     *
     * In 3D, a scanner/beacon may "look" at 6 directions, while being turned in 4 possible ways, So 24 combinations...
     * starting witb x=1,y=2,z=3 and assuming:
     *  - x increases left->right
     *  - y increases bottom->top
     *  - z increases screen->viewer
     * 1:2:3 coordinates could be treated relatively to a scanner "facing" screen-top with scanner's "top" being
     * visible on the screen
     *   - scanner going its "forward" increases y
     *   - scanner going its "right" increases x
     *   - scanner going its "top" increases z
     * 4 transformations of the coordinates when the scanner's top is always visible on the screen and the scanner
     * turns clockwise (as seen by the viewer of the screen) are:
     *  -  1: 2:3 (facing up, original)
     *  - -2: 1:3 (facing right)
     *  - -1:-2:3 (facing down)
     *  -  2:-1:3 (facing left)
     *
     * Each of the 3 initial coordinates becomes twice (+ and -) a fixed coordinate (x, y or z) == 6. Then two
     * remaining coordinates (y+z, x+z, x+y respectively) are used in 4 combinations (rotation relatively to the fixed
     * coordinate), which gives 24 combinations.
     */

    // an array of 24-element arrays of vectors.
    //  - the array size is the number of beacons seen by the scanner
    //  - each element in the array is for one beacon - contains a 24-element array of vectors of coordinates of remaining beacons
    typedef vector<relative_coords *> coord_list;
    typedef coord_list *coord_list_p;
    typedef coord_list_p *coord_list_views;
    coord_list_views *b2b;

    // a 24-element array of vectors showing 24 views of the beacons stored at this->beacons
    // each vector has the same size as this->beacons vector
    coord_list_p *s2b;

    explicit single_scanner(int id) : id(id), resolved(false), resolved_view(-1), position(nullptr), beacons(nullptr),
                                      b2b(nullptr), s2b(nullptr) {}

    void deleteMaps(unsigned int count) const {
        for (unsigned int i = 0; i < count; i++) {
            for (unsigned int j = 0; j < 24; j++) {
                for (auto *&c: *b2b[i][j]) {
                    delete c;
                }
//                cout << "Deleting (" << i << ", " << j << ") coord_list " << (void *)b2b[i][j] << endl;
                delete b2b[i][j];
            }
//            cout << "Deleting (" << i << ") coord_list_p " << (void *)b2b[i] << endl;
            delete[] b2b[i];
        }
        delete[] b2b;
        for (unsigned int j = 0; j < 24; j++) {
            for (auto *&c: *s2b[j]) {
                delete c;
            }
            delete s2b[j];
        }
        delete[] s2b;
    }

    ~single_scanner() {
        unsigned int count = beacons->size();
        for (auto *&coords: *beacons) {
            delete coords;
        }
        delete beacons;
        deleteMaps(count);
        delete position;
    }

    explicit operator string() const {
        ostringstream oss;
        oss << "Scanner [" << id << "]" << endl;
        int pos = 0;
        for (auto *&coords: *beacons) {
            oss << " - " << static_cast<string>(*coords) << endl;
            for (int i = 0; i < 24; i++) {
                oss << "    - [" << setw(2) << (i + 1) << "]" << setw(0) << ": ";
                for (auto *&c: *b2b[pos][i]) {
                    oss << static_cast<string>(*c) << " ";
                }
                oss << endl;
            }
            pos++;
        }
        oss << " Views:" << endl;
        for (int i = 0; i < 24; i++) {
            oss << "    - [" << setw(2) << (i + 1) << "]" << setw(0) << ": ";
            for (auto *&c: *s2b[i]) {
                oss << static_cast<string>(*c) << " ";
            }
            oss << endl;
        }
        return oss.str();
    }

    void initialize() {
        sort(this->beacons->begin(), this->beacons->end(), rc_less());
        // prepare all information about previously parsed scanner data
        this->b2b = new single_scanner::coord_list_views[this->beacons->size()];
//        cout << "Created coord_list_views[" << s->beacons->size() << "] " << (void *)s->b2b << endl;
        for (vector<relative_coords *>::size_type pos = 0; pos < this->beacons->size(); pos++) {
            // creating 24 vectors
            this->b2b[pos] = new single_scanner::coord_list_p[24];
//            cout << "Created (" << pos << ") coord_list_p " << (void *)s->b2b[pos] << endl;
            for (int i = 0; i < 24; i++) {
                this->b2b[pos][i] = new single_scanner::coord_list;
//                cout << "Created (" << pos << ", " << i << ") coord_list " << (void *)s->b2b[pos][i] << endl;
            }
        }
        this->s2b = new single_scanner::coord_list_p[24];
        for (int i = 0; i < 24; i++) {
            this->s2b[i] = new single_scanner::coord_list;
        }

        // this scanner knows its beacons, time to prepare views of the beacons from other beacons
        int b = 0;
        for (auto *&coords: *beacons) {
            coord_list_views views = b2b[b++];
            // views is a 24-element array of vectors == 24 different ways this beacon sees other beacons
            // each of the 24 vectors has to contain relative coords of other beacons sorted by z/y/x
            for (auto *&other: *beacons) {
//                if (*coords == *other) {
//                    continue;
//                }
                prepare_views(views, coords, other);
            }
            // each of the 24 vectors has to be sorted
            for (int v = 0; v < 24; v++) {
                sort(views[v]->begin(), views[v]->end(), rc_less());
            }

            // each beacon also has to be presented in 24 (actually 16) different relative positions according to
            // the scanner, because the scanner doesn't know how it's oriented (nor even where it's located)
            int x = coords->x;
            int y = coords->y;
            int z = coords->z;
            s2b[0]->push_back(new relative_coords{.x=x, .y=y, .z=z});
            s2b[1]->push_back(new relative_coords{.x=-y, .y=x, .z=z});
            s2b[2]->push_back(new relative_coords{.x=-x, .y=-y, .z=z});
            s2b[3]->push_back(new relative_coords{.x=y, .y=-x, .z=z});
            s2b[4]->push_back(new relative_coords{.x=-x, .y=y, .z=-z});
            s2b[5]->push_back(new relative_coords{.x=y, .y=x, .z=-z});
            s2b[6]->push_back(new relative_coords{.x=x, .y=-y, .z=-z});
            s2b[7]->push_back(new relative_coords{.x=-y, .y=-x, .z=-z});
            s2b[8]->push_back(new relative_coords{.x=x, .y=z, .z=-y});
            s2b[9]->push_back(new relative_coords{.x=z, .y=-x, .z=-y});
            s2b[10]->push_back(new relative_coords{.x=-x, .y=-z, .z=-y});
            s2b[11]->push_back(new relative_coords{.x=-z, .y=x, .z=-y});
            s2b[12]->push_back(new relative_coords{.x=-x, .y=z, .z=y});
            s2b[13]->push_back(new relative_coords{.x=z, .y=x, .z=y});
            s2b[14]->push_back(new relative_coords{.x=x, .y=-z, .z=y});
            s2b[15]->push_back(new relative_coords{.x=-z, .y=-x, .z=y});
            s2b[16]->push_back(new relative_coords{.x=z, .y=y, .z=-x});
            s2b[17]->push_back(new relative_coords{.x=y, .y=-z, .z=-x});
            s2b[18]->push_back(new relative_coords{.x=-z, .y=-y, .z=-x});
            s2b[19]->push_back(new relative_coords{.x=-y, .y=z, .z=-x});
            s2b[20]->push_back(new relative_coords{.x=y, .y=z, .z=x});
            s2b[21]->push_back(new relative_coords{.x=z, .y=-y, .z=x});
            s2b[22]->push_back(new relative_coords{.x=-y, .y=-z, .z=x});
            s2b[23]->push_back(new relative_coords{.x=-z, .y=y, .z=x});
        }
    }

    static void prepare_views(coord_list_views &views24, relative_coords *c1, relative_coords *c2) {
//        cout << "Checking " << static_cast<string>(*c1) << " and " << static_cast<string>(*c2) << endl;
        int dx = c2->x - c1->x;
        int dy = c2->y - c1->y;
        int dz = c2->z - c1->z;
        // fixed z for { 1, 2, 3 } (front:up, right:right, top:viewer) (e.g., { 1, 1, 1 } beacon looks at { 2, 3, 4 })
        // { x, y, z } { 1, 2, 3 }
        // { -y, x, z } { -2, 1, 3 }
        // { -x, -y, z } { -1, -2, 3 }
        // { y, -x, z } { 2, -1, 3 }
        // fixed z (flipped by Y - Z and X change the signs) { 1, 2, 3 } changes to { -1, 2, -3 }
        // { -x, y, -z } { -1, 2, -3 }
        // { y, x, -z } { 2, 1, -3 }
        // { x, -y, -z } { 1, -2, -3 }
        // { -y, -x, -z } { -2, -1, -3 }
        views24[0]->push_back(new relative_coords{.x=dx, .y=dy, .z=dz});
        views24[1]->push_back(new relative_coords{.x=-dy, .y=dx, .z=dz});
        views24[2]->push_back(new relative_coords{.x=-dx, .y=-dy, .z=dz});
        views24[3]->push_back(new relative_coords{.x=dy, .y=-dx, .z=dz});
        views24[4]->push_back(new relative_coords{.x=-dx, .y=dy, .z=-dz});
        views24[5]->push_back(new relative_coords{.x=dy, .y=dx, .z=-dz});
        views24[6]->push_back(new relative_coords{.x=dx, .y=-dy, .z=-dz});
        views24[7]->push_back(new relative_coords{.x=-dy, .y=-dx, .z=-dz});
        // { x, z, -y } { 1, 3, -2 }
        // { z, -x, -y } { 3, -1, -2 }
        // { -x, -z, -y } { -1, -3, -2 }
        // { -z, x, -y } { -3, 1, -2 }
        // { -x, z, y } { -1, 3, 2 }
        // { z, x, y } { 3, 1, 2 }
        // { x, -z, y } { 1, -3, 2 }
        // { -z, -x, y } { -3, -1, 2 }
        views24[8]->push_back(new relative_coords{.x=dx, .y=dz, .z=-dy});
        views24[9]->push_back(new relative_coords{.x=dz, .y=-dx, .z=-dy});
        views24[10]->push_back(new relative_coords{.x=-dx, .y=-dz, .z=-dy});
        views24[11]->push_back(new relative_coords{.x=-dz, .y=dx, .z=-dy});
        views24[12]->push_back(new relative_coords{.x=-dx, .y=dz, .z=dy});
        views24[13]->push_back(new relative_coords{.x=dz, .y=dx, .z=dy});
        views24[14]->push_back(new relative_coords{.x=dx, .y=-dz, .z=dy});
        views24[15]->push_back(new relative_coords{.x=-dz, .y=-dx, .z=dy});
        // { z, y, -x } { 3, 2, -1 }
        // { y, -z, -x } { 2, -3, -1 }
        // { -z, -y, -x } { -3, -2, -1 }
        // { -y, z, -x } { -2, 3, -1 }
        // { y, z, x } { 2, 3, 1 }
        // { z, -y, x } { 3, -2, 1 }
        // { -y, -z, x } { -2, -3, 1 }
        // { -z, y, x } { -3, 2, 1 }
        views24[16]->push_back(new relative_coords{.x=dz, .y=dy, .z=-dx});
        views24[17]->push_back(new relative_coords{.x=dy, .y=-dz, .z=-dx});
        views24[18]->push_back(new relative_coords{.x=-dz, .y=-dy, .z=-dx});
        views24[19]->push_back(new relative_coords{.x=-dy, .y=dz, .z=-dx});
        views24[20]->push_back(new relative_coords{.x=dy, .y=dz, .z=dx});
        views24[21]->push_back(new relative_coords{.x=dz, .y=-dy, .z=dx});
        views24[22]->push_back(new relative_coords{.x=-dy, .y=-dz, .z=dx});
        views24[23]->push_back(new relative_coords{.x=-dz, .y=dy, .z=dx});
    }
};

void show_space(set<relative_coords *, rc_less> &space) {
    for (auto &c: space) {
        cout << static_cast<string>(*c) << endl;
    }
}

int main(int argc, char *argv[]) {
    Options options("Day 19", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<single_scanner *> scanners;
    single_scanner *current_scanner;

    while (getline(*input, line)) {
        trim(line);
        if (line.length() == 0) {
            continue;
        }
        if (line.starts_with("---")) {
            int id;
            sscanf(line.c_str(), "--- scanner %d ---", &id);
            current_scanner = new single_scanner(id);
            current_scanner->beacons = new vector<relative_coords *>;
            scanners.push_back(current_scanner);
        } else {
            auto coords = new relative_coords;
            sscanf(line.c_str(), "%d,%d,%d", &coords->x, &coords->y, &coords->z);
            current_scanner->beacons->push_back(coords);
        }
    }

    for (auto *&s: scanners) {
        s->initialize();
    }

//    cout << endl;
//    for (auto *&scanner: scanners) {
//        cout << static_cast<string>(*scanner) << endl;
//    }

    // part 1

    // this will gather beacons from other scanners after they're resolved. Initially, "view 0" of the beacons
    // from scanner 0 will be the only available beacons

    // https://stackoverflow.com/questions/2620862/using-custom-stdset-comparator
    set<relative_coords *, rc_less> space;
    set<relative_coords *, rc_less> tmp_space;

    for (relative_coords *&coords: *scanners[0]->s2b[0]) {
        space.insert(new relative_coords{coords->x, coords->y, coords->z});
    }

    // treat first scanner as resolved - we can resolve unresolved scanners by matching with a resolved one only
    scanners[0]->resolved = true;
    scanners[0]->resolved_view = 0;
    scanners[0]->position = new relative_coords{0, 0, 0};

    while (true) {
        bool change = false;
        for (auto *&s1: scanners) {
            // first scanner has to be resolved
            if (!s1->resolved) {
                continue;
            }
            for (auto *&s2: scanners) {
                if (!s2->resolved) {
                    cout << "Checking " << s1->id << " with " << s2->id << endl;
                    // checking if two scanners have at least 12 beacons in common 24x24 views will be checked for each
                    // combination of beacons...
                    for (unsigned int b1c = 0; b1c < s1->beacons->size(); b1c++) {
                        for (unsigned int b2c = 0; b2c < s2->beacons->size(); b2c++) {
                            for (int p1 = s1->resolved_view; p1 < s1->resolved_view + 1; p1++) {
                                for (int p2 = 0; p2 < 24; p2++) {
                                    tmp_space.clear();
                                    int v1count = static_cast<int>(s1->b2b[b1c][p1]->size());
                                    int v2count = static_cast<int>(s2->b2b[b2c][p2]->size());
                                    tmp_space.insert(s1->b2b[b1c][p1]->begin(), s1->b2b[b1c][p1]->end());
                                    tmp_space.insert(s2->b2b[b2c][p2]->begin(), s2->b2b[b2c][p2]->end());
                                    if (v1count + v2count - tmp_space.size() + 1 >= 12) {
                                        cout << "Common " << v1count + v2count - tmp_space.size() + 1 << endl;
                                    }
                                    if (v1count + v2count - tmp_space.size() + 1 >= 12) {
                                        // there are at least 12 common beacons:
                                        //  - scanner1 - beacon1 and at least 11 beacons in beacon1's view number 0
                                        //  - scanner2 - beacon2 and at least 11 beacons in beacon2's view number p2
                                        // we can mark s2 as resolved
                                        s2->resolved = true;
                                        change = true;
                                        // if s2 was resolved by comparing view v1 of beacon1 and view v2 of beacon2
                                        s2->resolved_view = 0;
                                        // each of the beacon in beacon2 view number p2 can be added to the space after
                                        // unrelativizing the coordinates by the difference of b2 and b1 (because that's the
                                        // same beacon actually!)

                                        int dx = (*s1->s2b[p1])[b1c]->x - (*s2->s2b[p2])[b2c]->x;
                                        int dy = (*s1->s2b[p1])[b1c]->y - (*s2->s2b[p2])[b2c]->y;
                                        int dz = (*s1->s2b[p1])[b1c]->z - (*s2->s2b[p2])[b2c]->z;

                                        s2->position = new relative_coords{dx, dy, dz};

                                        for (auto *&c: *s2->s2b[p2]) {
                                            space.insert(new relative_coords{c->x + dx, c->y + dy, c->z + dz});
                                        }
                                        // let's alter the resolved scanner as if it was in the same position as the base scanner
                                        for (auto *&b: *s2->beacons) {
                                            delete b;
                                        }
                                        s2->beacons->clear();
                                        for (auto *&c: *s2->s2b[p2]) {
                                            s2->beacons->push_back(
                                                    new relative_coords{c->x + dx, c->y + dy, c->z + dz});
                                        }
                                        s2->deleteMaps(s2->beacons->size());
                                        s2->initialize();
                                        //                                    cout << "Changed scanner " << s2->id << ":" << endl;
                                        //                                    cout << static_cast<string>(*s2) << endl;
                                        break;
                                    }
                                }
                            }
                            if (s2->resolved) {
                                break;
                            }
                        }
                        if (s2->resolved) {
                            break;
                        }
                    }
                }
            }
        }
        if (!change) {
            break;
        }
    }

    unsigned long answer1 = space.size();
    for (auto *const &c: space) {
        cout << static_cast<string>(*c) << endl;
    }

    // part 2

    int answer2 = 0;

    for (auto *&s1: scanners) {
        for (auto *&s2: scanners) {
            if (s1->id == s2->id) {
                continue;
            }
            int dist = abs(s1->position->x - s2->position->x) + abs(s1->position->y - s2->position->y) +
                       abs(s1->position->z - s2->position->z);
            if (answer2 < dist) {
                answer2 = dist;
            }
        }
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    for (auto *&scanner: scanners) {
        delete scanner;
    }
    for (auto *const &c: space) {
        delete c;
    }

    return EXIT_SUCCESS;
}
