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
	"sort"
	"strings"
)

var file string

func init() {
	var cmd = &cobra.Command{
		Use:   "day21",
		Short: "Day 21 example",
		Run:   run,
	}
	cmd.Flags().StringVarP(&file, "file", "f", "", "Input file with puzzle data")
	aoc2020.RootCmd.AddCommand(cmd)
}

type food struct {
	id          int // just a position in the list
	ingredients []string
	allergens   []string
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

	foods := make([]*food, 0)
	ingredients := make(map[string][]int) // ingredient -> food ids
	allergens := make(map[string][]int)   // allergen -> food ids

	id := 0
	for {
		if !scanner.Scan() {
			break
		}
		line := scanner.Text()
		if line == "" {
			continue
		}
		f := &food{id: id}
		f.ingredients = make([]string, 0)
		f.allergens = make([]string, 0)
		id++
		foods = append(foods, f)

		idx := strings.Index(line, " (contains ")
		in := line[0:idx]
		al := line[idx+11:len(line)-1]

		sc1 := bufio.NewScanner(strings.NewReader(in))
		sc1.Split(bufio.ScanWords)
		for ; sc1.Scan(); {
			f.ingredients = append(f.ingredients, sc1.Text())
		}
		sc2 := bufio.NewScanner(strings.NewReader(al))
		sc2.Split(bufio.ScanWords)
		for ; sc2.Scan(); {
			a := sc2.Text()
			if strings.HasSuffix(a, ",") {
				a = a[:len(a)-1]
			}
			f.allergens = append(f.allergens, a)
		}

		for _, i := range f.ingredients {
			if _, ok := ingredients[i]; !ok {
				ingredients[i] = make([]int, 0)
			}
			ingredients[i] = append(ingredients[i], f.id)
		}
		fmt.Printf("food %d: %d ingredients, allergens:", f.id, len(f.ingredients))
		for _, a := range f.allergens {
			if _, ok := allergens[a]; !ok {
				allergens[a] = make([]int, 0)
			}
			allergens[a] = append(allergens[a], f.id)
			fmt.Printf(" %s", a)
		}
		fmt.Println()
	}

	// part 1

	var currentIngredients map[string]bool
	a2i := make(map[string]map[string]bool)
	for allergen, foodIds := range allergens {
		for _, foodId := range foodIds {
			if currentIngredients == nil {
				currentIngredients = make(map[string]bool)
				for _, ingredient := range foods[foodId].ingredients {
					currentIngredients[ingredient] = true
				}
			} else {
				fi := make(map[string]bool)
				for _, ingredient := range foods[foodId].ingredients {
					fi[ingredient] = true
				}
				for k := range currentIngredients {
					if _, ok := fi[k]; !ok {
						delete(currentIngredients, k)
					}
				}
			}
		}
		a2i[allergen] = make(map[string]bool)
		for i := range currentIngredients {
			a2i[allergen][i] = true
		}
		currentIngredients = nil
	}

	fmt.Println(">>> BEFORE")
	for allergen, ingredients := range a2i {
		fmt.Printf("%s ->", allergen)
		for i := range ingredients {
			fmt.Printf(" %s", i)
		}
		fmt.Println()
	}

	for {
		change := false
		for allergen, ingredients := range a2i {
			if len(ingredients) == 1 {
				i1 := ""
				for i := range ingredients {
					i1 = i
					break
				}
				// check other allergens and remove this ingredient from their list
				for a2, i2 := range a2i {
					if allergen == a2 {
						continue
					}
					if _, ok := i2[i1]; ok {
						delete(i2, i1)
						change = true
					}
				}
			}
		}
		if !change {
			break
		}
	}

	fmt.Println(">>> AFTER")
	for allergen, ingredients := range a2i {
		fmt.Printf("%s ->", allergen)
		for i := range ingredients {
			fmt.Printf(" %s", i)
		}
		fmt.Println()
	}

	// now each allergen should have only one ingredient associated

	dangerousIngredients := make(map[string]string)
	for a, is := range a2i {
		for i := range is {
			dangerousIngredients[i] = a
		}
	}

	count1 := 0
	for _, food := range foods {
		for _, ingredient := range food.ingredients {
			if _, ok := dangerousIngredients[ingredient]; !ok {
				count1++
			}
		}
	}

	// part 2

	dangerousIngredientsList := make([]string, 0)
	for di := range dangerousIngredients {
		dangerousIngredientsList = append(dangerousIngredientsList, di)
	}

	sort.Slice(dangerousIngredientsList, func(i int, j int) bool {
		return dangerousIngredients[dangerousIngredientsList[i]] < dangerousIngredients[dangerousIngredientsList[j]]
	})

	v := 0
	v2 := ""
	for _, di := range dangerousIngredientsList {
		if v > 0 {
			v2 += ","
		}
		v2 += di
		v++
	}

	// answer1 = 2389, answer2 = fsr,skrxt,lqbcg,mgbv,dvjrrkv,ndnlm,xcljh,zbhp
	fmt.Printf("Answer 1: %d, Answer 2: %s\n", count1, v2)
}
