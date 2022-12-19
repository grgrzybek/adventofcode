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
    valve *el_current;
    valve *next;
    valve *el_next;
    set<string> *unvisited;
    vector<string> visited;
    vector<string> el_visited;
    int travel_time_to_next = 0;
    int el_travel_time_to_next = 0;
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

    int answer1 = 0;
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
            q.push_back(
                    wanderer{ .current = aa, .next = v2, .unvisited = w_unvisited, .travel_time_to_next = v1.second, .time_left = 30 });
        }
    }
    while (!q.empty()) {
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
                        potential += next2->pressure * (max(0,
                                w.time_left - w.current->travel_times[u1] - 1 - next1->travel_times[u2] - 1));
                    }
                }
                if (w.current_pressure + potential > answer1) {
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
            if (w.current_pressure > answer1) {
                answer1 = w.current_pressure;
            }
        }

        delete w.unvisited;
    }

    cout << "Answer 1: " << answer1 << endl;

    // part 2

    int answer2 = 0;
    q.clear();
    aa = valve_index["AA"];
    unvisited.clear();
    for (auto &p: valve_index) {
        if (p.second->pressure > 0) {
            unvisited.insert(p.first);
        }
    }
    auto w_unvisited = new set<string>(unvisited.begin(), unvisited.end());
    q.push_back(wanderer{ .current = nullptr, .el_current = nullptr, .next = aa, .el_next = aa,
            .unvisited = w_unvisited, .travel_time_to_next = 0, .el_travel_time_to_next = 0, .time_left = 26 });

    while (!q.empty()) {
        wanderer w = q.back();
        q.pop_back();

        // find the time passed to a moment when something interesting happens
        int passed_time = 0;
        if (w.next != nullptr && w.el_next != nullptr) {
            passed_time = min(w.travel_time_to_next, w.el_travel_time_to_next);
        } else if (w.next != nullptr) {
            passed_time = w.travel_time_to_next;
        } else if (w.el_next != nullptr) {
            passed_time = w.el_travel_time_to_next;
        }
        w.time_left -= min(w.time_left, passed_time);

        w.travel_time_to_next -= passed_time;
        w.el_travel_time_to_next -= passed_time;

        if (w.travel_time_to_next <= 0) {
            // I've arrived
            if (w.next == nullptr) {
                cout << "";
            }
            w.current = w.next;
            w.next = nullptr;
            if (w.current != nullptr) {
                if (w.current->name != "AA") {
                    w.visited.push_back(w.current->name);
                }
                w.unvisited->erase(w.current->name);
            }
        }
        if (w.el_travel_time_to_next <= 0) {
            // Elephant has arrived
            if (w.el_next == nullptr) {
                cout << "";
            }
            w.el_current = w.el_next;
            w.el_next = nullptr;
            if (w.el_current != nullptr) {
                if (w.el_current->name != "AA") {
                    w.el_visited.push_back(w.el_current->name);
                }
                w.unvisited->erase(w.el_current->name);
            }
        }

        if (w.visited.size() == 1 && w.visited[0] == "JJ" && w.el_visited.size() == 1 && w.el_visited[0] == "DD") {
            cout << "\n";
        }

        if (w.time_left > 0 && w.current != nullptr && w.current->name != "AA") {
            w.time_left--;
            if (w.next == nullptr && w.el_next == nullptr) {
                // we've both arrived - easy
                if (w.current != nullptr) {
                    w.current_pressure += w.current->pressure * (w.time_left);
                }
                if (w.el_current != nullptr) {
                    w.current_pressure += w.el_current->pressure * (w.time_left);
                }
            } else if (w.next != nullptr) {
                // Only elephant has arrived
                if (w.el_current != nullptr) {
                    w.current_pressure += w.el_current->pressure * (w.time_left);
                }
                w.travel_time_to_next--;
            } else if (w.el_next != nullptr) {
                // Only I have arrived
                if (w.current != nullptr) {
                    w.current_pressure += w.current->pressure * (w.time_left);
                }
                w.el_travel_time_to_next--;
            }
        }

        bool moving = false;
        if (w.time_left > 0) {
            // let's try to move further - me and the elephant
            vector<valve *> to_visit;
            for (auto &u1: *w.unvisited) {
                valve *next1 = valve_index[u1];
                if (next1 == w.next || next1 == w.el_next) {
                    // me or the elephant is already going there
                    continue;
                }
                int my_potential = w.current != nullptr ? next1->pressure * (max(0, w.time_left - 1 - w.current->travel_times[u1])) : 0;
                int el_potential = w.el_current != nullptr ? next1->pressure * (max(0, w.time_left - 1 - w.el_current->travel_times[u1])) : 0;
                for (auto &u2: *w.unvisited) {
                    if (u1 == u2) {
                        continue;
                    }
                    valve *next2 = valve_index[u2];
                    if (w.current != nullptr) {
                        if (w.current->travel_times[u2] < w.time_left) {
                            my_potential += next2->pressure * (max(0,
                                    w.time_left - w.current->travel_times[u2]));
                        }
                    }
                    if (w.el_current != nullptr) {
                        if (w.el_current->travel_times[u2] < w.time_left) {
                            el_potential += next2->pressure * (max(0,
                                    w.time_left - w.el_current->travel_times[u2]));
                        }
                    }
                }
                if (w.current_pressure + max(my_potential, el_potential) > answer2) {
                    to_visit.push_back(next1);
                }
            }
            if (w.next == nullptr && w.el_next == nullptr && w.current != nullptr && w.el_current != nullptr) {
                // we both can go
                if (to_visit.size() == 1) {
                    if (w.current != nullptr) {
                        wanderer w2a = w;
                        w2a.unvisited = new set<string>(w.unvisited->begin(), w.unvisited->end());
                        w2a.next = to_visit[0];
                        w2a.el_next = nullptr;
                        w2a.travel_time_to_next = w2a.current->travel_times[w2a.next->name];
                        w2a.el_travel_time_to_next = 0;
                        q.push_back(w2a);
                        moving = true;
                    }

                    if (w.el_current != nullptr) {
                        wanderer w2b = w;
                        w2b.unvisited = new set<string>(w.unvisited->begin(), w.unvisited->end());
                        w2b.next = nullptr;
                        w2b.el_next = to_visit[0];
                        w2b.travel_time_to_next = 0;
                        w2b.el_travel_time_to_next = w2b.el_current->travel_times[w2b.el_next->name];
                        q.push_back(w2b);
                        moving = true;
                    }
                } else {
                    for (size_t i = 0; i < to_visit.size(); i++) {
                        for (size_t j = 0; j < to_visit.size(); j++) {
                            if (to_visit[i] == to_visit[j]) {
                                continue;
                            }
                            wanderer w2 = w;
                            w2.unvisited = new set<string>(w.unvisited->begin(), w.unvisited->end());
                            w2.next = to_visit[i];
                            w2.el_next = to_visit[j];
                            w2.travel_time_to_next = w2.current->travel_times[w2.next->name];
                            w2.el_travel_time_to_next = w2.el_current->travel_times[w2.el_next->name];
                            q.push_back(w2);
                            moving = true;
                        }
                    }
                }
            } else {
                // I or elephant can go further
                for (size_t i = 0; i < to_visit.size(); i++) {
                    wanderer w2 = w;
                    w2.unvisited = new set<string>(w.unvisited->begin(), w.unvisited->end());
                    if (w.el_next == nullptr && w.el_current != nullptr) {
                        // elephant
                        w2.el_next = to_visit[i];
                        w2.el_travel_time_to_next = w2.el_current->travel_times[w2.el_next->name];
                    } else if (w.next == nullptr && w.current != nullptr) {
                        // me
                        w2.next = to_visit[i];
                        w2.travel_time_to_next = w2.current->travel_times[w2.next->name];
                    }
                    q.push_back(w2);
                    moving = true;
                }
            }
        }

        if (!moving) {
            if (w.next != nullptr || w.el_next != nullptr) {
                wanderer w2 = w;
                w2.unvisited = new set<string>(w.unvisited->begin(), w.unvisited->end());
                q.push_back(w2);
            } else if (w.current_pressure > answer2) {
                answer2 = w.current_pressure;
//                cout << "END: " << w.current_pressure << "\n    me:       AA";
//                valve *current = valve_index["AA"];
//                int left = 26;
//                for (auto &v: w.visited) {
//                    cout << " (" << current->travel_times[v] << ") " << v << "[";
//                    left -= current->travel_times[v] + 1;
//                    cout << valve_index[v]->pressure << "p * " << max(0, left) << "m]";
//                    current = valve_index[v];
//                }
//                cout << "\n    elephant: AA";
//                current = valve_index["AA"];
//                left = 26;
//                for (auto &v: w.el_visited) {
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

    cout << "Answer 2: " << answer2 << endl;

    for (auto &v: valves) {
        delete v;
    }

    return EXIT_SUCCESS;
}
