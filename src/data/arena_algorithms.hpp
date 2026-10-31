#pragma once

#include <cmath>
#include <vector>

#include "field.hpp"

namespace wg {

std::vector<int> get_neighbour_ids(const int id, const int size_x, const int size_y);
std::vector<double> calc_gradients(int id, std::vector<int> neighbour_ids, std::vector<Element> elements);

}  // namespace wg
