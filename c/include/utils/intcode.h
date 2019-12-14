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

#ifndef AOC2019_INTCODE_H
#define AOC2019_INTCODE_H

#include <vector>

#include <getopt.h>

namespace aoc2019 {

	/**
	 * Class representing an instance of Intcode program (https://adventofcode.com/2019/day/9).
	 * It should be initialized with a file containing the instructions and may be run using different methods.
	 * We can run single instance, keep instance between runs, etc.
	 */
	class IntcodeProgram {
		const size_t chunk_size = 4096;
		// the initially empty memory buffer for intcode/data
		long long *memory = nullptr;
		// initially 0 size of memory as multiplication of chunk_size
		size_t mem_size = 0;
		// actual pointer to next-after-last used memory element
		size_t mem_end = 0;

		// index of instruction pointer
		int ip = 0;
		// current base
		int base = 0;
		// is the program finished?
		bool finished = false;

		bool _debug = true;

		/**
		 * Should be called when attempting to set (or read) value from memory at
		 * given index
		 * @param idx
		 */
		void ensure_capacity(int idx);

		/**
		 * Gets the value from given position using current base and passed mode
		 * @param mode
		 * @param pos
		 * @return
		 */
		long long val(int mode, int pos);

		/**
		 * Private default constructor
		 */
		IntcodeProgram() = default;

	public:
		const char *program_name = nullptr;

		/**
		 * Creates a new program from input data
		 * @param input
		 */
		IntcodeProgram(std::basic_istream<char> *input);

		/**
		 * Does a copy of existing program
		 * @param program
		 */
		IntcodeProgram(IntcodeProgram &program);

		~IntcodeProgram() {
			::free(memory);
		}

		/**
		 * Shows the state of memory with the program
		 */
		void show() __attribute__((unused));

		/**
		 * Runs a program until the end. There's single input and output will be put to passed vector
		 * @param input
		 * @param output
		 * @param output_count
		 */
		void run(long long input, std::vector<long long> &output, std::vector<long long>::size_type output_count = -1);

		inline bool is_finished() {
			return finished;
		}

		inline void debug(bool d) {
			this->_debug = d;
		}
	};

}

#endif //AOC2019_INTCODE_H
