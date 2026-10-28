#include "arena.hpp"

namespace wg {

Arena::Arena() {}

void Arena::resize(int x, int y) {
    field.clear();
    field.resize(y, vector<Element>(x));
}

}  // namespace wg