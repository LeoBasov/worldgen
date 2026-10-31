#include "field.hpp"

namespace wg{

Element& Field::get_element(int x, int y) { return elements.at(size_x * y + x); }

Element Field::get_element(int x, int y) const { return elements.at(size_x * y + x); }

} // namespace wg
