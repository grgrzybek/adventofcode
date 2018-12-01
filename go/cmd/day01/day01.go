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
	var day01Cmd = &cobra.Command{
		Use:   "day01",
		Short: "Day 01 example",
		Run:   run,
	}
	day01Cmd.Flags().StringVarP(&file, "input", "i", "", "Input file with puzzle data")
	aoc2018.RootCmd.AddCommand(day01Cmd)
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

	var frequency1, v int
	frequency1 = 0

	frequencies := make([]int, 0, 128)

	// part 1

	for ; scanner.Scan(); {
		line := scanner.Text()
		if line == "" {
			break
		}
		if _, e := fmt.Sscanf(line, "%d", &v); e == nil {
			frequencies = append(frequencies, v)
			frequency1 += v
		} else {
			_, _ = fmt.Fprintln(os.Stderr, e)
		}
	}

	// part 2

	frequency2 := 0
	duplicates := make(map[int]int)
	duplicates[0] = 1
loop:
	for {
		for _, v := range frequencies {
			frequency2 += v
			if _, ok := duplicates[frequency2]; !ok {
				duplicates[frequency2] = 1
			} else {
				break loop
			}
		}
	}

	fmt.Printf("Answer 1: %d, Answer 2: %d\n", frequency1, frequency2)
}
