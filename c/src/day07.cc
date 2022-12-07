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
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "utils/utils.h"

using namespace std;

struct dir {
    string name;
    unsigned long size;
    dir *parent = nullptr;
    vector<dir *> children;
    map<string, unsigned long> files;

    explicit dir(string name, dir *parent) : name(std::move(name)), size(0l), parent(parent) {}

    ~dir() {
        for (auto c: children) {
            delete c;
        }
    }

    dir *get_child(string &where) const {
        for (auto d: children) {
            if (d->name == where) {
                return d;
            }
        }
        return nullptr;
    }

    void add_dir(dir *child) {
        children.push_back(child);
    }

    unsigned long calculate_size() { // NOLINT(misc-no-recursion)
        for (auto &ns: files) {
            size += ns.second;
        }
        for (auto &child: children) {
            size += child->calculate_size();
        }
        return size;
    }

    void ls(int indent) const { // NOLINT(misc-no-recursion)
        for (int i = 0; i < indent; i++) {
            cout << " ";
        }
        cout << "DIR: " << name << " (" << size << ")" << endl;
        for (auto &f: files) {
            for (int i = 0; i < indent + 2; i++) {
                cout << " ";
            }
            cout << f.first << " (" << f.second << ")" << endl;
        }
        for (auto d: children) {
            d->ls(indent + 2);
        }
    }
};

void find_answer1(dir *dir, unsigned long *answer);

void find_answer2(dir *dir, vector<unsigned long> &sizes, unsigned long to_delete);

int main(int argc, char *argv[]) {
    aoc2022::Options options("Day 07", argc, argv);
    if (!options.check())
        return options.result();

    cout << "Starting " << options.program_name << endl;

    // $ cd /
    // $ ls
    // dir a
    // 14848514 b.txt
    // 8504156 c.dat
    // dir d
    // $ cd a
    // $ ls
    // dir e
    // 29116 f
    // 2557 g
    // 62596 h.lst
    // $ cd e
    // $ ls
    // 584 i
    // $ cd ..
    // $ cd ..
    // $ cd d
    // $ ls
    // 4060174 j
    // 8033020 d.log
    // 5626152 d.ext
    // 7214296 k

    ifstream *input = options.file();

    string line;

    dir *root = new dir("/", nullptr);
    dir *pwd = root;
    string name;
    unsigned long size;

    while (getline(*input, line)) {
        if (line[0] != '$') {
            // collect dir and file entries into pwd
            if (line[0] == 'd' && line[1] == 'i' && line[2] == 'r') {
                pwd->add_dir(new dir(line.substr(4), pwd));
            } else {
                istringstream iss(line);
                iss >> size >> name;
                pwd->files[name] = size;
            }
        } else {
            // command
            if (line[2] == 'c' && line[3] == 'd') {
                // cd
                dir *cd = nullptr;
                string where = line.substr(5);
                if (where == "/") {
                    cd = root;
                } else if (where == ".") {
                    cd = pwd;
                } else if (where == "..") {
                    cd = pwd->parent;
                } else {
                    cd = pwd->get_child(where);
                    if (cd == nullptr) {
                        cd = new dir(where, pwd);
                        pwd->add_dir(cd);
                    }
                }
                pwd = cd;
            } else if (line[2] == 'l' && line[3] == 's') {
                // ls
            }
        }
    }

    // part 1

    unsigned long answer1 = 0;
    root->calculate_size();
    root->ls(0);

    find_answer1(root, &answer1);

    // part 2

    unsigned long answer2 = 0;
    unsigned long space = 70000000;
    space -= root->size;
    unsigned long to_delete = 30000000 - space;
    if (to_delete > 0) {
        vector<unsigned long> sizes;
        find_answer2(root, sizes, to_delete);
        sort(sizes.begin(), sizes.end());
        answer2 = sizes[0];
    }

    cout << "Answer 1: " << answer1 << endl;
    cout << "Answer 2: " << answer2 << endl;

    delete root;

    return EXIT_SUCCESS;
}

void find_answer1(dir *dir, unsigned long *answer) { // NOLINT(misc-no-recursion)
    if (dir->size <= 100000) {
        *answer += dir->size;
    }
    for (auto &d: dir->children) {
        find_answer1(d, answer);
    }
}

void find_answer2(dir *dir, vector<unsigned long> &sizes, unsigned long to_delete) { // NOLINT(misc-no-recursion)
    if (dir->size >= to_delete) {
        sizes.push_back(dir->size);
    }
    for (auto d: dir->children) {
        find_answer2(d, sizes, to_delete);
    }
}
