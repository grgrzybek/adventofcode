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
		Use:   "day11",
		Short: "Day 11 example",
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

	seats0 := make([]rune, 0)
	seats1 := make([]rune, 0)
	W, H := 0, 0

	for ; scanner.Scan(); {
		line := scanner.Text()
		if line == "" {
			break
		}
		if W == 0 {
			W = len(line)
		}
		H++
		for _, r := range line {
			seats0 = append(seats0, r)
			seats1 = append(seats1, r)
		}
	}

	// part 1

	seats2 := make([]rune, len(seats1))
	copy(seats2, seats1)

	p1, p2 := &seats1, &seats2

	for {
		for y := 0; y < H; y++ {
			for x := 0; x < W; x++ {
				(*p2)[y*W+x] = check(*p1, x, y, W, H)
			}
		}
		if string(seats1) == string(seats2) {
			break
		}
		p1, p2 = p2, p1
	}

	count1 := 0
	for _, r := range seats1 {
		if r == '#' {
			count1++
		}
	}

	// part 2

	copy(seats1, seats0)
	copy(seats2, seats0)

	for y:=0; y<H; y++ {
		for x:=0; x<W; x++ {
			fmt.Printf("%c", seats2[y*W+x])
		}
		fmt.Println()
	}

	p1, p2 = &seats1, &seats2

	for {
		for y := 0; y < H; y++ {
			for x := 0; x < W; x++ {
				(*p2)[y*W+x] = check2(*p1, x, y, W, H)
			}
		}
		if string(seats1) == string(seats2) {
			break
		}
		p1, p2 = p2, p1
	}

	count2 := 0
	for _, r := range seats1 {
		if r == '#' {
			count2++
		}
	}

	// answer1 = 2265, answer2 = 2045
	fmt.Printf("Answer 1: %d, Answer 2: %d\n", count1, count2)
}

func check(seats []rune, x int, y int, W int, H int) rune {
	if seats[y*W+x] == '.' {
		return '.'
	}
	free := 0
	nonFree := 0
	if y > 0 {
		// top row
		if x > 0 {
			// left
			if seats[(y-1)*W+x-1] == 'L' {
				free++
			} else if seats[(y-1)*W+x-1] == '#' {
				nonFree++
			}
		}
		if seats[(y-1)*W+x] == 'L' {
			// top
			free++
		} else if seats[(y-1)*W+x] == '#' {
			nonFree++
		}
		if x < W-1 {
			// right
			if seats[(y-1)*W+x+1] == 'L' {
				free++
			} else if seats[(y-1)*W+x+1] == '#' {
				nonFree++
			}
		}
	}
	// middle row
	if x > 0 {
		// left
		if seats[y*W+x-1] == 'L' {
			free++
		} else if seats[y*W+x-1] == '#' {
			nonFree++
		}
	}
	if x < W-1 {
		// right
		if seats[y*W+x+1] == 'L' {
			free++
		} else if seats[y*W+x+1] == '#' {
			nonFree++
		}
	}
	if y < H-1 {
		// bottom row
		if x > 0 {
			// left
			if seats[(y+1)*W+x-1] == 'L' {
				free++
			} else if seats[(y+1)*W+x-1] == '#' {
				nonFree++
			}
		}
		if seats[(y+1)*W+x] == 'L' {
			// bottom
			free++
		} else if seats[(y+1)*W+x] == '#' {
			nonFree++
		}
		if x < W-1 {
			// right
			if seats[(y+1)*W+x+1] == 'L' {
				free++
			} else if seats[(y+1)*W+x+1] == '#' {
				nonFree++
			}
		}
	}

	if seats[y*W+x] == 'L' && nonFree == 0 {
		return '#'
	} else if seats[y*W+x] == '#' && nonFree >= 4 {
		return 'L'
	}
	return seats[y*W+x]
}

func check2(seats []rune, x int, y int, W int, H int) rune {
	if seats[y*W+x] == '.' {
		return '.'
	}
	nonFree := 0
	if y > 0 {
		// top row
		if x > 0 {
			// left
			if !free(x, y, -1, -1, W, H, seats) {
				nonFree++
			}
		}
		// top
		if !free(x, y, 0, -1, W, H, seats) {
			nonFree++
		}
		if x < W-1 {
			// right
			if !free(x, y, 1, -1, W, H, seats) {
				nonFree++
			}
		}
	}
	// middle row
	if x > 0 {
		// left
		if !free(x, y, -1, 0, W, H, seats) {
			nonFree++
		}
	}
	if x < W-1 {
		// right
		if !free(x, y, 1, 0, W, H, seats) {
			nonFree++
		}
	}
	if y < H-1 {
		// bottom row
		if x > 0 {
			// left
			if !free(x, y, -1, 1, W, H, seats) {
				nonFree++
			}
		}
		// bottom
		if !free(x, y, 0, 1, W, H, seats) {
			nonFree++
		}
		if x < W-1 {
			// right
			if !free(x, y, 1, 1, W, H, seats) {
				nonFree++
			}
		}
	}

	if seats[y*W+x] == 'L' && nonFree == 0 {
		return '#'
	} else if seats[y*W+x] == '#' && nonFree >= 5 {
		return 'L'
	}
	return seats[y*W+x]
}

func free(x int, y int, dx int, dy int, W int, H int, seats []rune) bool {
	for {
		x += dx
		y += dy
		if x < 0 || x >= W || y < 0 || y >= H {
			return true
		}
		if seats[y*W+x] == 'L' {
			return true
		} else if seats[y*W+x] == '#' {
			return false
		}
	}
}
