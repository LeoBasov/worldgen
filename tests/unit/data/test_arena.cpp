#include <doctest/doctest.h>

#include "data/arena.hpp"

TEST_CASE("resize") {
    wg::Config::Map map;
    wg::Arena arena;
    const int x = 7;
    const int y = 13;

    map.size_x = x;
    map.size_y = y;

    arena.resize(map);

    CHECK(arena.get_size_x() == x);
    CHECK(arena.get_size_y() == y);
}