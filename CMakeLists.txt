cmake_minimum_required(VERSION 3.25)
project(worldgen LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

find_package(doctest REQUIRED)

enable_testing()
add_subdirectory(src)
add_subdirectory(cli)
add_subdirectory(tests/unit)