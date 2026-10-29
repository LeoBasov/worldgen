#pragma once

#include <vector>

#include "perlin_noise.hpp"

namespace wg {

using namespace std;

struct Element {
    double elevation = 0.0;
};

class Arena {
   public:
    Arena();
    ~Arena() = default;

    void resize(int x, int y);
    void add_noise();
    Element& get_element(int x, int y);
    Element get_element(int x, int y) const;
    size_t get_size_x() const;
    size_t get_size_y() const;

   private:
    vector<Element> field;
    size_t size_x = 0, size_y = 0;
};

}  // namespace wg
