#pragma once

#include <algorithm>
#include <cmath>
#include <numeric>
#include <vector>

#include "field.hpp"

namespace wg {

std::vector<int> get_neighbour_ids(const int id, const int size_x, const int size_y);
std::vector<double> calc_gradients(int id, const std::vector<int>& neighbour_ids, const std::vector<Element>& elements);
std::vector<int> get_lake_boundary_ids(const std::vector<int>& lake_ids, const std::vector<Element>& elements,
                                       const int size_x, const int size_y);
void track_river(int idx, double sea_level, const int size_x, const int size_y, std::vector<Element>& elements);
void fill_lake(int idx, double sea_level, const int size_x, const int size_y, std::vector<Element>& elements);
std::vector<double> calc_2dgradients(const int size_x, const int size_y, const std::vector<Element>& elements);

}  // namespace wg
