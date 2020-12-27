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
)

var file string

func init() {
	var cmd = &cobra.Command{
		Use:   "day23",
		Short: "Day 23 example",
		Run:   run,
	}
	cmd.Flags().StringVarP(&file, "file", "f", "", "Input file with puzzle data")
	aoc2020.RootCmd.AddCommand(cmd)
}

type cup struct {
	v          int
	next, prev *cup
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

	var current1, first1 *cup
	var current2, first2 *cup
	pointers := make([]*cup, 1000001) // label -> pointer
	scanner.Scan()
	line := scanner.Text()
	var min, max = math.MaxInt64, 0

	for _, r := range line {
		v, _ := strconv.ParseInt(string(r), 10, 0)
		if int(v) > max {
			max = int(v)
		}
		if int(v) < min {
			min = int(v)
		}
		if first1 == nil {
			first1 = &cup{v: int(v)}
			first2 = &cup{v: int(v)}
			current1 = first1
			current2 = first2
		} else {
			current1.next = &cup{v: int(v)}
			current2.next = &cup{v: int(v)}
			current1.next.prev = current1
			current2.next.prev = current2
			current1 = current1.next
			current2 = current2.next
		}
		pointers[v] = current2
	}
	current1.next = first1
	current2.next = first2
	first1.prev = current1
	first2.prev = current2
	current1 = first1
	current2 = first2

	if current1 == nil || current2 == nil {
		panic("What?")
	}

	// part 1

	for n := 0; n < 100; n++ {
		// The crab picks up the three cups that are immediately clockwise of the current cup
		// C
		// a-b-c-d-e-f-g-h-i-j
		ptr := current1.next
		// C P
		// a-b-c-d-e-f-g-h-i-j
		current1.next = ptr.next.next.next
		current1.next.prev = current1
		v := current1.v - 1

		// The crab selects a destination cup: the cup with a label equal to the current cup's label minus one
		for {
			if v < min {
				v = max
			}
			if v == ptr.v || v == ptr.next.v || v == ptr.next.next.v {
				v--
			} else {
				break
			}
		}
		ptr2 := current1
		for ; ptr2.v != v; ptr2 = ptr2.next {
		}

		// The crab places the cups it just picked up so that they are immediately clockwise of the destination cup
		// C     D
		// a-e-f-g-h-i-j
		// P
		// b-c-d
		ptr2.next.prev = ptr.next.next
		ptr.next.next.next = ptr2.next
		ptr2.next = ptr
		ptr.prev = ptr2

		// The crab selects a new current cup: the cup which is immediately clockwise of the current cup.
		current1 = current1.next
	}

	var ptr *cup
	for ptr = current1; ptr.v != 1; ptr = ptr.next {
	}
	ptr = ptr.next

	order := ""
	for ; ptr.v != 1; ptr = ptr.next {
		order += strconv.FormatInt(int64(ptr.v), 10)
	}

	// part 2

	p := current2.prev
	for ad := max + 1; ad <= 1_000_000; ad++ {
		p.next = &cup{v: ad, prev: p}
		p = p.next
		pointers[ad] = p
	}
	p.next = first2
	first2.prev = p

	max = 1_000_000
	for n := 0; n < 10_000_000; n++ {
		// The crab picks up the three cups that are immediately clockwise of the current cup
		// C
		// a-b-c-d-e-f-g-h-i-j
		ptr := current2.next
		// C P
		// a-b-c-d-e-f-g-h-i-j
		current2.next = ptr.next.next.next
		current2.next.prev = current2

		// The crab selects a destination cup: the cup with a label equal to the current cup's label minus one
		v := current2.v - 1
		for {
			if v < min {
				v = max
			}
			if v == ptr.v || v == ptr.next.v || v == ptr.next.next.v {
				v--
			} else {
				break
			}
		}

		ptr2 := pointers[v]

		// The crab places the cups it just picked up so that they are immediately clockwise of the destination cup
		// C     D
		// a-e-f-g-h-i-j
		// P
		// b-c-d
		// C     D P
		// a-e-f-g-b-c-d-h-i-j
		ptr2.next.prev = ptr.next.next
		ptr.next.next.next = ptr2.next
		ptr2.next = ptr
		ptr.prev = ptr2

		// The crab selects a new current cup: the cup which is immediately clockwise of the current cup.
		current2 = current2.next
	}

	v2 := pointers[1].next.v * pointers[1].next.next.v

	// answer1 = 72496583, answer2 = 41785843847
	fmt.Printf("Answer 1: %s, Answer 2: %d\n", order, v2)
}
