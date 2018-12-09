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
	var day09Cmd = &cobra.Command{
		Use:   "day09",
		Short: "Day 09 example",
		Run:   run,
	}
	day09Cmd.Flags().StringVarP(&file, "input", "i", "", "Input file with puzzle data")
	aoc2018.RootCmd.AddCommand(day09Cmd)
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

	for n := 0; n < 2; n++ {
		_, _ = f.Seek(0, 0)
		scanner := bufio.NewScanner(f)
		scanner.Split(bufio.ScanLines)
		for ; scanner.Scan(); {
			l := scanner.Text()
			if l == "" {
				break
			}
			if l[0] == '#' {
				continue
			}

			var players, last, player int
			_, _ = fmt.Sscanf(l, "%d players; last marble is worth %d points", &players, &last)
			score := make([]int, players)

			ring := &marble{v: 0}
			ring.next = ring
			ring.prev = ring
			current := ring

			if n == 1 {
				last *= 100
			}

			fmt.Printf("%d players, %d marbles\n", players, last)
			for m := 1; m <= last; m++ {
				if m%23 == 0 {
					toRemove := current.prev.prev.prev.prev.prev.prev.prev
					current = toRemove.next
					current.prev = toRemove.prev
					current.prev.next = current
					score[player] += m + toRemove.v
				} else {
					nm := &marble{v: m, prev: current.next, next: current.next.next}
					current.next.next.prev = nm
					current.next.next = nm
					current = nm
				}
				player = (player + 1) % players
			}

			max := 0
			for _, s := range score {
				if s > max {
					max = s
				}
			}
			fmt.Printf("Answer %d: %d\n", n+1, max)
		}
	}
}

type marble struct {
	v          int
	next, prev *marble
}
