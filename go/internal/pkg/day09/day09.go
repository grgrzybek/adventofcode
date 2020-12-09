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
	"os"
	"strconv"
)

var file string

func init() {
	var cmd = &cobra.Command{
		Use:   "day09",
		Short: "Day 09 example",
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

	numbers := make([]uint64, 0)

	for ; scanner.Scan(); {
		line := scanner.Text()
		if line == "" {
			break
		}
		i, _ := strconv.ParseInt(line, 10, 64)
		numbers = append(numbers, uint64(i))
	}

	// part 1

	a, b := 0, 25
	var wrong uint64 = 0

	for idx, v := range numbers {
		if idx < b {
			continue
		}
		ok := false
		// fmt.Printf("numbers[%d] = %d (a=%d, b=%d)\n", idx, v, a, b)
		for _, i := range numbers[a:b] {
			for _, j := range numbers[a:b] {
				if i != j {
					ok = ok || i + j == v
				}
			}
		}
		if !ok {
			wrong = v
			break
		}
		a++
		b++
	}

	// part 2

	startOfSequence := 0
	var sequence []uint64
	done := false

	for {
		var currentSum uint64 = 0
		for idx, v := range numbers[startOfSequence:] {
			currentSum += v
			// fmt.Printf("[%d]=%d -> %d\n", idx+startOfSequence, v, currentSum)
			if currentSum == wrong {
				sequence = make([]uint64, idx+1)
				copy(sequence, numbers[startOfSequence:startOfSequence+idx+1])
				done = true
				break
			}
			if currentSum > wrong {
				startOfSequence++
				break
			}
		}
		if done {
			break
		}
	}

	var min, max uint64 = ^uint64(0), 0
	for _, v := range sequence {
		// fmt.Printf("%d\n", v)
		if v < min {
			min = v
		}
		if v > max {
			max = v
		}
		// fmt.Printf(" -> %d\n", v)
	}

	// answer1 = 88311122, answer2 = 13549369
	fmt.Printf("Answer 1: %d, Answer 2: %d\n", wrong, min + max)
}
