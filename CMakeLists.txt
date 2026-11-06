cmake_minimum_required(VERSION 3.25)
project(worldgen LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

option(WORLDGEN_COVERAGE "Instrument build for code coverage (GCC/Clang)" OFF)
if(WORLDGEN_COVERAGE)
  add_compile_options(--coverage -O0 -g)
  add_link_options(--coverage)
endif()

option(WORLDGEN_PROFILING "Instrument build for gprof profiling (GCC)" OFF)
if(WORLDGEN_PROFILING)
  add_compile_options(-pg -g -fno-omit-frame-pointer -fno-inline-functions)
  add_link_options(-pg)
endif()

find_package(doctest REQUIRED)

enable_testing()
add_subdirectory(src)
add_subdirectory(cli)
add_subdirectory(tests/unit)