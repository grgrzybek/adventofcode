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
	"strings"
)

var file string

func init() {
	var cmd = &cobra.Command{
		Use:   "day07",
		Short: "Day 07 example",
		Run:   run,
	}
	cmd.Flags().StringVarP(&file, "file", "f", "", "Input file with puzzle data")
	aoc2020.RootCmd.AddCommand(cmd)
}

type bag struct {
	bags map[string]int
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

	tree1 := make(map[string][]string)
	tree2 := make(map[string]*bag)

	for ; scanner.Scan(); {
		line := scanner.Text()
		if line == "" {
			break
		}
		// light red bags contain 1 bright white bag, 2 muted yellow bags.
		lineScanner := bufio.NewScanner(strings.NewReader(line))
		lineScanner.Split(bufio.ScanWords)

		lineScanner.Scan()
		upper := lineScanner.Text()
		lineScanner.Scan()
		upper += " " + lineScanner.Text()
		lineScanner.Scan()
		lineScanner.Text() // bags
		lineScanner.Scan()
		lineScanner.Text() // contain
		for ; ; {
			lineScanner.Scan()
			howMany := lineScanner.Text()
			if howMany == "no" {
				tree1[upper] = make([]string, 0)
				tree2[upper] = &bag{}
				tree2[upper].bags = make(map[string]int)
				break
			}
			count, _ := strconv.ParseInt(lineScanner.Text(), 10, 0)
			lineScanner.Scan()
			lower := lineScanner.Text()
			lineScanner.Scan()
			lower += " " + lineScanner.Text()
			if _, ok := tree1[upper]; !ok {
				tree1[upper] = make([]string, 0)
				tree2[upper] = &bag{}
				tree2[upper].bags = make(map[string]int)
			}
			tree1[upper] = append(tree1[upper], lower)
			tree2[upper].bags[lower] = int(count)
			lineScanner.Scan()
			bags := lineScanner.Text()
			if strings.HasSuffix(bags, ".") {
				break
			}
		}
	}

	for u, list := range tree1 {
		fmt.Printf("%s:\n", u)
		for _, l := range list {
			fmt.Printf("\t%s:\n", l)
		}
	}

	// part 1

	count1 := 0
	for u := range tree1 {
		if contains(tree1, u) {
			count1++
		}
	}

	// part 2

	count2 := howMany(tree2, "shiny gold", 0)

	// answer1 = 172, answer2 = 39645
	fmt.Printf("Answer 1: %d, Answer 2: %d\n", count1, count2)
}

func contains(tree map[string][]string, u string) bool {
	for _, v := range tree[u] {
		if v == "shiny gold" {
			return true
		}
	}
	for _, u := range tree[u] {
		if contains(tree, u) {
			return true
		}
	}
	return false
}

func howMany(tree2 map[string]*bag, name string, indent int) int {
	var b = tree2[name]
	for i := 0; i < indent; i++ {
		fmt.Printf("  ")
	}
	fmt.Printf("%s\n", name)
	count := 0
	for k, c := range b.bags {
		for i := 0; i < indent+1; i++ {
			fmt.Printf("  ")
		}
		fmt.Printf("%s -> %d\n", k, c)
		count += c
		count += c*howMany(tree2, k, indent+1)
	}
	return count
}
