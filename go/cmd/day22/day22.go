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
	"github.com/grgrzybek/adventofcode2018/go/cmd"
	"github.com/spf13/cobra"
	"os"
)

var file string

func init() {
	var cmd = &cobra.Command{
		Use:   "day22",
		Short: "Day 22 example",
		Run:   run,
	}
	cmd.Flags().StringVarP(&file, "input", "i", "", "Input file with puzzle data")
	aoc2018.RootCmd.AddCommand(cmd)
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
	defer (func(f *os.File) { _ = f.Close() })(f)

	scanner := bufio.NewScanner(f)
	scanner.Split(bufio.ScanLines)

	var depth int64
	target := []int64{-1, -1}

	scanner.Scan()
	_, _ = fmt.Sscanf(scanner.Text(), "depth: %d", &depth)
	scanner.Scan()
	_, _ = fmt.Sscanf(scanner.Text(), "target: %d,%d", &target[0], &target[1])

	width := target[0] + 1
	height := target[1] + 1
	fmt.Printf("%d x %d\n", width, height)

	regionEL := make([]int64, width*height)
	regionTypes := make([]int, width*height)

	var x int64
	var y int64
	for y = 0; y <= target[1]; y++ {
		for x = 0; x <= target[0]; x++ {
			var gi int64
			if (x == 0 && y == 0) || (x == target[0] && y == target[1]) {
				gi = 0
			} else if y == 0 {
				gi = int64(x * 16807)
			} else if x == 0 {
				gi = int64(y * 48271)
			} else {
				gi = regionEL[(y-1)*width+x] * regionEL[y*width+x-1]
			}
			fmt.Printf("gi(%d, %d) = %d\n", x, y, gi)
			var el int64
			el = (gi + depth) % 20183
			regionEL[y*width+x] = el
			regionTypes[y*width+x] = int(el % 3)
		}
	}

	risk := 0
	for y = 0; y <= target[1]; y++ {
		for x = 0; x <= target[0]; x++ {
			risk += regionTypes[y*width+x]
			switch regionTypes[y*width+x] {
			case 0: fmt.Printf(".") // rocky
			case 1: fmt.Printf("=") // wet
			case 2: fmt.Printf("|") // narrow
			}
		}
		fmt.Println()
	}

	fmt.Printf("Answer 1: %d, Answer 2: %d\n", risk, 0)
}
