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
	"strconv"
	"strings"

	"github.com/grgrzybek/adventofcode2017/go/cmd"
	"github.com/spf13/cobra"
)

var file string

func init() {
	var day05Cmd = &cobra.Command{
		Use:   "day05",
		Short: "Day 05 example",
		Run:   run,
	}
	day05Cmd.Flags().StringVarP(&file, "input", "i", "", "Input file with puzzle data")
	aoc2017.RootCmd.AddCommand(day05Cmd)
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

	var tab1 = make([]int, 0, 1024)

	fileScanner := bufio.NewScanner(bytes.NewReader(buf))
	for fileScanner.Scan() {
		line := strings.Trim(fileScanner.Text(), " \n\r\t")
		i, _ := strconv.Atoi(line)
		tab1 = append(tab1, i)
	}
	var tab2 = make([]int, len(tab1))
	copy(tab2, tab1)

	fmt.Printf("Found %d lines\n", len(tab1))
	pos := 0
	steps := 0
	for ; pos >= 0 && pos < len(tab1); {
		tab1[pos], pos = tab1[pos]+1, pos+tab1[pos]
		steps++
	}

	pos2 := 0
	steps2 := 0
	for ; pos2 >= 0 && pos2 < len(tab2); {
		inc := tab2[pos2]
		if inc >= 3 {
			tab2[pos2]--
		} else {
			tab2[pos2]++
		}
		pos2 += inc
		steps2 ++
	}
	fmt.Printf("Answer 1: %d, Answer 2: %d\n", steps, steps2)
}
