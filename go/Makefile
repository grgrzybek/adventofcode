#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.
#

FILES = internal/pkg/day01/day01.go \
	internal/pkg/day02/day02.go \
	internal/pkg/day03/day03.go \
	internal/pkg/day04/day04.go \
	internal/pkg/day05/day05.go

all: bin/hello bin/aoc2020

bin/hello: cmd/hello/main.go
	go build -o bin/hello github.com/grgrzybek/adventofcode2020/cmd/hello

bin/aoc2020: cmd/aoc2020/main.go $(FILES)
	go build -o bin/aoc2020 github.com/grgrzybek/adventofcode2020/cmd/aoc2020

clean:
	rm -rf bin

.PHONY: clean
