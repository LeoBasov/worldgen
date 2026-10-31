#include "field.hpp"

namespace wg{

void Field::resize(int x, int y) {
    size_x = x;
    size_y = y;
    elements = std::vector<Element>(size_x * size_y);
}

Element& Field::get_element(int x, int y) { return elements.at(size_x * y + x); }

Element Field::get_element(int x, int y) const { return elements.at(size_x * y + x); }

} // namespace wg
