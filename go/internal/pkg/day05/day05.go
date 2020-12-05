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
)

var file string

func init() {
	var cmd = &cobra.Command{
		Use:   "day05",
		Short: "Day 05 example",
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

	lines := make([]*string, 0)

	for ; scanner.Scan(); {
		line := scanner.Text()
		if line == "" {
			break
		}
		lines = append(lines, &line)
	}

	// part 1

	var highest1 uint = 0
	seats := make([]uint, 0)

	for _, l := range lines {
		var vmin, vmax uint = 1, 128
		var hmin, hmax uint = 1, 8
		for _, v := range (*l)[:7] {
			if v == 'F' {
				vmax -= (vmax - vmin + 1) / 2
			} else if v == 'B' {
				vmin += (vmax - vmin + 1) / 2
			}
			fmt.Printf(">%c: %d - %d<\n", v, vmin-1, vmax-1)
		}
		for _, v := range (*l)[7:] {
			if v == 'L' {
				hmax -= (hmax - hmin + 1) / 2
			} else if v == 'R' {
				hmin += (hmax - hmin + 1) / 2
			}
			fmt.Printf(">%c: %d - %d<\n", v, hmin-1, hmax-1)
		}
		hmax--
		vmax--
		hmin--
		vmin--
		seats = append(seats, vmax * 8 + hmax)
		if highest1 < (vmax * 8 + hmax) {
			highest1 = vmax * 8 + hmax
		}
	}

	// part 2

	sort.Slice(seats, func(i, j int) bool {
		return seats[i] < seats[j]
	})
	s1 := seats[0]
	var mySeat uint = 0
	for _, s := range seats[1:] {
		fmt.Printf("%d\n", s)
		if s == s1 + 2 {
			mySeat = s - 1
			break
		}
		s1 = s
	}

	fmt.Printf("Answer 1: %d, Answer 2: %d\n", highest1, mySeat)
}
