#pragma once

#include <algorithm>
#include <numeric>
#include <queue>
#include <unordered_set>

#include "arena/field.hpp"

namespace wg {
namespace arena_alg {

void track_river(int idx, double sea_level, Field<double>& elevation, Field<double>& grad_x_norm,
                 Field<double>& grad_y_norm, Field<double>& grad_norm, Field<bool>& rivers, Field<bool>& lakes,
                 Field<bool>& trapped);
void fill_lake(int idx, double sea_level, Field<double>& elevation, Field<double>& grad_x_norm,
               Field<double>& grad_y_norm, Field<double>& grad_norm, Field<bool>& rivers, Field<bool>& lakes,
               Field<bool>& trapped);
std::vector<double> calc_gradients(int id, const std::vector<int>& neighbour_ids, const Field<double>& elevation);

}  // namespace arena_alg
}  // namespace wg
