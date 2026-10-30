#include "arena.hpp"

#include <algorithm>
#include <iostream>

namespace wg {

Arena::Arena() {}

void Arena::resize(Config::Map map) {
    size_x = map.size_x;
    size_y = map.size_y;
    field = std::vector<Element>(size_x * size_y);
}

void Arena::add_noise(Config::Noise noise) {
    const siv::PerlinNoise::seed_type seed = 123456u;
    const siv::PerlinNoise perlin{seed};

    for (int i = 0; i < field.size(); i++) {
        const int x = i % size_y;
        const int y = i / size_y;
        const double fx = noise.frequency / size_x;
        const double fy = noise.frequency / size_y;

        field.at(i).elevation = perlin.octave2D_01((x * fx), (y * fy), 4);
    }
}

void Arena::create_rivers(Config::Hydrography hydrography) {
    std::vector<size_t> ids(field.size());
    std::iota(ids.begin(), ids.end(), 0);  // 0, 1, 2, ..., n-1
    std::mt19937 rng(std::random_device{}());
    std::shuffle(ids.begin(), ids.end(), rng);
    int counter = 0;

    for (int i = 0; i < field.size(); i++) {
        const size_t id = ids.at(i);
        const double elevation = field.at(id).elevation;
        const bool is_river = field.at(id).is_river;

        if (!is_river && elevation > hydrography.min_elevation && elevation < hydrography.max_elevation) {
            track_river(id, hydrography.sea_level);
            counter++;
        }

        if (counter >= hydrography.n_rivers) {
            break;
        }
    }
}

Element& Arena::get_element(int x, int y) { return field.at(size_x * y + x); }

Element Arena::get_element(int x, int y) const { return field.at(size_x * y + x); }

size_t Arena::get_size_x() const { return size_x; }

size_t Arena::get_size_y() const { return size_y; }

std::vector<int> Arena::get_neighbour_ids(int id) const {
    const int x = id % size_y;
    const int y = id / size_y;
    const int idr = x < size_x - 1 ? size_x * y + x + 1 : id;
    const int idru = (x < size_x - 1) && (y < size_y - 1) ? size_x * (y + 1) + x + 1 : id;
    const int idu = y < size_y - 1 ? size_x * (y + 1) + x : id;
    const int idul = (y < size_y - 1) && (x > 0) ? size_x * (y + 1) + x - 1 : id;
    const int idl = x > 0 ? size_x * y + x - 1 : id;
    const int idld = (x > 0) && (y > 0) ? size_x * (y - 1) + x - 1 : id;
    const int idd = y > 0 ? size_x * (y - 1) + x : id;
    const int iddr = (y > 0) && (x < size_x - 1) ? size_x * (y - 1) + x + 1 : id;

    return {idr, idru, idu, idul, idl, idld, idd, iddr};
}

std::vector<double> Arena::calc_gradients(int id, std::vector<int> neighbour_ids) const {
    std::vector<double> gradients(8);

    gradients.at(0) = field.at(neighbour_ids.at(0)).elevation - field.at(id).elevation;
    gradients.at(1) = (field.at(neighbour_ids.at(1)).elevation - field.at(id).elevation) / sqrt(2.0);

    gradients.at(2) = field.at(neighbour_ids.at(2)).elevation - field.at(id).elevation;
    gradients.at(3) = (field.at(neighbour_ids.at(3)).elevation - field.at(id).elevation) / sqrt(2.0);

    gradients.at(4) = field.at(neighbour_ids.at(4)).elevation - field.at(id).elevation;
    gradients.at(5) = (field.at(neighbour_ids.at(5)).elevation - field.at(id).elevation) / sqrt(2.0);

    gradients.at(6) = field.at(neighbour_ids.at(6)).elevation - field.at(id).elevation;
    gradients.at(7) = (field.at(neighbour_ids.at(7)).elevation - field.at(id).elevation) / sqrt(2.0);

    return gradients;
}

void Arena::track_river(int idx, double sea_level) {
    field.at(idx).is_river = true;

    while (true) {
        const std::vector<int> neighbour_ids = get_neighbour_ids(idx);
        const std::vector<double> gradients = calc_gradients(idx, neighbour_ids);
        vector<size_t> grad_ids(gradients.size());
        int idx_new = -1;

        iota(grad_ids.begin(), grad_ids.end(), 0);
        stable_sort(grad_ids.begin(), grad_ids.end(),
                    [&gradients](size_t i1, size_t i2) { return gradients[i1] < gradients[i2]; });

        for (int g = 0; g < grad_ids.size(); g++) {
            const int gidx = grad_ids.at(g);

            if (gradients.at(gidx) < 0.0 && neighbour_ids.at(gidx) != idx) {
                idx_new = neighbour_ids.at(gidx);
                break;
            }
        }

        if (idx_new == -1) {
            field.at(idx).is_river = false;
            field.at(idx).is_lake = true;
            break;
        } else if (field.at(idx_new).is_river == true || field.at(idx_new).elevation <= sea_level) {
            break;
        } else {
            idx = idx_new;
            field.at(idx).is_river = true;
        }
    }

    if (field.at(idx).is_lake) {
        fill_lake(idx, sea_level);
    }
}

void Arena::fill_lake(int idx, double sea_level) {
    std::vector<int> lake_ids{idx};
    bool is_filling = true;

    while (is_filling) {
        // get lak boundary
        std::vector<int> lake_bounrady_ids = get_lake_boundary(lake_ids);

        // find boundary element with the smallest elevation
        double min_elevation = 1e16;
        int min_elev_id = -1;

        for (auto id : lake_bounrady_ids) {
            if (field.at(id).elevation < min_elevation) {
                min_elevation = field.at(id).elevation;
                min_elev_id = id;
            }
        }

        field.at(min_elev_id).is_lake;
        lake_ids.push_back(min_elev_id);

        // check if it has negative slope towards an non lake cell
        const std::vector<int> neighbour_ids = get_neighbour_ids(min_elev_id);
        const std::vector<double> gradients = calc_gradients(min_elev_id, neighbour_ids);
        vector<size_t> grad_ids(gradients.size());

        iota(grad_ids.begin(), grad_ids.end(), 0);
        stable_sort(grad_ids.begin(), grad_ids.end(),
                    [&gradients](size_t i1, size_t i2) { return gradients[i1] < gradients[i2]; });

        for (int g = 0; g < grad_ids.size(); g++) {
            const int gidx = grad_ids.at(g);

            if (gradients.at(gidx) < 0.0 && neighbour_ids.at(gidx) != min_elev_id && !field.at(gidx).is_lake) {
                track_river(gidx, sea_level);
                is_filling = false;
                break;
            }
        }
    }
}

std::vector<int> Arena::get_lake_boundary(std::vector<int> lake_ids) {
    std::vector<int> lake_bounrady_ids;

    for (auto id : lake_ids) {
        const std::vector<int> neighbour_ids = get_neighbour_ids(id);

        for (auto ne_id : neighbour_ids) {
            if (!field.at(ne_id).is_lake &&
                std::find(lake_bounrady_ids.begin(), lake_bounrady_ids.end(), ne_id) == lake_bounrady_ids.end()) {
                lake_bounrady_ids.push_back(ne_id);
            }
        }
    }

    return lake_bounrady_ids;
}

}  // namespace wg