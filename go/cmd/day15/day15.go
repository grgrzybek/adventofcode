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
	"math"
	"os"
	"sort"
)

var file string

var cavernMap = make([]string, 0) // to parse initial map
var cavern []byte

var heroMap = make(map[int]*hero)
var heroes = make([]*hero, 0)

var width, height int

func init() {
	var cmd = &cobra.Command{
		Use:   "day15",
		Short: "Day 15 example",
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

	scanner := bufio.NewScanner(f)
	scanner.Split(bufio.ScanLines)

	for ; scanner.Scan(); {
		l := scanner.Text()
		if l == "" {
			break
		}
		cavernMap = append(cavernMap, l)
		width = len(l)
	}
	height = len(cavernMap)
	_ = f.Close()

	cavern = make([]byte, height*width) // to keep map data
	heroId := 0

	for y := 0; y < height; y++ {
		row := cavernMap[y]
		for x := 0; x < width; x++ {
			switch row[x] {
			case '#': // wall
				cavern[y*width+x] = 1
			case '.', 'G', 'E': // empty, goblin or elf
				cavern[y*width+x] = 0
				if row[x] != '.' {
					h := &hero{id: heroId, kind: rune(row[x]), x: x, y: y, hp: 200, ap: 3, alive: true}
					heroMap[heroId] = h
					heroId++
					heroes = append(heroes, h)
				}
			}
		}
	}

	round := 0

out:
	for {
		fmt.Printf("\n===\nStart of Round %d\n", round+1)
		sortHeroes(heroes)

		var lastTurnMap []byte
		// each hero has a turn in a range
		for _, hero := range heroes {
			if !hero.alive {
				// hero might've died!
				continue
			}
			// find possible targets
			enemies := possibleTargets(hero)
			// find possible targets
			if len(enemies) == 0 {
				// no targets / enemies - even unreachable ones
				break out
			}

			// find cells within range of all targets
			cellsNearEnemies, turnMap := cellsNearEnemies(hero, enemies)
			lastTurnMap = turnMap

			// find reachable cells
			reachableCells, canAttack := reachableCells(hero, cellsNearEnemies, turnMap)
			if hero.x == 22 && hero.y == 11 {
				fmt.Printf("Cells reachable from [%s]: %v\n", hero, reachableCells)
			}

			if len(reachableCells) == 0 {
				// end of turn for given hero
				continue
			}

			// // if one of the reachable cells is a cell of a hero, we can attack
			if !canAttack {
				// move - we already have the cells sorted in both distance and reading order
				// we just have to find how to start moving to this cell
				move := shortestPath(hero, turnMap, reachableCells[0][0], reachableCells[0][1])
				// if hero.x == 22 && hero.y == 11 {
				// 	fmt.Printf("[%s] will go to: %v\n", hero, move)
				// }
				// // move hero one step on chosen path
				turnMap[hero.y*width+hero.x] = 0
				hero.x = move[0]
				hero.y = move[1]
				if hero.kind == 'E' {
					turnMap[hero.y*width+hero.x] = 2
				} else {
					turnMap[hero.y*width+hero.x] = 3
				}
				// fmt.Printf("Moving [%s] to %d:%d\n", hero, hero.x, hero.y)
			}
			// time.Sleep(time.Millisecond * 200)
			// show("[H[2J[3J", turnMap)

			// if there's enemy within range, attack - either after move or instead of move
			attackNearest(hero, turnMap)
		}

		// if we didn't "break out", it means there was at least one move or fight
		round++
		totalHP := 0

		show(fmt.Sprintf("After Round %d:", round), lastTurnMap)
		for _, h := range heroes {
			if h.alive {
				fmt.Printf("hero: %s\n", h)
				totalHP += h.hp
			}
		}
	}

	totalHP := 0
	for _, h := range heroes {
		if h.alive {
			fmt.Printf("hero: %s\n", h)
			totalHP += h.hp
		}
	}

	fmt.Printf("Answer 1: %d*%d=%d, Answer 2: %d\n", round, totalHP, round * totalHP, 0)
}

// returns pointers to enemies - all living enemies despite them being out of range / blocked
func possibleTargets(h *hero) []*hero {
	res := make([]*hero, 0)
	for _, e := range heroes {
		if h.kind == e.kind || !e.alive {
			continue
		}
		res = append(res, e)
	}
	return res
}

// returns cells NESW for all identified enemies - no need to sort here
func cellsNearEnemies(h *hero, enemies []*hero) ([][]int, []byte) {
	cellsNearEnemies := make([][]int, 0) // list of coordinates
	tempMap := mapWithOthers(h)
	added := make(map[string]bool)
	for _, e := range enemies {
		checking := [][]int{{e.x, e.y - 1}, {e.x - 1, e.y}, {e.x + 1, e.y}, {e.x, e.y + 1}}
		for _, xy := range checking {
			if tempMap[xy[1]*width+xy[0]] == 0 {
				// empty space
				if _, ok := added[key(xy)]; !ok {
					// new coordinate
					cellsNearEnemies = append(cellsNearEnemies, []int{xy[0], xy[1]})
				}
			}
		}
	}

	// show("temp map:", tempMap)

	return cellsNearEnemies, tempMap
}

// starting from hero's position, filter out the cells which are not reachable within given turn's map
// all the passed cells are unocupied - one of them may however be occupied by hero itself - it should be
// treated as reachable and this function returns additional flag in such case.
// cells are returned in order of increasing distance and in reading from original map: x, y, distance
func reachableCells(h *hero, cells [][]int, turnMap []byte) ([][]int, bool) {
	// copy of a map
	m := make([]byte, len(turnMap))
	copy(m, turnMap)
	// initially it'll have all checked cells - flooding will set there a distance from hero's cell
	checkedCells := make(map[string]int)
	for _, xy := range cells {
		checkedCells[key(xy)] = -1
	}

	// flooding the map and collecting reachable cells
	toFill := list.New()
	toFill.PushBack([]int{h.x, h.y})
	flood(m, checkedCells, toFill, 5)

	// list of reachable coordinates
	reachableCellKeys := make([]string, 0, 32)
	reachableCells := make([][]int, 0, 32)
	// hero is placed near enemy
	heroesCellReachable := false

	for xy, distance := range checkedCells {
		c := []int{-1, -1}
		// key back to coordinates
		_, _ = fmt.Sscanf(xy, "%d:%d", &c[0], &c[1])
		// fmt.Printf("hero: {%s} -> %d:%d == %d\n", h, c[0], c[1], distance)
		if distance == 0 && c[0] == h.x && c[1] == h.y {
			heroesCellReachable = true
		}
		if distance >= 0 {
			reachableCellKeys = append(reachableCellKeys, xy)
		}
	}

	sort.Slice(reachableCellKeys, func(i, j int) bool {
		d1 := checkedCells[reachableCellKeys[i]]
		d2 := checkedCells[reachableCellKeys[j]]
		if d1 < d2 {
			return true
		} else if d1 > d2 {
			return false
		} else {
			// reading order
			xy1 := []int{-1, -1}
			xy2 := []int{-1, -1}
			_, _ = fmt.Sscanf(reachableCellKeys[i], "%d:%d", &xy1[0], &xy1[1])
			_, _ = fmt.Sscanf(reachableCellKeys[j], "%d:%d", &xy2[0], &xy2[1])
			if xy1[1] < xy2[1] {
				return true
			} else if xy1[1] > xy2[1] {
				return false
			} else {
				if xy1[0] < xy2[0] {
					return true
				} else {
					return false
				}
			}
		}
	})

	for _, key := range reachableCellKeys {
		xy := []int{-1, -1}
		_, _ = fmt.Sscanf(key, "%d:%d", &xy[0], &xy[1])
		reachableCells = append(reachableCells, []int{xy[0], xy[1], checkedCells[key]})
		// fmt.Printf("%d:%d - %d\n", xy[0], xy[1], checkedCells[key])
	}

	return reachableCells, heroesCellReachable
}

// fills the map starting from x, y. works recursively for empty cels
// walls are '1', elves are '2', goblins are '3'. Anything >= 5 is flood
func flood(m []byte, cells map[string]int, toFill *list.List, v byte) {
	toFill2 := list.New()
	for e := toFill.Front(); e != nil; e = e.Next() {
		if xy, ok := e.Value.([]int); ok {
			x := xy[0]
			y := xy[1]
			if m[y*width+x] == 0 {
				// current cell empty, so we can recurse
				m[y*width+x] = v
				if _, ok := cells[key([]int{x, y})]; ok {
					// cell reachable - set the distance == 5 - v
					cells[key([]int{x, y})] = int(v) - 5
				}
				// spread the flood - BFS, so push to toFill
				toFill2.PushBack([]int{x, y - 1})
				toFill2.PushBack([]int{x - 1, y})
				toFill2.PushBack([]int{x + 1, y})
				toFill2.PushBack([]int{x, y + 1})
			}
		}
	}
	toFill.Init()
	if toFill2.Len() > 0 {
		flood(m, cells, toFill2, v+1)
	}
}

// Creates a key for pair of coordinates
func key(xy []int) string {
	return fmt.Sprintf("%d:%d", xy[0], xy[1])
}

// returns a map which is a copy of original map with heros other than "h" added as obstacles
func mapWithOthers(h *hero) []byte {
	c := make([]byte, len(cavern))
	copy(c, cavern)
	for _, o := range heroes {
		if h.id != o.id && o.alive {
			if o.kind == 'E' {
				c[o.y*width+o.x] = 2
			} else if o.kind == 'G' {
				c[o.y*width+o.x] = 3
			}
		}
	}
	return c
}

// We have hero's coordinates and target coordinates. This method returns coordinates
// of the next step on a shortest road to target coordinates
func shortestPath(h *hero, turnMap []byte, x, y int) []int {

	shortestDistance := math.MaxInt32
	var sx, sy int

	checking := [][]int{{h.x, h.y - 1}, {h.x - 1, h.y}, {h.x + 1, h.y}, {h.x, h.y + 1}}

	for _, xy := range checking {
		if turnMap[xy[1]*width+xy[0]] != 0 {
			continue
		}
		// copy of a map
		m := make([]byte, len(turnMap))
		copy(m, turnMap)

		// initially it'll have single target cell - we should be able to find the distance
		key := fmt.Sprintf("%d:%d", x, y)
		checkedCells := map[string]int{key: -1}

		// flooding the map and collecting reachable cells
		toFill := list.New()
		toFill.PushBack([]int{xy[0], xy[1]})
		flood(m, checkedCells, toFill, 5)

		// fmt.Printf("Checking %d:%d - %d\n", xy[0], xy[1], checkedCells[key])
		if checkedCells[key] <= shortestDistance {
			if checkedCells[key] < shortestDistance {
				// no doubt
				sx = xy[0]
				sy = xy[1]
			} else {
				// check reading order
				if xy[1] < sy {
					sx = xy[0]
					sy = xy[1]
				} else if xy[1] == sy {
					if xy[0] < sx {
						sx = xy[0]
						sy = xy[1]
					}
					// no other possibilities
				}
			}
			shortestDistance = checkedCells[key]
		}
	}

	return []int{sx, sy}
}

// hero attacks on current turn's map
func attackNearest(h *hero, turnMap []byte) {
	lowestHP := math.MaxInt32
	var target *hero
	checking := [][]int{{h.x, h.y - 1}, {h.x - 1, h.y}, {h.x + 1, h.y}, {h.x, h.y + 1}}

	for _, xy := range checking {
		// goblin attacks elf, elf attacks goblin
		if (h.kind == 'E' && turnMap[xy[1]*width+xy[0]] != 3) || (h.kind == 'G' && turnMap[xy[1]*width+xy[0]] != 2) {
			continue
		}

		var enemy *hero
		for _, e := range heroes {
			if e.x == xy[0] && e.y == xy[1] && e.alive {
				enemy = e
				break
			}
		}
		if enemy != nil {
			if enemy.hp <= lowestHP {
				if enemy.hp < lowestHP {
					// no doubt
					target = enemy
				} else {
					// check reading order
					if enemy.y < target.y {
						target = enemy
					} else if enemy.y == target.y {
						if enemy.x < target.x {
							target = enemy
						}
						// no other possibilities
					}
				}
				lowestHP = enemy.hp
			}
		}
	}

	if target != nil {
		fmt.Printf("Hero %s attacks %s -> ", h, target)
		target.hp -= h.ap
		fmt.Printf("%s\n", target)
		if target.hp <= 0 {
			target.alive = false
		}
	}
}

func sortHeroes(heroes []*hero) {
	sort.Slice(heroes, func(i, j int) bool {
		if heroes[i].y > heroes[j].y {
			return false
		} else if heroes[i].y < heroes[j].y {
			return true
		} else {
			return heroes[i].x < heroes[j].x
		}
	})
}

func show(header string, cavern []byte) {
	if header == "" {
		fmt.Printf("===\n")
	} else {
		fmt.Printf("%s ===\n", header)
	}
	lines := make([][]rune, height)
	for y := 0; y < height; y++ {
		lines[y] = make([]rune, width)
		for _, h := range heroes {
			if h.y == y && h.alive {
				([]rune(lines[y]))[h.x] = h.kind
			}
		}
		for x := 0; x < width; x++ {
			switch cavern[y*width+x] {
			case 0:
				lines[y][x] = '.'
			case 1:
				lines[y][x] = '#'
			case 2:
				lines[y][x] = 'e'
			case 3:
				lines[y][x] = 'g'
			default:
				lines[y][x] = '~'
			}
		}
	}
	for _, l := range lines {
		fmt.Printf("%s\n", string(l))
	}
	fmt.Println("===")
}

type hero struct {
	id           int
	x, y, hp, ap int
	kind         rune
	alive        bool
}

func (h *hero) String() string {
	return fmt.Sprintf("#%d: %c) %d:%d (HP %03d) (alive: %t)", h.id, h.kind, h.x, h.y, h.hp, h.alive)
}
