#include <doctest/doctest.h>

#include "data/arena_algorithms.hpp"

TEST_CASE("get_neighbour_ids") {
    /*
    +--→ x
    |
    ↓
    y
    */

    /* ids
    0---1---2
    |   |   |
    3---4---5
    |   |   |
    6---7---8
    */

    /* neighbourhood
    3---2---1
    |   |   |
    4--idx--0
    |   |   |
    5---6---7
    */

    const int size_x = 3;
    const int size_y = 3;

    const int id_top_left = 0;
    const int id_center = 4;
    const int id_down_right = 8;
    const int id_right_top = 2;

    const std::vector<int> neighbours_topleft = wg::get_neighbour_ids(id_top_left, size_x, size_y);
    const std::vector<int> neighbours_center = wg::get_neighbour_ids(id_center, size_x, size_y);
    const std::vector<int> neighbours_down_right = wg::get_neighbour_ids(id_down_right, size_x, size_y);
    const std::vector<int> neighbours_right_top = wg::get_neighbour_ids(id_right_top, size_x, size_y);

    CHECK(neighbours_topleft.size() == 8);
    CHECK(neighbours_center.size() == 8);
    CHECK(neighbours_down_right.size() == 8);
    CHECK(neighbours_right_top.size() == 8);

    CHECK(neighbours_topleft.at(0) == 1);            // right
    CHECK(neighbours_topleft.at(1) == id_top_left);  // right up
    CHECK(neighbours_topleft.at(2) == id_top_left);  // up
    CHECK(neighbours_topleft.at(3) == id_top_left);  // up left
    CHECK(neighbours_topleft.at(4) == id_top_left);  // left
    CHECK(neighbours_topleft.at(5) == id_top_left);  // left down
    CHECK(neighbours_topleft.at(6) == 3);            // down
    CHECK(neighbours_topleft.at(7) == 4);            // down right

    CHECK(neighbours_center.at(0) == 5);  // right
    CHECK(neighbours_center.at(1) == 2);  // right up
    CHECK(neighbours_center.at(2) == 1);  // up
    CHECK(neighbours_center.at(3) == 0);  // up left
    CHECK(neighbours_center.at(4) == 3);  // left
    CHECK(neighbours_center.at(5) == 6);  // left down
    CHECK(neighbours_center.at(6) == 7);  // down
    CHECK(neighbours_center.at(7) == 8);  // down right

    CHECK(neighbours_down_right.at(0) == id_down_right);  // right
    CHECK(neighbours_down_right.at(1) == id_down_right);  // right up
    CHECK(neighbours_down_right.at(2) == 5);              // up
    CHECK(neighbours_down_right.at(3) == 4);              // up left
    CHECK(neighbours_down_right.at(4) == 7);              // left
    CHECK(neighbours_down_right.at(5) == id_down_right);  // left down
    CHECK(neighbours_down_right.at(6) == id_down_right);  // down
    CHECK(neighbours_down_right.at(7) == id_down_right);  // down right

    CHECK(neighbours_right_top.at(0) == id_right_top);  // right
    CHECK(neighbours_right_top.at(1) == id_right_top);  // right up
    CHECK(neighbours_right_top.at(2) == id_right_top);  // up
    CHECK(neighbours_right_top.at(3) == id_right_top);  // up left
    CHECK(neighbours_right_top.at(4) == 1);             // left
    CHECK(neighbours_right_top.at(5) == 4);             // left down
    CHECK(neighbours_right_top.at(6) == 5);             // down
    CHECK(neighbours_right_top.at(7) == id_right_top);  // down right
}

TEST_CASE("calc_gradients") {
    /*
    +--→ x
    |
    ↓
    y
    */

    /*
    3---2---1
    |   |   |
    4--idx--0
    |   |   |
    5---6---7
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

    const std::vector<int> neighbour_ids = wg::get_neighbour_ids(id, size_x, size_y);
    const std::vector<double> gradients = wg::calc_gradients(id, neighbour_ids, field.elements);

    CHECK(gradients.size() == 8);

    CHECK(gradients.at(0) == 6.0);                   // right
    CHECK(gradients.at(1) == 3.0 / std::sqrt(2.0));  // right up
    CHECK(gradients.at(2) == 2.0);                   // up
    CHECK(gradients.at(3) == 1.0 / std::sqrt(2.0));  // up left
    CHECK(gradients.at(4) == 4.0);                   // left
    CHECK(gradients.at(5) == 7.0 / std::sqrt(2.0));  // left down
    CHECK(gradients.at(6) == 8.0);                   // down
    CHECK(gradients.at(7) == 9.0 / std::sqrt(2.0));  // down right
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

    std::vector<int> lake_boundary_ids = wg::get_lake_boundary_ids(lake_ids, field.elements, size_x, size_y);

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

    field.elements.at(7).elevation = 4.0;
    field.elements.at(12).elevation = 6.0;
    field.elements.at(13).elevation = 5.0;

    field.elements.at(16).elevation = 3.0;
    field.elements.at(20).elevation = 1.0;

    track_river(start_id, sea_level, size_x, size_y, field.elements);

    CHECK(field.elements.at(start_id).is_river);
    CHECK(!field.elements.at(start_id).is_lake);
    CHECK(!field.elements.at(start_id).is_trapped);

    CHECK(!field.elements.at(7).is_river);
    CHECK(field.elements.at(7).is_lake);
    CHECK(!field.elements.at(7).is_trapped);

    CHECK(!field.elements.at(8).is_river);
    CHECK(field.elements.at(8).is_lake);
    CHECK(!field.elements.at(8).is_trapped);

    CHECK(!field.elements.at(12).is_river);
    CHECK(field.elements.at(12).is_lake);
    CHECK(!field.elements.at(12).is_trapped);

    CHECK(!field.elements.at(13).is_river);
    CHECK(field.elements.at(13).is_lake);
    CHECK(!field.elements.at(13).is_trapped);

    CHECK(field.elements.at(16).is_river);
    CHECK(!field.elements.at(16).is_lake);
    CHECK(!field.elements.at(16).is_trapped);
}

TEST_CASE("calc_2dgradients") {
    /*
+--→ x
|
↓
y
*/

    /*
    0----1----2
    |    |    |
    3----4----5
    |    |    |
    6----7----8
    */

    wg::Field field;
    const int size_x = 3;
    const int size_y = 3;
    const int tot = size_x * size_y;

    field.resize(size_x, size_y);

    field.get_element(0, 0).elevation = 1.0;
    field.get_element(1, 0).elevation = 3.0;
    field.get_element(2, 0).elevation = 5.0;

    field.get_element(0, 1).elevation = 7.0;
    field.get_element(1, 1).elevation = 11.0;
    field.get_element(2, 1).elevation = 13.0;

    field.get_element(0, 2).elevation = 17.0;
    field.get_element(1, 2).elevation = 23.0;
    field.get_element(2, 2).elevation = 31.0;

    const std::vector<double> gradients = calc_2dgradients(size_x, size_y, field.elements);

    const double gradx_00 = gradients.at(0);
    const double grady_00 = gradients.at(tot + 0);
    const double gradx_10 = gradients.at(1);
    const double grady_10 = gradients.at(tot + 1);
    const double gradx_20 = gradients.at(2);
    const double grady_20 = gradients.at(tot + 2);

    const double gradx_01 = gradients.at(3);
    const double grady_01 = gradients.at(tot + 3);
    const double gradx_11 = gradients.at(4);
    const double grady_11 = gradients.at(tot + 4);
    const double gradx_21 = gradients.at(5);
    const double grady_21 = gradients.at(tot + 5);

    const double gradx_02 = gradients.at(6);
    const double grady_02 = gradients.at(tot + 6);
    const double gradx_12 = gradients.at(7);
    const double grady_12 = gradients.at(tot + 7);
    const double gradx_22 = gradients.at(8);
    const double grady_22 = gradients.at(tot + 8);

    CHECK(gradx_00 == 2.0);
    CHECK(grady_00 == 6.0);
    CHECK(gradx_10 == 2.0);
    CHECK(grady_10 == 8.0);
    CHECK(gradx_20 == 2.0);
    CHECK(grady_20 == 8.0);

    CHECK(gradx_01 == 4.0);
    CHECK(grady_01 == 8.0);
    CHECK(gradx_11 == 3.0);
    CHECK(grady_11 == 10.0);
    CHECK(gradx_21 == 2.0);
    CHECK(grady_21 == 13.0);

    CHECK(gradx_02 == 6.0);
    CHECK(grady_02 == 10.0);
    CHECK(gradx_12 == 7.0);
    CHECK(grady_12 == 12.0);
    CHECK(gradx_22 == 8.0);
    CHECK(grady_22 == 18.0);
}