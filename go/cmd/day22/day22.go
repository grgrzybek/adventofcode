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
	"os"
)

var file string

func init() {
	var cmd = &cobra.Command{
		Use:   "day22",
		Short: "Day 22 example",
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

	var depth int64
	target := []int64{-1, -1}

	scanner.Scan()
	_, _ = fmt.Sscanf(scanner.Text(), "depth: %d", &depth)
	scanner.Scan()
	_, _ = fmt.Sscanf(scanner.Text(), "target: %d,%d", &target[0], &target[1])

	// for part 2, we'll calculate a bit more

	// width := (target[0]) + 1
	// height := (target[1]) + 1
	width := (target[0]) + 200
	height := (target[1]) * 2
	fmt.Printf("%d x %d\n", width, height)

	size := (width + 1) * (height + 1)
	regionEL := make([]int64, size)
	regionTypes := make([]int, size)

	var x int64
	var y int64
	for y = 0; y < height; y++ {
		for x = 0; x < width; x++ {
			var gi int64
			if (x == 0 && y == 0) || (x == target[0] && y == target[1]) {
				gi = 0
			} else if y == 0 {
				gi = int64(x * 16807)
			} else if x == 0 {
				gi = int64(y * 48271)
			} else {
				gi = regionEL[(y-1)*width+x] * regionEL[y*width+x-1]
			}
			// fmt.Printf("gi(%d, %d) = %d\n", x, y, gi)
			var el int64
			el = (gi + depth) % 20183
			// fmt.Printf("idx: %d\n", y*width+x)
			regionEL[y*width+x] = el
			regionTypes[y*width+x] = int(el % 3)
		}
	}

	risk := 0
	for y = 0; y <= target[1]; y++ {
		for x = 0; x <= target[0]; x++ {
			risk += regionTypes[y*width+x]
		}
	}
	for y = 0; y < height; y++ {
		for x = 0; x < width; x++ {
			if x == 0 && y == 0 {
				fmt.Print("X")
			} else if x == target[0] && y == target[1] {
				fmt.Print("T")
			} else {
				switch regionTypes[y*width+x] {
				case 0:
					fmt.Printf(".") // rocky
				case 1:
					fmt.Printf("=") // wet
				case 2:
					fmt.Printf("|") // narrow
				}
			}
		}
		fmt.Println()
	}

	// part2

	check := list.New()
	check.PushBack([]int{0, 0, torch, 0}) // x, y, equipment, time
	regionTimes := make([]int, size*3)
	for i := 0; i < int(size*3); i++ {
		regionTimes[i] = -1
	}
	// regionTimes[target[1]*width+target[0]] = -2

	calculate(regionTypes, regionTimes, check, int(width), int(height), int(target[0]), int(target[1]))

	// torch
	distance := regionTimes[target[1]*width*3+(target[0]*3)+1]

	// for y = 0; y < height; y++ {
	// 	for x = 0; x < width-7; x ++ {
	// 		if x == 0 && y == 0 {
	// 			fmt.Print("[     X     ] ")
	// 		} else if x == target[0] && y == target[1] {
	// 			fmt.Print("[     T     ] ")
	// 		} else {
	// 			fmt.Printf("[%03d|%03d|%03d] ", regionTimes[y*width*3+(x*3)], regionTimes[y*width*3+(x*3)+1], regionTimes[y*width*3+(x*3)+2])
	// 		}
	// 	}
	// 	fmt.Println()
	// }

	fmt.Printf("Answer 1: %d, Answer 2: %d\n", risk, distance)
}

func calculate(types []int, times []int, toCheck *list.List, width, height int, tx, ty int) {
	toCheck2 := list.New()
	for e := toCheck.Front(); e != nil; e = e.Next() {
		if xye, ok := e.Value.([]int); ok {
			x := xye[0] // x of current region
			y := xye[1] // y of current region
			e := xye[2] // equipment with which we've entered
			d := xye[3] // time spent before entering - will increase by 1 or 8 (when switching equipment)

			// equipment we can use here
			// what's current terrain?
			terrain := types[y*width+x]
			switch terrain {
			case rocky: // rocky - need torch | climbingGear
				if e == neither {
					d += 7
				}
			case wet: // wet - need neither | climbingGear
				if e == torch {
					d += 7
				}
			case narrow: // narrow - need neither | torch
				if e == climbingGear {
					d += 7
				}
			}

			proceed := false

			// 3 ints for each cell
			if e == neither {
				if times[y*width*3+(x*3)+0] == -1 || times[y*width*3+(x*3)+0] > d {
					times[y*width*3+(x*3)+0] = d
					proceed = true
				}
			} else if e == torch {
				if times[y*width*3+(x*3)+1] == -1 || times[y*width*3+(x*3)+1] > d {
					times[y*width*3+(x*3)+1] = d
					proceed = true
				}
			} else if e == climbingGear {
				if times[y*width*3+(x*3)+2] == -1 || times[y*width*3+(x*3)+2] > d {
					times[y*width*3+(x*3)+2] = d
					proceed = true
				}
			}

			// we can go to neighbour cells with equipment permitted in current cell
			if proceed {
				// 1 minute for walking
				d++
				dneither := d
				if e != neither {
					dneither += 7
				}
				dtorch := d
				if e != torch {
					dtorch += 7
				}
				dclimbingGear := d
				if e != climbingGear {
					dclimbingGear += 7
				}
				// up
				if y > 0 {
					// if (times[(y-1)*width*3+(x*3)+0] == -1 || times[(y-1)*width*3+(x*3)+0] > d) {
					if types[(y-1)*width+x] != rocky {
						toCheck2.PushBack([]int{x, y - 1, neither, dneither})
					}
					if types[(y-1)*width+x] != wet {
						toCheck2.PushBack([]int{x, y - 1, torch, dtorch})
					}
					if types[(y-1)*width+x] != narrow {
						toCheck2.PushBack([]int{x, y - 1, climbingGear, dclimbingGear})
					}
				}
				// down
				if y < height {
					if types[(y+1)*width+x] != rocky {
						toCheck2.PushBack([]int{x, y + 1, neither, dneither})
					}
					if types[(y+1)*width+x] != wet {
						toCheck2.PushBack([]int{x, y + 1, torch, dtorch})
					}
					if types[(y+1)*width+x] != narrow {
						toCheck2.PushBack([]int{x, y + 1, climbingGear, dclimbingGear})
					}
				}
				// left
				if x > 0 {
					if types[y*width+x-1] != rocky {
						toCheck2.PushBack([]int{x - 1, y, neither, dneither})
					}
					if types[y*width+x-1] != wet {
						toCheck2.PushBack([]int{x - 1, y, torch, dtorch})
					}
					if types[y*width+x-1] != narrow {
						toCheck2.PushBack([]int{x - 1, y, climbingGear, dclimbingGear})
					}
				}
				// right
				if x < width {
					if types[y*width+x+1] != rocky {
						toCheck2.PushBack([]int{x + 1, y, neither, dneither})
					}
					if types[y*width+x+1] != wet {
						toCheck2.PushBack([]int{x + 1, y, torch, dtorch})
					}
					if types[y*width+x+1] != narrow {
						toCheck2.PushBack([]int{x + 1, y, climbingGear, dclimbingGear})
					}
				}
			}
			// fmt.Print("[H[2J[3J")
			// for y = 0; y < height; y++ {
			// 	for x = 0; x < width; x++ {
			// 		if x == 0 && y == 0 {
			// 			fmt.Print("[ X ]")
			// 		} else {
			// 			fmt.Printf("[%03d]", times[y*width+x] & (0xffff<<16)>>16)
			// 		}
			// 	}
			// 	fmt.Println()
			// }
			// time.Sleep(time.Millisecond * 30)
		}
	}
	toCheck.Init()
	if toCheck2.Len() > 0 {
		calculate(types, times, toCheck2, width, height, tx, ty)
	}
}

const (
	neither      = 1 // for wet and narrow
	torch        = 2 // for rocky and narrow
	climbingGear = 4 // for rocky and wet
)
const (
	rocky  = 0
	wet    = 1
	narrow = 2
)
