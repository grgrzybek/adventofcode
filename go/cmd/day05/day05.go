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
	"io"
	"os"
	"strings"
)

var file string

func init() {
	var day05Cmd = &cobra.Command{
		Use:   "day05",
		Short: "Day 05 example",
		Run:   run,
	}
	day05Cmd.Flags().StringVarP(&file, "input", "i", "", "Input file with puzzle data")
	aoc2018.RootCmd.AddCommand(day05Cmd)
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

	fi, _ := f.Stat()
	buf := make([]byte, fi.Size())
	_, e = io.ReadFull(f, buf)
	if e != nil {
		fmt.Printf("Error: %s\n", e)
		os.Exit(1)
	}

	originalPolymer := make([]rune, 0, fi.Size())
	polymer := make([]rune, fi.Size(), fi.Size())
	for _, b := range buf {
		originalPolymer = append(originalPolymer, rune(b))
	}

	units := "_abcdefghijklmnopqrstuvwxyz"
	bound := len(originalPolymer) - 1

	length1 := 0
	length2 := 0

	for _, unit := range units {
		change := true
		copy(polymer, originalPolymer)
		if unit != '_' {
			for i := 0; i < bound; i++ {
				if polymer[i] == unit || strings.ToLower(string(polymer[i])) == string(unit) {
					polymer[i] = '_'
				}
			}
		}
		for ; change; {
			change = false
			for i := 0; i < bound; i++ {
				if polymer[i] != '_' {
					c1 := strings.ToUpper(string(polymer[i]))
					c2 := "_"
					j := i + 1
					for ; j < bound; j++ {
						if polymer[j] != '_' {
							c2 = strings.ToUpper(string(polymer[j]))
							break
						}
					}
					if c1 == c2 && polymer[i] != polymer[j] {
						// fmt.Printf("[%s:%s]", string(polymer[i]), string(polymer[j]))
						polymer[i] = '_'
						polymer[j] = '_'
						change = true
					}
					i = j - 1
				}
			}
		}

		length := 0
		for _, r := range polymer {
			if r != '_' && r != '\n' {
				if unit == '_' {
					length1++
				}
				length++
			}
		}
		if unit != '_' && (length2 == 0 || length2 > length) {
			length2 = length
		}
	}

	fmt.Printf("Answer 1: %d, Answer 2: %d\n", length1, length2)
}
