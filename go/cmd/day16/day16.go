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
	"bufio"
	"fmt"
	"github.com/grgrzybek/adventofcode2018/go/cmd"
	"github.com/spf13/cobra"
	"os"
)

var file string

func init() {
	var dayCmd = &cobra.Command{
		Use:   "day16",
		Short: "Day 16 example",
		Run:   run,
	}
	dayCmd.Flags().StringVarP(&file, "input", "i", "", "Input file with puzzle data")
	aoc2018.RootCmd.AddCommand(dayCmd)
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
	defer (func(f *os.File) { _ = f.Close() })(f)

	InitializeInstructions()

	scanner := bufio.NewScanner(f)
	scanner.Split(bufio.ScanLines)

	samples := make([]*sample, 0)
	program := make([][]int, 0)

	for ; scanner.Scan(); {
		l := scanner.Text()
		if l == "" {
			break
		}

		s := &sample{before: make([]int, 4), after: make([]int, 4), instruction: make([]int, 4)}
		_, _ = fmt.Sscanf(l, "Before: [%d, %d, %d, %d]", &s.before[0], &s.before[1], &s.before[2], &s.before[3])
		scanner.Scan()
		l = scanner.Text()
		_, _ = fmt.Sscanf(l, "%d %d %d %d", &s.instruction[0], &s.instruction[1], &s.instruction[2], &s.instruction[3])
		scanner.Scan()
		l = scanner.Text()
		_, _ = fmt.Sscanf(l, "After: [%d, %d, %d, %d]", &s.after[0], &s.after[1], &s.after[2], &s.after[3])
		scanner.Scan() // empty line

		samples = append(samples, s)
	}

	language := make(map[int]*Opcode)
	codes := make(map[int]string)

	result := 0
	for _, sample := range samples {
		fmt.Printf("Checking %s\n", sample)
		n, opcode, name, f := howManyOpcodes(sample)
		if n >= 3 {
			result++
		}
		if f != nil {
			language[opcode] = f
			codes[opcode] = *name
		}
	}

	fmt.Printf("language:\n")
	for opcode, f := range language {
		fmt.Printf("[%d]: %q\n", opcode, f)
	}
	for opcode, n := range codes {
		fmt.Printf("[%d]: %s\n", opcode, n)
	}

	// skip empty lines
	for ; scanner.Scan(); {
		l := scanner.Text()
		if l != "" {
			line := make([]int, 4)
			_, _ = fmt.Sscanf(l, "%d %d %d %d", &line[0], &line[1], &line[2], &line[3])
			program = append(program, line)
			fmt.Printf("Broke at line %v\n", l)
			break
		}
	}

	// collect sample program lines
	for ; scanner.Scan(); {
		l := scanner.Text()
		line := make([]int, 4)
		_, _ = fmt.Sscanf(l, "%d %d %d %d", &line[0], &line[1], &line[2], &line[3])
		program = append(program, line)
	}

	fmt.Printf("program length: %d\n", len(program))

	registers := []int{0, 0, 0, 0}
	for _, l := range program {
		Opcodes[l[0]](registers, l[1], l[2], l[3])
	}

	fmt.Printf("Answer 1: %d, Answer 2: %d\n", result, registers[0])
}

func howManyOpcodes(s *sample) (int, int, *string, *Opcode) {
	n := 0
	regs := make([]int, 4)
	var matchingOC int
	var matchingName string
	var matchingF *Opcode
	for _, Opcode := range Opcodes {
		copy(regs, s.before)
		name, _ := Opcode(regs, s.instruction[1], s.instruction[2], s.instruction[3])
		same := true
		for i, v := range s.after {
			if v != regs[i] {
				same = false
				break
			}
		}
		if same {
			n++
			matchingF = &Opcode
			matchingOC = s.instruction[0]
			matchingName = name
		}
	}
	if n == 1 {
		return 1, matchingOC, &matchingName, matchingF
	} else {
		return n, 0, nil, nil
	}
}

type sample struct {
	before, after []int
	instruction   []int
}

func (s *sample) String() string {
	return fmt.Sprintf("[%d] p1=%d p2=%d p3=%d", s.instruction[0], s.instruction[1], s.instruction[2], s.instruction[3])
}
