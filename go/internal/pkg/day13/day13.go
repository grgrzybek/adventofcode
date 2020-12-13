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
	"strings"
)

var file string

func init() {
	var cmd = &cobra.Command{
		Use:   "day13",
		Short: "Day 13 example",
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

	buses := make([]uint64, 0)
	indexes := make([]uint64, 0)

	scanner.Scan()
	timestamp, _ := strconv.ParseInt(scanner.Text(), 0, 0)

	scanner.Scan()
	line := scanner.Text()
	idx := 0
	counter := 0
	for {
		i := strings.IndexRune(line[idx:], ',')
		if i == -1 {
			i = len(line)
		} else {
			i += idx
		}
		if line[idx] != 'x' {
			nr, _ := strconv.ParseInt(line[idx:i], 0, 0)
			buses = append(buses, uint64(nr))
			indexes = append(indexes, uint64(counter))
		}
		counter++
		idx = i + 1
		if idx >= len(line) {
			break
		}
	}

	// part 1

	start := uint64(timestamp)
	var bus uint64 = 0

	for {
		for _, b := range buses {
			if start%b == 0 {
				bus = b
				break
			}
		}
		if bus != 0 {
			break
		}
		start++
	}

	// part 2

	// var ts uint64 = 1068381
	// var ts uint64 = 100000000000000
	// var ts, tsb uint64 = 0, 0
	var ts uint64 = 0

	idx1, idx2 := 0, 1
	a, b := buses[idx1], buses[idx2]
	aidx, bidx := indexes[idx1], indexes[idx2]
	for {
		// ts = a
		for {
			if (ts+aidx)%a == 0 && (ts+bidx)%b == 0 {
				break
			}
			ts += a
		}
		idx1++
		idx2++
		if idx2 == len(buses) {
			break
		}

		// https://math.stackexchange.com/questions/2218763/how-to-find-lcm-of-two-numbers-when-one-starts-with-an-offset
		a = a * b
		// ts = a
		aidx = -ts
		b = buses[idx2]
		bidx = indexes[idx2]
	}
	// ts += buses[0]

	// answer1 = 4808, answer2 = 741745043105674
	fmt.Printf("Answer 1: %d, Answer 2: %d\n", (start-uint64(timestamp))*bus, ts)
}
