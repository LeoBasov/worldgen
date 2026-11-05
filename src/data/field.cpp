#include "field.hpp"

namespace wg{

void Field::resize(int x, int y) {
    size_x = x;
    size_y = y;
    elements = std::vector<Element>(size_x * size_y);
}

int Field::get_element_id(int x, int y) const {
    if (x < 0 || x >= size_x || y < 0 || y >= size_y) {
        return -1;
    } else {
        return size_x * y + x;
    }
}

Element& Field::get_element(int x, int y) {
    const int element_id = get_element_id(x, y);

    return elements.at(element_id);
}

Element Field::get_element(int x, int y) const {
    const int element_id = get_element_id(x, y);

    return elements.at(element_id);
}

std::vector<int> Field::get_neighbourhood(const int id, const int size, const Neighbourhood type) const {
    std::vector<int> neighbourhood;

    // ToDo

    return neighbourhood;
}

} // namespace wg
