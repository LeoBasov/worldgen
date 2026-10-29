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

    vector<vector<Element>> field;
};

}  // namespace wg
