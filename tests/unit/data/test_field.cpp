#include <doctest/doctest.h>

#include "data/field.hpp"

TEST_CASE("resize") {
    wg::Field field;
    const int x = 12;
    const int y = 24;

    field.resize(x, y);

    CHECK(field.size_x == x);
    CHECK(field.size_y == y);
    CHECK(field.elements.size() == x * y);
}

TEST_CASE("get_element_id") {
    wg::Field field;
    const int x = 3;
    const int y = 4;

    field.resize(x, y);

    // center elements
    CHECK(field.get_element_id(0, 0) == 0);
    CHECK(field.get_element_id(1, 0) == 1);
    CHECK(field.get_element_id(2, 0) == 2);

    CHECK(field.get_element_id(0, 1) == 3);
    CHECK(field.get_element_id(1, 1) == 4);
    CHECK(field.get_element_id(2, 1) == 5);

    CHECK(field.get_element_id(0, 2) == 6);
    CHECK(field.get_element_id(1, 2) == 7);
    CHECK(field.get_element_id(2, 2) == 8);

    CHECK(field.get_element_id(0, 3) == 9);
    CHECK(field.get_element_id(1, 3) == 10);
    CHECK(field.get_element_id(2, 3) == 11);

    // outer elements
    CHECK(field.get_element_id(-1, 0) == -1);
    CHECK(field.get_element_id(0, -1) == -1);
    CHECK(field.get_element_id(-1, -1) == -1);

    CHECK(field.get_element_id(3, 0) == -1);
    CHECK(field.get_element_id(0, 4) == -1);
    CHECK(field.get_element_id(3, 4) == -1);
}

TEST_CASE("get_element") {
    wg::Field field;
    const int x = 3;
    const int y = 7;
    int counter = 0;

    field.resize(x, y);

    for (int ix = 0; ix < x; ix++) {
        for (int iy = 0; iy < y; iy++) {
            field.get_element(1, 3).elevation = counter++;
        }
    }

    CHECK(field.get_element(1, 3).elevation == field.elements.at(x * 3 + 1).elevation);
    CHECK(field.get_element(2, 6).elevation == field.elements.at(x * 6 + 2).elevation);
}