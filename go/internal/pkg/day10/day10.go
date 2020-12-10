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
	"sort"
	"strconv"
)

var file string

func init() {
	var cmd = &cobra.Command{
		Use:   "day10",
		Short: "Day 10 example",
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

	numbers := make([]int, 0)
	numbers = append(numbers, 0)

	for ; scanner.Scan(); {
		line := scanner.Text()
		if line == "" {
			break
		}
		i, _ := strconv.ParseInt(line, 10, 0)
		numbers = append(numbers, int(i))
	}
	sort.Slice(numbers, func(i, j int) bool {
		return numbers[i] < numbers[j]
	})
	numbers = append(numbers, numbers[len(numbers)-1]+3)

	// part 1

	n1, n3 := 0, 0

	for idx, v := range numbers {
		if idx == 0 {
			continue
		} else {
			if v-numbers[idx-1] == 1 {
				n1++
			} else if v-numbers[idx-1] == 3 {
				n3++
			}
		}
	}

	// part 2

	cache := make(map[int]uint64)
	count2 := count(numbers, 0, cache)

	// answer1 = 2775, answer2 = 0
	fmt.Printf("Answer 1: %d, Answer 2: %d\n", n1*n3, count2)
}

func count(numbers []int, start int, cache map[int]uint64) uint64 {
	if start == len(numbers)-1 {
		return 1
	}
	if _, ok := cache[start]; ok {
		return cache[start]
	}
	var res uint64 = 0
	// fmt.Printf("[%d]=%d\n", start, numbers[start])
	// if start+1 < len(numbers) {
	// 	fmt.Printf(" [%d]=%d\n", start+1, numbers[start+1])
	// }
	// if start+2 < len(numbers) {
	// 	fmt.Printf("  [%d]=%d\n", start+2, numbers[start+2])
	// }
	// if start+3 < len(numbers) {
	// 	fmt.Printf("   [%d]=%d\n", start+3, numbers[start+3])
	// }
	if start+1 < len(numbers) && numbers[start] >= numbers[start+1]-3 {
		res += count(numbers, start+1, cache)
	}
	if start+2 < len(numbers) && numbers[start] >= numbers[start+2]-3 {
		res += count(numbers, start+2, cache)
	}
	if start+3 < len(numbers) && numbers[start] >= numbers[start+3]-3 {
		res += count(numbers, start+3, cache)
	}
	cache[start] = res
	// fmt.Printf("start=%d, res=%d\n", start, res)
	return res
}
