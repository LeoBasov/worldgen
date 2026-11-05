#pragma once

#include <vector>

namespace wg
{

struct Element {
    double attraktivness_factor = 0.0;
    double elevation = 0.0;
    bool is_river = false;
    bool is_lake = false;
    bool is_trapped = false;
};

struct Field{
    enum Neighbourhood { NEUMANN, MOORE };

    std::vector<Element> elements;
    size_t size_x = 0, size_y = 0;

    void resize(int x, int y);
    int get_element_id(int x, int y) const;
    Element& get_element(int x, int y);
    Element get_element(int x, int y) const;
    std::vector<int> get_neighbourhood(const int id, const int size, const Neighbourhood type) const;
};

} // namespace wg
