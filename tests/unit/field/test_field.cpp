#include <doctest/doctest.h>

#include "field/field.hpp"

using namespace wg;

TEST_CASE("resize") {
    Field field;
    const int x = 7;
    const int y = 13;

    field.resize(x, y);

    CHECK(field.size_x == x);
    CHECK(field.size_y == y);
    CHECK(field.size == x * y);
    CHECK(field.elements.size() == x * y);
}

TEST_CASE("coords") {
    Field field;
    const int x = 3;
    const int y = 4;

    field.resize(x, y);

    // center elements
    CHECK(field.coords(0).first == 0);
    CHECK(field.coords(0).second == 0);
    CHECK(field.coords(1).first == 1);
    CHECK(field.coords(1).second == 0);
    CHECK(field.coords(2).first == 2);
    CHECK(field.coords(2).second == 0);

    CHECK(field.coords(3).first == 0);
    CHECK(field.coords(3).second == 1);
    CHECK(field.coords(4).first == 1);
    CHECK(field.coords(4).second == 1);
    CHECK(field.coords(5).first == 2);
    CHECK(field.coords(5).second == 1);

    CHECK(field.coords(6).first == 0);
    CHECK(field.coords(6).second == 2);
    CHECK(field.coords(7).first == 1);
    CHECK(field.coords(7).second == 2);
    CHECK(field.coords(8).first == 2);
    CHECK(field.coords(8).second == 2);

    CHECK(field.coords(9).first == 0);
    CHECK(field.coords(9).second == 3);
    CHECK(field.coords(10).first == 1);
    CHECK(field.coords(10).second == 3);
    CHECK(field.coords(11).first == 2);
    CHECK(field.coords(11).second == 3);
}

TEST_CASE("elem_id") {
    Field field;
    const int x = 3;
    const int y = 4;

    field.resize(x, y);

    // center elements
    CHECK(field.elem_id(0, 0) == 0);
    CHECK(field.elem_id(1, 0) == 1);
    CHECK(field.elem_id(2, 0) == 2);

    CHECK(field.elem_id(0, 1) == 3);
    CHECK(field.elem_id(1, 1) == 4);
    CHECK(field.elem_id(2, 1) == 5);

    CHECK(field.elem_id(0, 2) == 6);
    CHECK(field.elem_id(1, 2) == 7);
    CHECK(field.elem_id(2, 2) == 8);

    CHECK(field.elem_id(0, 3) == 9);
    CHECK(field.elem_id(1, 3) == 10);
    CHECK(field.elem_id(2, 3) == 11);

    CHECK(field.elem_id(-1, 0) == -1);
    CHECK(field.elem_id(3, 0) == -1);
    CHECK(field.elem_id(0, -1) == -1);
    CHECK(field.elem_id(0, 4) == -1);

    CHECK(field.elem_id(-1, -1) == -1);
    CHECK(field.elem_id(3, 4) == -1);
    CHECK(field.elem_id(-1, 4) == -1);
    CHECK(field.elem_id(-3, -1) == -1);
}

TEST_CASE("operator(x,y)") {
    Field field;
    const int x = 3;
    const int y = 4;

    field.resize(x, y);

    for (int iy = 0; iy < y; iy++) {
        for (int ix = 0; ix < x; ix++) {
            const int i = ix + iy * field.size_x;

            field(ix, iy) = i;
        }
    }

    for (int i = 0; i < field.size; i++) {
        CHECK(field.elements.at(i) == i);
    }
}

TEST_CASE("neighbourhood_moore") {
    Field field;
    const int x = 5;
    const int y = 5;

    field.resize(x, y);

    const int id1 = field.elem_id(2, 2);
    const int id2 = field.elem_id(1, 0);
    const int neighbourhood_size1 = 1;
    const int neighbourhood_size2 = 2;
    const std::vector<int> neighbourhood_ids1 = field.neighbourhood(id1, neighbourhood_size1, Field::MOORE);
    const std::vector<int> neighbourhood_ids2 = field.neighbourhood(id2, neighbourhood_size2, Field::MOORE);

    // check size
    CHECK(neighbourhood_ids1.size() == 9);
    CHECK(neighbourhood_ids2.size() == 25);

    // check ids of set 1
    CHECK(neighbourhood_ids1.at(0) == 6);
    CHECK(neighbourhood_ids1.at(1) == 7);
    CHECK(neighbourhood_ids1.at(2) == 8);

    CHECK(neighbourhood_ids1.at(3) == 11);
    CHECK(neighbourhood_ids1.at(4) == 12);
    CHECK(neighbourhood_ids1.at(5) == 13);

    CHECK(neighbourhood_ids1.at(6) == 16);
    CHECK(neighbourhood_ids1.at(7) == 17);
    CHECK(neighbourhood_ids1.at(8) == 18);

    // check ids of set 2
    CHECK(neighbourhood_ids2.at(0) == -1);
    CHECK(neighbourhood_ids2.at(1) == -1);
    CHECK(neighbourhood_ids2.at(2) == -1);
    CHECK(neighbourhood_ids2.at(3) == -1);
    CHECK(neighbourhood_ids2.at(4) == -1);

    CHECK(neighbourhood_ids2.at(5) == -1);
    CHECK(neighbourhood_ids2.at(6) == -1);
    CHECK(neighbourhood_ids2.at(7) == -1);
    CHECK(neighbourhood_ids2.at(8) == -1);
    CHECK(neighbourhood_ids2.at(9) == -1);

    CHECK(neighbourhood_ids2.at(10) == -1);
    CHECK(neighbourhood_ids2.at(11) == 0);
    CHECK(neighbourhood_ids2.at(12) == 1);
    CHECK(neighbourhood_ids2.at(13) == 2);
    CHECK(neighbourhood_ids2.at(14) == 3);

    CHECK(neighbourhood_ids2.at(15) == -1);
    CHECK(neighbourhood_ids2.at(16) == 5);
    CHECK(neighbourhood_ids2.at(17) == 6);
    CHECK(neighbourhood_ids2.at(18) == 7);
    CHECK(neighbourhood_ids2.at(19) == 8);

    CHECK(neighbourhood_ids2.at(20) == -1);
    CHECK(neighbourhood_ids2.at(21) == 10);
    CHECK(neighbourhood_ids2.at(22) == 11);
    CHECK(neighbourhood_ids2.at(23) == 12);
    CHECK(neighbourhood_ids2.at(24) == 13);
}

TEST_CASE("neighbourhood_neumann") {
    Field field;
    const int x = 5;
    const int y = 5;

    field.resize(x, y);

    const int id1 = field.elem_id(2, 2);
    const int id2 = field.elem_id(1, 0);
    const int neighbourhood_size1 = 1;
    const int neighbourhood_size2 = 2;
    const std::vector<int> neighbourhood_ids1 = field.neighbourhood(id1, neighbourhood_size1, Field::NEUMANN);
    const std::vector<int> neighbourhood_ids2 = field.neighbourhood(id2, neighbourhood_size2, Field::NEUMANN);

    // check size
    CHECK(neighbourhood_ids1.size() == 9);
    CHECK(neighbourhood_ids2.size() == 25);

    // check ids of set 1
    CHECK(neighbourhood_ids1.at(0) == -1);
    CHECK(neighbourhood_ids1.at(1) == 7);
    CHECK(neighbourhood_ids1.at(2) == -1);

    CHECK(neighbourhood_ids1.at(3) == 11);
    CHECK(neighbourhood_ids1.at(4) == 12);
    CHECK(neighbourhood_ids1.at(5) == 13);

    CHECK(neighbourhood_ids1.at(6) == -1);
    CHECK(neighbourhood_ids1.at(7) == 17);
    CHECK(neighbourhood_ids1.at(8) == -1);

    // check ids of set 2
    CHECK(neighbourhood_ids2.at(0) == -1);
    CHECK(neighbourhood_ids2.at(1) == -1);
    CHECK(neighbourhood_ids2.at(2) == -1);
    CHECK(neighbourhood_ids2.at(3) == -1);
    CHECK(neighbourhood_ids2.at(4) == -1);

    CHECK(neighbourhood_ids2.at(5) == -1);
    CHECK(neighbourhood_ids2.at(6) == -1);
    CHECK(neighbourhood_ids2.at(7) == -1);
    CHECK(neighbourhood_ids2.at(8) == -1);
    CHECK(neighbourhood_ids2.at(9) == -1);

    CHECK(neighbourhood_ids2.at(10) == -1);
    CHECK(neighbourhood_ids2.at(11) == 0);
    CHECK(neighbourhood_ids2.at(12) == 1);
    CHECK(neighbourhood_ids2.at(13) == 2);
    CHECK(neighbourhood_ids2.at(14) == 3);

    CHECK(neighbourhood_ids2.at(15) == -1);
    CHECK(neighbourhood_ids2.at(16) == 5);
    CHECK(neighbourhood_ids2.at(17) == 6);
    CHECK(neighbourhood_ids2.at(18) == 7);
    CHECK(neighbourhood_ids2.at(19) == -1);

    CHECK(neighbourhood_ids2.at(20) == -1);
    CHECK(neighbourhood_ids2.at(21) == -1);
    CHECK(neighbourhood_ids2.at(22) == 11);
    CHECK(neighbourhood_ids2.at(23) == -1);
    CHECK(neighbourhood_ids2.at(24) == -1);
}