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
		Use:   "day24",
		Short: "Day 24 example",
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


	lines := make([]string, 0)

	for {
		if !scanner.Scan() {
			break
		}
		line := scanner.Text()
		if line == "" {
			break
		}
		lines = append(lines, line)
	}

	// row 1:    | -2:1 | -1:1 |  0:1 |  1:1 |  2:1 |  3:1 |
	//         /    \ /    \ /    \ /    \ /    \ /    \ /    \
	// row 0: | -3:0 | -2:0 | -1:0 |  0:0 |  1:0 |  2:0 |  3:0 |
	//         \    / \    / \    / \    / \    / \    / \    /
	// row -1:   | -3:-1| -2:-1| -1:-1| 0:-1 | 1:-1 | 2:-1 |

	// part 1

	floor := make(map[string]bool) // true == black
	for _, line := range lines {
		idx := 0
		x, y := 0, 0
		for ; idx < len(line); {
			r := line[idx]
			if r == 's' {
				y--
				if line[idx+1] == 'w' {
					x--
				}
				idx++
			} else if r == 'n' {
				y++
				if line[idx+1] == 'e' {
					x++
				}
				idx++
			} else if r == 'w' {
				x--
			} else if r == 'e' {
				x++
			}
			idx++
		}
		key := fmt.Sprintf("%d:%d", x, y)
		v, ok := floor[key]
		if !ok {
			floor[key] = true
		} else if v {
			delete(floor, key)
		}
	}

	count1 := len(floor)

	// part 2 - the floor is ready

	count2 := 0
	for i := 0; i < 100; i++ {
		count2 = 0
		for _, v := range floor {
			if v {
				count2++
			}
		}
		fmt.Printf("Day %02d: %d\n", i+1, count2)
		addWhites(floor)
		floor = flipTiles(floor)
	}

	count2 = 0
	for _, v := range floor {
		if v {
			count2++
		}
	}

	// answer1 = 269, answer2 = 3667 (but slow)
	fmt.Printf("Answer 1: %d, Answer 2: %d\n", count1, count2)
}

func addWhites(floor map[string]bool) {
	for k := range floor {
		addNeighboursIfMissing(floor, k)
	}
}

func addNeighboursIfMissing(floor map[string]bool, k string) {
	x, y := 0, 0
	_, _ = fmt.Sscanf(k, "%d:%d", &x, &y)
	neighbours := make([]string, 6)
	// NW, NE, E, SE, SW, W
	neighbours[0] = fmt.Sprintf("%d:%d", x, y+1)
	neighbours[1] = fmt.Sprintf("%d:%d", x+1, y+1)
	neighbours[2] = fmt.Sprintf("%d:%d", x+1, y)
	neighbours[3] = fmt.Sprintf("%d:%d", x, y-1)
	neighbours[4] = fmt.Sprintf("%d:%d", x-1, y-1)
	neighbours[5] = fmt.Sprintf("%d:%d", x-1, y)
	for _, k := range neighbours {
		if _, ok := floor[k]; !ok {
			floor[k] = false // it's white
		}
	}
}

func flipTiles(floor map[string]bool) map[string]bool {
	newFloor := make(map[string]bool)

	// Any black tile with zero or more than 2 black tiles immediately adjacent to it is flipped to white.
	// Any white tile with exactly 2 black tiles immediately adjacent to it is flipped to black.

	for k, v := range floor {
		count := blackNeighbours(floor, k)
		if v && (count == 0 || count > 2) {
			newFloor[k] = false
		} else if !v && count == 2 {
			newFloor[k] = true
		} else {
			newFloor[k] = v
		}
	}

	return newFloor
}

func blackNeighbours(floor map[string]bool, k string) int {
	count := 0
	x, y := 0, 0
	_, _ = fmt.Sscanf(k, "%d:%d", &x, &y)
	neighbours := make([]string, 6)
	neighbours[0] = fmt.Sprintf("%d:%d", x, y+1)
	neighbours[1] = fmt.Sprintf("%d:%d", x+1, y+1)
	neighbours[2] = fmt.Sprintf("%d:%d", x+1, y)
	neighbours[3] = fmt.Sprintf("%d:%d", x, y-1)
	neighbours[4] = fmt.Sprintf("%d:%d", x-1, y-1)
	neighbours[5] = fmt.Sprintf("%d:%d", x-1, y)
	for _, k := range neighbours {
		if v, ok := floor[k]; ok && v {
			count++
		}
	}

	return count
}
