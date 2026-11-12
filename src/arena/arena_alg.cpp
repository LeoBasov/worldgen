#include "arena_alg.hpp"

namespace wg {
namespace arena_alg {

void track_river(int idx, double sea_level, Field<double>& elevation, Field<double>& grad_x_norm,
                 Field<double>& grad_y_norm, Field<double>& grad_norm, Field<bool>& rivers, Field<bool>& lakes,
                 Field<bool>& trapped) {
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
        fill_lake(idx, sea_level, elevation, grad_x_norm, grad_y_norm, grad_norm, rivers, lakes, trapped);
    }
}

void fill_lake(int idx, double sea_level, Field<double>& elevation, Field<double>& grad_x_norm,
               Field<double>& grad_y_norm, Field<double>& grad_norm, Field<bool>& rivers, Field<bool>& lakes,
               Field<bool>& trapped) {
    // Priority flood: the lake boundary is kept incrementally in a min-heap ordered by
    // (elevation, id) instead of being rebuilt after every added cell. Nothing else
    // modifies is_lake while this lake fills, so heap entries never go stale.
    using Entry = std::pair<double, int>;
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> boundary;
    std::unordered_set<int> seen{idx};  // lake cells of this fill + cells already pushed
    std::vector<int> lake_ids{idx};

    const auto push_boundary = [&](const std::vector<int>& neighbour_ids) {
        for (auto ne_id : neighbour_ids) {
            if (ne_id >= 0 && !lakes(ne_id) && seen.insert(ne_id).second) {
                boundary.emplace(elevation(ne_id), ne_id);
            }
        }
    };

    push_boundary(elevation.neighbourhood(idx, 1, Field<double>::MOORE));

    while (true) {
        // ToDo: find why he gets stuck here
        if (boundary.empty()) {
            for (auto id : lake_ids) {
                trapped(id) = true;
            }

            return;
        }

        // boundary element with the smallest elevation
        const int min_elev_id = boundary.top().second;
        boundary.pop();

        lakes(min_elev_id) = true;
        rivers(min_elev_id) = false;
        lake_ids.push_back(min_elev_id);

        const std::vector<int> neighbour_ids = elevation.neighbourhood(min_elev_id, 1, Field<double>::MOORE);
        push_boundary(neighbour_ids);

        // check if it has negative slope towards an non lake cell
        const std::vector<double> gradients = calc_gradients(min_elev_id, neighbour_ids, elevation);
        std::vector<size_t> grad_ids(gradients.size());

        iota(grad_ids.begin(), grad_ids.end(), 0);
        stable_sort(grad_ids.begin(), grad_ids.end(),
                    [&gradients](size_t i1, size_t i2) { return gradients[i1] < gradients[i2]; });

        for (int g = 0; g < grad_ids.size(); g++) {
            const int gidx = grad_ids.at(g);

            if (gradients.at(gidx) < 0.0 && !lakes(neighbour_ids.at(gidx))) {
                track_river(neighbour_ids.at(gidx), sea_level, elevation, grad_x_norm, grad_y_norm, grad_norm, rivers,
                            lakes, trapped);
                return;
            }
        }
    }
}

std::vector<double> calc_gradients(int id, const std::vector<int>& neighbour_ids, const Field<double>& elevation) {
    std::vector<double> gradients(9);
    const double id_height = elevation(id);

    gradients.at(0) = neighbour_ids.at(0) != -1 ? (elevation(neighbour_ids.at(0)) - id_height) / sqrt(2.0) : 0.0;
    gradients.at(1) = neighbour_ids.at(1) != -1 ? (elevation(neighbour_ids.at(1)) - id_height) : 0.0;
    gradients.at(2) = neighbour_ids.at(2) != -1 ? (elevation(neighbour_ids.at(2)) - id_height) / sqrt(2.0) : 0.0;

    gradients.at(3) = neighbour_ids.at(3) != -1 ? (elevation(neighbour_ids.at(3)) - id_height) : 0.0;
    gradients.at(4) = 0.0;
    gradients.at(5) = neighbour_ids.at(5) != -1 ? (elevation(neighbour_ids.at(5)) - id_height) : 0.0;

    gradients.at(6) = neighbour_ids.at(6) != -1 ? (elevation(neighbour_ids.at(6)) - id_height) / sqrt(2.0) : 0.0;
    gradients.at(7) = neighbour_ids.at(7) != -1 ? (elevation(neighbour_ids.at(7)) - id_height) : 0.0;
    gradients.at(8) = neighbour_ids.at(8) != -1 ? (elevation(neighbour_ids.at(8)) - id_height) / sqrt(2.0) : 0.0;

    return gradients;
}

}  // namespace arena_alg
}  // namespace wg