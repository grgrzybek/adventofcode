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

#include <algorithm>
#include <cstring>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <vector>

#include "utils/utils.h"

using namespace std;

struct valve {
    string name;
    const int pressure = 0;
    bool on = false;
    vector<string> outputs;
    vector<valve *> output_valves;
    map<string, int> travel_times;

    explicit valve(string &n, int p = 0) : name(n), pressure(p) {}

    [[maybe_unused]] void print() const {
        cout << "[" << name << ", p:" << pressure << " (" << (on ? "on" : "off") << ")]\n";
        cout << "    -> ";
        bool first = true;
        for (auto &v: output_valves) {
            if (!first) {
                cout << ", ";
            }
            cout << v->name;
            first = false;
        }
        cout << "\n";
    }
};

void bfs(valve *start);

int calculate_pressure_from(const char *start, map<string, valve *> &index,
                            set<string> &visited, set<string> &turned_on, int tick,
                            int current_pressure, int current_pressure_increase, int current_max);

void calculate_combinations(vector<valve *> &valves, vector<valve *> &current, int size, int cidx, int iidx,
                            list<vector<valve *>> &result);

void calculate_permutations(vector<valve *> &valves, int size, list<vector<valve *>> &result);

int main(int argc, char *argv[]) {
    aoc2022::Options options("Day 16", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<valve *> valves;
    map<string, valve *> valve_index;

    while (getline(*input, line)) {
        if (!aoc2022::has_text(line)) {
            continue;
        }

        // Valve AA has flow rate=0; tunnels lead to valves DD, II, BB
        size_t start = line.find(' ') + 1;
        size_t pos = line.find(' ', start);
        string name = line.substr(start, pos - start);

        start = line.find('=', pos) + 1;
        pos = line.find(';', start);
        int pressure = stoi(line.substr(start, pos - start));

        auto v = new valve(name, pressure);
        valves.push_back(v);
        valve_index[name] = v;

        start = line.find("to valve", pos) + 8;
        if (line[start] == 's') {
            start++;
        }
        start++;
        while (start < line.length()) {
            v->outputs.push_back(line.substr(start, 2));
            start += 4;
        }
    }

    for (auto &v: valves) {
        for (auto &s: v->outputs) {
            v->output_valves.push_back(valve_index[s]);
        }
//        v->print();
    }

    for (auto &v: valves) {
        if (v->name == "AA" || v->pressure > 0) {
            bfs(v);
        }
    }

    // part 1

    set<string> visited;
    set<string> turned_on;
    visited.insert("AA");
    int answer1 = calculate_pressure_from("AA", valve_index, visited, turned_on, 1, 0, 0, 0);

    cout << "Answer 1: " << answer1 << endl;

    // part 2

    vector<valve *> remaining;
    for (auto &v: valves) {
        if (v->pressure > 0) {
            remaining.push_back(v);
        }
    }
    valve *aa = valve_index["AA"];
    sort(remaining.begin(), remaining.end(), [aa](valve *v1, valve *v2) {
        return aa->travel_times[v1->name] < aa->travel_times[v2->name];
    });

    const int COMBINATION_SIZE = 6;

    int answer2 = 0;

    list<valve *> remaining_list(remaining.begin(), remaining.end());
    list<valve *> my_list;
    list<valve *> my_full_list;
    list<valve *> my_best_full_list;
    list<valve *> el_list;
    list<valve *> el_full_list;
    list<valve *> el_best_full_list;
    while (my_best_full_list.size() + el_best_full_list.size() < remaining.size()) {
        if (remaining_list.empty()) {
            cout << "!!! my list: " << my_best_full_list.size() << ", el list: " << el_best_full_list.size() << "\n";
            break;
        }
        vector<valve *> current_combination;
        int combination_size = min((int) remaining_list.size(), COMBINATION_SIZE);
        for (int i = 0; i < combination_size; i++) {
            current_combination.push_back(nullptr);
        }
        vector<valve *> remaining_v(remaining_list.begin(), remaining_list.end());
        list<vector<valve *>> combinations;
        calculate_combinations(remaining_v, current_combination, combination_size, 0, 0, combinations);
        list<vector<valve *>> permutations;
        for (auto &v: combinations) {
            calculate_permutations(v, combination_size, permutations);
        }

        int max = 0;
        list<valve *> my_best_perm;
        list<valve *> el_best_perm;

        cout << "checking " << permutations.size() << " permutations, remaining: " << remaining_list.size() << "\n";
        for (auto &perm: permutations) {
            if (remaining_list.size() == 2) {
                cout << "2\n";
            }
//            cout << "PERM: ";
//            for (auto &v: perm) {
//                cout << v->name << " ";
//            }
//            if (perm[0]->name == "JJ" && perm[1]->name == "BB" && perm[2]->name == "CC"
//                    && perm[3]->name == "DD" && perm[4]->name == "HH" && perm[5]->name == "EE") {
//                cout << "--\n";
//            }
//            cout << "\n";
            for (int split = 0; split < combination_size; split++) {
                my_list.assign(perm.begin(), perm.begin() + split);
                my_full_list.assign(my_best_full_list.begin(), my_best_full_list.end());
                my_full_list.insert(my_full_list.end(), my_list.begin(), my_list.end());
                el_list.assign(perm.begin() + split, perm.end());
                el_full_list.assign(el_best_full_list.begin(), el_best_full_list.end());
                el_full_list.insert(el_full_list.end(), el_list.begin(), el_list.end());

                valve *current = valve_index["AA"];
                int tick = 0;
                int my_pressure = 0;
                int my_pressure_increase = 0;
                for (auto &my_valve: my_full_list) {
                    if (tick >= 26) {
                        break;
                    }
                    my_pressure += (min(current->travel_times[my_valve->name], 26 - tick) * my_pressure_increase);
                    tick += current->travel_times[my_valve->name];
                    if (tick < 26) {
                        // there's time to open the valve
                        my_pressure += (min(1, 26 - tick) * my_pressure_increase);
                        tick++;
                        my_pressure_increase += my_valve->pressure;
                    }
                    current = my_valve;
                }
                my_pressure += (::max(0, 26 - tick) * my_pressure_increase);
                current = valve_index["AA"];
                tick = 0;
                int el_pressure = 0;
                int el_pressure_increase = 0;
                for (auto &el_valve: el_full_list) {
                    if (tick >= 26) {
                        break;
                    }
                    el_pressure += (min(current->travel_times[el_valve->name], 26 - tick) * el_pressure_increase);
                    tick += current->travel_times[el_valve->name];
                    if (tick < 26) {
                        // there's time to open the valve
                        el_pressure += (min(1, 26 - tick) * el_pressure_increase);
                        tick++;
                        el_pressure_increase += el_valve->pressure;
                    }
                    current = el_valve;
                }
                el_pressure += (::max(0, 26 - tick) * el_pressure_increase);

                if (my_pressure + el_pressure > max) {
                    max = my_pressure + el_pressure;
                    cout << "new local max: " << max << "\n";
                    my_best_perm.assign(my_list.begin(), my_list.end());
                    el_best_perm.assign(el_list.begin(), el_list.end());
                }
            }
        }

//        if (max > answer2) {
            cout << "new max: " << max << "\n";
            if (!my_best_perm.empty()) {
                my_best_full_list.push_back(*my_best_perm.begin());
                remaining_list.remove_if([&my_best_perm](auto &tr) {
                    return tr->name == (*my_best_perm.begin())->name;
                });
            }
            if (!el_best_perm.empty()) {
                el_best_full_list.push_back(*el_best_perm.begin());
                remaining_list.remove_if([&el_best_perm](auto &tr) {
                    return tr->name == (*el_best_perm.begin())->name;
                });
            }
            answer2 = max;
//        }

//        for (auto &v: my_best_perm) {
//            remaining_list.remove_if([&v](auto &tr) {
//                return tr->name == v->name;
//            });
//        }
//        for (auto &v: el_best_perm) {
//            remaining_list.remove_if([&v](auto &tr) {
//                return tr->name == v->name;
//            });
//        }
        cout << "remaining " << remaining_list.size() << "\n";
    }

    cout << "Answer 2: " << answer2 << endl;

    for (auto &v: valves) {
        delete v;
    }

    return EXIT_SUCCESS;
}

void calculate_combinations(vector<valve *> &valves, vector<valve *> &current, int size, int cidx, int iidx, // NOLINT(misc-no-recursion)
                            list<vector<valve *>> &result) {
    if (cidx == size) {
        // end of current combination
        result.push_back(current);
        return;
    }

    if (iidx >= (int) valves.size()) {
        // end of input data
        return;
    }

    current[cidx] = valves[iidx];
    calculate_combinations(valves, current, size, cidx + 1, iidx + 1, result);
    calculate_combinations(valves, current, size, cidx, iidx + 1, result);
}

void
calculate_permutations(vector<valve *> &valves, int size, list<vector<valve *>> &result) { // NOLINT(misc-no-recursion)
    if (size == 1) {
        result.push_back(valves);
        return;
    }

    for (int i = 0; i < size; i++) {
        calculate_permutations(valves, size - 1, result);
        if (size % 2 == 1) {
            swap(valves[0], valves[size - 1]);
        } else {
            swap(valves[i], valves[size - 1]);
        }
    }
}

int calculate_pressure_from(const char *start, map<string, valve *> &index, // NOLINT(misc-no-recursion)
                            set<string> &visited, set<string> &turned_on, int tick,
                            int current_pressure, int current_pressure_increase, int current_max) {
    if (tick >= 30) {
        return 0;
    }

    valve *from = index[start];

    if (!turned_on.contains(from->name) && from->pressure > 0) {
        turned_on.insert(from->name);
        // we know exactly how this just turned on valve will increase the pressure till the end of 30 minutes
        current_pressure += from->pressure * (30 - tick);
        // a minute spent on turning on the valve
        tick++;
    }

    int potential_pressure = 0;
    for (auto &p: index) {
        if (!turned_on.contains(p.first)) {
            potential_pressure += p.second->pressure * (30 - tick);
        }
    }
    if (current_pressure + potential_pressure < current_max) {
//        cout << "no need to go (" << current_pressure << ", " << potential_pressure << ", " << current_max << "\n";
        return current_pressure;
    }

    int max = current_pressure;

    vector<pair<string, int>> best;
    for (auto &p: from->travel_times) {
        if (!turned_on.contains(p.first) && index[p.first]->pressure > 0) {
            best.emplace_back(p);
        }
    }
//    sort(best.begin(), best.end(), [&index](const pair<string, int> &p1, const pair<string, int> &p2) {
//        return index[p1.first]->pressure * p1.second > index[p2.first]->pressure * p2.second;
    sort(best.begin(), best.end(), [](const pair<string, int> &p1, const pair<string, int> &p2) {
        return p1.second < p2.second;
    });

    for (size_t i = 0; i < min((size_t) 3, best.size()); i++) {
        pair<string, int> p = best[i];
        valve *to = index[p.first];
        // there's a point to travel there. don't mark valve as "on", but mark it as visited
        set<string> _visited(visited.begin(), visited.end());
        _visited.insert(to->name);
        set<string> _turned_on(turned_on.begin(), turned_on.end());
        int travel_time = p.second;
        // recursively start from valve node, but using visit tick as current tick + travel time + switch on time (1)
        int _max = calculate_pressure_from(to->name.c_str(), index, _visited, _turned_on,
                tick + travel_time, current_pressure, current_pressure_increase, max);
        if (_max > max) {
            max = _max;
        }
    }

    return max;
}

void bfs(valve *start) {
    queue<valve *> q;

    start->travel_times[start->name] = 0;
    for (auto &to: start->output_valves) {
        start->travel_times[to->name] = 1;
        q.push(to);
    }

    valve *v;
    while (!q.empty()) {
        v = q.front();
        q.pop();

        for (auto &to: v->output_valves) {
            if (!start->travel_times.contains(to->name) ||
                start->travel_times[to->name] > start->travel_times[v->name] + 1) {
                start->travel_times[to->name] = start->travel_times[v->name] + 1;
                q.push(to);
            }
        }
    }
}
