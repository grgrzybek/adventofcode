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
var generations int64

func init() {
	var cmd = &cobra.Command{
		Use:   "day12",
		Short: "Day 12 example",
		Run:   run,
	}
	cmd.Flags().StringVarP(&file, "input", "i", "", "Input file with puzzle data")
	cmd.Flags().Int64VarP(&generations, "generations", "g", 20, "Number of generations")
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
	if f, e = os.Open(file, ); e != nil {
		_, _ = fmt.Fprintln(os.Stderr, e)
		os.Exit(1)
	}
	defer (func(f *os.File) { _ = f.Close() })(f)

	scanner := bufio.NewScanner(f)
	scanner.Split(bufio.ScanLines)

	margin := 50

	scanner.Scan()
	header := scanner.Text()
	state := header[15:]
	state1 := make([]rune, len(state)+margin*2)
	state2 := make([]rune, len(state)+margin*2)

	for i := 0; i < len(state1); i++ {
		state1[i] = '.'
		state2[i] = '.'
	}
	copy(state1[margin:], []rune(state))

	scanner.Scan()
	rules := make([]*rule, 0)

	for ; scanner.Scan(); {
		l := scanner.Text()
		if l == "" {
			break
		}
		rules = append(rules, &rule{state: l[0:5], grow: l[len(l)-1] == '#'})
	}

	var s int64

	fmt.Printf("%02d: %s\n", 0, string(state1))
	for s = 0; s < 20; s++ {
		for pot := 0; pot < len(state1)-4; pot++ {
			for _, r := range rules {
				if string(state1[pot:pot+5]) == r.state {
					if r.grow {
						state2[pot+2] = '#'
					} else {
						state2[pot+2] = '.'
					}
					break
				}
			}
		}
		for pot := 0; pot < len(state1); pot++ {
			state1[pot] = '.'
		}
		state1, state2 = state2, state1
		fmt.Printf("%02d: %s\n", s+1, string(state1))
	}

	var sum int64
	for pot := 0; pot < len(state1); pot++ {
		if state1[pot] == '#' {
			sum += int64(pot - margin)
		}
	}

	fmt.Printf("Answer 1: %d\n", sum)

	// start again
	for i := 0; i < len(state1); i++ {
		state1[i] = '.'
		state2[i] = '.'
	}
	copy(state1[margin:], []rune(state))

	diff := 0
	lastPattern := ""
	var lastSum, change int64

	fmt.Printf("   : .         .         .         .         .         .         .         .         .         .         .         .         .\n")
	fmt.Printf("%03d: %s\n", 0, string(state1))
	for s = 0; s < 50000000000; s++ {
		// normal step
		for pot := 0; pot < len(state1)-4; pot++ {
			for _, r := range rules {
				if string(state1[pot:pot+5]) == r.state {
					if r.grow {
						state2[pot+2] = '#'
					} else {
						state2[pot+2] = '.'
					}
					break
				}
			}
		}
		for pot := 0; pot < len(state1); pot++ {
			state1[pot] = '.'
		}
		fmt.Printf("%03d: %s", s+1, string(state2))

		// optimize srcIdx to copy from

		srcPos := 0
		srcPosSet := false
		for pot := 0; pot < margin; pot++ {
			if state2[pot] == '#' {
				srcPosSet = true
			}
		}

		if !srcPosSet {
			// we can optimize
			for pot := margin; pot < len(state2); pot++ {
				if state2[pot] == '#' {
					srcPos = pot
					break
				}
			}
			fmt.Printf("+ (%d, %d)", srcPos, diff)

			// copy state2 to state1 with shift
			for pot := srcPos; pot < len(state2); pot++ {
				state1[margin+pot-srcPos] = state2[pot]
				state2[pot] = '.'
			}
			diff -= margin - srcPos
		} else {
			// copy as is
			state1, state2 = state2, state1
			fmt.Printf("- (%d, %d)", srcPos, diff)
		}

		sum = 0
		for pot := 0; pot < len(state1); pot++ {
			if state1[pot] == '#' {
				sum += int64(pot - margin + diff)
			}
		}
		fmt.Printf(" [%d]", sum)

		change = sum - lastSum
		fmt.Printf(" {%d}\n", change)
		if lastPattern == string(state1) {
			break
		}
		lastPattern = string(state1)
		lastSum = sum
	}

	fmt.Printf("change: %d\n", change)

	sum = 0
	for pot := 0; pot < len(state1); pot++ {
		if state1[pot] == '#' {
			sum += int64(pot - margin + diff)
		}
	}
	// sum += (change * (50000000000-s))
	fmt.Printf("   : .         .         .         .         .         .         .         .         .         .         .         .         .\n")

	fmt.Printf("Answer 2: %d\n", sum)
	sum += change * (50000000000 - s - 1)
	fmt.Printf("Answer 2: %d\n", sum)
}

type rule struct {
	state string
	grow  bool
}
