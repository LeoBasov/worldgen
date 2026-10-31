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
            track_river(id, hydrography.sea_level, field.size_x, field.size_y, field.elements);
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

}  // namespace wg