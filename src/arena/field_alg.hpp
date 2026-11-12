#pragma once

#include "field.hpp"

namespace wg {
namespace field_alg {

template <typename T>
T calc_grad(const int id0, const int id1, const int id2, const Field<T>& field);

template <typename T>
Field<T> calc_grad_x(const Field<T>& field);

template <typename T>
Field<T> calc_grad_y(const Field<T>& field);

}  // namespace field_alg
}  // namespace wg
