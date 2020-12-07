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
)

var file string

func init() {
	var cmd = &cobra.Command{
		Use:   "day06",
		Short: "Day 06 example",
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

	groups := make([][]map[rune]bool, 0)
	currentGroup := make([]map[rune]bool, 0)

	for ; scanner.Scan(); {
		line := scanner.Text()
		if line == "" {
			groups = append(groups, currentGroup)
			currentGroup = make([]map[rune]bool, 0)
			continue
		}
		person := make(map[rune]bool)
		currentGroup = append(currentGroup, person)
		for _, r := range line {
			person[r] = true
		}
	}
	groups = append(groups, currentGroup)

	// part 1

	count1 := 0
	for _, group := range groups {
		all := make(map[rune]bool)
		for _, person := range group {
			for k, v := range person {
				all[k] = v
			}
		}
		count1 += len(all)
	}

	// part 2

	count2 := 0
	for _, group := range groups {
		all := make(map[rune]bool)
		for _, person := range group {
			for k, v := range person {
				all[k] = v
			}
		}
		for _, person := range group {
			for k := range all {
				if _, ok := person[k]; !ok {
					delete(all, k)
				}
			}
		}
		count2 += len(all)
	}

	// answer1 = 6596, answer2 = 3219
	fmt.Printf("Answer 1: %d, Answer 2: %d\n", count1, count2)
}
