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
package aoc2020

import (
	"bufio"
	"fmt"
	"github.com/grgrzybek/adventofcode2020/internal/app/aoc2020"
	"github.com/spf13/cobra"
	"math"
	"os"
	"strconv"
	"strings"
)

var file string

func init() {
	var cmd = &cobra.Command{
		Use:   "day14",
		Short: "Day 14 example",
		Run:   run,
	}
	cmd.Flags().StringVarP(&file, "file", "f", "", "Input file with puzzle data")
	aoc2020.RootCmd.AddCommand(cmd)
}

func run(cmd *cobra.Command, _ []string) {
	if file == "" {
		_ = cmd.Usage()
		_, _ = fmt.Fprintln(os.Stderr, "File argument is required")
		os.Exit(1)
	}

	var f *os.File
	var e error
	if f, e = os.Open(file); e != nil {
		_, _ = fmt.Fprintln(os.Stderr, e)
		os.Exit(1)
	}

	scanner := bufio.NewScanner(f)
	scanner.Split(bufio.ScanLines)

	lines := make([]string, 0)

	for ; scanner.Scan(); {
		line := scanner.Text()
		if line == "" {
			break
		}
		lines = append(lines, line)
	}

	// part 1

	var currentAnd uint64 = math.MaxUint64
	var currentOr uint64 = 0
	mem := make(map[int]uint64)

	for _, line := range lines {
		if strings.HasPrefix(line, "mask") {
			mask := strings.Split(line, " = ")[1]
			currentOr, _ = strconv.ParseUint(strings.ReplaceAll(mask, "X", "0"), 2, 64)  // puts "1"s in the value
			currentAnd, _ = strconv.ParseUint(strings.ReplaceAll(mask, "X", "1"), 2, 64) // puts "0"s in the value
			// fmt.Printf("AND: %036b\n OR: %036b\n", currentAnd, currentOr)
		} else {
			var idx int
			var v uint64
			_, _ = fmt.Sscanf(line, "mem[%d] = %d", &idx, &v)
			// fmt.Printf("\t[%d] = %d", idx, v)
			v &= currentAnd
			v |= currentOr
			// fmt.Printf(" = %d\n", v)
			mem[idx] = v
		}
	}

	var sum1 uint64 = 0
	for _, v := range mem {
		sum1 += v
	}

	// part 2

	mem2 := make(map[uint64]uint64)
	var mask string
	var addresses []string

	for _, line := range lines {
		if strings.HasPrefix(line, "mask") {
			mask = strings.Split(line, " = ")[1]
			currentOr, _ = strconv.ParseUint(strings.ReplaceAll(mask, "X", "0"), 2, 64)  // puts "1"s in the value
			power := 0
			for _, bit := range mask {
				if bit == 'X' {
					power++
				}
			}
			// fmt.Printf("mask: %s\n", mask)
			addresses = make([]string, 0)
			for p := 0; p < 1<<power; p++ {
				addresses = append(addresses, fmt.Sprintf(fmt.Sprintf("%%0%db", power), p))
			}
			// for _, a := range addresses {
			// 	fmt.Printf("a: %s\n", a)
			// }
		} else {
			var idx uint64
			var v uint64
			_, _ = fmt.Sscanf(line, "mem[%d] = %d", &idx, &v)
			address := idx|currentOr
			for _, addressMask := range addresses {
				maskedAddress := []rune(fmt.Sprintf("%036b", address))
				i := 0
				for idx, r := range mask {
					if r == 'X' {
						maskedAddress[idx] = []rune(addressMask)[i]
						i++
					}
				}
				address, _ = strconv.ParseUint(string(maskedAddress), 2, 0)
				fmt.Printf("\t[%d] = %d\n", address, v)
				mem2[address] = v
			}
		}
	}

	var sum2 uint64 = 0
	for _, v := range mem2 {
		sum2 += v
	}

	// answer1 = 14722016054794, answer2 = 3618217244644
	fmt.Printf("Answer 1: %d, Answer 2: %d\n", sum1, sum2)
}
