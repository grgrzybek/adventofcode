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

#include <cstring>
#include <iostream>

#include "config.h"
#include "utils/intcode.h"
#include "utils/utils.h"

namespace aoc2019 {

	IntcodeProgram::IntcodeProgram(std::basic_istream<char> *input) {
		std::string line;
		long long v;
		int idx = 0;

		while (getline(*input, line)) {
			aoc2019::trim(line);

			std::string::size_type pos1 = 0;
			std::string::size_type pos2 = 0;
			pos2 = line.find(',', pos1);
			while ((pos2 = line.find(',', pos1)) != std::string::npos) {
				v = std::stoll(line.substr(pos1, pos2 - pos1));
				ensure_capacity(idx);
				this->memory[idx++] = v;
				pos1 = pos2 + 1;
			}
			v = std::stoll(line.substr(pos1, pos2 - pos1));
			ensure_capacity(idx);
			memory[idx++] = v;
		}
		mem_end = idx;
	}

	IntcodeProgram::IntcodeProgram(IntcodeProgram &program) {
		this->mem_size = program.mem_size;
		this->mem_end = program.mem_end;
		this->program_name = program.program_name;
		this->ip = program.ip;
		this->base = program.base;
		this->memory = static_cast<long long int *>(::calloc(mem_size, sizeof(long long)));
		std::memcpy((void *) this->memory, (const void *) program.memory, mem_size);
	}

	void IntcodeProgram::show() {
		for (size_t idx = 0; idx < mem_end; idx++) {
			if (idx > 0)
				std::cout << ',';
			std::cout << memory[idx];
		}
		std::cout << std::endl;
	}

	void IntcodeProgram::run(long long input, std::vector<long long> &output, std::vector<long long>::size_type output_count) {
		int &pos = this->ip;
		bool end = false;

		for (;;) {
			if (this->_debug)
				std::cout << std::endl << "pos: " << pos << ", base: " << base << std::endl;
			int op = (int)this->memory[pos];
			int instruction = op % 100;
			int m1 = op % 1000 / 100;
			int m2 = op % 10000 / 1000;
			int m3 = op % 100000 / 10000;
	
			long long arg1 = 0, arg2 = 0;
	
			switch (instruction) {
				case 1: // ADD
					if (this->_debug)
						std::cout << "================== ADD " << this->memory[pos] << "," << this->memory[pos+1] << "," << this->memory[pos+2] << "," << this->memory[pos+3] << std::endl;
					arg1 = val(m1, pos+1);
					arg2 = val(m2, pos+2);
					if (this->_debug)
						std::cout << "    " << arg1 << "+" << arg2 << " -> " << this->memory[pos+3] + (m3 == 2 ? base : 0) << std::endl;
					ensure_capacity((int)this->memory[pos+3] + (m3 == 2 ? base : 0));
					this->memory[this->memory[pos+3] + (m3 == 2 ? base : 0)] = arg1 + arg2;
					pos += 4;
					break;
				case 2: // MUL
					if (this->_debug)
						std::cout << "================== MUL " << this->memory[pos] << "," << this->memory[pos+1] << "," << this->memory[pos+2] << "," << this->memory[pos+3] << std::endl;
					arg1 = val(m1, pos+1);
					arg2 = val(m2, pos+2);
					if (this->_debug)
						std::cout << "    " << arg1 << "*" << arg2 << " -> " << this->memory[pos+3] + (m3 == 2 ? base : 0) << std::endl;
					ensure_capacity((int)this->memory[pos+3] + (m3 == 2 ? base : 0));
					this->memory[this->memory[pos+3] + (m3 == 2 ? base : 0)] = arg1 * arg2;
					pos += 4;
					break;
				case 3: // READ
					if (this->_debug)
						std::cout << "================== READ " << this->memory[pos] << "," << this->memory[pos+1] << std::endl;
					if (m1 == 0) {
						ensure_capacity((int)this->memory[pos+1]);
						if (this->_debug)
							std::cout << "    " << "read: " << input << " -> " << this->memory[pos+1] << std::endl;
						this->memory[this->memory[pos+1]] = input;
					} else if (m1 == 2) {
						ensure_capacity((int)this->memory[pos+1] + base);
						if (this->_debug)
							std::cout << "    " << "read: " << input << " -> " << this->memory[pos+1] + base << std::endl;
						this->memory[this->memory[pos+1] + base] = input;
					}
					pos += 2;
					break;
				case 4: // WRITE
					if (this->_debug)
						std::cout << "================== WRITE " << this->memory[pos] << "," << this->memory[pos+1] << std::endl;
					arg1 = val(m1, pos+1);
					if (this->_debug)
						std::cout << "    " << "write: " << arg1 << std::endl;
					output.push_back(arg1);
					pos += 2;
					if (output_count == output.size()) {
						// stop after collecting N output
						return;
					}
					break;
				case 5: // jump-if-true
					if (this->_debug)
						std::cout << "================== JUMP-IF-TRUE " << this->memory[pos] << "," << this->memory[pos+1] << "," << this->memory[pos+2] << std::endl;
					arg1 = val(m1, pos+1);
					arg2 = val(m2, pos+2);
					if (arg1) {
						if (this->_debug)
							std::cout << "    " << arg1 << ": pos = " << arg2	 << std::endl;
						pos = (int)arg2;
					} else {
						pos += 3;
						if (this->_debug)
							std::cout << "    " << arg1 << ": pos+=3 = " << pos << std::endl;
					}
					break;
				case 6: // jump-if-false
					if (this->_debug)
						std::cout << "================== JUMP-IF-FALSE " << this->memory[pos] << "," << this->memory[pos+1] << "," << this->memory[pos+2] << std::endl;
					arg1 = val(m1, pos+1);
					arg2 = val(m2, pos+2);
					if (!arg1) {
						if (this->_debug)
							std::cout << "    !" << arg1 << ": pos = " << arg2	 << std::endl;
						pos = (int)arg2;
					} else {
						pos += 3;
						if (this->_debug)
							std::cout << "    " << arg1 << ": pos+=3 = " << pos << std::endl;
					}
					break;
				case 7: // less-than
					if (this->_debug)
						std::cout << "================== LESS-THAN " << this->memory[pos] << "," << this->memory[pos+1] << "," << this->memory[pos+2] << "," << this->memory[pos+3] << std::endl;
					arg1 = val(m1, pos+1);
					arg2 = val(m2, pos+2);
					ensure_capacity((int)this->memory[pos+3] + (m3 == 2 ? base : 0));
					if (this->_debug)
						std::cout << "    " << arg1 << "<" << arg2 << " " << (arg1 < arg2 ? 1 : 0) << " -> " << this->memory[pos+3] + (m3 == 2 ? base : 0) << std::endl;
					this->memory[this->memory[pos+3] + (m3 == 2 ? base : 0)] = arg1 < arg2 ? 1 : 0;
					pos += 4;
					break;
				case 8: // equals
					if (this->_debug)
						std::cout << "================== EQUALS " << this->memory[pos] << "," << this->memory[pos+1] << "," << this->memory[pos+2] << "," << this->memory[pos+3] << std::endl;
					arg1 = val(m1, pos+1);
					arg2 = val(m2, pos+2);
					ensure_capacity((int)this->memory[pos+3] + (m3 == 2 ? base : 0));
					if (this->_debug)
						std::cout << "    " << arg1 << "==" << arg2 << " " << (arg1 == arg2 ? 1 : 0) << " -> " << this->memory[pos+3] + (m3 == 2 ? base : 0) << std::endl;
					this->memory[this->memory[pos+3] + (m3 == 2 ? base : 0)] = arg1 == arg2 ? 1 : 0;
					pos += 4;
					break;
				case 9: // change base
					if (this->_debug)
						std::cout << "================== BASE " << this->memory[pos] << "," << this->memory[pos+1] << std::endl;
					arg1 = val(m1, pos+1);
					if (this->_debug)
						std::cout << "    " << "base " << base << "+" << arg1 << std::endl;
					base += (int)arg1;
					pos += 2;
					break;
				case 99:
					if (this->_debug)
						std::cout << "================== END " << this->memory[pos] << std::endl;
					end = true;
					break;
				default:
					if (this->_debug)
						std::cout << "================== ? " << this->memory[pos] << std::endl;
			}
			if (end) {
				finished = true;
				break;
			}
		}
	}

	long long IntcodeProgram::val(int mode, int pos) {
		switch (mode) {
		case 0: // absolute
			return this->memory[this->memory[pos]];
		case 1: // immediate
			return this->memory[pos];
		case 2: // relative
			return this->memory[this->memory[pos] + base];
		default:
			return 0LL;
		}
	}

	void IntcodeProgram::ensure_capacity(int idx) {
		if (!this->memory) {
			// for simplicity - one chunk
			if (this->_debug)
				std::cout << "Expanding from " << this->mem_size << " to " << chunk_size << std::endl;
			this->memory = (long long *) ::calloc(chunk_size, sizeof(long long));
			mem_size = chunk_size * sizeof(long long);
		}
		if ((size_t) idx >= mem_size) {
			int chunks = (int)(idx / chunk_size + 1);
			long long *new_memory = static_cast<long long int *>(::calloc(chunks * chunk_size, sizeof(long long)));
			std::memcpy((void *) new_memory, (const void *) memory, mem_size * sizeof(long long));
			if (this->_debug)
				std::cout << "Expanding from " << mem_size << " to " << chunks * chunk_size << " (idx=" << idx << ")"
					  << std::endl;
			mem_size = chunks * chunk_size * sizeof(long long);
			::free(memory);
			this->memory = new_memory;
		}
	}

}
