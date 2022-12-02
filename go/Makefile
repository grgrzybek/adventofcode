#
# Copyright 2022 Grzegorz Grzybek
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

FILES = \
	internal/pkg/day01/day01.go \
	internal/pkg/day02/day02.go \
	cmd/aoc2022/main.go

all: bin/hello bin/aoc2022

bin/hello: cmd/hello/main.go
	go build -o bin/hello github.com/grgrzybek/adventofcode2022/cmd/hello

bin/aoc2022: $(FILES)
	go build -o bin/aoc2022 github.com/grgrzybek/adventofcode2022/cmd/aoc2022

clean:
	rm -rf bin

.PHONY: clean
