#include "field_alg.hpp"

namespace wg {
namespace field_alg {

Field calc_grad_x(const Field& field) {
    Field grad;

    grad.resize(field.size_x, field.size_y);

    for (int y = 0; y < field.size_y; y++) {
        for (int x = 0; x < field.size_x; x++) {
            const int id0 = field.elem_id(x - 1, y);
            const int id1 = field.elem_id(x, y);
            const int id2 = field.elem_id(x + 1, y);

            grad(x, y) = calc_grad(id0, id1, id2, field);
        }
    }

    return grad;
}

Field calc_grad_y(const Field& field) {
    Field grad;

    grad.resize(field.size_x, field.size_y);

    for (int y = 0; y < field.size_y; y++) {
        for (int x = 0; x < field.size_x; x++) {
            const int id0 = field.elem_id(x, y - 1);
            const int id1 = field.elem_id(x, y);
            const int id2 = field.elem_id(x, y + 2);

            grad(x, y) = calc_grad(id0, id1, id2, field);
        }
    }

    return grad;
}

double calc_grad(const int id0, const int id1, const int id2, const Field& field) {
    if (id0 == -1) {
        return field(id2) - field(id1);
    } else if (id2 == -1) {
        return field(id1) - field(id0);
    } else {
        return (field(id2) - field(id0)) * 0.5;
    }
}

}  // namespace field_alg
}  // namespace wg