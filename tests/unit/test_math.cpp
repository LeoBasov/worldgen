#include <doctest/doctest.h>
#include "math.hpp"

TEST_CASE("add") {
    CHECK(add(2, 3) == 5);
}