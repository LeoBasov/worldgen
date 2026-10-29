#include "arena.hpp"

#include <algorithm>
#include <iostream>

namespace wg {

Arena::Arena() {}

void Arena::resize(int x, int y) {
    size_x = x;
    size_y = y;
    field = std::vector<Element>(size_x * size_y);
}

void Arena::add_noise(double frequency) {
    const siv::PerlinNoise::seed_type seed = 123456u;
    const siv::PerlinNoise perlin{seed};

    for (int i = 0; i < field.size(); i++) {
        const int x = i % size_y;
        const int y = i / size_y;
        const double fx = frequency / size_x;
        const double fy = frequency / size_y;

        field.at(i).elevation = perlin.octave2D_01((x * fx), (y * fy), 4);
    }
}

void Arena::create_rivers(double elev_min, double elev_max, int nseeds) {
    std::vector<size_t> ids(field.size());
    std::iota(ids.begin(), ids.end(), 0);  // 0, 1, 2, ..., n-1
    std::mt19937 rng(std::random_device{}());
    std::shuffle(ids.begin(), ids.end(), rng);
    int counter = 0;

    for (int i = 0; i < field.size(); i++) {
        const size_t id = ids.at(i);
        const double elevation = field.at(id).elevation;
        const bool is_river = field.at(id).is_river;

        if (!is_river && elevation > elev_min && elevation < elev_max) {
            bool is_flowing = true;
            int idx = id;
            field.at(id).is_river = true;
            counter++;

            while (is_flowing) {
                const int x = idx % size_y;
                const int y = idx / size_y;
                const int idr = x < size_x - 1 ? size_x * y + x + 1 : idx;
                const int idl = x > 0 ? size_x * y + x - 1 : idx;
                const int idu = y < size_y - 1 ? size_x * (y + 1) + x : idx;
                const int idd = y > 0 ? size_x * (y - 1) + x : idx;
                const double dxr = field.at(idr).elevation - field.at(idx).elevation;
                const double dxl = field.at(idl).elevation - field.at(idx).elevation;
                const double dxu = field.at(idu).elevation - field.at(idx).elevation;
                const double dxd = field.at(idd).elevation - field.at(idx).elevation;
                std::vector<int> idss{idr, idl, idu, idd};
                std::vector<double> grads{dxr, dxl, dxu, dxd};
                int idx_new = -1;

                vector<size_t> idxx(grads.size());
                iota(idxx.begin(), idxx.end(), 0);
                stable_sort(idxx.begin(), idxx.end(), [&grads](size_t i1, size_t i2) { return grads[i1] < grads[i2]; });

                for (int g = 0; g < idss.size(); g++) {
                    int idxxx = idxx.at(g);

                    if (grads.at(idxxx) < 0.0 && idss.at(idxxx) != idx) {
                        idx_new = idss.at(idxxx);
                        break;
                    }
                }

                if (idx_new == -1) {
                    is_flowing = false;
                } else {
                    idx = idx_new;
                    field.at(idx).is_river = true;
                }
            }
        }

        if (counter >= nseeds) {
            break;
        }
    }
}

Element& Arena::get_element(int x, int y) { return field.at(size_x * y + x); }

Element Arena::get_element(int x, int y) const { return field.at(size_x * y + x); }

size_t Arena::get_size_x() const { return size_x; }

size_t Arena::get_size_y() const { return size_y; }

}  // namespace wg