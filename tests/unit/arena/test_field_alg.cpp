#include <doctest/doctest.h>

#include "arena/field_alg.hpp"

using namespace wg;
using namespace wg::field_alg;

TEST_CASE("calc_grad") {
    Field<double> field;
    const int size_x = 4;
    const int size_y = 3;

    field.resize(size_x, size_y);

    field(0, 0) = 1.0;
    field(1, 0) = 2.0;
    field(2, 0) = 3.0;
    field(3, 0) = 4.0;

    field(0, 1) = 1.0 + 1.0;
    field(1, 1) = 2.0 + 1.0;
    field(2, 1) = 3.0 + 1.0;
    field(3, 1) = 4.0 + 1.0;

    field(0, 2) = 1.0 + 4.0;
    field(1, 2) = 2.0 + 4.0;
    field(2, 2) = 3.0 + 4.0;
    field(3, 2) = 4.0 + 4.0;

    Field<double> grad_x = calc_grad_x<double>(field);

    CHECK(grad_x(0, 0) == 1.0);
    CHECK(grad_x(1, 0) == 1.0);
    CHECK(grad_x(2, 0) == 1.0);
    CHECK(grad_x(3, 0) == 1.0);

    CHECK(grad_x(0, 1) == 1.0);
    CHECK(grad_x(1, 1) == 1.0);
    CHECK(grad_x(2, 1) == 1.0);
    CHECK(grad_x(3, 1) == 1.0);

    CHECK(grad_x(0, 2) == 1.0);
    CHECK(grad_x(1, 2) == 1.0);
    CHECK(grad_x(2, 2) == 1.0);
    CHECK(grad_x(3, 2) == 1.0);

    Field<double> grad_y = calc_grad_y<double>(field);

    CHECK(grad_y(0, 0) == 1.0);
    CHECK(grad_y(1, 0) == 1.0);
    CHECK(grad_y(2, 0) == 1.0);
    CHECK(grad_y(3, 0) == 1.0);

    CHECK(grad_y(0, 1) == 1.0);
    CHECK(grad_y(1, 1) == 1.0);
    CHECK(grad_y(2, 1) == 1.0);
    CHECK(grad_y(3, 1) == 1.0);

    CHECK(grad_y(0, 2) == 3.0);
    CHECK(grad_y(1, 2) == 3.0);
    CHECK(grad_y(2, 2) == 3.0);
    CHECK(grad_y(3, 2) == 3.0);
}