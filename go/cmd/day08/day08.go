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
	"strings"

	"github.com/grgrzybek/adventofcode2017/go/cmd"
	"github.com/spf13/cobra"
)

var file string

func init() {
	var day08Cmd = &cobra.Command{
		Use:   "day08",
		Short: "Day 08 example",
		Run:   run,
	}
	day08Cmd.Flags().StringVarP(&file, "input", "i", "", "Input file with puzzle data")
	aoc2017.RootCmd.AddCommand(day08Cmd)
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

	regs := make(map[string]int32)
	var max2 int32 = 0

	lineScanner := bufio.NewScanner(bytes.NewReader(buf))
	for lineScanner.Scan() {
		line := strings.Trim(lineScanner.Text(), " \n\r\t")
		if line != "" {
			var reg1, reg2, op1, op2 string
			var v1, v2 int32
			fmt.Sscanf(line, "%s %s %d if %s %s %d", &reg1, &op1, &v1, &reg2, &op2, &v2)

			for _, r := range []string{ reg1, reg2 } {
				if _, ok := regs[r]; !ok {
					regs[r] = 0
				}
			}
			cv := regs[reg2]
			var cond bool
			switch op2 {
			case "<": cond = cv < v2
			case ">": cond = cv > v2
			case "<=": cond = cv <= v2
			case ">=": cond = cv >= v2
			case "==": cond = cv == v2
			case "!=": cond = cv != v2
			}
			if cond {
				if op1 == "inc" {
					regs[reg1] += v1
				} else {
					regs[reg1] -= v1
				}
			}

			for _, v := range regs {
				if v > max2 {
					max2 = v
				}
			}

		}
	}

	var max1 int32 = 0
	for _, v := range regs {
		if v > max1 {
			max1 = v
		}
	}

	fmt.Printf("Answer 1: %d\n", max1)
	fmt.Printf("Answer 2: %d\n", max2)
}
