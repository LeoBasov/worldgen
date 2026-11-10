#include "arena_algorithms.hpp"

namespace wg {

std::vector<double> calc_gradients(int id, const std::vector<int>& neighbour_ids, const Field& field) {
    std::vector<double> gradients(9);
    const double id_height = field.elements.at(id).height;

    gradients.at(0) =
        neighbour_ids.at(0) != -1 ? (field.elements.at(neighbour_ids.at(0)).height - id_height) / sqrt(2.0) : 0.0;
    gradients.at(1) = neighbour_ids.at(1) != -1 ? (field.elements.at(neighbour_ids.at(1)).height - id_height) : 0.0;
    gradients.at(2) =
        neighbour_ids.at(2) != -1 ? (field.elements.at(neighbour_ids.at(2)).height - id_height) / sqrt(2.0) : 0.0;

    gradients.at(3) = neighbour_ids.at(3) != -1 ? (field.elements.at(neighbour_ids.at(3)).height - id_height) : 0.0;
    gradients.at(4) = 0.0;
    gradients.at(5) = neighbour_ids.at(5) != -1 ? (field.elements.at(neighbour_ids.at(5)).height - id_height) : 0.0;

    gradients.at(6) =
        neighbour_ids.at(6) != -1 ? (field.elements.at(neighbour_ids.at(6)).height - id_height) / sqrt(2.0) : 0.0;
    gradients.at(7) = neighbour_ids.at(7) != -1 ? (field.elements.at(neighbour_ids.at(7)).height - id_height) : 0.0;
    gradients.at(8) =
        neighbour_ids.at(8) != -1 ? (field.elements.at(neighbour_ids.at(8)).height - id_height) / sqrt(2.0) : 0.0;

    return gradients;
}

void track_river(int idx, double sea_level, Field& field) {
    field.elements.at(idx).is_river = true;
    int idx_old = -1;

    while (true) {
        const int dx = std::round(field.elements.at(idx).dheight_dx_norm);
        const int dy = std::round(field.elements.at(idx).dheight_dy_norm);
        const auto [x, y] = field.get_id_coordinates(idx);
        const int idx_new = field.get_element_id(x - dx, y - dy);

        if (idx_new == -1) {
            break;
        } else if (idx_new == idx_old) {
            field.elements.at(idx).is_river = false;
            field.elements.at(idx).is_lake = true;
            break;
        } else if (field.elements.at(idx_new).is_lake == true || field.elements.at(idx_new).is_river == true ||
                   field.elements.at(idx_new).height <= sea_level) {
            break;
        } else if (field.elements.at(idx_new).grad_height_norm < 1e-3) {
            field.elements.at(idx).is_river = false;
            field.elements.at(idx).is_lake = true;
            break;
        } else {
            idx_old = idx;
            idx = idx_new;
            field.elements.at(idx).is_river = true;
        }
    }

    if (field.elements.at(idx).is_lake) {
        fill_lake(idx, sea_level, field);
    }
}

void fill_lake(int idx, double sea_level, Field& field) {
    // Priority flood: the lake boundary is kept incrementally in a min-heap ordered by
    // (elevation, id) instead of being rebuilt after every added cell. Nothing else
    // modifies is_lake while this lake fills, so heap entries never go stale.
    using Entry = std::pair<double, int>;
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> boundary;
    std::unordered_set<int> seen{idx};  // lake cells of this fill + cells already pushed
    std::vector<int> lake_ids{idx};

    const auto push_boundary = [&](const std::vector<int>& neighbour_ids) {
        for (auto ne_id : neighbour_ids) {
            if (ne_id >= 0 && !field.elements.at(ne_id).is_lake && seen.insert(ne_id).second) {
                boundary.emplace(field.elements.at(ne_id).height, ne_id);
            }
        }
    };

    push_boundary(field.get_neighbourhood(idx, 1, Field::NeighbourhoodType::MOORE));

    while (true) {
        // ToDo: find why he gets stuck here
        if (boundary.empty()) {
            for (auto id : lake_ids) {
                field.elements.at(id).is_trapped = true;
            }

            return;
        }

        // boundary element with the smallest elevation
        const int min_elev_id = boundary.top().second;
        boundary.pop();

        field.elements.at(min_elev_id).is_lake = true;
        field.elements.at(min_elev_id).is_river = false;
        lake_ids.push_back(min_elev_id);

        const std::vector<int> neighbour_ids = field.get_neighbourhood(min_elev_id, 1, Field::NeighbourhoodType::MOORE);
        push_boundary(neighbour_ids);

        // check if it has negative slope towards an non lake cell
        const std::vector<double> gradients = calc_gradients(min_elev_id, neighbour_ids, field);
        std::vector<size_t> grad_ids(gradients.size());

        iota(grad_ids.begin(), grad_ids.end(), 0);
        stable_sort(grad_ids.begin(), grad_ids.end(),
                    [&gradients](size_t i1, size_t i2) { return gradients[i1] < gradients[i2]; });

        for (int g = 0; g < grad_ids.size(); g++) {
            const int gidx = grad_ids.at(g);

            if (gradients.at(gidx) < 0.0 && !field.elements.at(neighbour_ids.at(gidx)).is_lake) {
                track_river(neighbour_ids.at(gidx), sea_level, field);
                return;
            }
        }
    }
}

}  // namespace wg