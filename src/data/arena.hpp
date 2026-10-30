#pragma once

#include <vector>

#include "config.hpp"
#include "perlin_noise.hpp"

namespace wg {

using namespace std;

struct Element {
    double elevation = 0.0;
    bool is_river = false;
    bool is_lake = false;
    bool is_trapped = false;
};

class Arena {
   public:
    Arena();
    ~Arena() = default;

    void resize(Config::Map map);
    void add_noise(Config::Noise noise);
    void create_rivers(Config::Hydrography hydrography);
    Element& get_element(int x, int y);
    Element get_element(int x, int y) const;
    size_t get_size_x() const;
    size_t get_size_y() const;
    std::vector<int> get_neighbour_ids(int id) const;
    std::vector<double> calc_gradients(int id, std::vector<int> neighbour_ids) const;

   private:
    vector<Element> field;
    size_t size_x = 0, size_y = 0;

    void track_river(int idx, double sea_level);
    void fill_lake(int idx, double sea_level);
    std::vector<int> get_lake_boundary(std::vector<int> lake_ids);
};

}  // namespace wg
