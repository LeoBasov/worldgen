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

TEST_CASE("operator(id)") {
    Field field;
    const Field::Type type = Field::Type::ELEVATION;
    const int x = 3;
    const int y = 4;

    field.resize(x, y);

    for (int i = 0; i < field.size; i++) {
        field(i, Field::ELEVATION, Field::VALUE) = i;
        field(i, Field::ELEVATION, Field::GRAD_X) = i + 1;
        field(i, Field::ELEVATION, Field::GRAD_Y) = i + 2;
        field(i, Field::ELEVATION, Field::GRAD_XX) = i + 3;
        field(i, Field::ELEVATION, Field::GRAD_XY) = i + 4;
        field(i, Field::ELEVATION, Field::GRAD_YY) = i + 5;
        field(i, Field::ELEVATION, Field::GRAD_NORM) = i + 6;

        field(i, Field::RHO, Field::VALUE) = i + 7;
        field(i, Field::RHO, Field::GRAD_X) = i + 8;
        field(i, Field::RHO, Field::GRAD_Y) = i + 9;
        field(i, Field::RHO, Field::GRAD_XX) = i + 10;
        field(i, Field::RHO, Field::GRAD_XY) = i + 11;
        field(i, Field::RHO, Field::GRAD_YY) = i + 12;
        field(i, Field::RHO, Field::GRAD_NORM) = i + 13;
    }

    for (int i = 0; i < field.size; i++) {
        CHECK(field.elements.at(i).vals.at(0) == i);
        CHECK(field.elements.at(i).grad_x.at(0) == i + 1);
        CHECK(field.elements.at(i).grad_y.at(0) == i + 2);
        CHECK(field.elements.at(i).grad_xx.at(0) == i + 3);
        CHECK(field.elements.at(i).grad_xy.at(0) == i + 4);
        CHECK(field.elements.at(i).grad_yy.at(0) == i + 5);
        CHECK(field.elements.at(i).grad_norm.at(0) == i + 6);

        CHECK(field.elements.at(i).vals.at(1) == i + 7);
        CHECK(field.elements.at(i).grad_x.at(1) == i + 8);
        CHECK(field.elements.at(i).grad_y.at(1) == i + 9);
        CHECK(field.elements.at(i).grad_xx.at(1) == i + 10);
        CHECK(field.elements.at(i).grad_xy.at(1) == i + 11);
        CHECK(field.elements.at(i).grad_yy.at(1) == i + 12);
        CHECK(field.elements.at(i).grad_norm.at(1) == i + 13);
    }
}

TEST_CASE("operator(x,y)") {
    Field field;
    const Field::Type type = Field::Type::ELEVATION;
    const int x = 3;
    const int y = 4;

    field.resize(x, y);

    for (int iy = 0; iy < y; iy++) {
        for (int ix = 0; ix < x; ix++) {
            const int i = ix + iy * field.size_x;

            field(ix, iy, Field::ELEVATION, Field::VALUE) = i;
            field(ix, iy, Field::ELEVATION, Field::GRAD_X) = i + 1;
            field(ix, iy, Field::ELEVATION, Field::GRAD_Y) = i + 2;
            field(ix, iy, Field::ELEVATION, Field::GRAD_XX) = i + 3;
            field(ix, iy, Field::ELEVATION, Field::GRAD_XY) = i + 4;
            field(ix, iy, Field::ELEVATION, Field::GRAD_YY) = i + 5;
            field(ix, iy, Field::ELEVATION, Field::GRAD_NORM) = i + 6;

            field(ix, iy, Field::RHO, Field::VALUE) = i + 7;
            field(ix, iy, Field::RHO, Field::GRAD_X) = i + 8;
            field(ix, iy, Field::RHO, Field::GRAD_Y) = i + 9;
            field(ix, iy, Field::RHO, Field::GRAD_XX) = i + 10;
            field(ix, iy, Field::RHO, Field::GRAD_XY) = i + 11;
            field(ix, iy, Field::RHO, Field::GRAD_YY) = i + 12;
            field(ix, iy, Field::RHO, Field::GRAD_NORM) = i + 13;
        }
    }

    for (int i = 0; i < field.size; i++) {
        CHECK(field.elements.at(i).vals.at(0) == i);
        CHECK(field.elements.at(i).grad_x.at(0) == i + 1);
        CHECK(field.elements.at(i).grad_y.at(0) == i + 2);
        CHECK(field.elements.at(i).grad_xx.at(0) == i + 3);
        CHECK(field.elements.at(i).grad_xy.at(0) == i + 4);
        CHECK(field.elements.at(i).grad_yy.at(0) == i + 5);
        CHECK(field.elements.at(i).grad_norm.at(0) == i + 6);

        CHECK(field.elements.at(i).vals.at(1) == i + 7);
        CHECK(field.elements.at(i).grad_x.at(1) == i + 8);
        CHECK(field.elements.at(i).grad_y.at(1) == i + 9);
        CHECK(field.elements.at(i).grad_xx.at(1) == i + 10);
        CHECK(field.elements.at(i).grad_xy.at(1) == i + 11);
        CHECK(field.elements.at(i).grad_yy.at(1) == i + 12);
        CHECK(field.elements.at(i).grad_norm.at(1) == i + 13);
    }
}