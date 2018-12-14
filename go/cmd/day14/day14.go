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
	"strconv"
)

var recipes int

func init() {
	var dayCmd = &cobra.Command{
		Use:   "day14",
		Short: "Day 14 example",
		Run:   run,
	}
	dayCmd.Flags().IntVarP(&recipes, "recipes", "r", 9, "number of recipes")
	aoc2018.RootCmd.AddCommand(dayCmd)
}

func run(cmd *cobra.Command, _ []string) {
	if recipes == -1 {
		_ = cmd.Usage()
		_, _ = fmt.Fprintln(os.Stderr, "Number of recipes is required")
		os.Exit(1)
	}

	score := make([]int, 2, 2048)
	score[0] = 3
	score[1] = 7

	// indices of two elves
	e1 := 0
	e2 := 1

	for i := 0; i < 10+recipes-2; i++ {
		sum := score[e1] + score[e2]
		if sum > 9 {
			score = append(score, 1, sum-10)
		} else {
			score = append(score, sum)
		}
		e1 += 1 + score[e1]
		e1 %= len(score)
		e2 += 1 + score[e2]
		e2 %= len(score)

		if len(score) >= 10+recipes {
			break
		}
	}

	v := make([]rune, 10)
	for i := 0; i < 10; i++ {
		v[i] = rune(strconv.Itoa(score[recipes+i])[0])
	}

	// part 2
	score = make([]int, 2, 1024*1024*100)
	score[0] = 3
	score[1] = 7
	sl := 2

	// indices of two elves
	e1 = 0
	e2 = 1
	before := 0
	pattern := make([]int, 0)
	for _, r := range []rune(strconv.Itoa(recipes)) {
		pattern = append(pattern, int(r-'0'))
	}
	if len(pattern) > 5 {
		pattern = pattern[:5]
	}
	pl := len(pattern)

	for {
		// fmt.Printf("%d + %d = %d\n", score[e1], score[e2], score[e1] + score[e2])
		sum := score[e1] + score[e2]
		if sum > 9 {
			score = append(score, 1, sum-10)
			sl += 2
		} else {
			score = append(score, sum)
			sl++
		}
		e1 += 1 + score[e1]
		e1 %= len(score)
		e2 += 1 + score[e2]
		e2 %= len(score)

		if sl < pl {
			continue
		}
		ok := true
		if sum > 9 {
			// two trailings!
			for s := 0; s < 2; s++ {
				ok = true
				for i := sl - pl - s; i < sl-s; i++ {
					if score[i] != pattern[i-(sl-pl-s)] {
						ok = false
						break
					}
				}
				if ok {
					break
				}
			}
		} else {
			for i := sl - pl; i < sl; i++ {
				if score[i] != pattern[i-(sl-pl)] {
					ok = false
					break
				}
			}
		}
		if ok {
			before = sl - pl
			break
		}
	}

	fmt.Printf("Answer 1: %s, Answer 2: %d\n", string(v), before)
}
