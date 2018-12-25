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
	var cmd = &cobra.Command{
		Use:   "day25",
		Short: "Day 25 example",
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

	allPoints := make([]*point, 0)
	constellations := make([]*constellation, 0)

	for ; scanner.Scan(); {
		l := scanner.Text()
		if l == "" {
			break
		}
		p := new(point)

		_, _ = fmt.Sscanf(l, "%d,%d,%d,%d", &p.x, &p.y, &p.z, &p.t)
		allPoints = append(allPoints, p)
	}

	// create 1st constellation
	c1 := &constellation{points: make([]*point, 0)}
	constellations = append(constellations, c1)

	// assign points to constellations

	for {
		change := false
		end := true
		for i, p1 := range allPoints {
			if p1 == nil {
				continue
			}
			end = false
			if len(c1.points) == 0 {
				c1.points = append(c1.points, p1)
				allPoints[i] = nil
				change = true
				break
			}
			for _, p2 := range c1.points {
				if p1 == nil {
					continue
				}
				if distance(p1, p2) <= 3 {
					c1.points = append(c1.points, p1)
					allPoints[i] = nil
					change = true
					break
				}
			}
		}
		if end {
			break
		}
		if !change {
			c1 = &constellation{points: make([]*point, 0)}
			constellations = append(constellations, c1)
		}
	}

	fmt.Printf("Answer 1: %d, Answer 2: %d\n", len(constellations), 0)
}

func distance(p1 *point, p2 *point) int {
	d := 0
	d1 := p1.x - p2.x
	d2 := p1.y - p2.y
	d3 := p1.z - p2.z
	d4 := p1.t - p2.t
	d = int(math.Abs(float64(d1)))+int(math.Abs(float64(d2)))+int(math.Abs(float64(d3)))+int(math.Abs(float64(d4)))
	return d
}

type point struct {
	x, y, z, t int
}

type constellation struct {
	points []*point
}
