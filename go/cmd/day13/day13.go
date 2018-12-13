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
		Use:   "day13",
		Short: "Day 13 example",
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

	trackMap := make([]string, 0)
	var width, height int

	for ; scanner.Scan(); {
		l := scanner.Text()
		if l == "" {
			break
		}
		trackMap = append(trackMap, l)
		width = len(l)
	}
	height = len(trackMap)

	tracks := make([]rune, height*width)
	trains := make([]*cart, 0)

	trainId := 0
	for y := 0; y < height; y++ {
		l := []rune(trackMap[y])
		for x := 0; x < width; x++ {
			switch l[x] {
			case ' ':
				tracks[y*width+x] = empty
			case '-':
				tracks[y*width+x] = we
			case '|':
				tracks[y*width+x] = ns
			case '\\':
				tracks[y*width+x] = nwse
			case '/':
				tracks[y*width+x] = nesw
			case '+':
				tracks[y*width+x] = cross
			case '>', '<':
				tracks[y*width+x] = we
				c := &cart{x: x, y: y, dir: l[x], turn: tl, alive: true, id: trainId}
				trains = append(trains, c)
				trainId++
			case '^', 'v':
				tracks[y*width+x] = ns
				c := &cart{x: x, y: y, dir: l[x], turn: tl, alive: true, id: trainId}
				trains = append(trains, c)
				trainId++
			}
		}
	}

	mapSlash := map[rune]rune{'>': '^', '<': 'v', '^': '>', 'v': '<'}
	mapBackSlash := map[rune]rune{'>': 'v', '<': '^', '^': '<', 'v': '>'}

	collision := []int{-1, -1}

	sortTrains(trains)

outer:
	for {
		sortTrains(trains)

		for _, c := range trains {
			if !c.alive {
				continue
			}
			// fmt.Printf("cart(%d): %s", idx, c)
			dx := 0
			dy := 0
			switch c.dir {
			case '>':
				dx = 1
			case '<':
				dx = -1
			case 'v':
				dy = 1
			case '^':
				dy = -1
			}
			c.x += dx
			c.y += dy
			// what will happen in new place?
			switch tracks[c.y*width+c.x] {
			case ns, we: // no change
			case nwse:
				c.dir = mapBackSlash[c.dir]
			case nesw:
				c.dir = mapSlash[c.dir]
			case cross:
				switch c.turn {
				case straight:
					c.turn = tr
				case tl:
					if c.dir == '^' || c.dir == 'v' {
						c.dir = mapBackSlash[c.dir]
					} else {
						c.dir = mapSlash[c.dir]
					}
					c.turn = straight
				case tr:
					if c.dir == '^' || c.dir == 'v' {
						c.dir = mapSlash[c.dir]
					} else {
						c.dir = mapBackSlash[c.dir]
					}
					c.turn = tl
				}
			}
			// fmt.Printf(" -> cart(%d): %s\n", idx, trains2[idx])

			// is there a collision?
			for _, cart1 := range trains {
				if cart1.alive && cart1.x == c.x && cart1.y == c.y && cart1.id != c.id {
					if collision[0] == -1 {
						collision[0] = cart1.x
						collision[1] = cart1.y
					}
					c.alive = false
					cart1.alive = false
				}
			}
		}

		ac := 0
		for _, t := range trains {
			if t.alive {
				ac++
			}
		}
		if ac <= 1 {
			break outer
		}
	}

	xy := make([]int, 2)
	for _, t := range trains {
		if t.alive {
			xy[0] = t.x
			xy[1] = t.y
		}
	}

	fmt.Printf("Answer 1: %d,%d, Answer 2: %d,%d\n", collision[0], collision[1], xy[0], xy[1])
}

func sortTrains(carts []*cart) {
	sort.Slice(carts, func(i, j int) bool {
		if carts[i].y > carts[j].y {
			return false
		} else if carts[i].y < carts[j].y {
			return true
		} else {
			return carts[i].x < carts[j].x
		}
	})
}

func printTracks(tracks []rune, carts []*cart, height int, width int) {
	fmt.Printf("===\n")
	lines := make([][]rune, height)
	for y := 0; y < height; y++ {
		lines[y] = make([]rune, width)
		for x := 0; x < width; x++ {
			switch tracks[y*width+x] {
			case empty:
				lines[y][x] = ' '
			case ns:
				lines[y][x] = '|'
			case we:
				lines[y][x] = '-'
			case nwse:
				lines[y][x] = '\\'
			case nesw:
				lines[y][x] = '/'
			case cross:
				lines[y][x] = '+'
			}
		}
		for _, c := range carts {
			if c.y == y && c.alive {
				([]rune(lines[y]))[c.x] = c.dir
			}
		}
	}
	for i, l := range lines {
		fmt.Printf("%03d #[ %s ]#\n", i, string(l))
	}
}

const (
	empty rune = ' '
	ns    rune = '|'
	we    rune = '-'
	nwse  rune = '\\'
	nesw  rune = '/'
	cross rune = '+'
)
const (
	tl       byte = 0 // turn left next time
	straight byte = 1 // go straight next time
	tr       byte = 2 // turn right next time
)

type cart struct {
	id    int
	x, y  int
	dir   rune
	turn  byte
	alive bool
}

func (c *cart) String() string {
	return fmt.Sprintf("%d:%d %c (%d)", c.x, c.y, c.dir, c.turn)
}
