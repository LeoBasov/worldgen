#pragma once

#include "../arena/arena.hpp"
#include "../arena/value.hpp"

namespace wg {

struct Solver {
    void solve(const Arena& arena, const double sea_level);

    Value rho;
};

}  // namespace wg
