#pragma once

#include <algorithm>
#include <cmath>
#include <numeric>
#include <vector>

#include "field.hpp"

namespace wg {

std::vector<int> get_neighbour_ids(const int id, const int size_x, const int size_y);
std::vector<double> calc_gradients(int id, const std::vector<int>& neighbour_ids, const Field& field);
std::vector<int> get_lake_boundary_ids(const std::vector<int>& lake_ids, const Field& field);
void track_river(int idx, double sea_level, Field& field);
void fill_lake(int idx, double sea_level, Field& field);

}  // namespace wg
