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

    field.elements.at(0).elevation = 1.0;
    field.elements.at(1).elevation = 2.0;
    field.elements.at(2).elevation = 3.0;

    field.elements.at(3).elevation = 4.0;
    field.elements.at(4).elevation = 0.0;  // id centered field
    field.elements.at(5).elevation = 6.0;

    field.elements.at(6).elevation = 7.0;
    field.elements.at(7).elevation = 8.0;
    field.elements.at(8).elevation = 9.0;

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

TEST_CASE("get_lake_boundary") {
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
    const std::vector<int> lake_ids = {0, 4, 5};

    field.resize(size_x, size_y);

    field.elements.at(0).is_lake = true;
    field.elements.at(4).is_lake = true;
    field.elements.at(5).is_lake = true;

    std::vector<int> lake_boundary_ids = wg::get_lake_boundary_ids(lake_ids, field);

    CHECK(lake_boundary_ids.size() == 6);

    bool not_found_id0 =
        std::find(lake_boundary_ids.begin(), lake_boundary_ids.end(), lake_ids.at(0)) == lake_boundary_ids.end();
    bool not_found_id4 =
        std::find(lake_boundary_ids.begin(), lake_boundary_ids.end(), lake_ids.at(1)) == lake_boundary_ids.end();
    bool not_found_id5 =
        std::find(lake_boundary_ids.begin(), lake_boundary_ids.end(), lake_ids.at(2)) == lake_boundary_ids.end();

    CHECK(not_found_id0);
    CHECK(not_found_id4);
    CHECK(not_found_id5);

    bool found_id1 = std::find(lake_boundary_ids.begin(), lake_boundary_ids.end(), 1) != lake_boundary_ids.end();
    bool found_id2 = std::find(lake_boundary_ids.begin(), lake_boundary_ids.end(), 2) != lake_boundary_ids.end();
    bool found_id3 = std::find(lake_boundary_ids.begin(), lake_boundary_ids.end(), 3) != lake_boundary_ids.end();
    bool found_id6 = std::find(lake_boundary_ids.begin(), lake_boundary_ids.end(), 6) != lake_boundary_ids.end();
    bool found_id7 = std::find(lake_boundary_ids.begin(), lake_boundary_ids.end(), 7) != lake_boundary_ids.end();
    bool found_id8 = std::find(lake_boundary_ids.begin(), lake_boundary_ids.end(), 8) != lake_boundary_ids.end();

    CHECK(found_id1);
    CHECK(found_id2);
    CHECK(found_id3);
    CHECK(found_id6);
    CHECK(found_id7);
    CHECK(found_id8);
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
        element.elevation = 10.0;
    }

    field.elements.at(8).elevation = 3.0;

    field.elements.at(3).elevation = 9.0;
    field.elements.at(9).elevation = 9.0;

    field.elements.at(7).elevation = 4.0;
    field.elements.at(12).elevation = 6.0;
    field.elements.at(13).elevation = 5.0;

    field.elements.at(16).elevation = 3.0;
    field.elements.at(20).elevation = 1.0;

    field.calc_gradients(wg::Field::Property::ELEVATION);
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