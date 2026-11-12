#include "arena_alg.hpp"

namespace wg {
namespace arena_alg {

void track_river(int idx, double sea_level, Field<double>& elevation, Field<double>& grad_x_norm,
                 Field<double>& grad_y_norm, Field<double>& grad_norm, Field<bool>& rivers, Field<bool>& lakes) {
    rivers(idx) = true;
    int idx_old = -1;

    while (true) {
        const int dx = std::round(grad_x_norm(idx));
        const int dy = std::round(grad_y_norm(idx));
        const auto [x, y] = elevation.coords(idx);
        const int idx_new = elevation.elem_id(x - dx, y - dy);

        if (idx_new == -1) {
            break;
        } else if (idx_new == idx_old) {
            rivers(idx) = false;
            lakes(idx) = true;
            break;
        } else if (lakes(idx_new) == true || rivers(idx_new) == true || elevation(idx_new) <= sea_level) {
            break;
        } else if (grad_norm(idx_new) < 1e-3) {
            rivers(idx) = false;
            lakes(idx) = true;
            break;
        } else {
            idx_old = idx;
            idx = idx_new;
            rivers(idx) = true;
        }
    }

    if (lakes(idx)) {
        // fill_lake(idx, sea_level, field);
    }
}

}  // namespace arena_alg
}  // namespace wg