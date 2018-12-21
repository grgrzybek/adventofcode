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
	"container/list"
	"fmt"
	"github.com/grgrzybek/adventofcode2018/go/cmd"
	"github.com/spf13/cobra"
	"io"
	"math"
	"os"
)

var file string

func init() {
	var cmd = &cobra.Command{
		Use:   "day20",
		Short: "Day 20 example",
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

	track := make([]rune, 0)

	// first read everything to rw buffer
	r := bufio.NewReader(f)
	for {
		c, _, e := r.ReadRune()
		if c == '\n' || c == '^' {
			continue
		}
		if c == '$' {
			break
		}
		if e != nil {
			if e == io.EOF {
				break
			}
			fmt.Printf("error: %s\n", e)
			os.Exit(1)
		}
		track = append(track, c)
	}

	// we start at beginning of initial branch, which we don't know yet where it splits
	bid := 0
	start := newBranchInfo(bid, 0)
	bid++
	current := start

	/*
	(( - no
	(| - no
	() - no
	|( - no
	|| - no
	|) - yes - empty alternative branch
	)( - yes - new segment that already starts with branches
	)| - yes - alternative branch following branches
	)) - yes
	*/

	// build directed graph

	for i, step := range track {
		if step == 'N' || step == 'E' || step == 'S' || step == 'W' {
			current.idx2 = i
			if !current.done {
				current.steps = append(current.steps, step)
			} else {
				current.stepsAfter = append(current.stepsAfter, step)
			}
		}
		if step == '(' {
			// end current branch info - new child branch(es) will start(s)
			// already add first child - there's always one and we have no '((', '(|' or '()' case
			newBranch := newBranchInfo(bid, i) // branch may start with '('
			bid++
			newBranch.previous = current
			current.branches = append(current.branches, newBranch)
			current = newBranch
		}
		if step == ')' {
			current.previous.done = true
			current = current.previous
		}
		if step == '|' {
			// found new sibling branch - record it
			newBranch := newBranchInfo(bid, i) // branch may start with '|'
			bid++
			newBranch.previous = current.previous // sibling
			newBranch.previous.branches = append(newBranch.previous.branches, newBranch)
			current = newBranch
		}
	}
	fmt.Printf("Branches: %d\n", bid)

	// show(start, []rune{}, []rune{})

	// build a graph

	grid := map[string]*room{}
	pos := []int{0, 0}

	// DFS walking and storing room information
	draw(grid)
	walk(start, grid, []rune{}, pos)
	draw(grid)

	// BFS finding the distance
	toFill := list.New()
	toFill.PushBack([]int{0, 0})
	distance(grid, toFill, 0)

	max := 0
	for _, r := range grid {
		if r.distance > max {
			max = r.distance
		}
	}

	fmt.Printf("Answer 1: %d, Answer 2: %d\n", max, 0)
}

func walk(bi *branchInfo, grid map[string]*room, after []rune, pos []int) {
	newAfter := make([]rune, len(bi.stepsAfter)+len(after))
	copy(newAfter, bi.stepsAfter)
	copy(newAfter[len(bi.stepsAfter):], after)

	np := []int{pos[0], pos[1]}
	walkPath(bi.steps, grid, np)

	if len(bi.branches) == 0 {
		np2 := []int{np[0], np[1]}
		// fmt.Printf("%s\n", string(newBefore)+string(newAfter))
		walkPath([]rune(string(newAfter)), grid, np2)
		count++
		// walkPath(newAfter, grid, np2)
	}
	for _, c := range bi.branches {
		np2 := []int{np[0], np[1]}
		walk(c, grid, newAfter, np2)
	}
}

var count = 0

func walkPath(steps []rune, grid map[string]*room, pos []int) {
	// if count >= 1 {
	// 	return
	// }
	// count++
	for _, step := range steps {
		var key1, key2 string
		key1 = fmt.Sprintf("%d:%d", pos[0], pos[1]) // x:y
		ok := false
		if _, ok = grid[key1]; !ok {
			// distance will be handled later
			grid[key1] = &room{n: false, e: false, s: false, w: false, distance: -1, key: key1, x: pos[0], y: pos[1]}
			// fmt.Printf("grid size: %d\n", len(grid))
		}
		from := grid[key1]

		// open doors in new direction and find next room
		if step == 'N' {
			pos[1]--
			from.n = true
		} else if step == 'E' {
			pos[0]++
			from.e = true
		} else if step == 'S' {
			pos[1]++
			from.s = true
		} else if step == 'W' {
			pos[0]--
			from.w = true
		}
		key2 = fmt.Sprintf("%d:%d", pos[0], pos[1])

		if _, ok = grid[key2]; !ok {
			// distance will be handled later
			grid[key2] = &room{n: false, e: false, s: false, w: false, distance: -1, key: key2, x: pos[0], y: pos[1]}
			// fmt.Printf("grid size: %d\n", len(grid))
		}
		to := grid[key2]

		// open doors to previous direction
		if step == 'N' {
			to.s = true
		} else if step == 'E' {
			to.w = true
		} else if step == 'S' {
			to.n = true
		} else if step == 'W' {
			to.e = true
		}
	}
}

func distance(cells map[string]*room, toFill *list.List, d int) {
	toFill2 := list.New()
	for e := toFill.Front(); e != nil; e = e.Next() {
		if xy, ok := e.Value.([]int); ok {
			key := fmt.Sprintf("%d:%d", xy[0], xy[1])
			if cells[key].distance == -1 {
				cells[key].distance = d
				// BFS
				if cells[key].e {
					toFill2.PushBack([]int{xy[0] + 1, xy[1]})
				}
				if cells[key].w {
					toFill2.PushBack([]int{xy[0] - 1, xy[1]})
				}
				if cells[key].n {
					toFill2.PushBack([]int{xy[0], xy[1] - 1})
				}
				if cells[key].s {
					toFill2.PushBack([]int{xy[0], xy[1] + 1})
				}
			}
		}
	}
	toFill.Init()
	if toFill2.Len() > 0 {
		distance(cells, toFill2, d+1)
	}
}

func draw(rooms map[string]*room) {
	maxx := math.MinInt32
	maxy := math.MinInt32
	minx := math.MaxInt32
	miny := math.MaxInt32

	for _, r := range rooms {
		if maxx < r.x {
			maxx = r.x
		}
		if maxy < r.y {
			maxy = r.y
		}
		if minx > r.x {
			minx = r.x
		}
		if miny > r.y {
			miny = r.y
		}
	}
	fmt.Print("#")
	for x := minx; x <= maxx; x++ {
		fmt.Print("##")
	}
	fmt.Println()
	for y := miny; y <= maxy; y++ {
		fmt.Print("#")
		for x := minx; x <= maxx; x++ {
			k := fmt.Sprintf("%d:%d", x, y)
			if r := rooms[k]; r != nil {
				if x == 0 && y == 0 {
					fmt.Printf("X")
				} else {
					fmt.Printf(".")
				}
				if r.e {
					fmt.Printf("|")
				} else {
					fmt.Print("#")
				}
			} else {
				fmt.Print("*#")
			}
		}
		fmt.Println()
		fmt.Print("#")
		for x := minx; x <= maxx; x++ {
			k := fmt.Sprintf("%d:%d", x, y)
			if r := rooms[k]; r != nil {
				if r.s {
					fmt.Printf("-")
				} else {
					fmt.Print("#")
				}
				fmt.Printf("#")
			} else {
				fmt.Print("##")
			}
		}
		fmt.Println()
	}
}

func show(bi *branchInfo, before, after []rune) {
	newBefore := make([]rune, len(bi.steps)+len(before))
	copy(newBefore, before)
	copy(newBefore[len(before):], bi.steps)
	newAfter := make([]rune, len(bi.stepsAfter)+len(after))
	copy(newAfter, bi.stepsAfter)
	copy(newAfter[len(bi.stepsAfter):], after)

	if len(bi.branches) == 0 {
		fmt.Printf("%s", string(newBefore))
		fmt.Printf("%s\n", string(newAfter))
	}
	for _, c := range bi.branches {
		show(c, newBefore, newAfter)
	}
}

type room struct {
	distance   int
	n, e, s, w bool // is there a door (true) of wall (false) in given direction?
	key        string
	x, y       int
}

func (r *room) String() string {
	return fmt.Sprintf("[%s] n:%t, e:%t, s:%t, w:%t\n", r.key, r.n, r.e, r.s, r.w)
}

type branchInfo struct {
	id         int
	idx1, idx2 int           // indices to track array
	steps      []rune        // actual steps
	stepsAfter []rune        // actual steps
	previous   *branchInfo   // previous branch
	branches   []*branchInfo // list of child branches where branch splits
	done       bool          // are child branches processed?
}

func (bi *branchInfo) String() string {
	// return fmt.Sprintf("%d → %d", bi.idx1, bi.idx2)
	return fmt.Sprintf("%s", string(bi.steps))
}

func newBranchInfo(id int, idx int) *branchInfo {
	return &branchInfo{
		id:         id,
		idx1:       idx,
		steps:      make([]rune, 0),
		stepsAfter: make([]rune, 0),
		previous:   nil,
		branches:   make([]*branchInfo, 0),
		done:       false,
	}
}
