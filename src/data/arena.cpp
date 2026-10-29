#include "arena.hpp"

#include <iostream>

namespace wg {

Arena::Arena() {}

void Arena::resize(int x, int y) {
    size_x = x;
    size_y = y;
    field = std::vector<Element>(size_x * size_y);
}

void Arena::add_noise() {
    const siv::PerlinNoise::seed_type seed = 123456u;
    const siv::PerlinNoise perlin{seed};

    for (int i = 0; i < field.size(); i++) {
        const int x = i % size_y;
        const int y = i / size_y;

        field.at(i).elevation = perlin.octave2D_01((x * 0.01), (y * 0.01), 4);
    }
}

Element& Arena::get_element(int x, int y) { return field.at(size_x * y + x); }

Element Arena::get_element(int x, int y) const { return field.at(size_x * y + x); }

size_t Arena::get_size_x() const { return size_x; }

size_t Arena::get_size_y() const { return size_y; }

}  // namespace wg