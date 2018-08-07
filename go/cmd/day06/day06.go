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
	var day06Cmd = &cobra.Command{
		Use:   "day06",
		Short: "Day 06 example",
		Run:   run,
	}
	day06Cmd.Flags().StringVarP(&file, "input", "i", "", "Input file with puzzle data")
	aoc2017.RootCmd.AddCommand(day06Cmd)
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

	var tab1 = make([]int, 0, 32)

	fileScanner := bufio.NewScanner(bytes.NewReader(buf))
	if fileScanner.Scan() {
		values := strings.Fields(strings.Trim(fileScanner.Text(), " \n\r\t"))
		for _, v := range values {
			i, _ := strconv.Atoi(v)
			tab1 = append(tab1, i)
		}
	}

	set := make(map[string]int)
	it := 0
	cycleSize := 0
	for {
		if pit, ok := set[fmt.Sprintf("%v", tab1)]; ok {
			cycleSize = it - pit
			break
		}
		set[fmt.Sprintf("%v", tab1)] = it
		it++
		pos := -1
		max := 0
		for p, v := range tab1 {
			if max < v {
				max = v
				pos = p
			}
		}
		v := tab1[pos]
		tab1[pos] = 0
		for i := 0; i < v; i++ {
			tab1[(pos + 1 + i) % len(tab1)]++
		}
	}
	fmt.Printf("Answer 1: %d, Answer 2: %d\n", len(set), cycleSize)
}
