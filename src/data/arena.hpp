#pragma once

#include <vector>

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

   private:
    vector<vector<Element>> field;
};

}  // namespace wg
