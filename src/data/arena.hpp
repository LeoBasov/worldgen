#pragma once

#include <vector>

#include "arena_algorithms.hpp"
#include "config.hpp"
#include "field.hpp"
#include "perlin_noise.hpp"

namespace wg {

using namespace std;

class Arena {
   public:
    Arena();
    ~Arena() = default;

    void resize(Config::Map map);
    void add_noise(Config::Noise noise);
    void create_rivers(Config::Hydrography hydrography, Config::Noise noise);
    Element& get_element(int x, int y);
    Element get_element(int x, int y) const;
    size_t get_size_x() const;
    size_t get_size_y() const;

   private:
    Field field;

    void track_river(int idx, double sea_level);
    void fill_lake(int idx, double sea_level);
    std::vector<int> get_lake_boundary(std::vector<int> lake_ids);
};

}  // namespace wg
