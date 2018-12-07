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
	"sort"
)

var file string
var workersCount int

func init() {
	var day07Cmd = &cobra.Command{
		Use:   "day07",
		Short: "Day 07 example",
		Run:   run,
	}
	day07Cmd.Flags().StringVarP(&file, "input", "i", "", "Input file with puzzle data")
	day07Cmd.Flags().IntVarP(&workersCount, "workers", "w", 5, "Number of workers")
	aoc2018.RootCmd.AddCommand(day07Cmd)
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

	befores := make(map[string]map[string]bool)
	var startWith *string

	for ; scanner.Scan(); {
		l := scanner.Text()
		if l == "" {
			break
		}
		var a, b string
		_, _ = fmt.Sscanf(l, "Step %s must be finished before step %s can begin.", &a, &b)

		if startWith == nil {
			startWith = &a
		}
		if _, ok := befores[a]; !ok {
			befores[a] = make(map[string]bool)
		}
		befores[a][b] = true
	}

	// track all befores

	// map of step -> steps that can be invoked after
	allbefores := make(map[string]map[string]bool)
	for step, afters := range befores {
		allbefores[step] = afters
		for a := range afters {
			if _, ok := befores[a]; !ok {
				allbefores[a] = make(map[string]bool)
			}
		}
	}
	for step := range allbefores {
		m := make(map[string]bool)
		m = afterStep(step, allbefores, m)
		allbefores[step] = m
	}

	// map of step -> steps that has to be invoked before
	allafters := make(map[string]map[string]bool)
	for step := range allbefores {
		allafters[step] = make(map[string]bool)
	}
	for step := range allbefores {
		for a := range allbefores[step] {
			allafters[a][step] = true
		}
	}
	allafters2 := make(map[string]map[string]bool)
	for step, befores := range allafters {
		allafters2[step] = befores
	}

	done := make(map[string]bool)
	instruction := ""

	// part 1

	for {
		stepsReady := make([]rune, 0)
		for step, before := range allafters {
			ready := true
			for b := range before {
				if _, ok := done[b]; !ok {
					ready = false
				}
			}
			if ready {
				stepsReady = append(stepsReady, rune(step[0]))
			}
		}
		if len(stepsReady) == 0 {
			break
		}
		sort.Slice(stepsReady, func(a, b int) bool { return stepsReady[a] < stepsReady[b] })
		fmt.Printf("ready: %s\n", string(stepsReady))
		instruction += string(stepsReady[0])
		for _, r := range stepsReady {
			done[string(r)] = true
			delete(allafters, string(r))
			break
		}
	}

	// part 2

	workers := make([]*work, workersCount)
	for k := range workers {
		workers[k] = &work{step: -1, tick: 0}
	}

	done = make(map[string]bool)
	tick := 0

	for {
		stepsReady := make([]rune, 0)
		for step, before := range allafters2 {
			ready := true
			for b := range before {
				if _, ok := done[b]; !ok {
					ready = false
				}
			}
			if ready {
				stepsReady = append(stepsReady, rune(step[0]))
			}
		}
		if len(stepsReady) > 0 {
			sort.Slice(stepsReady, func(a, b int) bool { return stepsReady[a] < stepsReady[b] })
			for _, r := range stepsReady {
				found := false
				for _, w := range workers {
					if w.step == -1 {
						w.step = r
						w.tick = 60 + int(r) - int('A') + 1
						found = true
						delete(allafters2, string(r))
						break
					}
				}
				if !found {
					break
				}
			}
		} else {
			pending := false
			for _, w := range workers {
				if w.tick > 0 {
					pending = true
				}
			}
			if !pending {
				break
			}
		}

		// ticking
		for _, w := range workers {
			w.tick--
			if w.tick == 0 {
				done[string(w.step)] = true
				w.step = -1
			}
		}
		tick++
	}

	fmt.Printf("Answer 1: %s, Answer 2: %d\n", instruction, tick)
}

func afterStep(step string, befores map[string]map[string]bool, afters map[string]bool) map[string]bool {
	result := make(map[string]bool)
	for k, v := range afters {
		result[k] = v
	}
	for after := range befores[step] {
		if _, ok := result[after]; !ok {
			result[after] = true
			transitive := afterStep(after, befores, result)
			for k, v := range transitive {
				result[k] = v
			}
		}
	}
	return result
}

type work struct {
	step rune
	tick int
}
