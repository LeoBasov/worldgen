#include "arena.hpp"

namespace wg {

void Arena::resize(int x, int y) {
    size_x = x;
    size_y = y;
    size = size_x * size_y;

    elevation.resize(size_x, size_y);
    rivers.resize(size_x, size_y);
    lakes.resize(size_x, size_y);
}

void Arena::add_noise(double frequency) {
    const siv::PerlinNoise::seed_type perlin_seed = seed;
    const siv::PerlinNoise perlin{perlin_seed};
    const double fx = frequency / size_x;
    const double fy = frequency / size_y;

    for (int y = 0; y < size_y; y++) {
        for (int x = 0; x < size_x; x++) {
            elevation.value(x, y) = perlin.octave2D_01((x * fx), (y * fy), 4);
        }
    }

    elevation.calc_gradients();
}

void Arena::create_rivers(int n_rivers, double min_height, double max_height, double sea_level) {
    std::vector<size_t> ids(size);
    std::iota(ids.begin(), ids.end(), 0);  // 0, 1, 2, ..., n-1
    std::mt19937 rng(seed);
    std::shuffle(ids.begin(), ids.end(), rng);
    int counter = 0;

    for (int i = 0; i < size; i++) {
        const size_t id = ids.at(i);
        const double height = elevation.value(i);
        const bool is_river = rivers(i);

        if (!is_river && height > min_height && height < max_height) {
            arena_alg::track_river(id, sea_level, elevation.value, elevation.grad_x_norm, elevation.grad_y_norm,
                                   elevation.grad_norm, rivers, lakes);
            counter++;
        }

        if (counter >= n_rivers) {
            break;
        }
    }
}

}  // namespace wg