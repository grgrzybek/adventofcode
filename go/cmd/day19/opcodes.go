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
package aoc2018

import "fmt"

type Opcode func(registers []int, p1, p2, p3 int) (string, int)

var Opcodes = make(map[string]Opcode, 0)

func init() {
	fmt.Println("init for opcodes from day 19")
	InitializeInstructions()
}

func InitializeInstructions() {
	// gtri
	Opcodes["gtri"] = func(registers []int, p1, p2, p3 int) (string, int) {
		if registers[p1] > p2 {
			registers[p3] = 1
		} else {
			registers[p3] = 0
		}
		return "gtri", 0
	}
	// bani
	Opcodes["bani"] = func(registers []int, p1, p2, p3 int) (string, int) {
		v := registers[p1] & p2
		registers[p3] = v
		return "bani", 1
	}
	// eqrr
	Opcodes["eqrr"] = func(registers []int, p1, p2, p3 int) (string, int) {
		if registers[p1] == registers[p2] {
			registers[p3] = 1
		} else {
			registers[p3] = 0
		}
		return "eqrr", 2
	}
	// gtir
	Opcodes["gtir"] = func(registers []int, p1, p2, p3 int) (string, int) {
		if p1 > registers[p2] {
			registers[p3] = 1
		} else {
			registers[p3] = 0
		}
		return "gtir", 3
	}
	// eqir
	Opcodes["eqir"] = func(registers []int, p1, p2, p3 int) (string, int) {
		if p1 == registers[p2] {
			registers[p3] = 1
		} else {
			registers[p3] = 0
		}
		return "eqir", 4
	}
	// bori
	Opcodes["bori"] = func(registers []int, p1, p2, p3 int) (string, int) {
		v := registers[p1] | p2
		registers[p3] = v
		return "bori", 5
	}
	// seti
	Opcodes["seti"] = func(registers []int, p1, p2, p3 int) (string, int) {
		registers[p3] = p1
		return "seti", 6
	}
	// setr
	Opcodes["setr"] = func(registers []int, p1, p2, p3 int) (string, int) {
		registers[p3] = registers[p1]
		return "setr", 7
	}
	// addr
	Opcodes["addr"] = func(registers []int, p1, p2, p3 int) (string, int) {
		v := registers[p1] + registers[p2]
		registers[p3] = v
		return "addr", 8
	}
	// borr
	Opcodes["borr"] = func(registers []int, p1, p2, p3 int) (string, int) {
		v := registers[p1] | registers[p2]
		registers[p3] = v
		return "borr", 9
	}
	// muli
	Opcodes["muli"] = func(registers []int, p1, p2, p3 int) (string, int) {
		v := registers[p1] * p2
		registers[p3] = v
		return "muli", 10
	}
	// banr
	Opcodes["banr"] = func(registers []int, p1, p2, p3 int) (string, int) {
		v := registers[p1] & registers[p2]
		registers[p3] = v
		return "banr", 11
	}
	// addi
	Opcodes["addi"] = func(registers []int, p1, p2, p3 int) (string, int) {
		v := registers[p1] + p2
		registers[p3] = v
		return "addi", 12
	}
	// eqri
	Opcodes["eqri"] = func(registers []int, p1, p2, p3 int) (string, int) {
		if registers[p1] == p2 {
			registers[p3] = 1
		} else {
			registers[p3] = 0
		}
		return "eqri", 13
	}
	// mulr
	Opcodes["mulr"] = func(registers []int, p1, p2, p3 int) (string, int) {
		v := registers[p1] * registers[p2]
		registers[p3] = v
		return "mulr", 14
	}
	// gtrr
	Opcodes["gtrr"] = func(registers []int, p1, p2, p3 int) (string, int) {
		if registers[p1] > registers[p2] {
			registers[p3] = 1
		} else {
			registers[p3] = 0
		}
		return "gtrr", 15
	}
}
