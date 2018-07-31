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
package main

import (
	"flag"
	"fmt"
	"os"
	"log"
)

var version bool
var help bool
var input string

func init() {
	flag.BoolVar(&version, "version", false, "prints version and exits")
	flag.BoolVar(&help, "help", false, "information about usage")
	flag.StringVar(&input, "input", "", "puzzle input")
	flag.StringVar(&input, "i", "", "puzzle input")
}

func main() {
	flag.Parse()
	log.SetFlags(log.Ltime | log.Lmicroseconds | log.Lshortfile)
	log.Println("Starting main()")

	if help {
		flag.Usage()
		os.Exit(0)
	} else if version {
		fmt.Println("1.0")
		os.Exit(0)
	}

	flag.Usage()
}
