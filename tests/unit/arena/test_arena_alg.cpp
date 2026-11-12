#include <doctest/doctest.h>

#include "arena/arena.hpp"
#include "arena/arena_alg.hpp"

using namespace wg;

TEST_CASE("calc_gradients") {
    /*
    +--→ x
    |
    ↓
    y
    */

    /*
    0---1---2
    |   |   |
    3---4---5
    |   |   |
    6---7---8
    */

    Field<double> field;
    const int size_x = 3;
    const int size_y = 3;
    const int id = 4;

    field.resize(size_x, size_y);

    field(0) = 1.0;
    field(1) = 2.0;
    field(2) = 3.0;

    field(3) = 4.0;
    field(4) = 0.0;  // id centered field
    field(5) = 6.0;

    field(6) = 7.0;
    field(7) = 8.0;
    field(8) = 9.0;

    const std::vector<int> neighbour_ids = field.neighbourhood(id, 1, Field<double>::MOORE);
    const std::vector<double> gradients = arena_alg::calc_gradients(id, neighbour_ids, field);

    CHECK(gradients.size() == 9);

    CHECK(gradients.at(0) == 1.0 / std::sqrt(2.0));
    CHECK(gradients.at(1) == 2.0);
    CHECK(gradients.at(2) == 3.0 / std::sqrt(2.0));

    CHECK(gradients.at(3) == 4.0);
    CHECK(gradients.at(4) == 0.0);
    CHECK(gradients.at(5) == 6.0);

    CHECK(gradients.at(6) == 7.0 / std::sqrt(2.0));
    CHECK(gradients.at(7) == 8.0);
    CHECK(gradients.at(8) == 9.0 / std::sqrt(2.0));
}

TEST_CASE("track_river") {
    /*
    +--→ x
    |
    ↓
    y
    */

    /*
    0----1----2----3----4
    |    |    |    |    |
    5----6----7----8----9
    |    |    |    |    |
    10---11---12---13---14
    |    |    |    |    |
    15---16---17---18---19
    |    |    |    |    |
    20---21---22---23---24
    */

    Arena arena;
    const int size_x = 5;
    const int size_y = 5;
    const int start_id = 4;
    const double sea_level = 2.0;

    arena.resize(size_x, size_y);

    for (auto& element : arena.elevation.value.elements) {
        element = 10.0;
    }

    arena.elevation.value(8) = 3.0;

    arena.elevation.value(3) = 9.0;
    arena.elevation.value(9) = 9.0;

    arena.elevation.value(7) = 4.0;
    arena.elevation.value(12) = 6.0;
    arena.elevation.value(13) = 5.0;

    arena.elevation.value(16) = 3.0;
    arena.elevation.value(20) = 1.0;

    arena.elevation.calc_gradients();

    arena_alg::track_river(start_id, sea_level, arena.elevation.value, arena.elevation.grad_x_norm,
                           arena.elevation.grad_y_norm, arena.elevation.grad_norm, arena.rivers, arena.lakes,
                           arena.trapped);

    CHECK(arena.rivers(4));
    CHECK(!arena.lakes(4));
    CHECK(!arena.trapped(4));

    CHECK(!arena.rivers(8));
    CHECK(arena.lakes(8));
    CHECK(!arena.trapped(8));

    CHECK(!arena.rivers(12));
    CHECK(arena.lakes(12));
    CHECK(!arena.trapped(12));

    CHECK(!arena.rivers(16));
    CHECK(arena.lakes(16));
    CHECK(!arena.trapped(16));

    CHECK(arena.rivers(20));
    CHECK(!arena.lakes(20));
    CHECK(!arena.trapped(20));
}