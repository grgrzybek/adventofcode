/*
 * Copyright 2022 Grzegorz Grzybek
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package aoc2022

import (
	"bufio"
	"fmt"
	"github.com/grgrzybek/adventofcode2022/internal/app/aoc2022"
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
	aoc2022.RootCmd.AddCommand(day01Cmd)
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

	for scanner.Scan() {
		line := scanner.Text()
		if line == "" {
			break
		}
		v, _ := strconv.ParseInt(line, 10, 0)
		values = append(values, v)
	}

	// part 1

	var answer1 int64

	// part 2

	var answer2 int64

	fmt.Printf("Answer 1: %d, Answer 2: %d\n", answer1, answer2)
}
