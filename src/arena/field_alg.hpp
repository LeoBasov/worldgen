#pragma once

#include "field.hpp"

namespace wg {
namespace field_alg {

Field calc_grad_x(const Field& field);
Field calc_grad_y(const Field& field);
double calc_grad(const int id0, const int id1, const int id2, const Field& field);

}  // namespace field_alg
}  // namespace wg
