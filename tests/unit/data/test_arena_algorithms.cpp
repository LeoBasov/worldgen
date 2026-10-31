#include <doctest/doctest.h>

#include "data/arena_algorithms.hpp"

TEST_CASE("get_neighbour_ids") {
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

    const int size_x = 3;
    const int size_y = 3;

    const int id_top_left = 0;
    const int id_center = 4;
    const int id_down_right = 8;

    const std::vector<int> neighbours_topleft = wg::get_neighbour_ids(id_top_left, size_x, size_y);
    const std::vector<int> neighbours_center = wg::get_neighbour_ids(id_center, size_x, size_y);
    const std::vector<int> neighbours_down_right = wg::get_neighbour_ids(id_down_right, size_x, size_y);

    CHECK(neighbours_topleft.size() == 8);
    CHECK(neighbours_center.size() == 8);
    CHECK(neighbours_down_right.size() == 8);

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