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
	"regexp"
	"strconv"
	"strings"
)

var file string

func init() {
	var cmd = &cobra.Command{
		Use:   "day04",
		Short: "Day 04 example",
		Run:   run,
	}
	cmd.Flags().StringVarP(&file, "file", "f", "", "Input file with puzzle data")
	aoc2020.RootCmd.AddCommand(cmd)
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

	passports := make([]*passport, 0)

	currentPassport := &passport{}

	for ; scanner.Scan(); {
		line := scanner.Text()
		if line == "" {
			passports = append(passports, currentPassport)
			currentPassport = &passport{}
		}
		for _, v := range strings.Split(line, " ") {
			pair := strings.Split(v, ":")
			switch pair[0] {
			case "byr":
				currentPassport.byr = pair[1]
				break
			case "iyr":
				currentPassport.iyr = pair[1]
				break
			case "eyr":
				currentPassport.eyr = pair[1]
				break
			case "hgt":
				currentPassport.hgt = pair[1]
				break
			case "hcl":
				currentPassport.hcl = pair[1]
				break
			case "ecl":
				currentPassport.ecl = pair[1]
				break
			case "pid":
				currentPassport.pid = pair[1]
				break
			case "cid":
				currentPassport.cid = pair[1]
				break
			}
		}
	}
	passports = append(passports, currentPassport)

	// part 1

	count1 := 0
	for _, p := range passports {
		if p.isValid() {
			count1++
		}
	}

	// part 2

	count2 := 0
	for _, p := range passports {
		if p.isValid2() {
			count2++
		}
	}

	for _, p := range passports {
		fmt.Printf("> %s: %v\n", p, p.isValid2())
	}

	fmt.Printf("Answer 1: %d, Answer 2: %d\n", count1, count2)
}

type passport struct {
	byr, iyr, eyr, hgt, hcl, ecl, pid, cid string
}

func (n *passport) String() string {
	return fmt.Sprintf("{byr:%s, iyr:%s, eyr:%s, hgt:%s, hcl:%s, ecl:%s, pid:%s, cid:%s}",
		n.byr, n.iyr, n.eyr, n.hgt, n.hcl, n.ecl, n.pid, n.cid)
}

func (n *passport) isValid() bool {
	return len(n.byr) > 0 && len(n.iyr) > 0 && len(n.eyr) > 0 && len(n.hgt) > 0 && len(n.hcl) > 0 && len(n.ecl) > 0 && len(n.pid) > 0
}

func (n *passport) isValid2() bool {
	if !n.isValid() {
		return false
	}
	/*
		byr (Birth Year) - four digits; at least 1920 and at most 2002.
		iyr (Issue Year) - four digits; at least 2010 and at most 2020.
		eyr (Expiration Year) - four digits; at least 2020 and at most 2030.
		hgt (Height) - a number followed by either cm or in:
			If cm, the number must be at least 150 and at most 193.
			If in, the number must be at least 59 and at most 76.
		hcl (Hair Color) - a # followed by exactly six characters 0-9 or a-f.
		ecl (Eye Color) - exactly one of: amb blu brn gry grn hzl oth.
		pid (Passport ID) - a nine-digit number, including leading zeroes.
		cid (Country ID) - ignored, missing or not.
	 */
	valid := true
	if v, e := strconv.ParseInt(n.byr, 10, 0); e == nil {
		valid = valid && (v >= 1920 && v <= 2002)
	} else {
		return false
	}
	if v, e := strconv.ParseInt(n.iyr, 10, 0); e == nil {
		valid = valid && (v >= 2010 && v <= 2020)
	} else {
		return false
	}
	if v, e := strconv.ParseInt(n.eyr, 10, 0); e == nil {
		valid = valid && (v >= 2020 && v <= 2030)
	} else {
		return false
	}
	if !(strings.HasSuffix(n.hgt, "cm") || strings.HasSuffix(n.hgt, "in")) {
		return false
	}
	if v, e := strconv.ParseInt(n.hgt[0:len(n.hgt)-2], 10, 0); e == nil {
		if strings.HasSuffix(n.hgt, "cm") {
			valid = valid && (v >= 150 && v <= 193)
		} else {
			valid = valid && (v >= 59 && v <= 76)
		}
	} else {
		return false
	}
	if !(strings.HasPrefix(n.hcl, "#")) {
		return false
	} else {
		m, e := regexp.MatchString("^[0-9a-f]{6}$", n.hcl[1:])
		valid = valid && (e == nil && m)
	}
	valid = valid && (n.ecl == "amb" || n.ecl == "blu" || n.ecl == "brn" || n.ecl == "gry" || n.ecl == "grn" || n.ecl == "hzl" || n.ecl == "oth")
	m, e := regexp.MatchString("^[0-9]{9}$", n.pid)
	valid = valid && (e == nil && m)

	return valid
}
