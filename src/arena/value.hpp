#pragma once

#include "field.hpp"
#include "field_alg.hpp"

namespace wg {

struct Value {
    void resize(int x, int y) {
        value.resize(x, y);
        grad_x.resize(x, y);
        grad_y.resize(x, y);
        grad_x_norm.resize(x, y);
        grad_y_norm.resize(x, y);
        grad_norm.resize(x, y);
    }

    void calc_gradients() {
        grad_x = field_alg::calc_grad_x(value);
        grad_y = field_alg::calc_grad_y(value);

        for (int i = 0; i < grad_x.size; i++) {
            grad_norm(i) = sqrt(pow(grad_x(i), 2) + pow(grad_y(i), 2));
            grad_x_norm(i) = grad_x(i) / grad_norm(i);
            grad_y_norm(i) = grad_y(i) / grad_norm(i);
        }
    }

    Field<double> value;
    Field<double> grad_x;
    Field<double> grad_y;
    Field<double> grad_x_norm;
    Field<double> grad_y_norm;
    Field<double> grad_norm;
};

}  // namespace wg
