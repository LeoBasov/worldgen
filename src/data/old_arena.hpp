#pragma once

#include <vector>

#include "arena_algorithms.hpp"
#include "config.hpp"
#include "old_field.hpp"
#include "perlin_noise.hpp"

namespace wg {

using namespace std;

class OldArena {
   public:
    OldArena();
    ~OldArena() = default;

    void resize(Config::Map map);
    void add_noise(Config::Noise noise);
    void create_rivers(Config::Hydrography hydrography, Config::Noise noise);
    void calc_attractiveness_factor(Config::Hydrography hydrography);
    OldElement& get_element(int x, int y);
    const OldElement& get_element(int x, int y) const;
    size_t get_size_x() const;
    size_t get_size_y() const;

   private:
    OldField field;
};

}  // namespace wg
