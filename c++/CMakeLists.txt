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

# CMakeLists.txt is placed at the root of the source tree of any application, library it will work for.

cmake_minimum_required(VERSION 3.12)

# https://cmake.org/cmake/help/v3.12/command/project.html
project(aoc2018 VERSION 0.1.0
                         DESCRIPTION "Advent of Code 2018 - C++ with CMake")
message("PROJECT_NAME: ${PROJECT_NAME}")       # or ${aoc2018_NAME}
message("PROJECT_VERSION: ${PROJECT_VERSION}") # or ${aoc2018_VERSION}
message("PROJECT_DESCRIPTION: ${PROJECT_DESCRIPTION}")
message("")

# https://cmake.org/cmake/help/v3.12/command/set.html
# Set a normal, cache, or environment variable to a given value.
set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/_install")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O0 -g")
set(CMAKE_CXX_STANDARD 14)

set(commands day01)

#message("CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
#message("CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
message("PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message("PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")
#message("CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT: ${CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT}")
message("CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
message("")

message("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message("CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")
message("CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")

# Environment variables are used to configure compiler flags, linker flags, test configurations for a regular build process.
# Some of the environment variables are overriden by predefined CMake Variables. e.g. CXXFLAGS is overriden when CMAKE_CXX_FLAGS is defined.
# https://cmake.org/cmake/help/v3.12/manual/cmake-env-variables.7.html

# https://cmake.org/cmake/help/v3.12/command/configure_file.html
# Copy a file to another location and modify its contents.
configure_file(config.h.in "${CMAKE_INSTALL_PREFIX}/include/config.h")
include_directories("${CMAKE_INSTALL_PREFIX}/include")

# https://cmake.org/cmake/help/v3.12/manual/cmake-buildsystem.7.html
#
# A CMake-based buildsystem is organized as a set of high-level logical targets.
# Each target corresponds to an executable or library, or is a custom target containing custom commands.
# Dependencies can be expressed between targets.

# https://cmake.org/cmake/help/v3.12/command/add_executable.html
# Adds an executable target called <name> to be built from the source files listed in the command invocation.
# (The source files can be omitted here if they are added later using target_sources().)
# The <name> corresponds to the logical target name and must be globally unique within a project.
foreach(cmd ${commands})
    add_executable(${cmd})
    target_sources(${cmd} PUBLIC src/${cmd}.cc)
endforeach(cmd)

# https://cmake.org/cmake/help/v3.12/command/install.html
# The TARGETS form specifies rules for installing targets from a project.
# There are several kinds of target files that may be installed: ARCHIVE, LIBRARY, RUNTIME, ...
# DESTINATION specifies the directory on disk to which a file will be installed.
# If a relative path is given it is interpreted relative to the value of the CMAKE_INSTALL_PREFIX variable.
#
# CMAKE_INSTALL_PREFIX - Install directory used by install().
# On UNIX one can use the DESTDIR mechanism in order to relocate the whole installation: `make DESTDIR=/home/john install`
install(TARGETS ${commands} RUNTIME DESTINATION bin)
