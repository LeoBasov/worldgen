#pragma once

#include <vector>

namespace wg
{

struct Element {
    double elevation = 0.0;
    bool is_river = false;
    bool is_lake = false;
    bool is_trapped = false;
};

struct Field{
    std::vector<Element> elements;
    size_t size_x = 0, size_y = 0;

    void resize(int x, int y);
    Element& get_element(int x, int y);
    Element get_element(int x, int y) const;
};

} // namespace wg
