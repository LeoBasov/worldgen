#include "arena.hpp"

namespace wg {

void Arena::resize(int x, int y) {
    elevation.resize(x, y);
    rivers.resize(x, y);
    lakes.resize(x, y);
}

void Arena::add_noise(int seed, double frequency) {
    const siv::PerlinNoise::seed_type perlin_seed = seed;
    const siv::PerlinNoise perlin{perlin_seed};
    Field<double>& elev = elevation.value;

    for (int i = 0; i < elev.size; i++) {
        const auto [x, y] = elev.coords(i);
        const double fx = frequency / elev.size_x;
        const double fy = frequency / elev.size_y;

        elev(i) = perlin.octave2D_01((x * fx), (y * fy), 4);
    }

    elevation.calc_gradients();
}

}  // namespace wg