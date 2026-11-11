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