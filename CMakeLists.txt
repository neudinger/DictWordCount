#
# File CMakeLists.txt
# Created on Mon May 29 2023
# Copyright (c) 2023 Etherogene
#
# The MIT License (MIT)
# Copyright (c) 2023 Barre kevin
# Contact barre_k@laposte.net, kevin.barre@epitech.eu
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software
# and associated documentation files (the "Software"), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or substantial
# portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
# TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#


cmake_minimum_required(VERSION 3.19...3.23 FATAL_ERROR)

set(TARGET "dict_word")
set(TARGET_VERSION 1.0)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
project(${TARGET} LANGUAGES C VERSION ${TARGET_VERSION})

# https://www.openmp.org/
option(OPENMP "Build ${TARGET} for with multithreading" OFF)
# https://github.com/catchorg/Catch2
option(UNIT_TEST "Build ${TARGET} with unit test" ON)
option(BDD_TEST "Build ${TARGET} with BDD Catch2 test tool" ON)
option(BENCH_TEST "Build ${TARGET} with Catch2 bench tool" ON)
# https://github.com/ivafanas/sltbench
option(SLT_BENCH "Build ${TARGET} with sltbench bench tool" ON)

# https://llvm.org/docs/CMake.html#frequently-used-cmake-variables
# 1. Release: `-O3 -DNDEBUG`
# 2. Debug: `-O0 -g`
# 3. RelWithDebInfo: `-O2 -g -DNDEBUG`
# 4. MinSizeRel: `-Os -DNDEBUG`
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

get_filename_component(HOME_DIR ${PROJECT_SOURCE_DIR} PATH)

set(WORK_DIR ${CMAKE_CURRENT_LIST_DIR})
set(SRCS_DIR ${CMAKE_CURRENT_LIST_DIR}/srcs)
set(INCLUDE_DIR ${WORK_DIR}/includes)
include_directories(${INCLUDE_DIR})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/modules)

file(GLOB
     SRCS
    "${SRCS_DIR}/*.c"
)

add_executable(
  ${TARGET}
  ${SRCS}
)

include(OpenMP)
include(InstallSetup)
