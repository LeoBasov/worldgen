#pragma once

#include "arena_alg.hpp"
#include "data/perlin_noise.hpp"
#include "value.hpp"

namespace wg {

struct Arena {
    Value elevation;
    Field<bool> rivers;
    Field<bool> lakes;
    Field<bool> trapped;
    int size_x = 0, size_y = 0, size = 0;
    int seed = 0;

    void resize(int x, int y);
    void add_noise(double frequency);
    void create_rivers(int n_rivers, double min_height, double max_height, double sea_level);
};

}  // namespace wg
