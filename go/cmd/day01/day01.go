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
	"fmt"
	"io/ioutil"
	"os"

	"github.com/grgrzybek/adventofcode2017/go/cmd"
	"github.com/spf13/cobra"
)

var file string

func init() {
	var day01Cmd = &cobra.Command{
		Use:   "day01",
		Short: "Day 01 example",
		Run:   run,
	}
	day01Cmd.Flags().StringVarP(&file, "input", "i", "", "Input file with puzzle data")
	aoc2017.RootCmd.AddCommand(day01Cmd)
}

func run(cmd *cobra.Command, _ []string) {
	if file == "" {
		cmd.Usage()
		// fmt.Fprintln(os.Stderr, "File argument is required")
		os.Exit(1)
	}

	var f *os.File
	var e error
	if f, e = os.Open(file, ); e != nil {
		fmt.Fprintln(os.Stderr, e)
		os.Exit(1)
	}

	var buf []byte
	if buf, e = ioutil.ReadAll(f); e != nil {
		fmt.Fprintln(os.Stderr, e)
		os.Exit(1)
	}

	var sum1, sum2, limit int

	for limit = len(buf); buf[limit-1] < byte('0') || buf[limit-1] > byte('9'); {
		limit -= 1
	}

	for i, c := range buf {
		if c == buf[(i + 1) % limit] {
			sum1 += int(c) - int('0')
		}
	}
	for i, c := range buf {
		if c == buf[(i + limit/2) % limit] {
			sum2 += int(c) - int('0')
		}
	}
	fmt.Printf("Answer 1: %d, Answer 2: %d\n", sum1, sum2)
}
