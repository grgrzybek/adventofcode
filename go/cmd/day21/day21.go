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
	"strconv"
)

var file string
var tabledata string

func init() {
	var dayCmd = &cobra.Command{
		Use:   "day21",
		Short: "Day 21 example",
		Run:   run,
	}
	dayCmd.Flags().StringVarP(&file, "input", "i", "", "Input file with puzzle data")
	dayCmd.Flags().StringVarP(&tabledata, "table-data", "f", "", "Input file possible answers")
	aoc2018.RootCmd.AddCommand(dayCmd)
}

func run(cmd *cobra.Command, _ []string) {
	if file == "" || tabledata == "" {
		_ = cmd.Usage()
		_, _ = fmt.Fprintln(os.Stderr, "File and input arguments are required")
		os.Exit(1)
	}

	var f1, f2 *os.File
	var e error
	if f1, e = os.Open(file); e != nil {
		_, _ = fmt.Fprintln(os.Stderr, e)
		os.Exit(1)
	}
	if f2, e = os.Open(tabledata); e != nil {
		_, _ = fmt.Fprintln(os.Stderr, e)
		os.Exit(1)
	}
	defer (func(f1, f2 *os.File) {
		_ = f1.Close()
		_ = f2.Close()
	})(f1, f2)

	scanner := bufio.NewScanner(f1)
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

	scanner = bufio.NewScanner(f2)
	scanner.Split(bufio.ScanLines)

	r2s := make([]int, 0)

	for ; scanner.Scan(); {
		l := scanner.Text()
		if l == "" {
			break
		}
		r2, _ := strconv.Atoi(l)
		r2s = append(r2s, r2)
	}

	bestR0 := -1

	registers := []int{0, 0, 0, 0, 0, 0}
	ip := 0
	count := 0
	repeats := make(map[int]bool)
	repeatsList := make([]int, 0)
	for {
		registers[ipr] = ip
		Opcodes[program[ip].opcode](registers, program[ip].args[0], program[ip].args[1], program[ip].args[2])
		count++
		ip = registers[ipr]
		ip++
		if ip == 29 {
			// last value before falling into cycle - unfortunately, hinted by reddit
			if _, ok := repeats[registers[2]]; !ok {
				repeats[registers[2]] = true
				repeatsList = append(repeatsList, registers[2])
			} else {
				bestR0 = repeatsList[len(repeatsList)-1]
				break
			}
		}
		if ip >= len(program) {
			break
		}
	}

	/*
00: [0 0 0 0 0 0] -> [0 0 123 0 0 0]
01: [0 0 123 0 0 0] -> [0 0 72 0 1 0]
02: [0 0 72 0 1 0] -> [0 0 1 0 2 0]
03: [0 0 1 0 2 0] -> [0 0 1 0 4 0]
05: [0 0 1 0 4 0] -> [0 0 0 0 5 0]
06: [0 0 0 0 5 0] -> [0 0 0 0 6 65536]
07: [0 0 0 0 6 65536] -> [0 0 2238642 0 7 65536]
08: [0 0 2238642 0 7 65536] -> [0 0 2238642 0 8 65536]
09: [0 0 2238642 0 8 65536] -> [0 0 2238642 0 9 65536]
10: [0 0 2238642 0 9 65536] -> [0 0 2238642 0 10 65536]
11: [0 0 2238642 0 10 65536] -> [0 0 147524269158 0 11 65536]
12: [0 0 147524269158 0 11 65536] -> [0 0 2208870 0 12 65536]
13: [0 0 2208870 0 12 65536] -> [0 0 2208870 0 13 65536]
14: [0 0 2208870 0 13 65536] -> [0 0 2208870 0 14 65536]
15: [0 0 2208870 0 14 65536] -> [0 0 2208870 0 16 65536]
17: [0 0 2208870 0 16 65536] -> [0 0 2208870 0 17 65536]

18: [0 0 2208870 0 17 65536] -> [0 1 2208870 0 18 65536]
19: [0 1 2208870 0 18 65536] -> [0 256 2208870 0 19 65536]
20: [0 256 2208870 0 19 65536] -> [0 0 2208870 0 20 65536]
21: [0 0 2208870 0 20 65536] -> [0 0 2208870 0 21 65536]
22: [0 0 2208870 0 21 65536] -> [0 0 2208870 0 23 65536]
24: [0 0 2208870 0 23 65536] -> [0 0 2208870 1 24 65536]
25: [0 0 2208870 1 24 65536] -> [0 0 2208870 1 17 65536]

18: [0 0 2208870 1 17 65536] -> [0 2 2208870 1 18 65536]
19: [0 2 2208870 1 18 65536] -> [0 512 2208870 1 19 65536]
20: [0 512 2208870 1 19 65536] -> [0 0 2208870 1 20 65536]
21: [0 0 2208870 1 20 65536] -> [0 0 2208870 1 21 65536]
22: [0 0 2208870 1 21 65536] -> [0 0 2208870 1 23 65536]
24: [0 0 2208870 1 23 65536] -> [0 0 2208870 2 24 65536]
25: [0 0 2208870 2 24 65536] -> [0 0 2208870 2 17 65536]
...
	*/

	r0 := 0
	// r1 := 0
	r2 := 0
	r3 := 0
	// r4 := 0 // bound to IP
	r5 := 0

	// program
	for {
		/*00*/ r2 = 0x7B /*123 = 0111 1011*/
		/*01*/ r2 &= 0x1C8 /*456 = 1 1100 1000*/ /*72 = 0x48 = 0100 1000*/
		/*02*/ if r2 == 72 {
			r2 = 1
			// /*03*/ r4 = 3; r4 = r2 + r4
			break
		} else {
			r2 = 0
			// /*04*/ r4 = 0
			continue
		}
	}

	// part 1 - with just break instead of continue
	/*05*/ r2 = 0
out:
	for {
		/*06*/ r5 = r2 | 65536
		/*07*/ r2 = 2238642
		for {
			/*08*/ r3 = r5 & 0xFF /*255*/
			/*09*/ r2 = r2 + r3
			/*10*/ r2 = r2 & 0xFFFFFF /*16777215*/
			/*11*/ r2 = r2 * 0x1016B /*65899*/
			/*12*/ r2 = r2 & 0xFFFFFF /*16777215*/
			/*13*/ if r5 < 256 {
				/*28*/ if r2 == r0 {
					break out
				} else {
					fmt.Printf("%020d\n", r2)
					break out
					// continue out
				}
			} else {
				r3 = 0
				for {
					/*20*/ if (r3+1)*256 > r5 {
						break
					} else {
						/*24*/ r3 = r3 + 1
					}
				}
				/*26*/ r5 = r3
			}
		}
	}

	// part 2 - knowing different answers
	fmt.Printf("Answer 1: %d", r2)
	fmt.Printf(", Answer 2: %d\n", bestR0)
}

type instruction struct {
	opcode string
	args   []int
}

func (i *instruction) String() string {
	return fmt.Sprintf("%s (%d, %d, %d)", i.opcode, i.args[0], i.args[1], i.args[2])
}
