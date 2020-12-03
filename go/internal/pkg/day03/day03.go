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
		Use:   "day03",
		Short: "Day 03 example",
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

	values := make([]bool, 0)

	var width, height uint16 = 0, 0

	for ; scanner.Scan(); {
		line := scanner.Text()
		if line == "" {
			break
		}
		if width == 0 {
			width = uint16(len(line))
		}
		for _, r := range line {
			values = append(values, '#' == r)
		}
		height++
	}

	// part 1

	var x, y, trees1 uint16 = 0, 0, 0

	for {
		if y == height {
			break
		}
		if x >= width {
			x = x%width
		}
		fmt.Printf("%d:%d\n", x, y)
		if values[y*width+x] {
			trees1++
		}
		x+=3
		y++
	}

	// part 2

	treesList := make([]uint, 0)
	treesList = append(treesList, slide(1, 1, width, height, values))
	treesList = append(treesList, slide(3, 1, width, height, values))
	treesList = append(treesList, slide(5, 1, width, height, values))
	treesList = append(treesList, slide(7, 1, width, height, values))
	treesList = append(treesList, slide(1, 2, width, height, values))

	var trees2 uint = 1
	for _, v := range treesList {
		trees2 *= v
	}

	// Answer 1: 237, Answer 2: 2106818610
	fmt.Printf("Answer 1: %d, Answer 2: %d\n", trees1, trees2)
}

func slide(dx, dy, width, height uint16, values []bool) uint {
	fmt.Printf(">> %d:%d\n", dx, dy)
	var x, y uint16 = 0, 0
	var trees uint
	for {
		if y >= height {
			break
		}
		if x >= width {
			x = x%width
		}
		// fmt.Printf("%d:%d\n", x, y)
		if values[y*width+x] {
			trees++
		}
		x+=dx
		y+=dy
	}
	fmt.Printf("<< %d\n", trees)
	return trees
}
