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

type Opcode func(registers []int, p1, p2, p3 int)

var Opcodes = make([]Opcode, 0)

func InitializeInstructions() {
	// addr
	Opcodes = append(Opcodes, func(registers []int, p1, p2, p3 int) {
		v := registers[p1] + registers[p2]
		registers[p3] = v
	})
	// addi
	Opcodes = append(Opcodes, func(registers []int, p1, p2, p3 int) {
		v := registers[p1] + p2
		registers[p3] = v
	})
	// mulr
	Opcodes = append(Opcodes, func(registers []int, p1, p2, p3 int) {
		v := registers[p1] * registers[p2]
		registers[p3] = v
	})
	// muli
	Opcodes = append(Opcodes, func(registers []int, p1, p2, p3 int) {
		v := registers[p1] * p2
		registers[p3] = v
	})
	// banr
	Opcodes = append(Opcodes, func(registers []int, p1, p2, p3 int) {
		v := registers[p1] & registers[p2]
		registers[p3] = v
	})
	// bani
	Opcodes = append(Opcodes, func(registers []int, p1, p2, p3 int) {
		v := registers[p1] & p2
		registers[p3] = v
	})
	// borr
	Opcodes = append(Opcodes, func(registers []int, p1, p2, p3 int) {
		v := registers[p1] | registers[p2]
		registers[p3] = v
	})
	// bori
	Opcodes = append(Opcodes, func(registers []int, p1, p2, p3 int) {
		v := registers[p1] | p2
		registers[p3] = v
	})
	// setr
	Opcodes = append(Opcodes, func(registers []int, p1, p2, p3 int) {
		registers[p3] = registers[p1]
	})
	// seti
	Opcodes = append(Opcodes, func(registers []int, p1, p2, p3 int) {
		registers[p3] = p1
	})
	// gtir
	Opcodes = append(Opcodes, func(registers []int, p1, p2, p3 int) {
		if p1 > registers[p2] {
			registers[p3] = 1
		} else {
			registers[p3] = 0
		}
	})
	// gtri
	Opcodes = append(Opcodes, func(registers []int, p1, p2, p3 int) {
		if registers[p1] > p2 {
			registers[p3] = 1
		} else {
			registers[p3] = 0
		}
	})
	// gtrr
	Opcodes = append(Opcodes, func(registers []int, p1, p2, p3 int) {
		if registers[p1] > registers[p2] {
			registers[p3] = 1
		} else {
			registers[p3] = 0
		}
	})
	// eqir
	Opcodes = append(Opcodes, func(registers []int, p1, p2, p3 int) {
		if p1 == registers[p2] {
			registers[p3] = 1
		} else {
			registers[p3] = 0
		}
	})
	// eqri
	Opcodes = append(Opcodes, func(registers []int, p1, p2, p3 int) {
		if registers[p1] == p2 {
			registers[p3] = 1
		} else {
			registers[p3] = 0
		}
	})
	// eqrr
	Opcodes = append(Opcodes, func(registers []int, p1, p2, p3 int) {
		if registers[p1] == registers[p2] {
			registers[p3] = 1
		} else {
			registers[p3] = 0
		}
	})
}
