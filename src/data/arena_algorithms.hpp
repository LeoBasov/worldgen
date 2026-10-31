#pragma once

#include <algorithm>
#include <cmath>
#include <numeric>
#include <vector>

#include "field.hpp"

namespace wg {

std::vector<int> get_neighbour_ids(const int id, const int size_x, const int size_y);
std::vector<double> calc_gradients(int id, std::vector<int> neighbour_ids, std::vector<Element> elements);
std::vector<int> get_lake_boundary_ids(std::vector<int> lake_ids, std::vector<Element> elements, const int size_x,
                                       const int size_y);
void track_river(int idx, double sea_level, const int size_x, const int size_y, std::vector<Element>& elements);
void fill_lake(int idx, double sea_level, const int size_x, const int size_y, std::vector<Element>& elements);

}  // namespace wg
