file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS *.cpp)
add_executable(worldgen_tests ${TEST_SOURCES})
target_link_libraries(worldgen_tests PRIVATE worldgen_core doctest::doctest)

add_test(NAME worldgen_tests COMMAND worldgen_tests)