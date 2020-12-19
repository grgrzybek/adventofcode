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
		Use:   "day19",
		Short: "Day 19 example",
		Run:   run,
	}
	cmd.Flags().StringVarP(&file, "file", "f", "", "Input file with puzzle data")
	aoc2020.RootCmd.AddCommand(cmd)
}

type rule struct {
	name    string
	pattern string
	rules1  []string
	rules2  []string // may be empty
	length  int
}

func (r rule) String() string {
	v := r.name + " (" + strconv.FormatInt(int64(r.length), 10) + "):"
	if r.pattern != "" {
		v += " \"" + r.pattern + "\""
	} else {
		for _, r1 := range r.rules1 {
			v += " " + r1
		}
		if r.rules2 != nil {
			v += " |"
			for _, r2 := range r.rules2 {
				v += " " + r2
			}
		}
	}
	return v
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

	rules := make(map[string]*rule)
	messages := make([]string, 0)

	messagesStage := false
	for ; scanner.Scan(); {
		line := scanner.Text()
		if line == "" {
			messagesStage = true
			continue
		}

		if !messagesStage {
			// rules
			idx := strings.Index(line, ":")
			r := &rule{name: line[0:idx], pattern: "", length: 0}
			rules[r.name] = r
			if line[idx+2] == '"' {
				// final rule
				r.pattern = line[idx+3 : len(line)-1]
			} else {
				// rules or alternatives
				def := line[idx+1:]
				if strings.Index(def, "|") > 0 {
					// two sets of rule references
					r.rules1 = make([]string, 0)
					r.rules2 = make([]string, 0)
					sc1 := bufio.NewScanner(strings.NewReader(def[0:strings.Index(def, "|")]))
					sc1.Split(bufio.ScanWords)
					for ; sc1.Scan(); {
						r.rules1 = append(r.rules1, sc1.Text())
					}
					sc2 := bufio.NewScanner(strings.NewReader(def[strings.Index(def, "|")+1:]))
					sc2.Split(bufio.ScanWords)
					for ; sc2.Scan(); {
						r.rules2 = append(r.rules2, sc2.Text())
					}
				} else {
					// one set of rule references
					r.rules1 = make([]string, 0)
					sc := bufio.NewScanner(strings.NewReader(def))
					sc.Split(bufio.ScanWords)
					for ; sc.Scan(); {
						r.rules1 = append(r.rules1, sc.Text())
					}
				}
			}
		} else {
			// messages
			messages = append(messages, line)
		}
	}

	for _, r := range rules {
		r.length = sizeOfRule(rules, r)
	}

	// part 1

	// for _, r := range rules {
	// 	fmt.Printf("Rule: %s\n", r)
	// }
	// fmt.Printf("Messages: %d\n", len(messages))

	count1 := 0

	for _, msg := range messages {
		if len(msg) == rules["0"].length && matchesRule1(msg, rules, rules["0"], 0, len(msg)) {
			// fmt.Printf("+: %s\n", msg)
			count1++
			// } else {
			// 	fmt.Printf("-: %s\n", msg)
		}
	}

	// part 2

	// 8: 42 | 42 8
	// 11: 42 31 | 42 11 31
	rules["8"].rules1 = make([]string, 1)
	rules["8"].rules1[0] = "42"
	rules["8"].rules2 = make([]string, 2)
	rules["8"].rules2[0] = "42"
	rules["8"].rules2[1] = "8"
	rules["11"].rules1 = make([]string, 2)
	rules["11"].rules1[0] = "42"
	rules["11"].rules1[1] = "31"
	rules["11"].rules2 = make([]string, 3)
	rules["11"].rules2[0] = "42"
	rules["11"].rules2[1] = "11"
	rules["11"].rules2[2] = "31"
	rules["0"].length = 0
	rules["8"].length = 0
	rules["11"].length = 0

	count2 := 0
	r42 := rules["42"]
	r31 := rules["31"]

	for _, msg := range messages {
		// must match AT LEAST "42 42 31"
		from := 0
		m1 := 0
		for {
			if from >= len(msg) {
				break
			}
			fmt.Printf(">>> r42: %d - %d\n", from, from+r42.length)
			if matchesRule2(msg, rules, r42, from, from+r42.length) {
				m1++
				from += r42.length
			} else {
				break
			}
		}
		if m1 < 2 {
			continue
		}

		m2 := 0
		for {
			if from >= len(msg) {
				break
			}
			fmt.Printf(">>> r31: %d - %d\n", from, from+r31.length)
			if matchesRule2(msg, rules, r31, from, from+r31.length) {
				m2++
				from += r31.length
			} else {
				break
			}
		}

		if m1 > 1 && m2 > 0 && m1-m2 >= 1 && from == len(msg) {
			fmt.Printf("Checking %s\n", msg)
			count2++
		}
	}

	// answer1 = 235, answer2 = 379
	fmt.Printf("Answer 1: %d, Answer 2: %d\n", count1, count2)
}

func sizeOfRule(rules map[string]*rule, r *rule) int {
	if r.pattern != "" {
		return len(r.pattern)
	} else {
		l1 := 0
		for _, r1 := range r.rules1 {
			l1 += sizeOfRule(rules, rules[r1])
		}
		if r.rules2 != nil {
			l2 := 0
			for _, r2 := range r.rules2 {
				l2 += sizeOfRule(rules, rules[r2])
			}
			if l1 != l2 {
				panic("!!")
			}
		}
		return l1
	}
}

func matchesRule1(msg string, rules map[string]*rule, r *rule, from int, to int) bool {
	if r.pattern != "" {
		return msg[from:to] == r.pattern
	} else {
		i := from
		m1 := true
		for _, rn := range r.rules1 {
			r1 := rules[rn]
			m1 = m1 && matchesRule1(msg, rules, r1, i, i+r1.length)
			i += r1.length
		}
		if r.rules2 != nil {
			m2 := true
			i = from
			for _, rn := range r.rules2 {
				r2 := rules[rn]
				m2 = m2 && matchesRule1(msg, rules, r2, i, i+r2.length)
				i += r2.length
			}
			return m1 || m2
		} else {
			return m1
		}
	}
}

func matchesRule2(msg string, rules map[string]*rule, r *rule, from int, to int) bool {
	var msg2 = make([]rune, len(msg))
	copy(msg2, []rune(msg))
	for i := 0; i < len(msg); i++ {
		if i < from || i >= to {
			msg2[i] = '.'
		}
	}

	fmt.Printf(" - %s (%d - %d) <- %s\n", string(msg2), from, to, r)
	if r.pattern != "" {
		return msg[from:to] == r.pattern
	} else {
		if to-from != r.length {
			return false
		}
		i := from
		m1 := true
		for _, rn := range r.rules1 {
			r1 := rules[rn]
			m1 = m1 && matchesRule2(msg, rules, r1, i, i+r1.length)
			if !m1 {
				break
			}
			i += r1.length
		}
		if r.rules2 != nil {
			m2 := true
			i = from
			for _, rn := range r.rules2 {
				r2 := rules[rn]
				m2 = m2 && matchesRule2(msg, rules, r2, i, i+r2.length)
				if !m2 {
					break
				}
				i += r2.length
			}
			return m1 || m2
		} else {
			return m1
		}
	}
}
