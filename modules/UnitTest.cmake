#
# File Catch2.cmake
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

# ------------- DEPENDENCIES ------------- #
include(FetchContent)
# https://github.com/catchorg/Catch2
# ---------------------------------------- #
set(TARGET_test "tests")
project(${TARGET_test} LANGUAGES CXX)

if(EXISTS ${EXTERNAL_DEPENDECIES_DIRECTORY}/Catch2/)
    message(STATUS "FOUND Catch2 in ${EXTERNAL_DEPENDECIES_DIRECTORY}/Catch2/")
    FetchContent_Declare(
        Catch2
        URL ${EXTERNAL_DEPENDECIES_DIRECTORY}/Catch2
    )
else()
    FetchContent_Declare(
        Catch2
        URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.3.2.tar.gz
    )
# FetchContent_Declare(
#   Catch2
#   GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#   GIT_TAG        v3.0.1 # or a later release
# )
endif()
FetchContent_MakeAvailable(Catch2)

# include_directories(${LIBDIR}/Catch2)

# get_filename_component(LIBDIR ${PROJECT_SOURCE_DIR} PATH)
# list(APPEND CMAKE_MODULE_PATH ${LIBDIR}/Catch2/Cmake)
# message(${LIBDIR}/Catch2)
# include(Catch2 REQUIRED)
# find_package(Catch2 REQUIRED)

# message(STATUS "Catch2 PATH is " ${CMAKE_CURRENT_SOURCE_DIR}/external/git/Catch2)



# message(STATUS "The project will build Catch2 tests")
# FetchContent_MakeAvailable(Catch2)

# include(CTest)

file(GLOB
    Cat_SRCS
    "${SRCS_DIR}/tests/catch2.cpp"
)
# enable_testing()
message(STATUS "Build TEST: ${Cat_SRCS}")
add_executable(
    ${TARGET_test}
    ${Cat_SRCS}
)
target_link_libraries(${TARGET_test} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)
# add_test(NAME ${TARGET_test} COMMAND ${TARGET_test})
# catch_discover_tests(${TARGET_test})

# FetchContent_Declare(
#     TBB
#     GIT_REPOSITORY https://github.com/oneapi-src/oneTBB.git
#     GIT_TAG v2021.5.0
#     GIT_SHALLOW TRUE
# )
# FetchContent_Populate(TBB)