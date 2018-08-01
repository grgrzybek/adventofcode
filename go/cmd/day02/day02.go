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
	"io"
	"math"
	"os"
	"regexp"
	"strconv"
	"strings"

	"github.com/grgrzybek/adventofcode2017/go/cmd"
	"github.com/spf13/cobra"
)

var file string

func init() {
	var day02Cmd = &cobra.Command{
		Use:   "day02",
		Short: "Day 02 example",
		Run:   run,
	}
	day02Cmd.Flags().StringVarP(&file, "input", "i", "", "Input file with puzzle data")
	aoc2017.RootCmd.AddCommand(day02Cmd)
}

func run(cmd *cobra.Command, _ []string) {
	if file == "" {
		cmd.Usage()
		os.Exit(1)
	}

	var f *os.File
	var e error
	if f, e = os.Open(file, ); e != nil {
		fmt.Fprintln(os.Stderr, e)
		os.Exit(1)
	}

	var size int64
	if fi, e := os.Stat(f.Name()); e == nil {
		size = fi.Size()
	} else {
		fmt.Fprintln(os.Stderr, e)
		os.Exit(1)
	}

	var buf = make([]byte, size)
	if _, e = io.ReadFull(f, buf[:]); e != nil {
		fmt.Fprintln(os.Stderr, e)
		os.Exit(1)
	}

	re := regexp.MustCompile("[\t ]+")
	var reader = bufio.NewReader(bytes.NewReader(buf))
	var sum1, sum2 int
	for {
		line, err := reader.ReadString(byte('\n'))
		line = strings.Trim(line, " \t\r\n")
		if err != nil {
			if err == io.EOF {
				break
			}
			fmt.Fprintln(os.Stderr, e)
			os.Exit(1)
		}
		if len(line) == 0 {
			break
		}

		var ints []int
		var min = math.MaxInt32
		var max = math.MinInt32
		var n1, n2 int

		for _, s := range re.Split(line, -1) {
			i, _ := strconv.ParseInt(s, 10, 32)
			ints = append(ints, int(i))
		}
		for _, i := range ints {
			if max < i {
				max = i
			}
			if min > i {
				min = i
			}
		}
		for _, i := range ints {
			for _, j := range ints {
				if i % j == 0 && i != j {
					n1 = i
					n2 = j
					break
				}
			}
		}
		sum1 += max - min
		sum2 += n1 / n2
	}
	fmt.Printf("Answer 1: %d, Answer 2: %d\n", sum1, sum2)
}
