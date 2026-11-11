#pragma once

#include <algorithm>
#include <cmath>
#include <functional>
#include <numeric>
#include <queue>
#include <unordered_set>
#include <vector>

#include "old_field.hpp"

namespace wg {

std::vector<double> calc_gradients(int id, const std::vector<int>& neighbour_ids, const OldField& field);
void track_river(int idx, double sea_level, OldField& field);
void fill_lake(int idx, double sea_level, OldField& field);

}  // namespace wg
