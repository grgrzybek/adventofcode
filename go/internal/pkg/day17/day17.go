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
)

var file string

func init() {
	var cmd = &cobra.Command{
		Use:   "day17",
		Short: "Day 17 example",
		Run:   run,
	}
	cmd.Flags().StringVarP(&file, "file", "f", "", "Input file with puzzle data")
	aoc2020.RootCmd.AddCommand(cmd)
}

type coords struct {
	x, y, z, w int
	key        string
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

	space1a := make(map[string]bool)
	space1b := make(map[string]bool)
	pspace1a := &space1a
	pspace1b := &space1b
	space2a := make(map[string]bool)
	space2b := make(map[string]bool)
	pspace2a := &space2a
	pspace2b := &space2b

	minx, miny, minz, minw := -1, -1, 0, 0
	maxx, maxy, maxz, maxw := 0, 0, 0, 0

	var MAXX, MAXY int

	for ; scanner.Scan(); {
		line := scanner.Text()
		if line == "" {
			break
		}

		for idx, v := range line {
			k := &coords{x: idx - 1, y: maxy - 1, z: 0, w: 0}
			k.key = fmt.Sprintf("%d,%d,%d,%d", idx-1, maxy-1, 0, 0)
			space1a[k.key] = v == '#'
			space2a[k.key] = v == '#'
		}
		if maxx == 0 {
			maxx = len(line) - 2
		}
		maxy++
	}
	maxy -= 2
	MAXX = maxx
	MAXY = maxy

	// part 1

	var key string
	var x, y, z, w int
	for round := 0; round < 6; round++ {
		minx--
		miny--
		minz--
		maxx++
		maxy++
		maxz++
		fmt.Printf("(%d) x: %d - %d, y: %d - %d, z: %d - %d\n", round+1, minx, maxx, miny, maxy, minz, maxz)

		// for y = miny; y <= maxy; y++ {
		// 	for z = minz; z <= maxz; z++ {
		// 		for x = minx; x <= maxx; x++ {
		// 			key = fmt.Sprintf("%d,%d,%d,%d", x, y, z, 0)
		// 			if (*pspace1a)[key] {
		// 				fmt.Print("#")
		// 			} else {
		// 				fmt.Print(".")
		// 			}
		// 		}
		// 		fmt.Print("  ")
		// 	}
		// 	fmt.Println()
		// }

		for z = minz; z <= maxz; z++ {
			for y = miny; y <= maxy; y++ {
				for x = minx; x <= maxx; x++ {
					key = fmt.Sprintf("%d,%d,%d,%d", x, y, z, 0)
					nc := neighbours1(pspace1a, x, y, z)
					// fmt.Printf("\t\tchecking %s: %d\n", key, nc)
					if (*pspace1a)[key] {
						if !(nc == 2 || nc == 3) {
							(*pspace1b)[key] = false
						} else {
							(*pspace1b)[key] = true
						}
					} else {
						if nc == 3 {
							(*pspace1b)[key] = true
						} else {
							(*pspace1b)[key] = false
						}
					}
				}
			}
		}
		pspace1a, pspace1b = pspace1b, pspace1a
	}

	count1 := 0
	for _, v := range *pspace1a {
		if v {
			count1++
		}
	}

	// part 2

	minx, miny, minz, minw = -1, -1, 0, 0
	maxx, maxy, maxz, maxw = MAXX, MAXY, 0, 0

	for round := 0; round < 6; round++ {
		minx--
		miny--
		minz--
		minw--
		maxx++
		maxy++
		maxz++
		maxw++
		fmt.Printf("(%d) x: %d - %d, y: %d - %d, z: %d - %d, w: %d - %d\n", round+1, minx, maxx, miny, maxy, minz, maxz, minw, maxw)

		// for w = minw; w <= maxw; w++ {
		// 	for y = miny; y <= maxy; y++ {
		// 		for z = minz; z <= maxz; z++ {
		// 			for x = minx; x <= maxx; x++ {
		// 				key = fmt.Sprintf("%d,%d,%d,%d", x, y, z, 0)
		// 				if (*pspace1a)[key] {
		// 					fmt.Print("#")
		// 				} else {
		// 					fmt.Print(".")
		// 				}
		// 			}
		// 			fmt.Print("  ")
		// 		}
		// 		fmt.Println()
		// 	}
		// }

		for w = minw; w <= maxw; w++ {
			for z = minz; z <= maxz; z++ {
				for y = miny; y <= maxy; y++ {
					for x = minx; x <= maxx; x++ {
						key = fmt.Sprintf("%d,%d,%d,%d", x, y, z, w)
						nc := neighbours2(pspace2a, x, y, z, w)
						// fmt.Printf("\t\tchecking %s: %d\n", key, nc)
						if (*pspace2a)[key] {
							if !(nc == 2 || nc == 3) {
								(*pspace2b)[key] = false
							} else {
								(*pspace2b)[key] = true
							}
						} else {
							if nc == 3 {
								(*pspace2b)[key] = true
							} else {
								(*pspace2b)[key] = false
							}
						}
					}
				}
			}
		}
		pspace2a, pspace2b = pspace2b, pspace2a
	}

	count2 := 0
	for _, v := range *pspace2a {
		if v {
			count2++
		}
	}

	// answer1 = 313, answer2 = 2640
	fmt.Printf("Answer 1: %d, Answer 2: %d\n", count1, count2)
}

func neighbours1(pspace *map[string]bool, px int, py int, pz int) int {
	nc := 0
	for z := pz - 1; z <= pz+1; z++ {
		for y := py - 1; y <= py+1; y++ {
			for x := px - 1; x <= px+1; x++ {
				if px == x && py == y && pz == z {
					continue
				}
				key := fmt.Sprintf("%d,%d,%d,%d", x, y, z, 0)
				if (*pspace)[key] {
					nc++
				}
			}
		}
	}

	return nc
}

func neighbours2(pspace *map[string]bool, px int, py int, pz int, pw int) int {
	nc := 0
	for w := pw - 1; w <= pw+1; w++ {
		for z := pz - 1; z <= pz+1; z++ {
			for y := py - 1; y <= py+1; y++ {
				for x := px - 1; x <= px+1; x++ {
					if px == x && py == y && pz == z && pw == w {
						continue
					}
					key := fmt.Sprintf("%d,%d,%d,%d", x, y, z, w)
					if (*pspace)[key] {
						nc++
					}
				}
			}
		}
	}

	return nc
}
