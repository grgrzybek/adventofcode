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
	var day06Cmd = &cobra.Command{
		Use:   "day06",
		Short: "Day 06 example",
		Run:   run,
	}
	day06Cmd.Flags().StringVarP(&file, "input", "i", "", "Input file with puzzle data")
	aoc2018.RootCmd.AddCommand(day06Cmd)
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

	coordinates := make([][]int, 0, 128)
	var minx, miny, maxx, maxy int
	minx = -1
	miny = -1

	// finding boundaries

	for ; scanner.Scan(); {
		l := scanner.Text()
		if l == "" {
			break
		}
		x := 0
		y := 0
		_, _ = fmt.Sscanf(l, "%d, %d", &x, &y)
		coordinates = append(coordinates, []int{x, y})
		if minx == -1 || minx > x {
			minx = x
		}
		if miny == -1 || miny > y {
			miny = y
		}
		if maxx < x {
			maxx = x
		}
		if maxy < y {
			maxy = y
		}
	}

	width := maxx - minx + 1
	height := maxy - miny + 1

	// a grid

	grid := make([][]int, width)
	for x := 0; x < width; x++ {
		grid[x] = make([]int, height)
	}

	// marking regions by setting id of coordinate with smallest distance from given point on grid

	for x := minx; x <= maxx; x++ {
		for y := miny; y <= maxy; y++ {
			minDistance := width + height + 1 // can't be larger
			dot := false
			coordinateIdFound := -1
			for cn, xy := range coordinates {
				dist := x - xy[0]
				if dist < 0 {
					dist = -dist
				}
				if y-xy[1] >= 0 {
					dist += y - xy[1]
				} else {
					dist += -(y - xy[1])
				}
				if minDistance > dist {
					minDistance = dist
					dot = false
					coordinateIdFound = cn
				} else if minDistance == dist {
					dot = true
				}
			}
			if !dot {
				grid[x-minx][y-miny] = coordinateIdFound
			} else {
				grid[x-minx][y-miny] = -1
			}
		}
	}

	// finding largest region

	maxSize := 0
	for cn := range coordinates {
		count := 0
		infinite := false
		for x := minx; x <= maxx; x++ {
			for y := miny; y <= maxy; y++ {
				if grid[x-minx][y-miny] == cn {
					count++
					if x == minx || x == maxx {
						infinite = true
					}
					if y == miny || y == maxy {
						infinite = true
					}
				}
			}
		}
		if maxSize <= count && !infinite {
			maxSize = count
		}
	}

	// finding region where all coordinates have total distance smaller to all coordinates

	minDistance := 10000
	minDistanceRegionSize := 0

	// for each point we calculate distance to
	for y := miny; y <= maxy; y++ {
		for x := minx; x <= maxx; x++ {
			distance := 0
			for _, xy := range coordinates {
				dist := x - xy[0]
				if dist < 0 {
					dist = -dist
				}
				if y-xy[1] >= 0 {
					dist += y - xy[1]
				} else {
					dist += -(y - xy[1])
				}
				distance += dist
			}
			if distance < minDistance {
				minDistanceRegionSize++
			}
		}
	}

	fmt.Printf("Answer 1: %d, Answer 2: %d\n", maxSize, minDistanceRegionSize)
}
