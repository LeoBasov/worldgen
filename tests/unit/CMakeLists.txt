cmake_minimum_required(VERSION 3.20)
project(unit_tests LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

find_package(doctest REQUIRED)

file(GLOB TEST_SOURCES CONFIGURE_DEPENDS *.cpp)
add_executable(unit_tests ${TEST_SOURCES})
target_link_libraries(unit_tests PRIVATE doctest::doctest)

enable_testing()
add_test(NAME unit_tests COMMAND unit_tests)