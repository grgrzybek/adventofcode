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

	"github.com/grgrzybek/adventofcode2018/go/cmd"
	"github.com/spf13/cobra"
)

var file string

func init() {
	var day02Cmd = &cobra.Command{
		Use:   "day02",
		Short: "Day 02 example",
		Run:   run,
	}
	day02Cmd.Flags().StringVarP(&file, "input", "i", "", "Input file with puzzle data")
	aoc2018.RootCmd.AddCommand(day02Cmd)
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

	var n2, n3 int
	lines := make([]string, 0, 128)

	// part 1

	for ; scanner.Scan(); {
		line := scanner.Text()
		if line == "" {
			break
		}
		// collect for part2
		lines = append(lines, line)

		letters := make(map[rune]int, 0)
		for _, l := range line {
			fmt.Printf("[%c]\n", l)
			if _, ok := letters[rune(l)]; !ok {
				letters[rune(l)] = 0
			}
			letters[rune(l)]++
		}
		var c2, c3 bool
		for _, v := range letters {
			if v == 2 && !c2 {
				n2++
				c2 = true
			}
			if v == 3 && !c3 {
				n3++
				c3 = true
			}
		}
	}

	// part 2

	result := ""
loop:
	for _, l1 := range lines {
		for _, l2 := range lines {
			differs := 0
			differsAt := 0
			for i := 0; i < len(l1); i++ {
				if l1[i] != l2[i] {
					differsAt = i
					differs++
				}
			}
			if differs == 1 {
				fmt.Printf("%s == %s\n", l1, l2)
				for i := 0; i < len(l1); i++ {
					if differsAt != i {
						fmt.Printf("%s\n", string(l1[i]))
						result += string(l1[i])
					}
				}
				break loop
			}
		}
	}

	fmt.Printf("Answer 1: %d, Answer 2: %s\n", n2*n3, result)
}
