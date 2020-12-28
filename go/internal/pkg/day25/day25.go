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
		Use:   "day25",
		Short: "Day 25 example",
		Run:   run,
	}
	cmd.Flags().StringVarP(&file, "file", "f", "", "Input file with puzzle data")
	aoc2020.RootCmd.AddCommand(cmd)
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

	var cardPubKey, doorPubKey int

	scanner.Scan()
	v, _ := strconv.ParseInt(scanner.Text(), 10, 0)
	cardPubKey = int(v)
	scanner.Scan()
	v, _ = strconv.ParseInt(scanner.Text(), 10, 0)
	doorPubKey = int(v)

	// part 1

	cardLoopSize := 1
	for {
		v := 1
		ok := false
		i := 1
		for ; ; i++ {
			v *= 7
			v = v % 20201227
			if v == cardPubKey {
				ok = true
				break
			}
		}
		if ok {
			cardLoopSize = i
			break
		}
	}

	doorLoopSize := 1
	for {
		v := 1
		ok := false
		i := 1
		for ; ; i++ {
			v *= 7
			v = v % 20201227
			if v == doorPubKey {
				ok = true
				break
			}
		}
		if ok {
			doorLoopSize = i
			break
		}
	}

	fmt.Printf("%d %d\n", cardLoopSize, doorLoopSize)

	key := 1
	for i := 0; i < doorLoopSize; i++ {
		key *= cardPubKey
		key = key % 20201227
	}

	// part 2

	// answer1 = 19924389, answer2 = 0
	fmt.Printf("Answer 1: %d, Answer 2: %d\n", key, 0)
}
