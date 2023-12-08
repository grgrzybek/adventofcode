#
# Copyright 2023 Grzegorz Grzybek
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

cmake_minimum_required(VERSION 3.26)

project(aoc2023 VERSION 1.0.0
                DESCRIPTION "Advent of Code 2023 - C/C++ with CMake"
                HOMEPAGE_URL "https://github.com/grgrzybek/adventofcode")

set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/_install")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O0 -g")

set(commands
    day01
    day02
    day03
    day04
    day05
    day06
    day07
    day08
)

configure_file(config.h.in "${CMAKE_INSTALL_PREFIX}/include/config.h")
include_directories("${CMAKE_INSTALL_PREFIX}/include")
include_directories("${CMAKE_SOURCE_DIR}/include")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIRS})

add_library(aoc2023 SHARED src/utils/utils.cc)

foreach(cmd ${commands})
    add_executable(${cmd})
    target_sources(${cmd} PUBLIC src/${cmd}.cc)
    target_link_libraries(${cmd} aoc2023)
endforeach(cmd)

#target_link_libraries(day09 ${PNG_LIBRARIES})
#target_link_libraries(day09 ${SDL2_LIBRARIES})

install(TARGETS ${commands} RUNTIME DESTINATION bin)
