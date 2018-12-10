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
	"math"
	"os"
)

var file string

func init() {
	var day10Cmd = &cobra.Command{
		Use:   "day10",
		Short: "Day 10 example",
		Run:   run,
	}
	day10Cmd.Flags().StringVarP(&file, "input", "i", "", "Input file with puzzle data")
	aoc2018.RootCmd.AddCommand(day10Cmd)
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
	defer (func(f *os.File) { _ = f.Close() })(f)

	scanner := bufio.NewScanner(f)
	scanner.Split(bufio.ScanLines)

	coordinates := make([][]int, 0)

	for ; scanner.Scan(); {
		l := scanner.Text()
		if l == "" {
			break
		}

		var x, y, dx, dy int
		_, _ = fmt.Sscanf(l, "position=<%d, %d> velocity=<%d, %d>", &x, &y, &dx, &dy)
		c := []int{x, y, dx, dy}
		coordinates = append(coordinates, c)
	}

	possibleImages := make([][][]int, 0)
	previousHeight := math.MaxInt32
	seconds := 0

out:
	for {
		miny := math.MaxInt32
		maxy := math.MinInt32
		for _, step := range coordinates {
			if miny > step[1] {
				miny = step[1]
			}
			if maxy < step[1] {
				maxy = step[1]
			}
			height := maxy - miny
			if previousHeight > height {
				previousHeight = height
			}
			if height == 9 {
				possibleImages = append(possibleImages, coordinates)
				break out
			}
		}

		// tick
		for _, step := range coordinates {
			step[0] += step[2]
			step[1] += step[3]
		}
		seconds++
	}

	minx := math.MaxInt32
	maxx := math.MinInt32
	miny := math.MaxInt32
	maxy := math.MinInt32

	for _, step := range possibleImages[0] {
		if minx > step[0] {
			minx = step[0]
		}
		if miny > step[1] {
			miny = step[1]
		}
		if maxx < step[0] {
			maxx = step[0]
		}
		if maxy < step[1] {
			maxy = step[1]
		}
	}

	fmt.Printf("%d %d %d %d\n", minx, maxx, miny, maxy)

	w := maxx - minx
	h := maxy - miny
	fmt.Printf("%d %d\n", w, h)

	image := make([][]int, h+1)
	for i := 0; i <= h; i++ {
		image[i] = make([]int, w+1)
	}
	for _, step := range possibleImages[0] {
		fmt.Printf("l: %d %d\n", len(image), len(image[step[1]-miny]))
		fmt.Printf("%d:%d\n", step[1]-miny, step[0]-minx)
		image[step[1]-miny][step[0]-minx] = 1
	}

	for i := 0; i <= h; i++ {
		for j := 0; j <= w; j++ {
			if image[i][j] == 1 {
				fmt.Printf("#")
			} else {
				fmt.Printf(".")
			}
		}
		fmt.Println()
	}

	fmt.Printf("Answer 2: %d\n", seconds)
}
