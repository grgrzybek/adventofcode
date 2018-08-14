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
	"strings"

	"github.com/grgrzybek/adventofcode2017/go/cmd"
	"github.com/spf13/cobra"
)

var file string

func init() {
	var day09Cmd = &cobra.Command{
		Use:   "day09",
		Short: "Day 09 example",
		Run:   run,
	}
	day09Cmd.Flags().StringVarP(&file, "input", "i", "", "Input file with puzzle data")
	aoc2017.RootCmd.AddCommand(day09Cmd)
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

	state := INIT
	groups := 0
	score := 1
	garbageChars := 0

	sr := strings.NewReader(string(buf))
	for {
		ch, _, err := sr.ReadRune()
		if err != nil {
			break
		}
		switch ch {
		case '{':
			if state != GARBAGE {
				groups += score
				score++
			} else {
				garbageChars++
			}
		case '}':
			if state != GARBAGE {
				score--
			} else {
				garbageChars++
			}
		case '<':
			if state != GARBAGE {
				state = GARBAGE
			} else {
				garbageChars++
			}
		case '>':
			state = GROUP
		case '!':
			ch, _, err = sr.ReadRune()
		default:
			if state == GARBAGE {
				garbageChars++
			}
		}
	}

	fmt.Printf("Answer 1: %d\n", groups)
	fmt.Printf("Answer 2: %d\n", garbageChars)
}

const (
	INIT = -(iota + 1)
	GARBAGE
	GROUP
)
