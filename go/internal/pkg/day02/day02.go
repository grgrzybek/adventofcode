/*
 * Copyright 2022 Grzegorz Grzybek
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package aoc2022

import (
	"bufio"
	"fmt"
	"github.com/grgrzybek/adventofcode2022/internal/app/aoc2022"
	"github.com/spf13/cobra"
	"os"
)

var file string

func init() {
	var day02Cmd = &cobra.Command{
		Use:   "day02",
		Short: "Day 02 example",
		Run:   run,
	}
	day02Cmd.Flags().StringVarP(&file, "file", "f", "", "Input file with puzzle data")
	aoc2022.RootCmd.AddCommand(day02Cmd)
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

	playerA := make([]rune, 0)
	playerB := make([]rune, 0)

	for scanner.Scan() {
		line := scanner.Text()
		if line == "" {
			break
		}
		playerA = append(playerA, []rune(line)[0])
		playerB = append(playerB, []rune(line)[2])
	}

	// part 1
	score := make(map[string]int)
	score["AX"] = 4
	score["AY"] = 8
	score["AZ"] = 3
	score["BX"] = 1
	score["BY"] = 5
	score["BZ"] = 9
	score["CX"] = 7
	score["CY"] = 2
	score["CZ"] = 6

	var answer1 = 0
	for idx, v1 := range playerA {
		v2 := playerB[idx]
		game := make([]rune, 2)
		game[0] = v1
		game[1] = v2
		answer1 += score[string(game)]
	}

	// part 2
	score = make(map[string]int)
	score["AX"] = 3
	score["AY"] = 4
	score["AZ"] = 8
	score["BX"] = 1
	score["BY"] = 5
	score["BZ"] = 9
	score["CX"] = 2
	score["CY"] = 6
	score["CZ"] = 7

	var answer2 = 0
	for idx, v1 := range playerA {
		v2 := playerB[idx]
		game := make([]rune, 2)
		game[0] = v1
		game[1] = v2
		answer2 += score[string(game)]
	}

	fmt.Printf("Answer 1: %d, Answer 2: %d\n", answer1, answer2)
}
