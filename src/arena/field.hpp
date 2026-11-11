#pragma once

#include <cmath>
#include <stdexcept>
#include <vector>

namespace wg {

using namespace std;

int a = 0;

struct Field {
    enum HoodType { NEUMANN, MOORE };

    vector<double> elements;
    int size_x = 0, size_y = 0, size = 0;

    void resize(int x, int y) {
        size_x = x;
        size_y = y;
        size = size_x * size_y;
        elements.resize(x * y);
    }

    int elem_id(int x, int y) const {
        if (x < 0 || x >= size_x || y < 0 || y >= size_y) {
            return -1;
        } else {
            return size_x * y + x;
        }
    }

    pair<int, int> coords(int id) const {
        if (id < 0 || id > elements.size()) {
            return {-1, -1};
        } else {
            return {id % size_x, id / size_x};
        }
    }

    double& operator()(int id) { return elements.at(id); }

    const double& operator()(int id) const { return elements.at(id); }

    double& operator()(int x, int y) { return operator()(elem_id(x, y)); }

    const double& operator()(int x, int y) const { return operator()(elem_id(x, y)); }

    vector<int> neighbourhood(const int id, const int nsize, const HoodType type) const {
        const int length = 2 * nsize + 1;
        const auto [x, y] = coords(id);
        vector<int> neighbourhood(length * length, -1);
        int counter = 0;

        if (nsize < 1) {
            throw domain_error("zero or negative neighbourhood size");
        }

        switch (type) {
            case HoodType::NEUMANN: {
                for (int iy = 0; iy < length; iy++) {
                    for (int ix = 0; ix < length; ix++) {
                        const int neighbourhood_id = elem_id(x + ix - nsize, y + iy - nsize);

                        if ((abs(ix - nsize) + abs(iy - nsize)) > nsize) {
                            neighbourhood.at(counter++) = -1;
                        } else {
                            neighbourhood.at(counter++) = neighbourhood_id;
                        }
                    }
                }
                break;
            }
            case HoodType::MOORE: {
                for (int iy = 0; iy < length; iy++) {
                    for (int ix = 0; ix < length; ix++) {
                        const int neighbourhood_id = elem_id(x + ix - nsize, y + iy - nsize);

                        neighbourhood.at(counter++) = neighbourhood_id;
                    }
                }
                break;
            }
            default: {
                throw domain_error("unrecognized neighbourhood type");
                break;
            }
        }

        return neighbourhood;
    }
};

}  // namespace wg
