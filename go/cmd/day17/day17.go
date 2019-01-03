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
	"math"
	"os"
)

var file string

func init() {
	var cmd = &cobra.Command{
		Use:   "day17",
		Short: "Day 17 example",
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

	minx := math.MaxInt32
	maxx := math.MinInt32
	miny := math.MaxInt32
	maxy := math.MinInt32

	data := make([]*def, 0)

	for ; scanner.Scan(); {
		l := scanner.Text()
		if l == "" {
			break
		}

		var x1, x2, y1, y2 int
		if n, _ := fmt.Sscanf(l, "x=%d, y=%d..%d", &x1, &y1, &y2); n != 3 {
			_, _ = fmt.Sscanf(l, "y=%d, x=%d..%d", &y1, &x1, &x2)
			y2 = y1
		} else {
			x2 = x1
		}

		if minx > x1 {
			minx = x1
		}
		if minx > x2 {
			minx = x2
		}
		if maxx < x1 {
			maxx = x1
		}
		if maxx < x2 {
			maxx = x2
		}
		if miny > y1 {
			miny = y1
		}
		if miny > y2 {
			miny = y2
		}
		if maxy < y1 {
			maxy = y1
		}
		if maxy < y2 {
			maxy = y2
		}
		data = append(data, &def{x1: x1, x2: x2, y1: y1, y2: y2})
	}
	// if miny > 0 {
	// 	miny = 0
	// }

	width := maxx - minx + 3  // padded with one column of sand
	height := maxy + 1
	dirt := make([]byte, width*height)

	for y := 0; y < height; y++ {
		for x := 0; x < width; x++ {
			dirt[y*width+x] = sand
		}
	}

	for _, d := range data {
		d.x2 -= minx - 1
		d.x1 -= minx - 1
		for y := d.y1; y <= d.y2; y++ {
			for x := d.x1; x <= d.x2; x++ {
				dirt[y*width+x] = clay
			}
		}
	}
	dirt[500-minx+1] = source

	// show(dirt, width, height)
	fmt.Printf("%d-%d, %d-%d (%dx%d)\n", 0, maxx-minx, miny, maxy, width, height)

	it := 0
	for {
		if !drop(dirt, width, height, 500-minx+1) {
			break
		}
		it++
		// if it == 30 {
		// 	break
		// }
		// fmt.Print("[H[2J[3J")
		// show(dirt, width, height)
		// time.Sleep(time.Millisecond * 500)
	}
	// show(dirt, width, height)

	count := 0
	count2 := 0
	for _, v := range dirt[miny*width:] {
		if v == water || v == stream {
			count++
		}
		if v == water {
			count2++
		}
	}

	fmt.Printf("Answer 1: %d, Answer 2: %d\n", count, count2)
}

// drop starts from x=500,y=0 and returns true if after dropping a unit of water from source, the water
// is accumulated somewhere in the dirt
func drop(dirt []byte, width int, height int, start int) bool {
	x := start
	y := 0
	going := down
	if dirt[(y+1)*width+x] == stream {
		return false
	}
	// x1, x2, y coordinates of cells to change from stream to water
	lr := []int{-1, -1, -1}

	lastxy := []int{-1, -1}
	for {
		if lastxy[0] == x && lastxy[1] == y {
			fmt.Printf("\t%d:%d\n", x, y)
			show(dirt, width, height)
			panic("A")
		}
		lastxy[0] = x
		lastxy[1] = y
		if y == height-1 {
			// reached botom
			dirt[y*width+x] = stream
			return true
		} else if going == down && dirt[(y+1)*width+x] == sand {
			// can drop below
			y++
		} else if going == down && dirt[(y+1)*width+x] == stream {
			// can't drop lower - put another stream
			dirt[y*width+x] = stream
			return true
		} else if going == down { // water or clay
			// have to move left or right
			lr[0] = x
			lr[1] = x
			if x > 0 && dirt[y*width+x-1] == sand {
				// can go left
				going = left
				x--
				lr[0] = x
				lr[2] = y
			} else if x < width-1 && dirt[y*width+x+1] == sand {
				// can go right
				going = right
				x++
				lr[1] = x
				lr[2] = y
				// } else {
				// 	// can't do anything
				// 	dirt[y*width+x] = water
				// 	return true
			} else {
				// neither left or right
				if dirt[y*width+x-1] != sand && dirt[y*width+x+1] != sand {
					dirt[y*width+x] = water
					return true
				}
			}
		} else {
			// either we'll start going down again or we change chosen streams to water
			if dirt[(y+1)*width+x] == sand {
				going = down
				y++
			} else {
				if going == left {
					if dirt[(y+1)*width+x] == stream {
						dirt[y*width+x] = stream
						return true
					} else if x > 0 && dirt[y*width+x-1] == sand {
						x--
						lr[0]--
					} else {
						// turn right
						lr[1] = x
						going = right
					}
				}
				if going == right {
					if dirt[(y+1)*width+x] == stream {
						dirt[y*width+x] = stream
						return true
					} else if x < width-1 && dirt[y*width+x+1] == sand {
						x++
						lr[1]++
					} else {
						// we can fill marked cells
						what := water
						if dirt[y*width+lr[1]+1] == stream || dirt[y*width+lr[0]-1] == stream {
							what = stream
						}
						for _x := lr[0]; _x <= lr[1]; _x++ {
							dirt[lr[2]*width+_x] = what
						}
						return true
					}
				}
			}
		}
	}
}

func show(data []byte, width, height int) {
	for y := 0; y < height; y++ {
		for x := 0; x < width; x++ {
			if data[y*width+x] == source {
				fmt.Print("+")
			} else if data[y*width+x] == sand {
				fmt.Print(".")
			} else if data[y*width+x] == clay {
				fmt.Print("#")
			} else if data[y*width+x] == stream {
				fmt.Print("|")
			} else if data[y*width+x] == water {
				fmt.Print("~")
			}
		}
		fmt.Println()
	}
}

type def struct {
	x1, x2, y1, y2 int
}

func (d *def) String() string {
	return fmt.Sprintf("%d-%d, %d-%d", d.x1, d.x2, d.y1, d.y2)
}

const (
	sand   byte = 0
	clay   byte = 1
	source byte = 2
	stream byte = 3
	water  byte = 4
)

const (
	down  = 0
	left  = 1
	right = 2
)
