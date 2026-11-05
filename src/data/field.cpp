#include "field.hpp"

namespace wg{

void Field::resize(int x, int y) {
    size_x = x;
    size_y = y;
    elements = std::vector<Element>(size_x * size_y);
}

std::pair<int, int> Field::get_id_coordinates(const int id) const {
    if (id < 0 || id > elements.size()) {
        throw std::domain_error("id out of bounds");
    }

    return {id % size_x, id / size_x};
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
    const auto [x, y] = get_id_coordinates(id);

    if (size < 1) {
        throw std::domain_error("zero or negative neighbourhood size");
    }

    switch (type) {
        case Neighbourhood::NEUMANN: {
            break;
        }
        case Neighbourhood::MOORE: {
            const int length = 2 * size + 1;
            int counter = 0;

            neighbourhood.resize(length * length);

            for (int iy = 0; iy < length; iy++) {
                for (int ix = 0; ix < length; ix++) {
                    const int neighbourhood_id = get_element_id(x + ix - size, y + iy - size);

                    neighbourhood.at(counter++) = neighbourhood_id;
                }
            }

            break;
        }
        default: {
            throw std::domain_error("unrecognized neighbourhood type");
            break;
        }
    }

    return neighbourhood;
}

} // namespace wg
