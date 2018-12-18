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
	"time"
)

var file string

func init() {
	var cmd = &cobra.Command{
		Use:   "day18",
		Short: "Day 18 example",
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

	forestMap := make([]string, 0)
	var width, height int

	for ; scanner.Scan(); {
		l := scanner.Text()
		if l == "" {
			break
		}
		forestMap = append(forestMap, l)
		width = len(l)
	}
	height = len(forestMap)

	trees1 := make([]rune, height*width)
	trees2 := make([]rune, height*width)

	for y := 0; y < height; y++ {
		l := []rune(forestMap[y])
		for x := 0; x < width; x++ {
			trees1[y*width+x] = l[x]
			trees2[y*width+x] = l[x]
		}
	}

	repeats := make(map[string]int)

	for answer, limit := range []int{10, 1000000000} {
		shortcut := false
		for m := 0; m < limit; m++ {
			if pm, ok := repeats[string(trees1)]; ok && !shortcut {
				cycle := m - pm
				for ; m < limit; m += cycle {
				}
				// fmt.Printf("it repeats after %d (%d - %d)\n", m-pm, m, pm)
				// limit = m + (limit % (m - pm))
				// fmt.Printf("new limit: %d\n", limit)
				shortcut = true
				// break
			}
			repeats[string(trees1)] = m
			if m%10000 == 0 {
				fmt.Printf("After %d minute\n", m)
			}
			if m >= 100000 && m < 101000 {
				fmt.Printf("[H[2J[3JAfter %d minute\n", m)
				show(trees1, width, height)
				time.Sleep(100 * time.Millisecond)
			}
			for y := 0; y < height; y++ {
				for x := 0; x < width; x++ {
					if trees1[y*width+x] == '.' {
						// open
						c := 0
						if y != 0 {
							if x != 0 && trees1[(y-1)*width+x-1] == '|' {
								c++
							}
							if trees1[(y-1)*width+x] == '|' {
								c++
							}
							if x != width-1 && trees1[(y-1)*width+x+1] == '|' {
								c++
							}
						}
						if x != 0 && trees1[y*width+x-1] == '|' {
							c++
						}
						if x != width-1 && trees1[y*width+x+1] == '|' {
							c++
						}
						if y != height-1 {
							if x != 0 && trees1[(y+1)*width+x-1] == '|' {
								c++
							}
							if trees1[(y+1)*width+x] == '|' {
								c++
							}
							if x != width-1 && trees1[(y+1)*width+x+1] == '|' {
								c++
							}
						}
						if c >= 3 {
							trees2[y*width+x] = '|'
						} else {
							trees2[y*width+x] = '.'
						}
					}
					if trees1[y*width+x] == '|' {
						// tree
						c := 0
						if y != 0 {
							if x != 0 && trees1[(y-1)*width+x-1] == '#' {
								c++
							}
							if trees1[(y-1)*width+x] == '#' {
								c++
							}
							if x != width-1 && trees1[(y-1)*width+x+1] == '#' {
								c++
							}
						}
						if x != 0 && trees1[y*width+x-1] == '#' {
							c++
						}
						if x != width-1 && trees1[y*width+x+1] == '#' {
							c++
						}
						if y != height-1 {
							if x != 0 && trees1[(y+1)*width+x-1] == '#' {
								c++
							}
							if trees1[(y+1)*width+x] == '#' {
								c++
							}
							if x != width-1 && trees1[(y+1)*width+x+1] == '#' {
								c++
							}
						}
						if c >= 3 {
							trees2[y*width+x] = '#'
						} else {
							trees2[y*width+x] = '|'
						}
					}
					if trees1[y*width+x] == '#' {
						// lumberyard
						l := 0
						t := 0
						if y != 0 {
							if x != 0 {
								if trees1[(y-1)*width+x-1] == '#' {
									l++
								}
								if trees1[(y-1)*width+x-1] == '|' {
									t++
								}
							}
							if trees1[(y-1)*width+x] == '#' {
								l++
							}
							if trees1[(y-1)*width+x] == '|' {
								t++
							}
							if x != width-1 {
								if trees1[(y-1)*width+x+1] == '#' {
									l++
								}
								if trees1[(y-1)*width+x+1] == '|' {
									t++
								}
							}
						}
						if x != 0 && trees1[y*width+x-1] == '#' {
							l++
						}
						if x != 0 && trees1[y*width+x-1] == '|' {
							t++
						}
						if x != width-1 && trees1[y*width+x+1] == '#' {
							l++
						}
						if x != width-1 && trees1[y*width+x+1] == '|' {
							t++
						}
						if y != height-1 {
							if x != 0 {
								if trees1[(y+1)*width+x-1] == '#' {
									l++
								}
								if trees1[(y+1)*width+x-1] == '|' {
									t++
								}
							}
							if trees1[(y+1)*width+x] == '#' {
								l++
							}
							if trees1[(y+1)*width+x] == '|' {
								t++
							}
							if x != width-1 {
								if trees1[(y+1)*width+x+1] == '#' {
									l++
								}
								if trees1[(y+1)*width+x+1] == '|' {
									t++
								}
							}
						}
						if l >= 1 && t >= 1 {
							trees2[y*width+x] = '#'
						} else {
							trees2[y*width+x] = '.'
						}
					}
				}
			}
			trees1, trees2 = trees2, trees1
		}

		t := 0
		l := 0
		for y := 0; y < height; y++ {
			for x := 0; x < width; x++ {
				if trees1[y*width+x] == '|' {
					t++
				}
				if trees1[y*width+x] == '#' {
					l++
				}
			}
		}

		fmt.Printf("Answer %d: %d\n", answer+1, t*l)
	}
}

func show(runes []rune, width, height int) {
	for y := 0; y < height; y++ {
		for x := 0; x < width; x++ {
			fmt.Printf("%c", runes[y*width+x])
		}
		fmt.Println()
	}

}
