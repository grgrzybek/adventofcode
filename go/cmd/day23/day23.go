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
	"sort"
)

var file string

func init() {
	var cmd = &cobra.Command{
		Use:   "day23",
		Short: "Day 23 example",
		Run:   run,
	}
	cmd.Flags().StringVarP(&file, "input", "i", "", "Input file with puzzle data")
	aoc2018.RootCmd.AddCommand(cmd)
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
	defer (func(f *os.File) { _ = f.Close() })(f)

	scanner := bufio.NewScanner(f)
	scanner.Split(bufio.ScanLines)

	id := 0
	bots := make([]*nanobot, 0)
	for ; scanner.Scan(); {
		l := scanner.Text()
		if l == "" {
			break
		}

		bot := &nanobot{id: id}
		_, _ = fmt.Sscanf(l, "pos=<%d,%d,%d>, r=%d", &bot.x, &bot.y, &bot.z, &bot.r)
		bots = append(bots, bot)
		id++
	}

	sort.Slice(bots, func(i, j int) bool {
		return bots[i].r > bots[j].r
	})
	fmt.Printf("Bot with largest range: %s\n", bots[0])

	b0 := bots[0]
	count := 0
	for _, b := range bots {
		dx := b0.x - b.x
		dy := b0.y - b.y
		dz := b0.z - b.z
		if dx < 0 {
			dx = -dx
		}
		if dy < 0 {
			dy = -dy
		}
		if dz < 0 {
			dz = -dz
		}
		d := dx + dy + dz
		if d <= b0.r {
			count++
		}
	}

	fmt.Printf("Answer 1: %d, Answer 2: %d\n", count, 0)
}

type nanobot struct {
	id      int
	x, y, z int
	r       int
}

func (b *nanobot) String() string {
	return fmt.Sprintf("[%d] %d,%d,%d: %d", b.id, b.x, b.y, b.z, b.r)
}
