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
	"encoding/hex"
	"fmt"
	"io/ioutil"
	"os"
	"regexp"
	"strconv"

	"github.com/grgrzybek/adventofcode2017/go/cmd"
	"github.com/spf13/cobra"
)

var file string

func init() {
	var day10Cmd = &cobra.Command{
		Use:   "day10 [input-length]",
		Short: "Day 10 example",
		Run:   run,
	}
	day10Cmd.Flags().StringVarP(&file, "input", "i", "", "Input file with puzzle data")
	aoc2017.RootCmd.AddCommand(day10Cmd)
}

func run(cmd *cobra.Command, args []string) {
	if file == "" || len(args) < 1 {
		_ = cmd.Usage()
		os.Exit(1)
	}

	var buf []byte
	var e error
	if buf, e = ioutil.ReadFile(file); e != nil {
		_, _ = fmt.Fprintf(os.Stderr, "Error reading file \"%s\": %s\n", file, e)
		os.Exit(1)
	}

	// input lengths to perform hash operations with
	var lengths1 = make([]int, 0, 32)
	// string version of input to perform hash operations with
	var lengthsString string
	var lengths2 = make([]int, 0, 32)

	// list of values to perform hash operations on
	N, _ := strconv.Atoi(args[0])
	var input1 = make([]int, 0, N)
	var sparseHash = make([]int, 0, N)
	var pos = 0
	var skip = 0

	for i := 0; i < N; i++ {
		input1 = append(input1, i)
		sparseHash = append(sparseHash, i)
	}

	fileScanner := bufio.NewScanner(bytes.NewReader(buf))
	re := regexp.MustCompile(", *")
	if fileScanner.Scan() {
		lengthsString = fileScanner.Text()
		values := re.Split(lengthsString, -1)
		for _, v := range values {
			i, _ := strconv.Atoi(v)
			lengths1 = append(lengths1, i)
		}
		// ascii to int
		for _, v := range []byte(lengthsString) {
			lengths2 = append(lengths2, int(v))
		}
	}
	// append "the standard length suffix values"
	lengths2 = append(lengths2, []int{17, 31, 73, 47, 23}...)

	for _, l := range lengths1 {
		sub := make([]int, l)
		for i := 0; i < l; i++ {
			sub[i] = input1[(pos+i)%len(input1)]
		}
		// in reversed order
		for i := 0; i < l; i++ {
			input1[(pos+i)%len(input1)] = sub[l-1-i]
		}
		pos += l + skip
		skip++
	}

	fmt.Printf("Answer 1: %d\n", input1[0]*input1[1])

	pos = 0
	skip = 0
	for i := 0; i < 64; i++ {
		for _, l := range lengths2 {
			sub := make([]int, l)
			for i := pos; i < pos+l; i++ {
				sub[i-pos] = sparseHash[i%len(sparseHash)]
			}
			for i := pos; i < pos+l; i++ {
				sparseHash[i%len(sparseHash)] = sub[pos+l-1-i]
			}
			pos += l + skip
			skip++
		}
	}

	// calculate dense hash
	denseHash := make([]byte, 16, 16)
	for i := 0; i < 16; i ++ {
		denseHash[i] = 0
		for j := 0; j < 16; j++ {
			denseHash[i] ^= byte(sparseHash[(i*16)+j])
		}
	}

	fmt.Printf("Answer 2: %s\n", hex.EncodeToString(denseHash))
}
