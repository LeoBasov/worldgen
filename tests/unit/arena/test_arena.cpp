#include <doctest/doctest.h>

#include "arena/arena.hpp"

using namespace wg;

TEST_CASE("resize") {
    Arena arena;
    const int size_x = 7;
    const int size_y = 13;

    arena.resize(size_x, size_y);

    CHECK(arena.size_x == size_x);
    CHECK(arena.size_y == size_y);
    CHECK(arena.size == size_x * size_y);
}