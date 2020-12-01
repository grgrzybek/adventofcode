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
)

var file string

func init() {
	var day01Cmd = &cobra.Command{
		Use:   "day01",
		Short: "Day 01 example",
		Run:   run,
	}
	day01Cmd.Flags().StringVarP(&file, "file", "f", "", "Input file with puzzle data")
	aoc2020.RootCmd.AddCommand(day01Cmd)
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

	values := make([]int64, 0)

	for ; scanner.Scan(); {
		line := scanner.Text()
		if line == "" {
			break
		}
		v, _ := strconv.ParseInt(line, 10, 0)
		values = append(values, v)
	}

	// part 1

	var n1a, n1b int64
	n1a, n1b = 0, 0

	for idx1, v1 := range values {
		for idx2, v2 := range values {
			if idx1 != idx2 {
				if v1 + v2 == 2020 {
					n1a, n1b = v1, v2
					break
				}
			}
		}
		if n1a != 0 {
			break
		}
	}

	// part 2

	var n2a, n2b, n2c int64
	n2a, n2b, n2c = 0, 0, 0

	for idx1, v1 := range values {
		for idx2, v2 := range values {
			for idx3, v3 := range values {
				if idx1 != idx2 && idx1 != idx3 && idx2 != idx3 {
					if v1 + v2 + v3 == 2020 {
						n2a, n2b, n2c = v1, v2, v3
						break
					}
				}
			}
			if n2a != 0 {
				break
			}
		}
		if n2a != 0 {
			break
		}
	}

	fmt.Printf("Answer 1: %d, Answer 2: %d\n", n1a*n1b, n2a*n2b*n2c)
}
