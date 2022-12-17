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
                            int current_pressure, int current_pressure_increase, int to_visit, int current_max);
int calculate_pressure_from_2(const char *start, map<string, valve *> &index,
                            set<string> &visited, set<string> &turned_on, int tick,
                            int current_pressure, int current_pressure_increase, int to_visit);

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

    ofstream ofs("/data/tmp/day16a.dot");
    ofs << "digraph {\n";
    ofs << "    rankdir=LR;\n";
    for (auto &v: valves) {
//        ofs << "    " << v->name << " [label=" << valve_index["EE"]->travel_times[v->name] << "]\n";
        ofs << "    " << v->name << " [label=\"" << v->name << ", " << valve_index["AA"]->travel_times[v->name] << "\"]\n";
    }
    for (auto &v: valves) {
        for (auto &s: v->outputs) {
            ofs << "    " << v->name << " -> " << s << "\n";
        }
    }
    ofs << "}\n";
    ofs.close();

    // part 1

    set<string> visited;
    int to_visit = 0;
    for (auto &v: valves) {
        if (v->pressure > 0) {
            to_visit++;
        }
    }
    set<string> turned_on;
    visited.insert("AA");
    int answer1 = calculate_pressure_from("AA", valve_index, visited, turned_on, 1, 0, 0, to_visit, 0);

    // part 2

    visited.clear();
    turned_on.clear();
    visited.insert("AA");
    int answer2 = 0;
//    int answer2 = calculate_pressure_from_2("AA", valve_index, visited, turned_on, 1, 0, 0, to_visit);

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    for (auto &v: valves) {
        delete v;
    }

    return EXIT_SUCCESS;
}

int calculate_pressure_from(const char *start, map<string, valve *> &index, // NOLINT(misc-no-recursion)
                            set<string> &visited, set<string> &turned_on, int tick,
                            int current_pressure, int current_pressure_increase, int to_visit, int current_max) {
    if (tick >= 30) {
        return 0;
    }

    valve *from = index[start];

    if (!turned_on.contains(from->name) && from->pressure > 0) {
        turned_on.insert(from->name);
        // we know exactly how this just turned on valve will increase the pressure till the end of 30 minutes
        current_pressure += from->pressure * (30-tick);
        // a minute spent on turning on the valve
        tick++;
    }

    int potential_pressure = 0;
    for (auto &p: index) {
        if (!turned_on.contains(p.first)) {
            potential_pressure += p.second->pressure * (30 - tick);
        }
    }
    potential_pressure = 0;
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
    sort(best.begin(), best.end(), [&index](const pair<string, int> &p1, const pair<string, int> &p2) {
//        return index[p1.first]->pressure * p1.second > index[p2.first]->pressure * p2.second;
        return p1.second < p2.second;
    });

    for (size_t i=0; i<min((size_t) 3, best.size()); i++) {
//        if (best.size() <= i) {
//            break;
//        }
        pair<string, int> p = best[i];
        valve *to = index[p.first];
        // there's a point to travel there. don't mark valve as "on", but mark it as visited
        set<string> _visited(visited.begin(), visited.end());
        _visited.insert(to->name);
        set<string> _turned_on(turned_on.begin(), turned_on.end());
        int travel_time = p.second;
        // recursively start from valve node, but using visit tick as current tick + travel time + switch on time (1)
        int _max = calculate_pressure_from(to->name.c_str(), index, _visited, _turned_on,
                tick + travel_time, current_pressure, current_pressure_increase, to_visit - 1, max);
        if (_max > max) {
            max = _max;
        }
    }

//    cout << "tick: " << tick << ", max: " << max << "\n";
//    cout << "returning: " << (current_pressure + max) << "\n";
    return max;
}

int calculate_pressure_from_2(const char *start, map<string, valve *> &index, // NOLINT(misc-no-recursion)
                            set<string> &visited, set<string> &turned_on, int tick,
                            int current_pressure, int current_pressure_increase, int to_visit) {
    if (tick > 30) {
        return 0;
    }

    valve *from = index[start];

    if (!turned_on.contains(from->name) && from->pressure > 0) {
        turned_on.insert(from->name);
        // we know exactly how this just turned on valve will increase the pressure till the end of 30 minutes
        current_pressure += from->pressure * (30-tick);
        // a minute spent on turning on the valve
        tick++;
    }

    int max = current_pressure;

    for (auto &p: from->travel_times) {
        valve *to = index[p.first];
        if (to->pressure > 0 && !visited.contains(to->name)) {
            // there's a point to travel there. don't mark valve as "on", but mark it as visited
            set<string> _visited(visited.begin(), visited.end());
            _visited.insert(to->name);
            set<string> _turned_on(turned_on.begin(), turned_on.end());
            int travel_time = p.second;
            // recursively start from valve node, but using visit tick as current tick + travel time + switch on time (1)
            int _max = calculate_pressure_from_2(to->name.c_str(), index, _visited, _turned_on,
                    tick + travel_time, current_pressure, current_pressure_increase, to_visit - 1);
            if (_max > max) {
                max = _max;
            }
        }
    }

//    cout << "tick: " << tick << ", max: " << max << "\n";
//    cout << "returning: " << (current_pressure + max) << "\n";
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
