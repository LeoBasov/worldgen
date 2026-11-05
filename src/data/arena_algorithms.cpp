#include "arena_algorithms.hpp"

namespace wg {

std::vector<int> get_neighbour_ids(const int id, const int size_x, const int size_y) {
    const int x = id % size_x;
    const int y = id / size_x;

    const bool right_condition = x < size_x - 1;
    const bool up_condition = y > 0;
    const bool left_condition = x > 0;
    const bool down_condition = y < size_y - 1;

    const int id_right = right_condition ? size_x * y + x + 1 : id;
    const int id_right_up = right_condition && up_condition ? size_x * (y - 1) + x + 1 : id;
    const int id_up = up_condition ? size_x * (y - 1) + x : id;
    const int id_up_left = up_condition && left_condition ? size_x * (y - 1) + x - 1 : id;
    const int id_left = left_condition ? size_x * y + x - 1 : id;
    const int id_left_dowd = left_condition && down_condition ? size_x * (y + 1) + x - 1 : id;
    const int id_down = down_condition ? size_x * (y + 1) + x : id;
    const int id_down_right = down_condition && right_condition ? size_x * (y + 1) + x + 1 : id;

    return {id_right, id_right_up, id_up, id_up_left, id_left, id_left_dowd, id_down, id_down_right};
}

std::vector<double> calc_gradients(int id, const std::vector<int>& neighbour_ids, const Field& field) {
    std::vector<double> gradients(8);

    gradients.at(0) = field.elements.at(neighbour_ids.at(0)).elevation - field.elements.at(id).elevation;
    gradients.at(1) = (field.elements.at(neighbour_ids.at(1)).elevation - field.elements.at(id).elevation) / sqrt(2.0);

    gradients.at(2) = field.elements.at(neighbour_ids.at(2)).elevation - field.elements.at(id).elevation;
    gradients.at(3) = (field.elements.at(neighbour_ids.at(3)).elevation - field.elements.at(id).elevation) / sqrt(2.0);

    gradients.at(4) = field.elements.at(neighbour_ids.at(4)).elevation - field.elements.at(id).elevation;
    gradients.at(5) = (field.elements.at(neighbour_ids.at(5)).elevation - field.elements.at(id).elevation) / sqrt(2.0);

    gradients.at(6) = field.elements.at(neighbour_ids.at(6)).elevation - field.elements.at(id).elevation;
    gradients.at(7) = (field.elements.at(neighbour_ids.at(7)).elevation - field.elements.at(id).elevation) / sqrt(2.0);

    return gradients;
}

std::vector<int> get_lake_boundary_ids(const std::vector<int>& lake_ids, const Field& field) {
    std::vector<int> lake_boundary_ids;

    for (auto id : lake_ids) {
        const std::vector<int> neighbour_ids = field.get_neighbourhood(id, 1, Field::NeighbourhoodType::MOORE);

        for (auto ne_id : neighbour_ids) {
            if (ne_id >= 0 && !field.elements.at(ne_id).is_lake &&
                std::find(lake_boundary_ids.begin(), lake_boundary_ids.end(), ne_id) == lake_boundary_ids.end()) {
                lake_boundary_ids.push_back(ne_id);
            }
        }
    }

    return lake_boundary_ids;
}

void track_river(int idx, double sea_level, Field& field) {
    field.elements.at(idx).is_river = true;
    int idx_old = -1;

    while (true) {
        const int dx = std::round(field.elements.at(idx).delevation_dx_norm);
        const int dy = std::round(field.elements.at(idx).delevation_dy_norm);
        const auto [x, y] = field.get_id_coordinates(idx);
        const int idx_new = field.get_element_id(x - dx, y - dy);

        if (idx_new == -1) {
            break;
        } else if (idx_new == idx_old) {
            field.elements.at(idx).is_river = false;
            field.elements.at(idx).is_lake = true;
            break;
        } else if (field.elements.at(idx_new).is_lake == true || field.elements.at(idx_new).is_river == true ||
                   field.elements.at(idx_new).elevation <= sea_level) {
            break;
        } else if (field.elements.at(idx_new).elevation_gradient_norm < 1e-3) {
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
    std::vector<int> lake_ids{idx};
    bool is_filling = true;

    while (is_filling) {
        // get lake boundary
        std::vector<int> lake_boundary_ids = get_lake_boundary_ids(lake_ids, field);

        // ToDo: find why he gets stuck here
        if (lake_boundary_ids.size() == 0) {
            for (auto id : lake_ids) {
                field.elements.at(id).is_trapped = true;
            }

            return;
        }

        // find boundary element with the smallest elevation
        double min_elevation = 1e16;
        int min_elev_id = -1;

        for (auto id : lake_boundary_ids) {
            if (field.elements.at(id).elevation < min_elevation) {
                min_elevation = field.elements.at(id).elevation;
                min_elev_id = id;
            }
        }

        field.elements.at(min_elev_id).is_lake = true;
        field.elements.at(min_elev_id).is_river = false;
        lake_ids.push_back(min_elev_id);

        // check if it has negative slope towards an non lake cell
        const std::vector<int> neighbour_ids = get_neighbour_ids(min_elev_id, field.size_x, field.size_y);
        const std::vector<double> gradients = calc_gradients(min_elev_id, neighbour_ids, field);
        std::vector<size_t> grad_ids(gradients.size());

        iota(grad_ids.begin(), grad_ids.end(), 0);
        stable_sort(grad_ids.begin(), grad_ids.end(),
                    [&gradients](size_t i1, size_t i2) { return gradients[i1] < gradients[i2]; });

        for (int g = 0; g < grad_ids.size(); g++) {
            const int gidx = grad_ids.at(g);

            if (gradients.at(gidx) < 0.0 && !field.elements.at(neighbour_ids.at(gidx)).is_lake) {
                track_river(neighbour_ids.at(gidx), sea_level, field);
                is_filling = false;
                break;
            }
        }
    }
}

}  // namespace wg