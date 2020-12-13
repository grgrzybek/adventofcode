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
	"strconv"
)

var file string

func init() {
	var cmd = &cobra.Command{
		Use:   "day12",
		Short: "Day 12 example",
		Run:   run,
	}
	cmd.Flags().StringVarP(&file, "file", "f", "", "Input file with puzzle data")
	aoc2020.RootCmd.AddCommand(cmd)
}

type step struct {
	dx, dy  int
	arg     int
	forward bool
	rl, rr  bool
}

func NewStep() *step {
	return &step{dx: 0, dy: 0, arg: 0, forward: false, rl: false, rr: false}
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

	steps := make([]*step, 0)

	for ; scanner.Scan(); {
		line := scanner.Text()
		if line == "" {
			break
		}
		s := NewStep()
		switch line[0] {
		case 'N':
			s.dx = 0
			s.dy = -1
			break
		case 'S':
			s.dx = 0
			s.dy = 1
			break
		case 'E':
			s.dx = 1
			s.dy = 0
			break
		case 'W':
			s.dx = -1
			s.dy = 0
			break
		case 'F':
			s.forward = true
			break
		case 'R':
			s.rr = true
			break
		case 'L':
			s.rl = true
			break
		}
		arg, _ := strconv.ParseInt(line[1:], 0, 0)
		s.arg = int(arg)
		steps = append(steps, s)
	}

	// part 1

	x, y := 0, 0
	dx, dy := 1, 0

	for _, step := range steps {
		if step.forward {
			x += dx * step.arg
			y += dy * step.arg
		} else if step.rr {
			for i := 0; i < step.arg; i += 90 {
				dx, dy = -dy, dx
			}
		} else if step.rl {
			for i := 0; i < step.arg; i += 90 {
				dx, dy = dy, -dx
			}
		} else {
			x += step.arg * step.dx
			y += step.arg * step.dy
		}
	}

	if x < 0 {
		x = -x
	}
	if y < 0 {
		y = -y
	}

	// part 2

	x2, y2 := 0, 0
	wx, wy := 10, -1

	for _, step := range steps {
		if step.forward {
			x2 += wx * step.arg
			y2 += wy * step.arg
		} else if step.rr {
			for i := 0; i < step.arg; i += 90 {
				wx, wy = -wy, wx
			}
		} else if step.rl {
			for i := 0; i < step.arg; i += 90 {
				wx, wy = wy, -wx
			}
		} else {
			// n, e, w, s - move the waypoint
			wx += step.arg * step.dx
			wy += step.arg * step.dy
		}
	}

	if x2 < 0 {
		x2 = -x2
	}
	if y2 < 0 {
		y2 = -y2
	}

	// answer1 = 2280, answer2 = 38693
	fmt.Printf("Answer 1: %d, Answer 2: %d\n", x+y, x2+y2)
}
