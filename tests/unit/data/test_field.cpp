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

TEST_CASE("get_id_coordinates") {
    wg::Field field;
    const int x = 3;
    const int y = 4;

    field.resize(x, y);

    // center elements
    CHECK(field.get_id_coordinates(0).first == 0);
    CHECK(field.get_id_coordinates(0).second == 0);
    CHECK(field.get_id_coordinates(1).first == 1);
    CHECK(field.get_id_coordinates(1).second == 0);
    CHECK(field.get_id_coordinates(2).first == 2);
    CHECK(field.get_id_coordinates(2).second == 0);

    CHECK(field.get_id_coordinates(3).first == 0);
    CHECK(field.get_id_coordinates(3).second == 1);
    CHECK(field.get_id_coordinates(4).first == 1);
    CHECK(field.get_id_coordinates(4).second == 1);
    CHECK(field.get_id_coordinates(5).first == 2);
    CHECK(field.get_id_coordinates(5).second == 1);

    CHECK(field.get_id_coordinates(6).first == 0);
    CHECK(field.get_id_coordinates(6).second == 2);
    CHECK(field.get_id_coordinates(7).first == 1);
    CHECK(field.get_id_coordinates(7).second == 2);
    CHECK(field.get_id_coordinates(8).first == 2);
    CHECK(field.get_id_coordinates(8).second == 2);

    CHECK(field.get_id_coordinates(9).first == 0);
    CHECK(field.get_id_coordinates(9).second == 3);
    CHECK(field.get_id_coordinates(10).first == 1);
    CHECK(field.get_id_coordinates(10).second == 3);
    CHECK(field.get_id_coordinates(11).first == 2);
    CHECK(field.get_id_coordinates(11).second == 3);
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