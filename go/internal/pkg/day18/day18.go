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
)

var file string

func init() {
	var cmd = &cobra.Command{
		Use:   "day18",
		Short: "Day 18 example",
		Run:   run,
	}
	cmd.Flags().StringVarP(&file, "file", "f", "", "Input file with puzzle data")
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

	expressions := make([]string, 0)

	for ; scanner.Scan(); {
		line := scanner.Text()
		if line == "" {
			break
		}
		expressions = append(expressions, line)
	}

	// part 1

	count1 := 0
	for _, e := range expressions {
		stack := list.New()
		operators := list.New()

		var v1, v2 int
		var op rune

		idx := -1
		// fmt.Printf("e: %s\n", e)
		for {
			idx++
			if idx == len(e) {
				break
			}
			v := e[idx]
			// e.g., "8 + ((9 * 7) + 2) + (4 * (9 * 3 * 9 + 3 + 8) + 6 + 5 + 8)"
			// 8: 8
			// (: 8 0
			// (: 8 0 0
			// 9: 8 0 9
			// 7: 8 0 63
			// ): 8 63
			// 2: 8 65
			// ): 73
			// e.g., "1 + 2 * 3 + 4 * 5 + 6" == 71
			// there are no 2-digit numbers
			if v == ' ' {
				continue
			}
			if v >= '0' && v <= '9' {
				// number
				v2 = int(v - '0')
				if stack.Len() == 0 {
					stack.PushBack(v2)
				} else {
					if stack.Back().Value.(int) == 0 {
						stack.Back().Value = v2
					} else {
						v1 = stack.Remove(stack.Back()).(int)
						op = operators.Remove(operators.Back()).(rune)
						if op == '*' {
							stack.PushBack(v1 * v2)
						} else {
							stack.PushBack(v1 + v2)
						}
					}
				}
			} else if v == '*' || v == '+' {
				operators.PushBack(rune(v))
			} else if v == '(' {
				stack.PushBack(0)
			} else if v == ')' {
				// in this order!
				if stack.Len() > 1 {
					v2 = stack.Remove(stack.Back()).(int)
					v1 = stack.Remove(stack.Back()).(int)
					if v1 == 0 {
						stack.PushBack(v2)
					} else {
						op = operators.Remove(operators.Back()).(rune)
						if op == '*' {
							stack.PushBack(v1 * v2)
						} else {
							stack.PushBack(v1 + v2)
						}
					}
				}
			}
		}
		count1 += stack.Remove(stack.Back()).(int)
	}

	// part 2

	count2 := 0
	for _, e := range expressions {
		stack := list.New()
		operators := list.New()

		var v1, v2 int

		e = "(" + e + ")"

		idx := -1
		fmt.Printf("e: %s\n", e)
		for {
			idx++
			if idx == len(e) {
				break
			}
			v := e[idx]
			// e.g., "8 + ((9 * 7) + 2) + (4 * (9 * 3 * 9 + 3 + 8) + 6 + 5 + 8)"
			// 8: 8
			// +: +
			// (: 8 0
			// (: 8 0 0
			// 9: 8 0 0 9
			// *: + *
			// 7: 8 0 0 9 7
			// ): 8 0 63
			// +:
			// 2: 8 0 65
			// ): 8 65
			if v == ' ' {
				continue
			}
			if v >= '0' && v <= '9' {
				// number
				stack.PushBack(int(v - '0'))
			} else if v == '*' || v == '+' {
				operators.PushBack(rune(v))
			} else if v == '(' {
				stack.PushBack(0)
			} else if v == ')' {
				// collapse full content - add first, then multiply
				lastZero := stack.Back()
				args := list.New()
				ops := list.New()
				for {
					if lastZero == nil {
						break
					}
					if lastZero.Value.(int) == 0 {
						stack.Remove(lastZero)
						break
					}
					args.PushFront(stack.Remove(stack.Back()))
					lastZero = stack.Back()
				}
				for i := 0; i < args.Len()-1; i++ {
					ops.PushFront(operators.Remove(operators.Back()))
				}
				fmt.Println("):")
				for v := args.Front(); v != nil; v = v.Next() {
					fmt.Printf("%d ", v.Value.(int))
				}
				fmt.Println()
				for v := ops.Front(); v != nil; v = v.Next() {
					fmt.Printf("%c ", v.Value.(rune))
				}
				fmt.Println()

				mul := 1
				for {
					start := args.Front()
					change := false
					for v := ops.Front(); v != nil; v = v.Next() {
						v1 = start.Value.(int)
						v2 = start.Next().Value.(int)
						if v.Value.(rune) == '+' {
							change = true
							ops.Remove(v)
							args.Remove(start.Next())
							start.Value = v1 + v2
							break
						} else {
							start = start.Next()
						}
					}
					if !change {
						// remaining multiplications
						for v := args.Front(); v != nil; v = v.Next() {
							mul *= v.Value.(int)
						}
						break
					}
					if !change {
						start = start.Next()
					}
					if start == nil {
						break
					}
				}
				stack.PushBack(mul)
			}
		}

		for v := stack.Front(); v != nil; v = v.Next() {
			fmt.Printf("%d ", v.Value.(int))
		}
		fmt.Println()
		for v := operators.Front(); v != nil; v = v.Next() {
			fmt.Printf("%c ", v.Value.(rune))
		}
		fmt.Println()

		count2 += stack.Front().Value.(int)
	}

	// answer1 = 464478013511, answer2 = 85660197232452
	fmt.Printf("Answer 1: %d, Answer 2: %d\n", count1, count2)
}
