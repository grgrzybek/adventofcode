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
#include <deque>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

struct mapping_range {
    unsigned long from;
    unsigned long to;
    long delta;
    unsigned long size;

    friend ostream &operator<<(ostream &os, const mapping_range &m);

    ~mapping_range() = default;
};

struct mapping {
    string from;
    string to;
    vector<mapping_range> *ranges;

    mapping() {
        ranges = new vector<mapping_range>;
    }

    friend ostream &operator<<(ostream &os, const mapping &mr);

    ~mapping() {
        delete ranges;
    }
};

ostream &operator<<(ostream &os, const mapping_range &m) {
    return os << m.from << ".." << (m.from + m.size - 1)
            << " -> " << m.to << ".." << (m.to + m.size - 1)
            << " +d=" << m.delta << " (" << m.size << ")";
}

ostream &operator<<(ostream &os, const mapping &mr) {
    os << mr.from << " -> " << mr.to << " (" << mr.ranges->size() << ")" << endl;
    for (auto &r: *mr.ranges) {
        os << " * " << r << endl;
    }
    return os;
}

int main(int argc, char *argv[]) {
    aoc2023::Options options("Day 05", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    unsigned long n;
    string line;
    string::size_type pos, p1, p2;
    istringstream iss;

    vector<unsigned long> seeds;
    vector<unsigned long> locations;
    vector<mapping *> mappings;

    // seeds: 79 14 55 13
    getline(*input, line);
    pos = line.find(' ');
    iss.clear();
    iss.str(line.substr(pos));
    while (!iss.eof()) {
        iss >> n;
        seeds.emplace_back(n);
        locations.emplace_back(n);
    }
    // empty line
    getline(*input, line);

    while (!input->eof()) {
        auto m = new mapping;
        mappings.emplace_back(m);

        // seed-to-soil map:
        getline(*input, line);
        pos = line.find(' ');
        p1 = line.find('-');
        p2 = line.find('-', p1 + 1);

        m->from = line.substr(0, p1);
        m->to = line.substr(p2 + 1, pos - p2 - 1);

        // 50 98 2
        // 52 50 48
        getline(*input, line);
        while (!line.empty() && !input->eof()) {
            iss.clear();
            iss.str(line);
            auto range = mapping_range{};
            iss >> range.to;
            iss >> range.from;
            iss >> range.size;
            range.delta = (long) range.to - (long) range.from;
            m->ranges->emplace_back(range);
            getline(*input, line);
        }

        sort(m->ranges->begin(), m->ranges->end(), [](mapping_range &r1, mapping_range &r2) {
            return r1.from < r2.from;
        });
    }

//    for (auto &m: mappings) {
//        cout << "mapping: " << *m;
//    }

    // part 1

    unsigned long answer1 = numeric_limits<unsigned long>::max();

    for (size_t i = 0; i < seeds.size(); i++) {
        auto seed = seeds[i];
        for (auto &m: mappings) {
            for (auto &r: *m->ranges) {
                if (seed >= r.from && seed < r.from + r.size) {
                    seed = r.to + seed - r.from;
                    break;
                }
            }
        }

        locations[i] = seed;
    }

    for (auto &l: locations) {
        if (answer1 > l) {
            answer1 = l;
        }
    }

    // part 2

    unsigned long answer2 = numeric_limits<unsigned long>::max();

    mapping one_step_mappings;
    one_step_mappings.from = "seed";
    one_step_mappings.to = "seed";
    one_step_mappings.ranges = new vector<mapping_range>;

    for (size_t i = 0; i < seeds.size(); i += 2) {
        one_step_mappings.ranges->emplace_back(
                mapping_range{ .from = seeds[i], .to = seeds[i], .delta = 0, .size = seeds[i + 1] });
    }
    sort(one_step_mappings.ranges->begin(), one_step_mappings.ranges->end(), [](mapping_range &r1, mapping_range &r2) {
        return r1.from < r2.from;
    });
    cout << "\none step mapping: " << one_step_mappings << endl;

    // further partition one_step_mappings
    for (auto &m: mappings) {
        // for each mapping we replace 1-step mapping with 2-steps mapping, e.g., seed-to-soil -> seed-to-fertilizer
        auto new_ranges = new vector<mapping_range>;
        one_step_mappings.to = m->to;

//        cout << "Checking mapping " << *m;

        // each range may split one or more existing ranges
        auto split_ranges = new deque<mapping_range>(one_step_mappings.ranges->begin(), one_step_mappings.ranges->end());
        for (auto &r: *m->ranges) {
            unsigned long map_begin = r.from;
            unsigned long map_end = r.from + r.size - 1;
//            cout << " - checking range " << r << endl;

            size_t size = split_ranges->size();
            size_t sr = 0;
            while (sr++ < size) {
                auto cr = split_ranges->front();
                split_ranges->pop_front();

//                cout << "    - splitting range " << cr << endl;
                // begin and end are values, not mapping
                unsigned long begin = cr.to;
                unsigned long end = cr.to + cr.size - 1;

                // obvious case
                if (map_end < begin || map_begin > end) {
//                    cout << "      skipped " << cr << endl;
                    // may be split by different range
                    split_ranges->push_back(cr);
                    continue;
                }
                // 4 more cases
                //  - 10-20 -> 5-25 - 1 range (the old one)
                if (begin >= map_begin && end <= map_end) {
//                    cout << "      1" << endl;
                    new_ranges->emplace_back(mapping_range{
                        .from = cr.from,
                        .to = (unsigned long) (cr.from + cr.delta + r.delta),
                        .delta = cr.delta + r.delta,
                        .size = cr.size
                    });
//                    cout << "       - added " << (*new_ranges)[new_ranges->size() - 1] << endl;
                    continue;
                }
                //  - 10-20 -> 11-19 - 3 new ranges
                if (begin < map_begin && end > map_end) {
//                    cout << "      3" << endl;
                    split_ranges->push_back(mapping_range{
                        .from = cr.from,
                        .to = begin + cr.delta,
                        .delta = cr.delta,
                        .size = map_begin - begin
                    });
                    new_ranges->emplace_back(mapping_range{
                        .from = cr.from + (map_begin - begin),
                        .to = (unsigned long) (map_begin + cr.delta + r.delta),
                        .delta = cr.delta + r.delta,
                        .size = map_end - map_begin + 1
                    });
                    split_ranges->push_back(mapping_range{
                        .from = cr.from + (end - map_end) + 1,
                        .to = end + cr.delta,
                        .delta = cr.delta,
                        .size = end - map_end
                    });
//                    cout << "       - pushed " << (*split_ranges)[split_ranges->size() - 2] << endl;
//                    cout << "       - added " << (*new_ranges)[new_ranges->size() - 1] << endl;
//                    cout << "       - pushed " << (*split_ranges)[split_ranges->size() - 1] << endl;
                    continue;
                }
                //  - 10-20 -> 15-25 - 2 new ranges
                if (begin < map_begin) {
//                    cout << "      2a" << endl;
                    split_ranges->push_back(mapping_range{
                        .from = cr.from,
                        .to = cr.from + cr.delta,
                        .delta = cr.delta,
                        .size = map_begin - begin
                    });
                    new_ranges->emplace_back(mapping_range{
                        .from = cr.from + map_begin - begin,
                        .to = (unsigned long) (cr.from + map_begin - begin + cr.delta + r.delta),
                        .delta = cr.delta + r.delta,
                        .size = map_end - end - 1
                    });
//                    cout << "       - pushed " << (*split_ranges)[split_ranges->size() - 1] << endl;
//                    cout << "       - added " << (*new_ranges)[new_ranges->size() - 1] << endl;
                    continue;
                }
                //  - 10-20 -> 5-15 - 2 new ranges
                if (end > map_end) {
//                    cout << "      2b" << endl;
                    new_ranges->emplace_back(mapping_range{
                        .from = cr.from,
                        .to = (unsigned long) (cr.from + cr.delta + r.delta),
                        .delta = cr.delta + r.delta,
                        .size = map_end - begin + 1
                    });
                    split_ranges->push_back(mapping_range{
                        .from = cr.from + (map_end - begin) + 1,
                        .to = (unsigned long) (cr.from + (map_end - begin) + 1 + cr.delta),
                        .delta = cr.delta,
                        .size = end - map_end
                    });
//                    cout << "       - added " << (*new_ranges)[new_ranges->size() - 1] << endl;
//                    cout << "       - pushed " << (*split_ranges)[split_ranges->size() - 1] << endl;
                    continue;
                }
            }

        }
        new_ranges->insert(new_ranges->end(), split_ranges->begin(), split_ranges->end());

        delete one_step_mappings.ranges;
        sort(new_ranges->begin(), new_ranges->end(), [](mapping_range &r1, mapping_range &r2) {
            return r1.from < r2.from;
        });
        auto new_ranges_unique = new vector<mapping_range>;
        unsigned long previous = 0;
        bool first = true;
        for (auto &r: *new_ranges) {
            if (first || previous != r.from) {
                new_ranges_unique->emplace_back(r);
                previous = r.from;
            }
            first = false;
        }
        delete new_ranges;
        one_step_mappings.ranges = new_ranges_unique;
//        cout << "\none step mapping: " << one_step_mappings << endl;
    }

    for (auto &r: *one_step_mappings.ranges) {
        if (r.to < answer2) {
            answer2 = r.to;
        }
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    for (auto &m: mappings) {
        delete m;
    }

    return EXIT_SUCCESS;
}
