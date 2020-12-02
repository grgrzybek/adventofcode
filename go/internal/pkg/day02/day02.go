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
	var day02Cmd = &cobra.Command{
		Use:   "day02",
		Short: "Day 02 example",
		Run:   run,
	}
	day02Cmd.Flags().StringVarP(&file, "file", "f", "", "Input file with puzzle data")
	aoc2020.RootCmd.AddCommand(day02Cmd)
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

	values := make([]*entry, 0)

	for ; scanner.Scan(); {
		line := scanner.Text()
		if line == "" {
			break
		}
		en := &entry{}
		_, e = fmt.Sscanf(line, "%d-%d %c: %s\n", &en.min, &en.max, &en.c, &en.password)
		if e != nil {
			_, _ = fmt.Fprintln(os.Stderr, e)
			os.Exit(1)
		}
		values = append(values, en)
	}

	// part 1

	var good1 uint

	for _, e := range values {
		var count uint = 0
		for _, c := range e.password {
			if c == e.c {
				count++
			}
		}
		if count >= e.min && count <= e.max {
			good1++
		}
	}

	// part 2

	var good2 uint

	for _, e := range values {
		var count uint = 0
		for idx, c := range e.password {
			if c == e.c && (idx+1 == int(e.min) || idx+1 == int(e.max)) {
				count++
			}
		}
		if count == 1 {
			good2++
		}
	}

	fmt.Printf("Answer 1: %d, Answer 2: %d\n", good1, good2)
}

type entry struct {
	min, max uint
	c        rune
	password string
}

func (n *entry) String() string {
	return fmt.Sprintf("%d-%d (%c): %s", n.min, n.max, n.c, n.password)
}
