cmake_minimum_required(VERSION 3.25)
project(worldgen LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

option(WORLDGEN_COVERAGE "Instrument build for code coverage (GCC/Clang)" OFF)
if(WORLDGEN_COVERAGE)
  add_compile_options(--coverage -O0 -g)
  add_link_options(--coverage)
endif()

find_package(doctest REQUIRED)

enable_testing()
add_subdirectory(src)
add_subdirectory(cli)
add_subdirectory(tests/unit)