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
	"strings"
)

var file string

func init() {
	var cmd = &cobra.Command{
		Use:   "day16",
		Short: "Day 16 example",
		Run:   run,
	}
	cmd.Flags().StringVarP(&file, "file", "f", "", "Input file with puzzle data")
	aoc2020.RootCmd.AddCommand(cmd)
}

type constraint struct {
	name                   string
	min1, max1, min2, max2 int
}

func (c *constraint) String() string {
	return fmt.Sprintf("\"%s\": %d-%d %d-%d", c.name, c.min1, c.max1, c.min2, c.max2)
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

	constraints := make([]*constraint, 0)
	nearbyTickets := make([][]int, 0)
	validNearbyTickets := make([][]int, 0)
	myTicket := make([]int, 0)
	constraintsStage := true
	nearbyStage := false
	myTicketStage := false

	for ; scanner.Scan(); {
		line := scanner.Text()
		if line == "" {
			constraintsStage = false
			continue
		} else if line == "your ticket:" {
			myTicketStage = true
			continue
		} else if line == "nearby tickets:" {
			nearbyStage = true
			myTicketStage = false
			continue
		}

		if constraintsStage {
			colon := strings.Index(line, ": ")
			name := line[0:colon]
			c := &constraint{name: name}
			_, _ = fmt.Sscanf(line[colon:], ": %d-%d or %d-%d", &c.min1, &c.max1, &c.min2, &c.max2)
			constraints = append(constraints, c)
		}

		if myTicketStage {
			idx := 0
			for {
				i := strings.IndexRune(line[idx:], ',')
				if i == -1 {
					i = len(line)
				} else {
					i += idx
				}
				nr, _ := strconv.ParseInt(line[idx:i], 0, 0)
				myTicket = append(myTicket, int(nr))
				idx = i + 1
				if idx >= len(line) {
					break
				}
			}
		}

		if nearbyStage {
			idx := 0
			nearbyTicket := make([]int, 0)
			for {
				i := strings.IndexRune(line[idx:], ',')
				if i == -1 {
					i = len(line)
				} else {
					i += idx
				}
				nr, _ := strconv.ParseInt(line[idx:i], 0, 0)
				nearbyTicket = append(nearbyTicket, int(nr))
				idx = i + 1
				if idx >= len(line) {
					break
				}
			}
			nearbyTickets = append(nearbyTickets, nearbyTicket)
		}
	}

	// for _, c := range constraints {
	// 	fmt.Printf("constraint: %s\n", c)
	// }
	// for _, nt := range nearbyTickets {
	// 	fmt.Printf("nearby ticket: %v\n", nt)
	// }

	// part 1

	wrongValues := 0

	for _, nt := range nearbyTickets {
		ticketValid := true
		for _, number := range nt {
			valid := false
			for _, c := range constraints {
				if (number >= c.min1 && number <= c.max1) || (number >= c.min2 && number <= c.max2) {
					valid = true
					break
				}
			}
			if !valid {
				wrongValues += number
				ticketValid = false
			}
		}
		if ticketValid {
			validNearbyTickets = append(validNearbyTickets, nt)
		}
	}

	// part 2

	mapping := make(map[string]int)
	done := make(map[int]bool)
	size := len(validNearbyTickets[0])
	for _, c := range constraints {
		mapping[c.name] = -1
	}

	// a position may be matched by many constraints, but single constraint matches only one position (I think)
	for {
		for _, c := range constraints {
			validCount := 0
			validPos := -1
			for pos := 0; pos < size; pos++ {
				if d, ok := done[pos]; ok && d {
					continue
				}
				valid := true
				for _, nt := range validNearbyTickets {
					number := nt[pos]
					if !((number >= c.min1 && number <= c.max1) || (number >= c.min2 && number <= c.max2)) {
						valid = false
						break
					}
				}
				number := myTicket[pos]
				if !((number >= c.min1 && number <= c.max1) || (number >= c.min2 && number <= c.max2)) {
					valid = false
				}
				if valid {
					validCount++
					validPos = pos
				}
			}
			if validCount == 1 {
				mapping[c.name] = validPos
				done[validPos] = true
				break
			}
		}
		end := true
		for _, m := range mapping {
			if m == -1 {
				end = false
			}
		}
		if end {
			break
		}
	}

	// for _, nt := range validNearbyTickets {
	// 	fmt.Printf("v: %v\n", nt)
	// }

	for k, v := range mapping {
		fmt.Printf("%s -> %d\n", k, v)
	}

	v2 := 1
	for k, v := range mapping {
		if strings.HasPrefix(k, "departure ") {
			v2 *= myTicket[v]
		}
	}

	// answer1 = 27911, answer2 = 737176602479
	fmt.Printf("Answer 1: %d, Answer 2: %d\n", wrongValues, v2)
}
