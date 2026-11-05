#include "field.hpp"

namespace wg{

void Field::resize(int x, int y) {
    size_x = x;
    size_y = y;
    elements = std::vector<Element>(size_x * size_y);
}

void Field::calc_gradients(const Property property) {
    for (int x = 0; x < size_x; x++) {
        for (int y = 0; y < size_y; y++) {
            const int id = get_element_id(x, y);
            const std::vector<int> neighbour_ids = get_neighbourhood(id, 1, NEUMANN);

            switch (property) {
                case ELEVATION: {
                    const double e_center = elements.at(id).elevation;
                    const double e_right = neighbour_ids.at(5) >= 0 ? elements.at(neighbour_ids.at(5)).elevation : 0.0;
                    const double e_top = neighbour_ids.at(1) >= 0 ? elements.at(neighbour_ids.at(1)).elevation : 0.0;
                    const double e_left = neighbour_ids.at(3) >= 0 ? elements.at(neighbour_ids.at(3)).elevation : 0.0;
                    const double e_buttom = neighbour_ids.at(7) >= 0 ? elements.at(neighbour_ids.at(7)).elevation : 0.0;

                    if (x == 0 && y == 0) {
                        elements.at(id).delevation_dx = (e_right - e_center);
                        elements.at(id).delevation_dy = (e_buttom - e_center);
                    } else if (x == (size_x - 1) && y == (size_y - 1)) {
                        elements.at(id).delevation_dx = (e_center - e_left);
                        elements.at(id).delevation_dy = (e_center - e_top);
                    } else if (x == (size_x - 1) && y == 0) {
                        elements.at(id).delevation_dx = (e_center - e_left);
                        elements.at(id).delevation_dy = (e_buttom - e_center);
                    } else if (x == 0 && y == (size_y - 1)) {
                        elements.at(id).delevation_dx = (e_right - e_center);
                        elements.at(id).delevation_dy = (e_center - e_top);
                    } else if (x == 0) {
                        elements.at(id).delevation_dx = (e_right - e_center);
                        elements.at(id).delevation_dy = (e_buttom - e_top) * 0.5;
                    } else if (y == 0) {
                        elements.at(id).delevation_dx = (e_right - e_left) * 0.5;
                        elements.at(id).delevation_dy = (e_buttom - e_center);
                    } else if (x == (size_x - 1)) {
                        elements.at(id).delevation_dx = (e_center - e_left);
                        elements.at(id).delevation_dy = (e_buttom - e_top) * 0.5;
                    } else if (y == (size_y - 1)) {
                        elements.at(id).delevation_dx = (e_right - e_left) * 0.5;
                        elements.at(id).delevation_dy = (e_center - e_top);
                    } else {
                        elements.at(id).delevation_dx = (e_right - e_left) * 0.5;
                        elements.at(id).delevation_dy = (e_buttom - e_top) * 0.5;
                    }
                }
                default: {
                    break;
                }
            }
        }
    }
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

std::vector<int> Field::get_neighbourhood(const int id, const int size, const NeighbourhoodType type) const {
    std::vector<int> neighbourhood;
    const auto [x, y] = get_id_coordinates(id);

    if (size < 1) {
        throw std::domain_error("zero or negative neighbourhood size");
    }

    switch (type) {
        case NeighbourhoodType::NEUMANN: {
            const int length = 2 * size + 1;
            int counter = 0;

            neighbourhood.resize(length * length);

            for (int iy = 0; iy < length; iy++) {
                for (int ix = 0; ix < length; ix++) {
                    const int neighbourhood_id = get_element_id(x + ix - size, y + iy - size);

                    if ((std::abs(ix - size) + std::abs(iy - size)) > size) {
                        neighbourhood.at(counter++) = -1;
                    } else {
                        neighbourhood.at(counter++) = neighbourhood_id;
                    }
                }
            }

            break;
        }
        case NeighbourhoodType::MOORE: {
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
