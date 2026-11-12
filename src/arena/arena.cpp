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

void Arena::add_noise(int seed, double frequency) {
    const siv::PerlinNoise::seed_type perlin_seed = seed;
    const siv::PerlinNoise perlin{perlin_seed};

    for (int i = 0; i < size; i++) {
        const auto [x, y] = elevation.value.coords(i);
        const double fx = frequency / size_x;
        const double fy = frequency / size_y;

        elevation.value(i) = perlin.octave2D_01((x * fx), (y * fy), 4);
    }

    elevation.calc_gradients();
}

}  // namespace wg