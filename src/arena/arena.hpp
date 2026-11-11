#pragma once

#include "field.hpp"

namespace wg {
struct Value {
    Field value;
    Field grad_x;
    Field grad_y;
    Field grad_xx;
    Field grad_xy;
    Field grad_yy;
    Field grad_norm;
};

struct Arena {
    Value elevation;
    Value rho;
};
}  // namespace wg
