#pragma once

#include "arena/field.hpp"

namespace wg {
namespace arena_alg {

void track_river(int idx, double sea_level, Field<double>& elevation, Field<double>& grad_x_norm,
                 Field<double>& grad_y_norm, Field<double>& grad_norm, Field<bool>& rivers, Field<bool>& lakes);

}  // namespace arena_alg
}  // namespace wg
