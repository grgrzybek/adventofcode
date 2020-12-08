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
		Use:   "day08",
		Short: "Day 08 example",
		Run:   run,
	}
	cmd.Flags().StringVarP(&file, "file", "f", "", "Input file with puzzle data")
	aoc2020.RootCmd.AddCommand(cmd)
}

type instruction struct {
	op  string
	arg int
}

func (ins *instruction) String() string {
	return fmt.Sprintf("%s %d", ins.op, ins.arg)
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

	program := make([]*instruction, 0)

	for ; scanner.Scan(); {
		line := scanner.Text()
		if line == "" {
			break
		}
		split := strings.Split(line, " ")
		arg, _ := strconv.ParseInt(split[1], 10, 0)
		program = append(program, &instruction{split[0], int(arg)})
	}

	// part 1

	acc1 := 0
	ip := 0
	ips := make(map[int]bool)

	for {
		if _, ok := ips[ip]; ok {
			break
		}
		ips[ip] = true
		instruction := program[ip]
		if instruction.op == "nop" {
			ip++
		} else if instruction.op == "acc" {
			acc1 += instruction.arg
			ip++
		} else if instruction.op == "jmp" {
			ip += instruction.arg
		}
	}

	// part 2

	acc2 := 0
	acc2Backup := 0
	ip = 0
	ipBackup := 0
	ips = make(map[int]bool)
	var ipsBackup map[int]bool

	checked := make(map[int]bool)
	inBranch := false

	for {
		if ip == len(program) {
			break
		}
		fmt.Printf("> %s (acc=%d)\n", program[ip], acc2)
		if _, ok := ips[ip]; ok {
			// we hit already visited instruction
			acc2 = acc2Backup
			ip = ipBackup
			ips = make(map[int]bool)
			for k, v := range ipsBackup {
				ips[k] = v
			}
			inBranch = false
		}

		instruction := program[ip]
		code := instruction.op
		if !inBranch && (code == "nop" || code == "jmp") {
			// even if directly after this nop/jmp, we won't hit already visited instruction, we have to check if
			// this nop/jmp should be changed to jmp/nop
			if _, ok := checked[ip]; !ok {
				inBranch = true
				if code == "nop" {
					code = "jmp"
				} else {
					code = "nop"
				}
				checked[ip] = true
				fmt.Printf(" -> %s\n", code)
			}
			ipBackup = ip
			acc2Backup = acc2
			ipsBackup = make(map[int]bool)
			for k, v := range ips {
				ipsBackup[k] = v
			}
		}
		ips[ip] = true
		if code == "nop" {
			ip++
		} else if code == "acc" {
			acc2 += instruction.arg
			ip++
		} else if code == "jmp" {
			ip += instruction.arg
		}
	}

	// answer1 = 1584, answer2 = 0
	fmt.Printf("Answer 1: %d, Answer 2: %d\n", acc1, acc2)
}
