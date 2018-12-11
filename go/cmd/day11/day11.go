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
	"fmt"
	"github.com/grgrzybek/adventofcode2018/go/cmd"
	"github.com/spf13/cobra"
	"os"
)

var serialNumber int

func init() {
	var day11Cmd = &cobra.Command{
		Use:   "day11",
		Short: "Day 11 example",
		Run:   run,
	}
	day11Cmd.Flags().IntVarP(&serialNumber, "serial-number", "s", 5719, "serial number - input data")
	aoc2018.RootCmd.AddCommand(day11Cmd)
}

func run(cmd *cobra.Command, _ []string) {
	if serialNumber == -1 {
		_ = cmd.Usage()
		_, _ = fmt.Fprintln(os.Stderr, "Serial Number argument is required")
		os.Exit(1)
	}

	grid := make([]int, 300*300)

	for x := 0; x < 300; x++ {
		for y := 0; y < 300; y++ {
			idx := x*300 + y
			rackId := x + 1 + 10
			power := rackId * (y + 1)
			power += serialNumber
			power *= rackId
			digit := power % 1000
			digit -= power % 100
			digit /= 100
			digit -= 5
			grid[idx] = digit
		}
	}

	max := 0
	xy := make([]int, 2)

	for x := 0; x < 297; x++ {
		for y := 0; y < 297; y++ {
			sum := 0
			for i := 0; i < 3; i++ {
				for j := 0; j < 3; j++ {
					sum += grid[(x + i) * 300 + (y + j)]
				}
			}
			if max < sum {
				max = sum
				xy[0] = x + 1
				xy[1] = y + 1
			}
		}
	}

	max2 := 0
	xy2 := make([]int, 2)
	s := 0

	for size := 1; size <= 300; size ++ {
		fmt.Printf("current size: %d\n", size)
		for x := 0; x < 300 - size + 1; x++ {
			for y := 0; y < 300 - size + 1; y++ {
				sum := 0
				for i := 0; i < size; i++ {
					for j := 0; j < size; j++ {
						sum += grid[(x + i) * 300 + (y + j)]
					}
				}
				if max2 < sum {
					max2 = sum
					xy2[0] = x + 1
					xy2[1] = y + 1
					s = size
				}
			}
		}
	}

	fmt.Printf("Answer 1: %d,%d, Answer 2: %d,%d,%d\n", xy[0], xy[1], xy2[0], xy2[1], s)
}
