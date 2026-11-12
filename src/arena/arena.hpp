#pragma once

#include "data/perlin_noise.hpp"
#include "field.hpp"
#include "field_alg.hpp"

namespace wg {
struct Value {
    void resize(int x, int y) {
        value.resize(x, y);
        grad_x.resize(x, y);
        grad_y.resize(x, y);
        grad_norm.resize(x, y);
    }

    void calc_gradients() {
        grad_x = field_alg::calc_grad_x(value);
        grad_y = field_alg::calc_grad_y(value);

        for (int i = 0; i < grad_x.size; i++) {
            grad_norm(i) = sqrt(pow(grad_x(i), 2) + pow(grad_y(i), 2));
        }
    }

    Field<double> value;
    Field<double> grad_x;
    Field<double> grad_y;
    Field<double> grad_norm;
};

struct Arena {
    Value elevation;
    Field<bool> rivers;
    Field<bool> lakes;
    int size_x = 0, size_y = 0, size = 0;

    void resize(int x, int y);
    void add_noise(int seed, double frequency);
};
}  // namespace wg
