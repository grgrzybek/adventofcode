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
	"container/list"
	"fmt"
	"github.com/grgrzybek/adventofcode2020/internal/app/aoc2020"
	"github.com/spf13/cobra"
	"os"
	"strconv"
	"strings"
)

var file string
var part2 bool

func init() {
	var cmd = &cobra.Command{
		Use:   "day22",
		Short: "Day 22 example",
		Run:   run,
	}
	cmd.Flags().StringVarP(&file, "file", "f", "", "Input file with puzzle data")
	cmd.Flags().BoolVarP(&part2, "part2", "2", false, "Run Part2 only")
	aoc2020.RootCmd.AddCommand(cmd)
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

	l1 := list.New()
	l2 := list.New()
	l1b := list.New()
	l2b := list.New()

	ln := 1
	for {
		if !scanner.Scan() {
			break
		}
		line := scanner.Text()
		if line == "" {
			ln = 2
			continue
		}
		if strings.HasPrefix(line, "Player") {
			continue
		}
		n, _ := strconv.ParseInt(line, 10, 0)
		if ln == 1 {
			l1.PushBack(int(n))
			l1b.PushBack(int(n))
		} else {
			l2.PushBack(int(n))
			l2b.PushBack(int(n))
		}
	}

	// fmt.Print("deck 1:")
	// for e := l1.Front(); e != nil; e = e.Next() {
	// 	fmt.Printf(" %d", e.Value.(int))
	// }
	// fmt.Print("\ndeck 2:")
	// for e := l2.Front(); e != nil; e = e.Next() {
	// 	fmt.Printf(" %d", e.Value.(int))
	// }
	// fmt.Println()

	// part 1

	if !part2 {
		for {
			if l1.Len() == 0 || l2.Len() == 0 {
				break
			}
			v1 := l1.Remove(l1.Front())
			v2 := l2.Remove(l2.Front())
			if v1.(int) > v2.(int) {
				l1.PushBack(v1)
				l1.PushBack(v2)
			} else {
				l2.PushBack(v2)
				l2.PushBack(v1)
			}
		}
	}

	count1 := 0
	var l *list.List = nil
	if !part2 {
		if l1.Len() == 0 {
			l = l2
		} else {
			l = l1
		}
		pos := l.Len()
		for e := l.Front(); e != nil; e = e.Next() {
			v := e.Value.(int)
			// fmt.Printf("> %d\n", v)
			count1 += pos * v
			pos--
		}
	}

	// part 2

	// decks1 := make(map[string]bool)
	// decks2 := make(map[string]bool)
	playAGame(l1b, l2b/*, decks1, decks2*/)

	count2 := 0
	l = nil
	if l1b.Len() == 0 {
		l = l2b
	} else {
		l = l1b
	}
	pos := l.Len()
	for e := l.Front(); e != nil; e = e.Next() {
		v := e.Value.(int)
		fmt.Printf("> %d\n", v)
		count2 += pos * v
		pos--
	}

	// answer1 = 32489, answer2 = 35676
	fmt.Printf("Answer 1: %d, Answer 2: %d\n", count1, count2)
}

func playAGame(l1 *list.List, l2 *list.List/*, decks1 map[string]bool, decks2 map[string]bool*/) bool {
	// fmt.Println("====================")
	// fmt.Print("deck 1:")
	// for e := l1.Front(); e != nil; e = e.Next() {
	// 	fmt.Printf(" %d", e.Value.(int))
	// }
	// fmt.Print("\ndeck 2:")
	// for e := l2.Front(); e != nil; e = e.Next() {
	// 	fmt.Printf(" %d", e.Value.(int))
	// }
	// fmt.Println()

	decks1 := make(map[string]bool)
	decks2 := make(map[string]bool)

	for {
		if l1.Len() == 0 || l2.Len() == 0 {
			break
		}
		d1 := ""
		d2 := ""
		for e := l1.Front(); e != nil; e = e.Next() {
			d1 += fmt.Sprintf("%d-", e.Value.(int))
		}
		for e := l2.Front(); e != nil; e = e.Next() {
			d2 += fmt.Sprintf("%d-", e.Value.(int))
		}
		if _, ok1 := decks1[d1]; ok1 {
			if _, ok2 := decks2[d2]; ok2 {
				// win for player 1
				return true
			}
		}
		decks1[d1] = true
		decks2[d2] = true

		v1 := l1.Remove(l1.Front()).(int)
		v2 := l2.Remove(l2.Front()).(int)
		p1Wins := false
		if v1 <= l1.Len() && v2 <= l2.Len() {
			l1b := list.New()
			l2b := list.New()
			c1 := 0
			c2 := 0
			for e := l1.Front();; e = e.Next() {
				l1b.PushBack(e.Value)
				c1++
				if c1 == v1 {
					break
				}
			}
			for e := l2.Front();; e = e.Next() {
				l2b.PushBack(e.Value)
				c2++
				if c2 == v2 {
					break
				}
			}
			p1Wins = playAGame(l1b, l2b/*, decks1, decks2*/)
		} else {
			p1Wins = v1 > v2
		}
		if p1Wins {
			l1.PushBack(v1)
			l1.PushBack(v2)
		} else {
			l2.PushBack(v2)
			l2.PushBack(v1)
		}
	}
	return l1.Len() > 0
}
