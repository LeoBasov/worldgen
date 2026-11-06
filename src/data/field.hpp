#pragma once

#include <cmath>
#include <stdexcept>
#include <vector>

namespace wg
{

struct Element {
    // attractiveness
    double density = 0.0;
    double ddensity_dx = 0.0;
    double ddensity_dy = 0.0;
    double density_gradient_norm = 0.0;

    // attractiveness
    double attractiveness_factor = 0.0;
    double dattractiveness_factor_dx = 0.0;
    double dattractiveness_factor_dy = 0.0;
    double attractiveness_factor_gradient_norm = 0.0;

    // elevation
    double elevation = 0.0;
    double delevation_dx = 0.0;
    double delevation_dy = 0.0;
    double delevation_dx_norm = 0.0;
    double delevation_dy_norm = 0.0;
    double elevation_gradient_norm = 0.0;

    // state flags
    bool is_river = false;
    bool is_lake = false;
    bool is_trapped = false;
};

struct Field{
    enum NeighbourhoodType { NEUMANN, MOORE };
    enum Property { ELEVATION, ATTRACTIVENESS, DENSITY };

    std::vector<Element> elements;
    size_t size_x = 0, size_y = 0;

    void resize(int x, int y);
    void calc_gradients(const Property property);
    std::pair<double, double> calc_gradients_new(const int x, const int y, const double e_right, const double e_top,
                                                 const double e_left, const double e_buttom,
                                                 const double e_center) const;

    std::pair<int, int> get_id_coordinates(const int id) const;
    int get_element_id(int x, int y) const;
    Element& get_element(int x, int y);
    Element get_element(int x, int y) const;
    std::vector<int> get_neighbourhood(const int id, const int size, const NeighbourhoodType type) const;
};

} // namespace wg
