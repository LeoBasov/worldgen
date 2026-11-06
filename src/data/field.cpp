#include "field.hpp"

namespace wg{

void Field::resize(int x, int y) {
    size_x = x;
    size_y = y;
    elements = std::vector<Element>(size_x * size_y);
}

std::pair<double, double> Field::calc_gradients_new(const int x, const int y, const double e_right, const double e_top,
                                                    const double e_left, const double e_buttom,
                                                    const double e_center) const {
    std::pair<double, double> grad = {0.0, 0.0};

    if (x == 0 && y == 0) {
        grad.first = (e_right - e_center);
        grad.second = (e_buttom - e_center);
    } else if (x == (size_x - 1) && y == (size_y - 1)) {
        grad.first = (e_center - e_left);
        grad.second = (e_center - e_top);
    } else if (x == (size_x - 1) && y == 0) {
        grad.first = (e_center - e_left);
        grad.second = (e_buttom - e_center);
    } else if (x == 0 && y == (size_y - 1)) {
        grad.first = (e_right - e_center);
        grad.second = (e_center - e_top);
    } else if (x == 0) {
        grad.first = (e_right - e_center);
        grad.second = (e_buttom - e_top) * 0.5;
    } else if (y == 0) {
        grad.first = (e_right - e_left) * 0.5;
        grad.second = (e_buttom - e_center);
    } else if (x == (size_x - 1)) {
        grad.first = (e_center - e_left);
        grad.second = (e_buttom - e_top) * 0.5;
    } else if (y == (size_y - 1)) {
        grad.first = (e_right - e_left) * 0.5;
        grad.second = (e_center - e_top);
    } else {
        grad.first = (e_right - e_left) * 0.5;
        grad.second = (e_buttom - e_top) * 0.5;
    }

    return grad;
}

void Field::calc_gradients(const Property property) {
    // y outer / x inner to walk elements in memory order (id = size_x * y + x)
    for (int y = 0; y < size_y; y++) {
        for (int x = 0; x < size_x; x++) {
            const int id = get_element_id(x, y);
            Element& element = elements.at(id);
            double& grad_x = element.delevation_dx;
            double& grad_y = element.delevation_dx;

            switch (property) {
                case ELEVATION: {
                    // direct von Neumann neighbour lookup (out of bounds -> 0.0)
                    const double e_center = elements[id].elevation;
                    const double e_right = x + 1 < size_x ? elements[id + 1].elevation : 0.0;
                    const double e_top = y > 0 ? elements[id - size_x].elevation : 0.0;
                    const double e_left = x > 0 ? elements[id - 1].elevation : 0.0;
                    const double e_buttom = y + 1 < size_y ? elements[id + size_x].elevation : 0.0;

                    const std::pair<double, double> grad =
                        calc_gradients_new(x, y, e_right, e_top, e_left, e_buttom, e_center);

                    elements.at(id).delevation_dx = grad.first;
                    elements.at(id).delevation_dy = grad.second;

                    elements.at(id).elevation_gradient_norm =
                        std::sqrt(std::pow(grad.first, 2) + std::pow(grad.second, 2));
                    elements.at(id).delevation_dx_norm = grad.first / elements.at(id).elevation_gradient_norm;
                    elements.at(id).delevation_dy_norm = grad.second / elements.at(id).elevation_gradient_norm;
                    break;
                }
                case ATTRACTIVENESS: {
                    // direct von Neumann neighbour lookup (out of bounds -> 0.0)
                    const double e_center = elements[id].attractiveness_factor;
                    const double e_right = x + 1 < size_x ? elements[id + 1].attractiveness_factor : 0.0;
                    const double e_top = y > 0 ? elements[id - size_x].attractiveness_factor : 0.0;
                    const double e_left = x > 0 ? elements[id - 1].attractiveness_factor : 0.0;
                    const double e_buttom = y + 1 < size_y ? elements[id + size_x].attractiveness_factor : 0.0;

                    const std::pair<double, double> grad =
                        calc_gradients_new(x, y, e_right, e_top, e_left, e_buttom, e_center);

                    elements.at(id).dattractiveness_factor_dx = grad.first;
                    elements.at(id).dattractiveness_factor_dy = grad.second;

                    elements.at(id).attractiveness_factor_gradient_norm =
                        std::sqrt(std::pow(grad.first, 2) + std::pow(grad.second, 2));
                    break;
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
