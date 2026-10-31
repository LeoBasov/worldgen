#include "arena.hpp"

#include <algorithm>
#include <iostream>

namespace wg {

Arena::Arena() {}

void Arena::resize(Config::Map map) { field.resize(map.size_x, map.size_y); }

void Arena::add_noise(Config::Noise noise) {
    const siv::PerlinNoise::seed_type seed = noise.seed;
    const siv::PerlinNoise perlin{seed};

    for (int i = 0; i < field.elements.size(); i++) {
        const int x = i % field.size_y;
        const int y = i / field.size_y;
        const double fx = noise.frequency / field.size_x;
        const double fy = noise.frequency / field.size_y;

        field.elements.at(i).elevation = perlin.octave2D_01((x * fx), (y * fy), 4);
    }
}

void Arena::create_rivers(Config::Hydrography hydrography, Config::Noise noise) {
    std::vector<size_t> ids(field.elements.size());
    std::iota(ids.begin(), ids.end(), 0);  // 0, 1, 2, ..., n-1
    std::mt19937 rng(noise.seed);
    std::shuffle(ids.begin(), ids.end(), rng);
    int counter = 0;

    for (int i = 0; i < field.elements.size(); i++) {
        const size_t id = ids.at(i);
        const double elevation = field.elements.at(id).elevation;
        const bool is_river = field.elements.at(id).is_river;

        if (!is_river && elevation > hydrography.min_elevation && elevation < hydrography.max_elevation) {
            track_river(id, hydrography.sea_level);
            counter++;
        }

        if (counter >= hydrography.n_rivers) {
            break;
        }
    }
}

Element& Arena::get_element(int x, int y) { return field.elements.at(field.size_x * y + x); }

Element Arena::get_element(int x, int y) const { return field.elements.at(field.size_x * y + x); }

size_t Arena::get_size_x() const { return field.size_x; }

size_t Arena::get_size_y() const { return field.size_y; }

void Arena::track_river(int idx, double sea_level) {
    field.elements.at(idx).is_river = true;

    while (true) {
        const std::vector<int> neighbour_ids = get_neighbour_ids(idx, field.size_x, field.size_y);
        const std::vector<double> gradients = calc_gradients(idx, neighbour_ids, field.elements);
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
            field.elements.at(idx).is_river = false;
            field.elements.at(idx).is_lake = true;
            break;
        } else if (field.elements.at(idx_new).is_lake == true || field.elements.at(idx_new).is_river == true ||
                   field.elements.at(idx_new).elevation <= sea_level) {
            break;
        } else {
            idx = idx_new;
            field.elements.at(idx).is_river = true;
        }
    }

    if (field.elements.at(idx).is_lake) {
        fill_lake(idx, sea_level);
    }
}

void Arena::fill_lake(int idx, double sea_level) {
    std::vector<int> lake_ids{idx};
    bool is_filling = true;

    while (is_filling) {
        // get lake boundary
        std::vector<int> lake_boundary_ids =
            get_lake_boundary_ids(lake_ids, field.elements, field.size_x, field.size_y);

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
        const std::vector<double> gradients = calc_gradients(min_elev_id, neighbour_ids, field.elements);
        vector<size_t> grad_ids(gradients.size());

        iota(grad_ids.begin(), grad_ids.end(), 0);
        stable_sort(grad_ids.begin(), grad_ids.end(),
                    [&gradients](size_t i1, size_t i2) { return gradients[i1] < gradients[i2]; });

        for (int g = 0; g < grad_ids.size(); g++) {
            const int gidx = grad_ids.at(g);

            if (gradients.at(gidx) < 0.0 && !field.elements.at(neighbour_ids.at(gidx)).is_lake) {
                track_river(neighbour_ids.at(gidx), sea_level);
                is_filling = false;
                break;
            }
        }
    }
}

}  // namespace wg