file(GLOB TEST_SOURCES CONFIGURE_DEPENDS *.cpp)
add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests PRIVATE doctest::doctest)

add_test(NAME tests COMMAND tests)