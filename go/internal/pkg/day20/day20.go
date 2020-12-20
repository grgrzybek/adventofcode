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
		Use:   "day20",
		Short: "Day 20 example",
		Run:   run,
	}
	cmd.Flags().StringVarP(&file, "file", "f", "", "Input file with puzzle data")
	aoc2020.RootCmd.AddCommand(cmd)
}

type tile struct {
	id            int
	originalPiece []string
	properPiece   []string
	croppedPiece  []string
	edges         []string // 8 different combinations of edges
	ut            *usedTile
}

type usedTile struct {
	// key            string // "x,y"
	t              *tile
	tt, rt, bt, lt *tile
	arranged       bool // is it rotated/flipped properly?
	done           bool // are all the neighbours found?
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

	tiles := make([]*tile, 0)

	for {
		if !scanner.Scan() {
			break
		}
		line := scanner.Text()
		if line == "" {
			continue
		}
		t := &tile{}
		_, _ = fmt.Sscanf(line, "Tile %d:", &t.id)
		tiles = append(tiles, t)
		// fmt.Printf("Found tile %d\n", t.id)

		t.originalPiece = make([]string, 10)
		for i := 0; i < 10; i++ {
			scanner.Scan()
			t.originalPiece[i] = scanner.Text()
		}

		// calculate 8 edges
		t.edges = calculate8Edges(t.originalPiece)
	}

	// part 1

	space := make([]*usedTile, 0)
	for _, t := range tiles {
		u := &usedTile{t: t, arranged: false, done: false}
		t.ut = u
		space = append(space, u)
	}

	v1 := 1
	// arrange (fix) the first tile
	rearrange(space[0], nil, 0, -1)
	todo := make([]*usedTile, 1)
	todo[0] = space[0]

	// continue to process other, not arranged tiles
	for {
		if len(todo) > 0 {
			todo2 := make([]*usedTile, 0)
			fmt.Print("Will check :")
			for _, t := range todo {
				if !t.done {
					fmt.Printf("%d ", t.t.id)
				}
			}
			fmt.Println()
			for _, t := range todo {
				matching := findMatching(t, tiles, &todo2)
				if matching == 2 {
					v1 *= t.t.id
				}
			}
			if len(todo2) == 0 {
				break
			} else {
				todo = todo2
			}
		}
	}

	// part 2

	for _, ut := range space {
		t, r, b, l := 0, 0, 0, 0
		if ut.tt != nil {
			t = ut.tt.id
		}
		if ut.rt != nil {
			r = ut.rt.id
		}
		if ut.bt != nil {
			b = ut.bt.id
		}
		if ut.lt != nil {
			l = ut.lt.id
		}
		fmt.Printf("%d) T: %d, R: %d, B: %d, L: %d\n", ut.t.id, t, r, b, l)
	}

	// find top-left corner tile
	var ct *usedTile = nil
	for _, ut := range space {
		if ut.tt == nil && ut.rt != nil && ut.bt != nil && ut.lt == nil {
			ct = ut
			break
		}
	}

	// start from this tile, follow the edges right and bottom, and construct the image after orienting the tile
	if ct == nil {
		panic("Can't find any edge tile")
	}

	var y = ct

	// print and check dimensions
	W, H := 0, 0
	for {
		var x = y
		for {
			fmt.Printf("%d ", x.t.id)
			if H == 0 {
				W += len(x.t.croppedPiece[0])
			}
			if x.rt == nil {
				break
			}
			x = x.rt.ut
		}
		H += len(y.t.croppedPiece)
		fmt.Println()
		if y.bt == nil {
			break
		}
		y = y.bt.ut
	}

	fmt.Printf("Image size: %dx%d\n", W, H)

	images := make([][]string, 8)
	image1 := make([]string, 0)

	// print tiles and collect first image
	y = ct
	size := len(y.t.croppedPiece)
	for /* each row of tiles*/ {
		// for each line of cropped image of the current row of tiles
		for yl := 0; yl < size; yl++ {
			var x = y
			line := make([]rune, 0)
			for {
				// fmt.Printf("%s", x.t.croppedPiece[yl])
				for _, r := range x.t.croppedPiece[yl] {
					line = append(line, r)
				}
				if x.rt == nil {
					break
				}
				x = x.rt.ut
			}
			image1 = append(image1, string(line))
			// fmt.Println()
		}
		if y.bt == nil {
			break
		}
		y = y.bt.ut
	}
	images[0] = image1
	t1 := &tile{originalPiece: image1}
	// 3x rotate and clear properPiece
	rotate(t1, 1)
	images[1] = t1.properPiece
	t1.properPiece = nil
	rotate(t1, 2)
	images[2] = t1.properPiece
	t1.properPiece = nil
	rotate(t1, 3)
	images[3] = t1.properPiece
	t1.properPiece = nil
	flipV(t1)
	images[4] = t1.properPiece
	t1.originalPiece = t1.properPiece
	t1.properPiece = nil
	rotate(t1, 1)
	images[5] = t1.properPiece
	t1.properPiece = nil
	rotate(t1, 2)
	images[6] = t1.properPiece
	t1.properPiece = nil
	rotate(t1, 3)
	images[7] = t1.properPiece
	t1.properPiece = nil

	for y := 0; y < len(images[0]); y++ {
		for i := 0; i < 4; i++ {
			fmt.Printf("%s   ", images[i][y])
		}
		fmt.Println()
	}
	fmt.Println()
	for y := 0; y < len(images[0]); y++ {
		for i := 4; i < 8; i++ {
			fmt.Printf("%s   ", images[i][y])
		}
		fmt.Println()
	}
	fmt.Println()

	monster := make([]string, 3)
	monster[0] = "                  # "
	monster[1] = "#    ##    ##    ###"
	monster[2] = " #  #  #  #  #  #   "

	MW := len(monster[0])
	MH := len(monster)

	// finally we have to search for the monster in 8 images
	v2 := 0
	for _, img := range images {
		v := hashTilesNotMonsters(img, monster, W, H, MW, MH)
		if v > 0 {
			v2 = v
			break
		}
	}

	// answer1 = 13983397496713, answer2 = 2424
	fmt.Printf("Answer 1: %d, Answer 2: %d\n", v1, v2)
}

func hashTilesNotMonsters(img []string, monster []string, w int, h int, mw int, mh int) int {
	img2 := make([][]rune, len(img))
	for idx, line := range img {
		img2[idx] = []rune(line)
	}
	mc := 0
	for {
		found := false
		for y := 0; y < h-mh; y++ {
			for x := 0; x < w-mw; x++ {
				if isAMonster(img2, x, y, monster, mw, mh) {
					found = true
					mc++
					// clean the hashes of the monster
					for dy := y; dy < y+mh; dy++ {
						for dx := x; dx < x+mw; dx++ {
							if monster[dy-y][dx-x] == '#' {
								img2[dy][dx] = '.'
							}
						}
					}
					break
				}
			}
			if found {
				break
			}
		}
		if !found {
			break
		}
	}
	if mc == 0 {
		return 0
	} else {
		hashes := 0
		// count remaining hashes
		for _, line := range img2 {
			for _, r := range line {
				if r == '#' {
					hashes++
				}
			}
		}
		return hashes
	}
}

func isAMonster(img [][]rune, x int, y int, monster []string, mw int, mh int) bool {
	for dy := y; dy < y+mh; dy++ {
		for dx := x; dx < x+mw; dx++ {
			if monster[dy-y][dx-x] == '#' && img[dy][dx] != '#' {
				return false
			}
		}
	}
	return true
}

func findMatching(checkedTile *usedTile, tiles []*tile, todo *[]*usedTile) interface{} {
	if checkedTile.done {
		return 0
	}
	if !checkedTile.arranged {
		panic("We can't find matches for the tile which is not arranged!")
	}
	c := 0
	// check 4 edges of "checkedTile" only - assuming the checked tile is already arranged
	for e1, edge1 := range checkedTile.t.edges[:4] {
		// check all other tiles (except the checked one)
		for _, otherTile := range tiles {
			// iterate over all tiles except checkedTile
			if checkedTile.t == otherTile {
				continue
			}
			br := false
			// check all the edges of the other tile
			for e2, edge2 := range otherTile.edges {
				if edge1 == edge2 {
					// a match means we've found one neighbour
					// however the "otherTile" may have to be arranged
					c++
					if !otherTile.ut.done {
						*todo = append(*todo, otherTile.ut)
					}
					if e1 == 0 || e1 == 4 {
						// top edge of checkedTile.t matches some edge of t
						checkedTile.tt = otherTile
						if !otherTile.ut.arranged {
							rearrange(otherTile.ut, checkedTile, e2, e1)
						} else {
							// straightforward backlink
							otherTile.ut.bt = checkedTile.t
						}
					} else if e1 == 1 || e1 == 5 {
						// right edge of checkedTile.t matches some edge of t
						checkedTile.rt = otherTile
						if !otherTile.ut.arranged {
							rearrange(otherTile.ut, checkedTile, e2, e1)
						} else {
							// straightforward backlink
							otherTile.ut.lt = checkedTile.t
						}
					} else if e1 == 2 || e1 == 6 {
						// bottom edge of checkedTile.t matches some edge of t
						checkedTile.bt = otherTile
						if !otherTile.ut.arranged {
							rearrange(otherTile.ut, checkedTile, e2, e1)
						} else {
							// straightforward backlink
							otherTile.ut.tt = checkedTile.t
						}
					} else if e1 == 3 || e1 == 7 {
						// left edge of checkedTile.t matches some edge of t
						checkedTile.lt = otherTile
						if !otherTile.ut.arranged {
							rearrange(otherTile.ut, checkedTile, e2, e1)
						} else {
							// straightforward backlink
							otherTile.ut.rt = checkedTile.t
						}
					}
					br = true
					break
				}
			}
			if e1 == 4 {
				// don't check reversed edges if the tile being checked - assume this tile doesn't have to be flipped
				break
			}
			if br {
				break
			}
		}
	}
	checkedTile.done = true
	return c
}

func rearrange(tile *usedTile, other *usedTile, tileEdge int, otherEdge int) {
	if otherEdge >= 4 {
		panic("It is assumed that \"tile\" is properly arranged")
	}
	// tile's tileEdge edge matches other's otherEdge edge.
	// Starting with tile:
	//   AB
	//   CD
	// These are valid matches:
	// - 0 (AB) - 2 or 6
	// - 1 (BD) - 3 or 7
	// - 2 (DC) - 0 or 4
	// - 3 (CA) - 1 or 5
	// - 4 (BA) - 2 or 6
	// - 5 (DB) - 3 or 7
	// - 6 (CD) - 0 or 4
	// - 7 (AC) - 1 or 5

	if other == nil {
		// special case for first tile - we only have to crop the image
		tile.t.properPiece = make([]string, len(tile.t.originalPiece))
		tile.t.croppedPiece = make([]string, len(tile.t.originalPiece)-2)
		for y := 0; y < len(tile.t.originalPiece); y++ {
			tile.t.properPiece[y] = tile.t.originalPiece[y]
		}
		for y := 1; y < len(tile.t.originalPiece)-1; y++ {
			tile.t.croppedPiece[y-1] = tile.t.originalPiece[y][1 : len(tile.t.originalPiece)-1]
		}
		tile.arranged = true
		return
	}

	// "other" tile is fine, we have to arrange "tile"
	if otherEdge == 0 {
		// match "tile" to top edge of "other" tile
		//   AB
		//   CD
		// tileEdge == 0: our top matches - needs to be flipped up-down
		//   AB
		//   xx
		if tileEdge == 0 {
			flipH(tile.t)
		}
		// tileEdge == 1: our right matches - needs to be flipped up-down + rotated right
		//   xA
		//   xB
		if tileEdge == 1 {
			flipH(tile.t)
			rotate(tile.t, 1)
		}
		// tileEdge == 2: our bottom matches - needs to be flipped left-right
		//   xx
		//   BA
		if tileEdge == 2 {
			flipV(tile.t)
		}
		// tileEdge == 3: our left matches - needs to be flipped left-right + rotated right
		//   Bx
		//   Ax
		if tileEdge == 3 {
			flipV(tile.t)
			rotate(tile.t, 1)
		}
		// tileEdge == 4: our top matches - needs to be rotated right 2x
		//   BA ([0] = BA, [4] = AB)
		//   xx
		if tileEdge == 4 {
			rotate(tile.t, 2)
		}
		// tileEdge == 5: our right matches - needs to be rotated right 1x
		//   xB
		//   xA
		if tileEdge == 5 {
			rotate(tile.t, 1)
		}
		// tileEdge == 6: our bottom matches - ok!
		//   xx
		//   AB
		if tileEdge == 6 {
			// no change
			tile.t.properPiece = tile.t.originalPiece
		}
		// tileEdge == 7: our left matches - needs to be rotated right 3x
		//   Ax
		//   Bx
		if tileEdge == 7 {
			rotate(tile.t, 3)
		}
		tile.bt = other.t
	} else if otherEdge == 1 {
		// match "tile" to right edge of "other" tile
		if tileEdge == 0 {
			flipH(tile.t)
			rotate(tile.t, 1)
		}
		if tileEdge == 1 {
			flipV(tile.t)
		}
		if tileEdge == 2 {
			flipV(tile.t)
			rotate(tile.t, 1)
		}
		if tileEdge == 3 {
			flipH(tile.t)
		}
		if tileEdge == 4 {
			rotate(tile.t, 3)
		}
		if tileEdge == 5 {
			rotate(tile.t, 2)
		}
		if tileEdge == 6 {
			rotate(tile.t, 1)
		}
		if tileEdge == 7 {
			// no change
			tile.t.properPiece = tile.t.originalPiece
		}
		tile.lt = other.t
	} else if otherEdge == 2 {
		// match "tile" to bottom edge of "other" tile
		if tileEdge == 0 {
			flipV(tile.t)
		}
		if tileEdge == 1 {
			flipV(tile.t)
			rotate(tile.t, 1)
		}
		if tileEdge == 2 {
			flipH(tile.t)
		}
		if tileEdge == 3 {
			flipH(tile.t)
			rotate(tile.t, 1)
		}
		if tileEdge == 4 {
			// no change
			tile.t.properPiece = tile.t.originalPiece
		}
		if tileEdge == 5 {
			rotate(tile.t, 3)
		}
		if tileEdge == 6 {
			rotate(tile.t, 2)
		}
		if tileEdge == 7 {
			rotate(tile.t, 1)
		}
		tile.tt = other.t
	} else if otherEdge == 3 {
		// match "tile" to left edge of "other" tile
		if tileEdge == 0 {
			flipV(tile.t)
			rotate(tile.t, 1)
		}
		if tileEdge == 1 {
			flipH(tile.t)
		}
		if tileEdge == 2 {
			flipH(tile.t)
			rotate(tile.t, 1)
		}
		if tileEdge == 3 {
			flipV(tile.t)
		}
		if tileEdge == 4 {
			rotate(tile.t, 1)
		}
		if tileEdge == 5 {
			// no change
			tile.t.properPiece = tile.t.originalPiece
		}
		if tileEdge == 6 {
			rotate(tile.t, 3)
		}
		if tileEdge == 7 {
			rotate(tile.t, 2)
		}
		tile.rt = other.t
	}
	// after rearranging, we have to recalculate the edges - now they're in better order
	tile.t.edges = calculate8Edges(tile.t.properPiece)
	crop(tile.t)
	tile.arranged = true
}

func crop(t *tile) {
	t.croppedPiece = make([]string, len(t.properPiece)-2)
	for y := 1; y < len(t.properPiece)-1; y++ {
		t.croppedPiece[y-1] = t.properPiece[y][1 : len(t.properPiece)-1]
	}
}

func flipH(t *tile) {
	source := t.originalPiece
	if t.properPiece != nil {
		source = t.properPiece
	}
	size := len(source)
	target := make([]string, size)
	for y := 0; y < size; y++ {
		target[size-1-y] = source[y]
	}
	t.properPiece = target
}

func flipV(t *tile) {
	source := t.originalPiece
	if t.properPiece != nil {
		source = t.properPiece
	}
	size := len(source)
	target := make([]string, size)
	for y := 0; y < size; y++ {
		target[y] = reverse(source[y])
	}
	t.properPiece = target
}

func rotate(t *tile, i int) {
	if i == 2 {
		flipH(t)
		flipV(t)
		return
	}

	source := t.originalPiece
	if t.properPiece != nil {
		source = t.properPiece
	}
	size := len(source)
	target := make([][]rune, size)
	target2 := make([]string, size)
	for i := 0; i < size; i++ {
		target[i] = make([]rune, size)
	}

	if i == 1 {
		// right
		for y := 0; y < size; y++ {
			for x, r := range source[y] {
				target[x][size-1-y] = r
			}
		}
	} else if i == 3 {
		// left
		for y := 0; y < size; y++ {
			for x, r := range source[y] {
				target[size-1-x][y] = r
			}
		}
	}
	for idx, rr := range target {
		target2[idx] = string(rr)
	}
	t.properPiece = target2
}

func calculate8Edges(piece []string) []string {
	res := make([]string, 8)

	size := len(piece)

	res[0] = piece[0]               // top edge
	res[2] = reverse(piece[size-1]) // bottom edge
	re := make([]rune, size)
	le := make([]rune, size)
	for i := 0; i < size; i++ {
		v := []rune(piece[i])
		re[i] = v[size-1]
		le[size-1-i] = v[0]
	}
	res[1] = string(re)
	res[3] = string(le)
	res[4] = reverse(res[0]) // top edge reversed
	res[6] = reverse(res[2]) // bottom edge reversed
	res[5] = reverse(res[1]) // right edge reversed
	res[7] = reverse(res[3]) // left edge reversed

	return res
}

func reverse(s string) string {
	v := make([]rune, len(s))
	for i := len(s) - 1; i >= 0; i-- {
		v[len(s)-1-i] = []rune(s)[i]
	}
	return string(v)
}
