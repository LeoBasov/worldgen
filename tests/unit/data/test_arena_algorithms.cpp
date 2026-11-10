#include <doctest/doctest.h>

#include "data/arena_algorithms.hpp"

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

    wg::Field field;
    const int size_x = 3;
    const int size_y = 3;
    const int id = 4;

    field.resize(size_x, size_y);

    field.elements.at(0).height = 1.0;
    field.elements.at(1).height = 2.0;
    field.elements.at(2).height = 3.0;

    field.elements.at(3).height = 4.0;
    field.elements.at(4).height = 0.0;  // id centered field
    field.elements.at(5).height = 6.0;

    field.elements.at(6).height = 7.0;
    field.elements.at(7).height = 8.0;
    field.elements.at(8).height = 9.0;

    const std::vector<int> neighbour_ids = field.get_neighbourhood(id, 1, wg::Field::NeighbourhoodType::MOORE);
    const std::vector<double> gradients = wg::calc_gradients(id, neighbour_ids, field);

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

    wg::Field field;
    const int size_x = 5;
    const int size_y = 5;
    const int start_id = 4;
    const double sea_level = 2.0;

    field.resize(size_x, size_y);

    for (auto& element : field.elements) {
        element.height = 10.0;
    }

    field.elements.at(8).height = 3.0;

    field.elements.at(3).height = 9.0;
    field.elements.at(9).height = 9.0;

    field.elements.at(7).height = 4.0;
    field.elements.at(12).height = 6.0;
    field.elements.at(13).height = 5.0;

    field.elements.at(16).height = 3.0;
    field.elements.at(20).height = 1.0;

    field.calc_gradients(wg::Field::Property::HEIGHT);
    track_river(start_id, sea_level, field);

    CHECK(field.elements.at(4).is_river);
    CHECK(!field.elements.at(4).is_lake);
    CHECK(!field.elements.at(4).is_trapped);

    CHECK(!field.elements.at(8).is_river);
    CHECK(field.elements.at(8).is_lake);
    CHECK(!field.elements.at(8).is_trapped);

    CHECK(!field.elements.at(12).is_river);
    CHECK(field.elements.at(12).is_lake);
    CHECK(!field.elements.at(12).is_trapped);

    CHECK(!field.elements.at(16).is_river);
    CHECK(field.elements.at(16).is_lake);
    CHECK(!field.elements.at(16).is_trapped);

    CHECK(field.elements.at(20).is_river);
    CHECK(!field.elements.at(20).is_lake);
    CHECK(!field.elements.at(20).is_trapped);
}