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
#include <deque>
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
    vector<string> route_ids;
    vector<valve *> routes;
    map<string, int> travel_times;

    explicit valve(string &n, int p = 0) : name(n), pressure(p) {}
};

struct wanderer {
    valve *current;
    valve *next;
    set<string> *unvisited;
    vector<string> visited;
    int travel_time_to_next = 0;
    int current_pressure = 0;
    int time_left = 0;
};

void bfs(valve *current) {
    queue<valve *> q;

    current->travel_times[current->name] = 0;
    for (auto &other: current->routes) {
        current->travel_times[other->name] = 1;
        q.push(other);
    }

    valve *other;
    while (!q.empty()) {
        other = q.front();
        q.pop();

        for (auto &another: other->routes) {
            if (!current->travel_times.contains(another->name) ||
                current->travel_times[another->name] > current->travel_times[other->name] + 1) {
                current->travel_times[another->name] = current->travel_times[other->name] + 1;
                q.push(another);
            }
        }
    }
}

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
            v->route_ids.push_back(line.substr(start, 2));
            start += 4;
        }
    }

    for (auto &v: valves) {
        for (auto &s: v->route_ids) {
            v->routes.push_back(valve_index[s]);
        }
        v->route_ids.clear();
    }

    for (auto &v: valves) {
        if (v->name == "AA" || v->pressure > 0) {
            bfs(v);
        }
    }

    // part 1

    int max_pressure = 0;
    deque<wanderer> q;
    auto aa = valve_index["AA"];
    set<string> unvisited;
    for (auto &p: valve_index) {
        if (p.second->pressure > 0) {
            unvisited.insert(p.first);
        }
    }
    for (auto &v1: aa->travel_times) {
        valve *v2 = valve_index[v1.first];
        if (v2->pressure > 0) {
            auto w_unvisited = new set<string>(unvisited.begin(), unvisited.end());
            q.push_back(wanderer{ .current = aa, .next = v2, .unvisited = w_unvisited, .travel_time_to_next = v1.second, .time_left = 30 });
        }
    }
    size_t max_size = 0;
    while (!q.empty()) {
        if (max_size < q.size()) {
            max_size = q.size();
        }
        wanderer w = q.back();
        q.pop_back();

        w.current = w.next;
        w.next = nullptr;
        w.unvisited->erase(w.current->name);
        w.visited.push_back(w.current->name);

        // travel
        w.time_left -= min(w.time_left, w.travel_time_to_next);

        if (w.time_left > 0) {
            // turning on
            w.time_left--;
            w.current_pressure += w.current->pressure * w.time_left;
        }

        bool moving = false;
        if (w.time_left > 0) {
            // let's try to move further
            for (auto &u1: *w.unvisited) {
                valve *next1 = valve_index[u1];
                int potential = next1->pressure * (max(0, w.time_left - 1 - w.current->travel_times[u1]));
                for (auto &u2: *w.unvisited) {
                    if (u1 == u2) {
                        continue;
                    }
                    valve *next2 = valve_index[u2];
                    if (w.current->travel_times[u1] + 1 + next1->travel_times[u2] + 1 < w.time_left) {
                        potential += next2->pressure * (max(0, w.time_left - w.current->travel_times[u1] - 1 - next1->travel_times[u2] - 1));
                    }
                }
                if (w.current_pressure + potential > max_pressure && w.unvisited->size() > 1) {
                    wanderer w2 = w;
                    w2.unvisited = new set<string>(w.unvisited->begin(), w.unvisited->end());
                    w2.current = w.current;
                    w2.next = next1;
                    w2.travel_time_to_next = w2.current->travel_times[u1];

                    q.push_back(w2);
                    moving = true;
                }
            }
        }

        if (!moving) {
            if (w.current_pressure > max_pressure) {
                max_pressure = w.current_pressure;
//                cout << "END: " << w.current_pressure << ", AA";
//                valve *current = valve_index["AA"];
//                int left = 30;
//                for (auto &v: w.visited) {
//                    cout << " (" << current->travel_times[v] << ") " << v << "[";
//                    left -= current->travel_times[v] + 1;
//                    cout << valve_index[v]->pressure << "p * " << max(0, left) << "m]";
//                    current = valve_index[v];
//                }
//                cout << "\n";
            }
        }

        delete w.unvisited;
    }

    cout << "Answer 1: " << max_pressure << " (max q size: " << max_size << ")" << endl;

    // part 2

    int answer2 = 0;

    cout << "Answer 2: " << answer2 << endl;

    for (auto &v: valves) {
        delete v;
    }

    return EXIT_SUCCESS;
}
