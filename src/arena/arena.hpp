#pragma once

#include "field.hpp"

namespace wg {
struct Value {
    Field<double> value;
    Field<double> grad_x;
    Field<double> grad_y;
    Field<double> grad_xx;
    Field<double> grad_xy;
    Field<double> grad_yy;
    Field<double> grad_norm;
};

struct Arena {
    Value elevation;
    Value rho;
};
}  // namespace wg
