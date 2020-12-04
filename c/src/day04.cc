/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <regex>

#include "utils/utils.h"

using namespace std;

struct passport {
	unique_ptr<string> byr, iyr, eyr, hgt, hcl, ecl, pid, cid;

	static unique_ptr<regex> hcl_regex;
	static unique_ptr<regex> pid_regex;

	bool is_valid();

	bool is_valid2();

	friend basic_ostream<char>& operator<<(basic_ostream<char> &os, passport &p) {
		return os
		<< "byr:" << (!p.byr ? "" : *p.byr)
		<< " iyr:" << (!p.iyr ? "" : *p.iyr)
		<< " eyr:" << (!p.eyr ? "" : *p.eyr)
		<< " hgt:" << (!p.hgt ? "" : *p.hgt)
		<< " hcl:" << (!p.hcl ? "" : *p.hcl)
		<< " ecl:" << (!p.ecl ? "" : *p.ecl)
		<< " pid:" << (!p.pid ? "" : *p.pid)
		<< " cid:" << (!p.cid ? "" : *p.cid);
	}
};

unique_ptr<regex> passport::hcl_regex = make_unique<regex>(regex("^[0-9a-f]{6}$"));
unique_ptr<regex> passport::pid_regex = make_unique<regex>(regex("^[0-9]{9}$"));

bool passport::is_valid() {
	return this->byr != nullptr
			&& this->iyr != nullptr
			&& this->eyr != nullptr
			&& this->hgt != nullptr
			&& this->hcl != nullptr
			&& this->ecl != nullptr
			&& this->pid != nullptr;
}

bool passport::is_valid2() {
	if (!this->is_valid()) {
		return false;
	}
	bool valid = true;
	try {
		int v = stoi(*this->byr);
		valid &= (v >= 1920 && v <= 2002);
	} catch (invalid_argument &e) {
		return false;
	}
	try {
		int v = stoi(*this->iyr);
		valid &= (v >= 2010 && v <= 2020);
	} catch (invalid_argument &e) {
		return false;
	}
	try {
		int v = stoi(*this->eyr);
		valid &= (v >= 2020 && v <= 2030);
	} catch (invalid_argument &e) {
		return false;
	}
	if (!(this->hgt->ends_with("cm") || this->hgt->ends_with("in"))) {
		return false;
	} else {
		try {
			int v = stoi(this->hgt->substr(0, hgt->length() - 2));
			if (this->hgt->ends_with("cm")) {
				valid &= (v >= 150 && v <= 193);
			} else {
				valid &= (v >= 59 && v <= 76);
			}
		} catch (invalid_argument &e) {
			return false;
		}
	}
	if (!this->hcl->starts_with("#")) {
		return false;
	} else {
		valid = valid && regex_match(this->hcl->substr(1).c_str(), *passport::hcl_regex);
	}
	valid = valid &&
			(*this->ecl == "amb" || *this->ecl == "blu" || *this->ecl == "brn" || *this->ecl == "gry"
					|| *this->ecl == "grn" || *this->ecl == "hzl" || *this->ecl == "oth");
	valid = valid && regex_match(this->pid->c_str(), *passport::pid_regex);

	return valid;
}

int main(int argc, char *argv[]) {
	aoc2020::Options options("Day 04", argc, argv);
	if (!options.check())
		return options.result();

	cout << "Starting " << options.program_name << endl;

	vector<unique_ptr<passport>> passports;

	passport *currentPassport = new passport{};

	string line;
	while (getline(*options.file(), line)) {
		aoc2020::trim(line);
		if (line.empty()) {
			passports.push_back(unique_ptr<passport>(currentPassport));
			currentPassport = new passport{};
		}
		istringstream iss(line);
		string v;
		for (;;) {
			iss >> v;
			cout << "\t[" << v << "]" << endl;
			if (v.empty()) {
				break;
			}
			if (v.starts_with("byr")) {
				currentPassport->byr = make_unique<string>(v.substr(4));
			} else if (v.starts_with("iyr")) {
				currentPassport->iyr = make_unique<string>(v.substr(4));
			} else if (v.starts_with("eyr")) {
				currentPassport->eyr = make_unique<string>(v.substr(4));
			} else if (v.starts_with("hgt")) {
				currentPassport->hgt = make_unique<string>(v.substr(4));
			} else if (v.starts_with("hcl")) {
				currentPassport->hcl = make_unique<string>(v.substr(4));
			} else if (v.starts_with("ecl")) {
				currentPassport->ecl = make_unique<string>(v.substr(4));
			} else if (v.starts_with("pid")) {
				currentPassport->pid = make_unique<string>(v.substr(4));
			} else if (v.starts_with("cid")) {
				currentPassport->cid = make_unique<string>(v.substr(4));
			}
			v.clear();
		}
	}
	passports.push_back(unique_ptr<passport>(currentPassport));

	for (auto &p : passports) {
		cout << ">> " << *p << endl;
	}

	// part 1

	int count1 = 0;
	for (auto &p : passports) {
		if (p->is_valid()) {
			count1++;
		}
	}

	// part 2

	int count2 = 0;
	for (auto &p : passports) {
		if (p->is_valid2()) {
			count2++;
		}
	}

	cout << "Answer 1: " << count1 << endl;
	cout << "Answer 2: " << count2 << endl;

	return EXIT_SUCCESS;
}
