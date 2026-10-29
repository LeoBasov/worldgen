#pragma once

#include <vector>

#include "perlin_noise.hpp"

namespace wg {

using namespace std;

struct Element {
    double elevation = 0.0;
    bool is_river = false;
};

class Arena {
   public:
    Arena();
    ~Arena() = default;

    void resize(int x, int y);
    void add_noise(double frequency = 5.0);
    void create_rivers(double elev_min, double elev_max, int nseeds);
    Element& get_element(int x, int y);
    Element get_element(int x, int y) const;
    size_t get_size_x() const;
    size_t get_size_y() const;
    std::vector<int> get_neighbour_ids(int id) const;
    std::vector<double> calc_gradients(int id, std::vector<int> neighbour_ids) const;

   private:
    vector<Element> field;
    size_t size_x = 0, size_y = 0;
};

}  // namespace wg
