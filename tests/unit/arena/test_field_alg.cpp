#include <doctest/doctest.h>

#include "arena/field_alg.hpp"

using namespace wg;
using namespace wg::field_alg;

TEST_CASE("calc_grad") {
    Field<double> fieldx1;
    Field<double> fieldx2;
    const int size_x = 11;
    const int size_y = 11;

    fieldx1.resize(size_x, size_y);
    fieldx2.resize(size_x, size_y);

    for (int y = 0; y < size_y; y++) {
        for (int x = 0; x < size_x; x++) {
            fieldx1(x, y) = 3.0 * (x - 5) + 7.0 * (y - 5);
            fieldx2(x, y) = 7.0 * pow(x - 5, 2) + 11.0 * pow(y - 5, 2);
        }
    }

    Field<double> grad_fieldx1_x = calc_grad_x(fieldx1);
    Field<double> grad_fieldx2_x = calc_grad_x(fieldx2);

    Field<double> grad_fieldx1_y = calc_grad_y(fieldx1);
    Field<double> grad_fieldx2_y = calc_grad_y(fieldx2);

    for (int y = 1; y < size_y - 1; y++) {
        for (int x = 1; x < size_x - 1; x++) {
            CHECK(grad_fieldx1_x(x, y) == 3.0);
            CHECK(grad_fieldx1_y(x, y) == 7.0);

            CHECK(grad_fieldx2_x(x, y) == 14.0 * (x - 5));
            CHECK(grad_fieldx2_y(x, y) == 22.0 * (y - 5));
        }
    }

    CHECK(grad_fieldx1_x(0, 5) == 3.0);
    CHECK(grad_fieldx2_x(0, 5) == -63.0);

    CHECK(grad_fieldx1_x(10, 5) == 3.0);
    CHECK(grad_fieldx2_x(10, 5) == 63.0);

    CHECK(grad_fieldx1_y(5, 0) == 7.0);
    CHECK(grad_fieldx2_y(5, 0) == -99.0);

    CHECK(grad_fieldx1_y(5, 10) == 7.0);
    CHECK(grad_fieldx2_y(5, 10) == 99.0);
}