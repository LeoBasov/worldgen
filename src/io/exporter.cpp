#include "exporter.hpp"

namespace wg {

Exporter::Exporter(/* args */) {}

bool Exporter::export2bmp(const Arena& arena) const {
    Image image{arena.size_x, arena.size_y};

    for (std::int32_t y = 0; y < image.height(); ++y) {
        for (std::int32_t x = 0; x < image.width(); ++x) {
            const RGB color(arena.field.at(y).at(x).elevation);
            image.set(x, y, color);
        }
    }

    return image.saveBMP("elevation.bmp");
}

}  // namespace wg
