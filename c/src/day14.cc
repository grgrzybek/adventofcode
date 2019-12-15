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

#include <cstdio>

#include <iostream>
#include <fstream>
#include <sstream>

#include <map>
#include <vector>

#include "utils/utils.h"

using namespace std;

struct Ingredient {
	string material;
	long amount = 0;

	Ingredient(string &_material, int _amount) : material(_material), amount(_amount) {}

	friend basic_ostream<char> &operator<<(basic_ostream<char> &os, Ingredient &i) {
		return os << i.material << ": " << i.amount;
	}
};

void make(long amount, const string &material, map<string, long> *stat, map<string, long> *stock, map<string, pair<int, vector<Ingredient> *>> *reactions) {
//	cout << "Creating " << amount << " of " << material << endl;
	while ((*stock)[material] < amount) {
		for (auto &r : *(*reactions)[material].second) {
			make(r.amount, r.material, stat, stock, reactions);
			(*stock)[r.material] -= r.amount;
		}
		(*stock)[material] += (*reactions)[material].first;
		(*stat)[material] += (*reactions)[material].first;
	}
}

void make2(long amount, const string &material, map<string, long> *stat, map<string, long> *stock, map<string, pair<int, vector<Ingredient> *>> *reactions, int indent = 0) {
//	if (material == "ORE")
	if ((*stock)[material] < amount) {
//		for (int i = 0; i < indent; i++)
//			cout << " ";
//		cout << "Creating " << amount << " of " << material << " (currently: " << (*stock)[material] << "), recipe gives " << (*reactions)[material].first << endl;
		// lacking "amount - (*stock)[material]" of required material
		// (*reactions)[material].first will be created when applying the reaction
		int will_get = (*reactions)[material].first;
		long times = ((amount - (*stock)[material] - 1) / will_get) + 1;
//		for (int i = 0; i < indent; i++)
//			cout << " ";
//		cout << "Will create " << times * will_get << " of " << material << endl;
		for (auto &r : *(*reactions)[material].second) {
//			for (int i = 0; i < indent; i++)
//				cout << " ";
//			cout << "Ingredient " << r.amount << " of " << r.material << " producing " << times << "x" << endl;
			// need  1 2 3 4 5 6 7 8 9 10
			// amnt  4 4 4 4 4 4 4 4 4  4
			// need2 1 1 1 1 2 2 2 2 3  3
			//  div  0 0 0 1 1 1 1 2 2  2
			make2(r.amount * times, r.material, stat, stock, reactions, indent + 4);
			(*stock)[r.material] -= r.amount * times;
		}
		(*stock)[material] += will_get * times;
		(*stat)[material] += will_get * times;
//		cout << "-- stock: ";
//		for (auto &s : *stock)
//			cout << s.first << ":" << s.second << " ";
//		cout << endl;
//	} else {
//		for (int i = 0; i < indent; i++)
//			cout << " ";
//		cout << "Have enough " << material << " (currently: " << (*stock)[material] << "), needs " << amount << endl;
	}
}

int main(int argc, char *argv[]) {
	aoc2019::Options options("Day 14", argc, argv);
	if (!options.check())
		return options.result();

	cout << "Starting " << options.program_name << endl;

	map<string, long> stock;                               // current amounts of each material
	map<string, long> stat;                                // creation statistics
	map<string, pair<int, vector<Ingredient>*>> reactions; // material -> output amount, input ingredient list

	string line;
	istringstream iss;
	string code;
	int amount;
	while (getline(*options.file(), line)) {
		aoc2019::trim(line);
		if (!line.length())
			continue;

		// 7 A, 1 E => 1 FUEL
		string::size_type p1 = 0;
		string::size_type p = line.find(" => "), _p;

		// result of the recipe
		iss.str(string(line, p + 4));
		iss.seekg(0);
		iss >> amount >> code;

		stock[code] = 0; // put 0 to stock
		stat[code] = 0; // put 0 to stat
		vector<Ingredient> *list = new vector<Ingredient>;
		reactions[code] = make_pair(amount, list); // empty reactions initially

		while ((_p = line.find(',', p1)) < p) {
			iss.str(string(line, p1, _p-p1));
			iss.seekg(0);
			iss >> amount >> code;
			p1 = _p + 1;
			list->emplace_back(Ingredient(code, amount));
		}
		// last ingredient (after comma, before "=>"
		iss.str(string(line, p1, p-p1));
		iss.seekg(0);
		iss >> amount >> code;
		list->emplace_back(Ingredient(code, amount));
	}
	stock["ORE"] = 0;
	stat["ORE"] = 0;
	reactions["ORE"] = make_pair(1, new vector<Ingredient>);

	make(1, "FUEL", &stat, &stock, &reactions);

	int initial_ore_per_fuel = stat["ORE"];
	cout << "Answer 1: " << initial_ore_per_fuel << endl;

	// initial amount of fuel we'll try to produce
	long try_amount = 1000000000000 / initial_ore_per_fuel;

	for (;;) {
		// clear stock and add 1000000000000 (10^12) ORE
		for (auto &m : stock) {
			stat[m.first] = 0;
			stock[m.first] = 0;
		}
		stock["ORE"] = 1000000000000;

		make2(try_amount, "FUEL", &stat, &stock, &reactions);
		if (stock["ORE"] >= initial_ore_per_fuel) {
			// we can do more!
			try_amount += stock["ORE"] / initial_ore_per_fuel;
		} else if (stock["ORE"] < 0) {
			// we have to do less
			try_amount -= -stock["ORE"] / initial_ore_per_fuel;
		} else {
			break;
		}
		stock["FUEL"] = 0;
	}

//	cout << "--- Final stock" << endl;
//	for (auto &s : stock)
//		cout << s.first << ":" << s.second << " ";
//	cout << endl;

	cout << "Answer 2: " << try_amount << endl;

	for (auto &it : reactions) {
		delete it.second.second;
	}

	return EXIT_SUCCESS;
}
