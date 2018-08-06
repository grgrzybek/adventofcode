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
package aoc2017

import (
	"bufio"
	"bytes"
	"fmt"
	"io/ioutil"
	"os"
	"sort"
	"strings"

	"github.com/grgrzybek/adventofcode2017/go/cmd"
	"github.com/spf13/cobra"
)

var file string

func init() {
	var day04Cmd = &cobra.Command{
		Use:   "day04",
		Short: "Day 04 example",
		Run:   run,
	}
	day04Cmd.Flags().StringVarP(&file, "input", "i", "", "Input file with puzzle data")
	aoc2017.RootCmd.AddCommand(day04Cmd)
}

func run(cmd *cobra.Command, _ []string) {
	if file == "" {
		cmd.Usage()
		os.Exit(1)
	}

	var buf []byte
	var e error
	if buf, e = ioutil.ReadFile(file); e != nil {
		fmt.Fprintf(os.Stderr, "Error reading file \"%s\": %s\n", file, e)
		os.Exit(1)
	}
	fmt.Printf("Size of file: %d\n", len(buf))

	var v1, v2 int

	fileScanner := bufio.NewScanner(bytes.NewReader(buf))
	for fileScanner.Scan() {
		lineScanner := bufio.NewScanner(strings.NewReader(fileScanner.Text()))
		tokens := make(map[string]bool)
		sortedTokens := make(map[string]bool)
		c := 0
		lineScanner.Split(bufio.ScanWords)
		for lineScanner.Scan() {
			c++
			tokens[lineScanner.Text()] = true
			var l2 = []rune(lineScanner.Text())
			sort.Slice(l2, func(a, b int) bool {
				return l2[a] < l2[b]
			})
			sortedTokens[string(l2)] = true
		}
		if c == len(tokens) && c > 0 {
			v1++
		}
		if c == len(sortedTokens) && c > 0 {
			v2++
		}
	}

	fmt.Printf("Answer 1: %d, Answer 2: %d\n", v1, v2)
}
