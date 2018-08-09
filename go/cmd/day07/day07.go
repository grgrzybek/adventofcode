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
	"regexp"
	"strconv"
	"strings"

	"github.com/grgrzybek/adventofcode2017/go/cmd"
	"github.com/spf13/cobra"
)

var file string

// go install -gcflags '-N -l'
// dlv exec --headless --listen :2345 --api-version=2 ./aoc2017 -- day07 -i ../src/github.com/grgrzybek/adventofcode2017/go/assets/day07a.txt

func init() {
	var day07Cmd = &cobra.Command{
		Use:   "day07",
		Short: "Day 07 example",
		Run:   run,
	}
	day07Cmd.Flags().StringVarP(&file, "input", "i", "", "Input file with puzzle data")
	aoc2017.RootCmd.AddCommand(day07Cmd)
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

	re1 := regexp.MustCompile("[ \t]*->[ \t]*")
	re2 := regexp.MustCompile(" \\((\\d+)\\)")

	allNames := make(map[string]bool)
	childNames := make(map[string]bool)

	discsByName := make(map[string]*Disc)
	childToParent := make(map[string]string)

	lineScanner := bufio.NewScanner(bytes.NewReader(buf))
	for lineScanner.Scan() {
		line := strings.Trim(lineScanner.Text(), " \n\r\t")
		if line != "" {
			valueStr := re2.FindStringSubmatch(line)
			value, _ := strconv.Atoi(valueStr[1])
			line = re2.ReplaceAllString(line, "")
			values := re1.Split(line, -1)
			allNames[values[0]] = true

			discsByName[values[0]] = &Disc{
				name: values[0],
				discs: make([]*Disc, 0),
				weight: uint(value),
			}
			if len(values) > 1 {
				for _, v := range strings.Split(values[1], ", ") {
					childNames[v] = true
					childToParent[v] = values[0]
				}
			}
		}
	}

	// part 1
	for k := range childNames {
		delete(allNames, k)
	}
	var root *Disc
	for k := range allNames {
		fmt.Printf("Answer 1: %s\n", k)
		root = discsByName[k]
		break
	}

	// part2
	for childName, parentName := range childToParent {
		discsByName[parentName].addChild(discsByName[childName])
	}

	// find the disc that is unbalanced among its siblings but has its own children balanced
	unbalanced := findUnbalanced(discsByName, root)

	// check the siblings, find the "right" balance and adjust own weight
	parent := discsByName[childToParent[unbalanced.name]]
	uw := unbalanced.totalWeight()
	var cw uint = 0
	for _, c := range parent.discs {
		if uw != c.totalWeight() {
			cw = c.totalWeight()
			break
		}
	}
	fmt.Printf("Answer 2: %d\n", unbalanced.weight - (uw - cw))
}

// findUnbalanced starts with bottom-most Disc and returns the top-most Disc
// that should be balanced
func findUnbalanced(discsByName map[string]*Disc, disc *Disc) *Disc {
	balance := make(map[uint][]string)
	for _, child := range discsByName[disc.name].discs {
		var w = child.totalWeight()
		if _, ok := balance[w]; !ok {
			balance[w] = make([]string, 0)
		}
		balance[w] = append(balance[w], child.name)
	}
	if len(balance) > 1 {
		for _, child := range balance {
			if len(child) == 1 {
				unbalancedChild := findUnbalanced(discsByName, discsByName[child[0]])
				if unbalancedChild == nil {
					return discsByName[child[0]]
				} else {
					return unbalancedChild
				}
			}
		}
	}
	return nil
}

type Disc struct {
	name string
	weight uint
	discs []*Disc
}

func (d *Disc) ownWeight() uint {
	return d.weight
}

// totalWeight() returns disc's weight + a sum of weights of all discs above
func (d *Disc) totalWeight() uint {
	var sum uint = 0
	for _, d := range d.discs {
		sum += d.totalWeight()
	}
	return d.weight + sum
}

func (d *Disc) addChild(child *Disc) {
	d.discs = append(d.discs, child)
}
