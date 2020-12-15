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
		Use:   "day15",
		Short: "Day 15 example",
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
	scanner.Scan()
	line := scanner.Text()

	numbers := make([]int, 0)
	numbers1 := make([]int, 0)
	numbers2 := make([]int, 0)
	idx := 0
	for {
		i := strings.IndexRune(line[idx:], ',')
		if i == -1 {
			i = len(line)
		} else {
			i += idx
		}
		nr, _ := strconv.ParseInt(line[idx:i], 0, 0)
		numbers = append(numbers, int(nr))
		numbers1 = append(numbers1, int(nr))
		numbers2 = append(numbers2, int(nr))
		idx = i + 1
		if idx >= len(line) {
			break
		}
	}

	// part 1

	// for idx, v := range numbers1 {
	// 	fmt.Printf("%d -> %d\n", idx+1, v)
	// }
	turn := len(numbers1) + 1
	for {
		last1Idx := len(numbers1) - 1
		last2Idx := -1
		last := numbers1[last1Idx]
		for i := len(numbers1) - 2; i >= 0; i-- {
			if numbers1[i] == last {
				last2Idx = i
				break
			}
		}
		if last2Idx >= 0 {
			numbers1 = append(numbers1, last1Idx-last2Idx)
		} else {
			numbers1 = append(numbers1, 0)
		}
		// fmt.Printf("%d -> %d\n", turn, numbers1[turn-1])
		turn++
		if turn > 2020 {
			break
		}
	}

	// part 2

	memory1 := make(map[int]int)
	memory2 := make(map[int]int)
	for idx, v := range numbers2 {
		memory2[v] = idx + 1
	}
	turn = len(numbers2) + 1
	lastV := numbers2[len(numbers2)-1]
	var v1 int
	var v2 int
	var ok bool
	for {
		if v1, ok = memory1[lastV]; !ok {
			// never spoken
			lastV = 0
			if v2, ok = memory2[lastV]; ok {
				memory1[lastV] = v2
				memory2[lastV] = turn
			} else {
				memory1[lastV] = turn
			}
		} else {
			lastV = memory2[lastV] - v1
			if v2, ok = memory2[lastV]; ok {
				memory1[lastV] = v2
				memory2[lastV] = turn
			} else {
				memory2[lastV] = turn
			}
		}
		turn++
		if turn > 30000000 {
			break
		}
	}

	// answer1 = 614, answer2 = 1065
	fmt.Printf("Answer 1: %d, Answer 2: %d\n", numbers1[len(numbers1)-1], lastV)
}
