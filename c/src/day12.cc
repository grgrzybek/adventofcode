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

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;
using namespace aoc2021;

typedef struct {
    string name;
    vector<string> outs;
} node;

void collect_paths(string &&name, string &&path, map<string, node *> &cave, set<string> &paths);
void collect_paths2(string &&name, string &&path, map<string, node *> &cave, set<string> &paths);

int main(int argc, char *argv[]) {
    Options options("Day 12", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    ifstream *input = options.file();

    string line;
    vector<string> lines;

    while (getline(*input, line)) {
        trim(line);
        lines.push_back(line);
    }

    // part 1

    map<string, node*> cave;

    for (auto &l: lines) {
        string::size_type pos = l.find('-');
        string from = string(l, 0, pos);
        string to = string(l, pos + 1, l.size());

        if (!cave.contains(from)) {
            cave[from] = new node;
            cave[from]->name = from;
        }
        if (!cave.contains(to)) {
            cave[to] = new node;
            cave[to]->name = to;
        }
        cave[from]->outs.push_back(to);
        cave[to]->outs.push_back(from);
    }

    set<string> paths;
    collect_paths(string("start"), string("-start-"), cave, paths);
    set<string>::size_type answer1 = paths.size();

    // part 2

    paths.clear();
    collect_paths2(string("start"), string("-start-"), cave, paths);
    set<string>::size_type answer2 = paths.size();

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    for (auto & it : cave) {
        delete it.second;
    }

    return EXIT_SUCCESS;
}

void collect_paths(string &&name, string &&path, map<string, node *> &cave, set<string> &paths) { // NOLINT(misc-no-recursion)
    if (!cave.contains(name)) {
        return;
    }
    for (auto &next: cave[name]->outs) {
        if (next[0] >= 'a' && next[0] <= 'z') {
            // can't repeat
            if (next == "end") {
                // end of traversing - add to paths
                paths.emplace(path + "end");
            } else if (path.find("-" + next + "-") == string::npos) {
                collect_paths(string(next), string(path + next + "-"), cave, paths);
            }
        } else {
            // can always repeat
            collect_paths(string(next), string(path + next + "-"), cave, paths);
        }
    }
}

void collect_paths2(string &&name, string &&path, map<string, node *> &cave, set<string> &paths) { // NOLINT(misc-no-recursion)
    if (!cave.contains(name)) {
        return;
    }
    for (auto &next: cave[name]->outs) {
        if (next == "start") {
            continue;
        }
        if (next[0] >= 'a' && next[0] <= 'z') {
            // can repeat only once for one small cave
            if (next == "end") {
                // end of traversing - add to paths
                paths.emplace(path + "end");
            } else if (path.find("-" + next + "-") == string::npos) {
                // safe - no repeat
                collect_paths2(string(next), string(path + next + "-"), cave, paths);
            } else if (path[0] != '!' && path.find("-" + next + "-") != string::npos) {
                // repeat - allow once
                string p = "!";
                p.append(path + next + "-");
                collect_paths2(string(next), move(p), cave, paths);
            }
        } else {
            // can always repeat
            collect_paths2(string(next), string(path + next + "-"), cave, paths);
        }
    }
}
