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
	"os"
	"sort"
	"strings"

	"github.com/grgrzybek/adventofcode2018/go/cmd"
	"github.com/spf13/cobra"
)

var file string

func init() {
	var day04Cmd = &cobra.Command{
		Use:   "day04",
		Short: "Day 04 example",
		Run:   run,
	}
	day04Cmd.Flags().StringVarP(&file, "input", "i", "", "Input file with puzzle data")
	aoc2018.RootCmd.AddCommand(day04Cmd)
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

	scanner := bufio.NewScanner(f)
	scanner.Split(bufio.ScanLines)

	lines := make([]*line, 0, 128)

	// scannning

	for ; scanner.Scan(); {
		l := scanner.Text()
		if l == "" {
			break
		}
		// c := &line{date: string(([]rune(l))[1:17])}
		c := &line{id: -1, date: l[1:17]}
		var s string
		_, _ = fmt.Sscanf(c.date, "%10s %d:%d", &s, &c.hour, &c.minute)

		if strings.Contains(l, "begins shift") {
			_, _ = fmt.Sscanf(l[25:], "#%d begins shift", &c.id)
			c.what = Starts
		} else if strings.Contains(l, "falls asleep") {
			c.what = FallsAsleep
		} else {
			c.what = WakesUp
		}

		lines = append(lines, c)
	}

	// sorting

	sort.Slice(lines, func(i, j int) bool {
		return lines[i].date < lines[j].date
	})

	// compacting

	// guard ID -> list of shifts
	shifts := make(map[int][]*shift)
	var peek *shift

	for _, v := range lines {
		if _, ok := shifts[v.id]; !ok {
			shifts[v.id] = make([]*shift, 0, 128)
		}
		if v.what == Starts {
			peek = &shift{id: v.id}
			shifts[v.id] = append(shifts[v.id], peek)
		}
		if v.what == FallsAsleep || v.what == WakesUp {
			if v.hour == 23 {
				// recently added shift
				peek.changes = append(peek.changes, 0)
			} else {
				peek.changes = append(peek.changes, v.minute)
			}
		}
	}

	// collecting

	// id -> minute -> sleep count
	stats := make(map[int][]int)

	for guardId, guardShifts := range shifts {
		if _, ok := stats[guardId]; !ok {
			stats[guardId] = make([]int, 60) // for every minute
		}
		for _, shift := range guardShifts {
			sleeps := false
			change := 0
			for tick := 0; tick < 60; {
				if change < len(shift.changes) {
					for ; tick < shift.changes[change]; tick++ {
						if sleeps {
							stats[guardId][tick]++
						}
					}
				} else {
					break
				}
				sleeps = !sleeps
				change++
			}
		}
	}

	// finding most sleeping guard

	guard := -1
	max := 0
	for guardId, minutes := range stats {
		sum := 0
		for _, m := range minutes {
			sum += m
		}
		if max <= sum {
			max = sum
			guard = guardId
		}
	}

	// finding his minute when he was sleeping most of the shifts

	max = 0
	idx := 0
	for i, m := range stats[guard] {
		if max <= m {
			max = m
			idx = i
		}
	}

	// finding the guard that sleeps most in given minute

	guard2 := -1
	max = 0
	idx2 := 0
	for guardId, minutes := range stats {
		for i, m := range minutes {
			if max <= m {
				max = m
				idx2 = i
				guard2 = guardId
			}
		}
	}

	fmt.Printf("Answer 1: %d, Answer 2: %v\n", guard*idx, guard2*idx2)
}

const (
	Starts      = 1
	FallsAsleep = 2
	WakesUp     = 3
)

type action int

type line struct {
	id                int    // guard ID
	date              string // to sort by
	day, hour, minute int
	what              action
}

type shift struct {
	id      int // guard ID
	changes []int
}
