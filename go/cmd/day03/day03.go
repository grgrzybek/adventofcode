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
	"os"

	"github.com/grgrzybek/adventofcode2018/go/cmd"
	"github.com/spf13/cobra"
)

var file string

func init() {
	var day03Cmd = &cobra.Command{
		Use:   "day03",
		Short: "Day 03 example",
		Run:   run,
	}
	day03Cmd.Flags().StringVarP(&file, "input", "i", "", "Input file with puzzle data")
	aoc2018.RootCmd.AddCommand(day03Cmd)
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

	lines := make([]*claim, 0, 128)

	var width uint
	var height uint

	// scannning

	for ; scanner.Scan(); {
		line := scanner.Text()
		if line == "" {
			break
		}
		c := &claim{}

		_, e := fmt.Sscanf(line, "#%d @ %d,%d: %dx%d", &c.id, &c.x, &c.y, &c.w, &c.h)
		if e != nil {
			_, _ = fmt.Fprintf(os.Stderr, "Error: %q\n", e)
			os.Exit(1)
		}
		lines = append(lines, c)

		if width < c.x+c.w {
			width = c.x + c.w
		}
		if height < c.y+c.h {
			height = c.y + c.h
		}
	}

	fabric := make([][]uint, height)
	var i uint
	for i = 0; i < height; i++ {
		fabric[i] = make([]uint, width)
	}

	// part 1

	for _, claim := range lines {
		x0 := claim.x
		y0 := claim.y
		for x := x0; x < x0+claim.w; x++ {
			for y := y0; y < y0+claim.h; y++ {
				fabric[y][x]++
			}
		}
	}

	var x, y, conflicts uint
	for x = 0; x < width; x++ {
		for y = 0; y < height; y++ {
			if fabric[y][x] > 1 {
				conflicts++
			}
		}
	}

	// part 2

	var id uint

	for _, claim := range lines {
		x0 := claim.x
		y0 := claim.y
		conflicts := false
	out:
		for x := x0; x < x0+claim.w; x++ {
			for y := y0; y < y0+claim.h; y++ {
				if fabric[y][x] > 1 {
					conflicts = true
					break out
				}
			}
		}
		if !conflicts {
			id = claim.id
			break
		}
	}

	fmt.Printf("Answer 1: %d, Answer 2: %d\n", conflicts, id)
}

type claim struct {
	id         uint
	x, y, w, h uint
}
