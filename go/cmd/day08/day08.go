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
	"strconv"
)

var file string

func init() {
	var day08Cmd = &cobra.Command{
		Use:   "day08",
		Short: "Day 08 example",
		Run:   run,
	}
	day08Cmd.Flags().StringVarP(&file, "input", "i", "", "Input file with puzzle data")
	aoc2018.RootCmd.AddCommand(day08Cmd)
}

func run(cmd *cobra.Command, _ []string) {
	if file == "" {
		_ = cmd.Usage()
		_, _ = fmt.Fprintln(os.Stderr, "File argument is required")
		os.Exit(1)
	}

	var f *os.File
	var e error
	if f, e = os.Open(file, ); e != nil {
		_, _ = fmt.Fprintln(os.Stderr, e)
		os.Exit(1)
	}
	defer (func(f *os.File) { _ = f.Close() })(f)

	scanner := bufio.NewScanner(f)
	scanner.Split(bufio.ScanWords)

	state := childNodes
	heapChildren := list.New()
	heapMetadata := list.New()

	md := 0

	var rootNode *node
	var currentNode *node
	parents := list.New()

	for ; scanner.Scan(); {
		var l string
		if l = scanner.Text(); l == "" {
			break
		}

		n, e := strconv.Atoi(l)
		if e != nil {
			_, _ = fmt.Fprintf(os.Stderr, "%s\n", e)
			os.Exit(1)
		}

		if state == childNodes {
			newChild := &node{childCount: n, children: make([]*node, 0), metadata: make([]int, 0)}
			if parents.Len() > 0 {
				if parent, ok := parents.Back().Value.(*node); ok {
					parent.children = append(currentNode.children, newChild)
					newChild.parent = parent
				}
			}
			currentNode = newChild
			if rootNode == nil {
				rootNode = currentNode
			}
			heapChildren.PushBack(n)
			parents.PushBack(newChild)
			state = metadataEntries
		} else if state == metadataEntries {
			currentNode.metadataCount = n
			heapMetadata.PushBack(n)
			if c, ok := heapChildren.Back().Value.(int); ok && c > 0 {
				state = childNodes
			} else {
				state = metadata
			}
		} else if state == metadata {
			e := heapMetadata.Remove(heapMetadata.Back())
			if mv, ok := e.(int); ok {
				md += n
				currentNode.metadataSum += n
				currentNode.metadata = append(currentNode.metadata, n)
				for m := 1; m < mv; m++ {
					scanner.Scan()
					n, e = strconv.Atoi(scanner.Text())
					md += n
					currentNode.metadataSum += n
					currentNode.metadata = append(currentNode.metadata, n)
				}
			}
			for {
				if heapChildren.Len() > 0 {
					if c, ok := heapChildren.Back().Value.(int); ok && c == 0 {
						parents.Remove(parents.Back())
						currentNode = currentNode.parent
						heapChildren.Remove(heapChildren.Back())
						continue
					} else {
						heapChildren.Back().Value = c - 1
						if c, ok := heapChildren.Back().Value.(int); ok && c == 0 {
							// end of children - metadata of parent
							state = metadata
						} else {
							// next child
							state = childNodes
						}
						break
					}
				} else {
					break
				}
			}
		} else {
			break
		}
	}

	metadata2 := calculate(rootNode)

	fmt.Printf("Answer 1: %d, Answer 2: %v\n", md, metadata2)
}

func calculate(n *node) int {
	res := 0
	if n.childCount == 0 {
		res = n.metadataSum
	} else {
		v := 0
		for _, i := range n.metadata {
			if len(n.children) >= i {
				v += calculate(n.children[i-1])
			}
		}
		res = v
	}
	return res
}

const (
	childNodes      = 1
	metadataEntries = 2
	metadata        = 3
)

type node struct {
	childCount    int
	metadataCount int
	metadataSum   int
	parent        *node
	children      []*node
	metadata      []int
}

func (n *node) String() string {
	return fmt.Sprintf("cc: %d, mc: %d, m: %v", n.childCount, n.metadataCount, n.metadata)
}
