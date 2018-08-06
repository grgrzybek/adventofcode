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
package aoc2017

import (
	"fmt"
	"os"

	"github.com/grgrzybek/adventofcode2017/go/cmd"
	"github.com/spf13/cobra"
)

var input int

func init() {
	var day03Cmd = &cobra.Command{
		Use:   "day03",
		Short: "Day 03 example",
		Run:   run,
	}
	day03Cmd.Flags().IntVarP(&input, "input", "i", 312051, "Input value")
	aoc2017.RootCmd.AddCommand(day03Cmd)
}

func run(_ *cobra.Command, _ []string) {
	fmt.Printf("Solving day03 with input = %d\n", input)

	// find the size of square which includes input number
	size := 1
	for {
		if size*size >= input {
			break
		}
		size += 2
	}
	fmt.Printf("Size: %d\n", size)

	// bottom-right, bottom-left, top-left, top-right
	br := size * size
	bl := br - size + 1
	tl := bl - size + 1
	tr := tl - size + 1

	// a bit above br
	br2 := tr - size + 2

	// first manhattan coordinate is always half of the size - 1
	c1 := (size - 1) / 2

	// second manhattan coordinate is the distance from the center of the edge.
	// finding correct edge of square
	a := br2
	b := tr
	if !(a <= input && input <= b) {
		a = tr
		b = tl
	}
	if !(a <= input && input <= b) {
		a = tl
		b = bl
	}
	if !(a <= input && input <= b) {
		a = bl
		b = br
	}
	if !(a <= input && input <= b) {
		fmt.Fprintf(os.Stderr, "Can't find location for %d!\n", input)
		os.Exit(1)
	}

	// to make right edge equal to others
	if a == br2 {
		a--
	}
	fmt.Printf("a=%d, b=%d\n", a, b)

	half := (size + 1) / 2
	middle := a + half - 1
	var c2 int
	if input <= middle {
		c2 = middle - input
	} else {
		c2 = input - middle
	}

	fmt.Printf("Answer 1: %d:%d=%d\n", c1, c2, c1+c2)

	// now with actual allocation, with additional frame of zeros
	var tab = make([][]int, size+2)
	var rows = make([]int, (size+2)*(size+2))
	for row := range tab {
		tab[row], rows = rows[:size+2], rows[size+2:]
	}

	x := (size + 1) / 2
	y := x

	tab[x][y] = 1
	greaterThanInput := 1

	// we'll move outside from center of the square by spiral. right x 1, top x 1, left x 2, bottom x 2,
	// right x 3, top x 3, ...
	dx := 0
	dy := 0
	step := 1
	ddx := []int{1, 0, -1, 0}
	ddy := []int{0, -1, 0, 1}
	ddidx := 0

	for ; greaterThanInput <= input; {
		dx = ddx[ddidx]
		dy = ddy[ddidx]
		ddidx = (ddidx + 1) % 4
		for n := 1; n <= step && greaterThanInput <= input; n++ {
			greaterThanInput = fill(tab, x+(dx*n), y+(dy*n))
		}
		x += dx * step
		y += dy * step
		dx = ddx[ddidx]
		dy = ddy[ddidx]

		ddidx = (ddidx + 1) % 4
		for n := 1; n <= step && greaterThanInput <= input; n++ {
			greaterThanInput = fill(tab, x+(dx*n), y+(dy*n))
		}
		x += dx * step
		y += dy * step
		step++
	}

	fmt.Printf("Answer 2: %d\n", greaterThanInput)
}

func fill(tab [][]int, sx, sy int) int {
	tab[sx][sy] = tab[sx+1][sy-1] + tab[sx+1][sy] + tab[sx+1][sy+1] + tab[sx][sy-1] + tab[sx][sy+1] + tab[sx-1][sy-1] + tab[sx-1][sy] + tab[sx-1][sy+1]
	return tab[sx][sy]
}
