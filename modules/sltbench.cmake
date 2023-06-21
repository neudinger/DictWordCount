# File sltbench.cmake
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

# ------------- DEPENDENCIES ------------- #
include(FetchContent)
# https://github.com/ivafanas/sltbench
# ---------------------------------------- #

# FetchContent_Declare(
#     TBB
#     GIT_REPOSITORY https://github.com/oneapi-src/oneTBB.git
#     GIT_TAG v2021.5.0
#     GIT_SHALLOW TRUE
# )
# FetchContent_Populate(TBB)

# if(BUILD_TESTING)

#   include(CTest)
#   add_executable(particle_test test.cu)
#   set_target_properties(particle_test PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
#   target_link_libraries(particle_test PRIVATE particles)
  
#   add_test(NAME particles_10k COMMAND particle_test 10000)
#   add_test(NAME particles_256k COMMAND particle_test 256000)
  
#   enable_testing()
#   if(APPLE)
#     # We need to add the default path to the driver (libcuda.dylib) as an rpath,
#     # So that the static cuda runtime can find it at runtime.
#     set_property(TARGET particle_test PROPERTY BUILD_RPATH ${CMAKE_CUDA_IMPLICIT_LINK_DIRECTORIES})
#   endif()
# endif()