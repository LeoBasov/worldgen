#include "arena.hpp"

namespace wg {

Arena::Arena() {}

void Arena::resize(int x, int y) {
    size_x = x;
    size_y = y;
    field.clear();
    field.resize(size_y, vector<Element>(size_x));
}

void Arena::add_noise() {
    const siv::PerlinNoise::seed_type seed = 123456u;
    const siv::PerlinNoise perlin{seed};

    for (int y = 0; y < field.size(); ++y) {
        for (int x = 0; x < field.at(y).size(); ++x) {
            field.at(y).at(x).elevation = perlin.octave2D_01((x * 0.01), (y * 0.01), 4);
        }
    }
}

}  // namespace wg