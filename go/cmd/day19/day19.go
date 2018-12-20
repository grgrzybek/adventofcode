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

import (
	"bufio"
	"fmt"
	"github.com/grgrzybek/adventofcode2018/go/cmd"
	"github.com/spf13/cobra"
	"os"
)

var file string

func init() {
	var dayCmd = &cobra.Command{
		Use:   "day19",
		Short: "Day 19 example",
		Run:   run,
	}
	dayCmd.Flags().StringVarP(&file, "input", "i", "", "Input file with puzzle data")
	aoc2018.RootCmd.AddCommand(dayCmd)
}

func run(cmd *cobra.Command, _ []string) {
	if file == "" {
		_ = cmd.Usage()
		_, _ = fmt.Fprintln(os.Stderr, "File argument is required")
		os.Exit(1)
	}

	var f *os.File
	var e error
	if f, e = os.Open(file, ); e != nil {
		_, _ = fmt.Fprintln(os.Stderr, e)
		os.Exit(1)
	}
	defer (func(f *os.File) { _ = f.Close() })(f)

	scanner := bufio.NewScanner(f)
	scanner.Split(bufio.ScanLines)

	program := make([]*instruction, 0)
	ipr := -1

	for ; scanner.Scan(); {
		l := scanner.Text()
		if l == "" {
			break
		}
		if l[0] == '#' {
			_, _ = fmt.Sscanf(l, "#ip %d", &ipr)
			continue
		}
		var ins string
		args := make([]int, 3)
		_, _ = fmt.Sscanf(l, "%s %d %d %d", &ins, &args[0], &args[1], &args[2])
		program = append(program, &instruction{opcode: ins, args: args})
	}

	registers := []int{0, 0, 0, 0, 0, 0}
	ip := 0

	for {
		// fmt.Printf("%02d: %v -> ", ip, registers)
		registers[ipr] = ip

		Opcodes[program[ip].opcode](registers, program[ip].args[0], program[ip].args[1], program[ip].args[2])

		ip = registers[ipr]
		ip++
		// fmt.Printf("%v\n", registers)
		if ip >= len(program) {
			break
		}
	}

	res1 := registers[0]

	registers = []int{1, 0, 0, 0, 0, 0}
	ip = 0

	r0 := 1
	r1 := 0
	r2 := 0
	r3 := 0 // bound to IP
	r4 := 0
	r5 := 0

	/*
04: [0 1 1  3 1 10551381] -> [0 0 1  4 1 10551381]
05: [0 0 1  4 1 10551381] -> [0 0 1  5 1 10551381]
06: [0 0 1  5 1 10551381] -> [0 0 1  7 1 10551381]
08: [0 0 1  7 1 10551381] -> [0 0 2  8 1 10551381]
09: [0 0 2  8 1 10551381] -> [0 0 2  9 1 10551381]
10: [0 0 2  9 1 10551381] -> [0 0 2 10 1 10551381]
11: [0 0 2 10 1 10551381] -> [0 0 2  2 1 10551381]
03: [0 0 2  2 1 10551381] -> [0 2 2  3 1 10551381]
04: [0 2 2  3 1 10551381] -> [0 0 2  4 1 10551381]
05: [0 0 2  4 1 10551381] -> [0 0 2  5 1 10551381]
06: [0 0 2  5 1 10551381] -> [0 0 2  7 1 10551381]
08: [0 0 2  7 1 10551381] -> [0 0 3  8 1 10551381]
09: [0 0 3  8 1 10551381] -> [0 0 3  9 1 10551381]
10: [0 0 3  9 1 10551381] -> [0 0 3 10 1 10551381]
11: [0 0 3 10 1 10551381] -> [0 0 3  2 1 10551381]
...
	*/

	// program
	/*00*/ r3 = 0; r3 += 16
	/*17*/ r3 = 17; r5 += 2
	/*18*/ r3 = 18; r5 = r5 * r5
	/*19*/ r3 = 19; r5 = r3 * r5
	/*20*/ r3 = 20; r5 *= 11
	/*21*/ r3 = 21; r1 += 6
	/*22*/ r3 = 22; r1 = r1 * r3
	/*23*/ r3 = 23; r1 += 13
	/*24*/ r3 = 24; r5 += r1
	/*25*/ r3 = 25; r3 += r0
	/*27*/ r3 = 27; r1 = r3
	/*28*/ r3 = 28; r1 = r1 * r3
	/*29*/ r3 = 29; r1 += r3
	/*30*/ r3 = 30; r1 = r3 * r1
	/*31*/ r3 = 31; r1 *= 14
	/*32*/ r3 = 32; r1 = r1 * r3
	/*33*/ r3 = 33; r5 += r1
	/*34*/ r3 = 34; r0 = 0
	/*35*/ r3 = 35; r3 = 0

	/*01*/ r4 = 1
	/*02*/ r2 = 1

	fmt.Printf("start: %v\n", []int{r0,r1,r2,r3,r4,r5})

	// for {
	// 	if r4 * r2 == r5 {
	// 		r0 += r4
	// 	}
	// 	r2++
	// 	if r2 > r5 {
	// 		r4++
	// 	} else {
	// 		continue
	// 	}
	// 	if r4 > r5 {
	// 		break
	// 	} else {
	// 		r2 = 1
	// 		continue
	// 	}
	// }

	factorisation := []int{1, 3, 71, 213, 15123, 49537, 148611, 3517127, 10551381}

	for _, r4 = range factorisation {
		for _, r2 = range factorisation {
			if r4 * r2 == 10551381 {
				r0 += r4
			}
		}
	}
	// /*03*/ r3 = 3; r1 = r4 * r2
	// /*04*/ r3 = 4; if r1 == r5 {r1 = 1} else {r1 = 0} // is it 10551381?
	// /*05*/ r3 = 5; r3 = r1 + r3
	// /*06*/ r3 = 6; r3++
	// /*07*/ r3 = 7; r0 += r4 // r1 == r5
	// /*08*/ r3 = 8; r2++     // r1 != 5
	// /*09*/ r3 = 9; if r2 > r5 {r1 = 1} else {r1 = 0}
	// /*10*/ r3 = 10; r3 += r1
	// /*11*/ r3 = 11; r3 = 2
	// /*12*/ r3 = 12; r4++
	// /*13*/ r3 = 13; if r4 > r5 {r1 = 1} else {r1 = 0}
	// /*14*/ r3 = 14; r3 += r1
	// /*15*/ r3 = 15; r3 = 1
	// /*16*/ r3 = 16; r3 = r3 * r3
	// {
	// 	/*26*/ r3 = 0
	// 	r3++ // r3 == 25
	// }

	// for {
	// 	registers[ipr] = ip
	//
	// 	Opcodes[program[ip].opcode](registers, program[ip].args[0], program[ip].args[1], program[ip].args[2])
	// 	// fmt.Printf("Invoking %s : %d\n", program[ip], registers[0])
	//
	// 	ip = registers[ipr]
	// 	ip++
	// 	if ip >= len(program) || ip < 0 {
	// 		break
	// 	}
	// 	fmt.Printf("%d", registers[0])
	// }

	fmt.Printf("Answer 1: %d", res1)
	fmt.Printf(", Answer 2: %d\n", r0)
}

type instruction struct {
	opcode string
	args   []int
}

func (i *instruction) String() string {
	return fmt.Sprintf("%s (%d, %d, %d)", i.opcode, i.args[0], i.args[1], i.args[2])
}
