#include "exporter.hpp"

namespace wg {

Exporter::Exporter(/* args */) {}

std::vector<double> Exporter::calc_rgb(double elevation, bool is_river) const {
    std::vector<double> rgb = {0.0, 0.0, 0.0};

    if (is_river) {
        return rgb;
    }

    // 1. Blue to Green
    if (elevation < 0.3) {
        double t = (elevation - 0.0) / 0.3;
        double r = 0.10 * (1 - t) + 0.20 * t;
        double g = 0.30 * (1 - t) + 0.60 * t;
        double b = 0.70 * (1 - t) + 0.20 * t;

        rgb = {r, g, b};
    }
    // 2. Green to Brown
    else if (elevation < 0.7) {
        double t = (elevation - 0.3) / 0.4;
        double r = 0.20 * (1 - t) + 0.55 * t;
        double g = 0.60 * (1 - t) + 0.40 * t;
        double b = 0.20 * (1 - t) + 0.25 * t;

        rgb = {r, g, b};
    }
    // 3. Brown to White
    else {
        double t = (elevation - 0.7) / 0.3;
        double r = 0.55 * (1 - t) + 1.00 * t;
        double g = 0.40 * (1 - t) + 1.00 * t;
        double b = 0.25 * (1 - t) + 1.00 * t;

        rgb = {r, g, b};
    }

    return rgb;
}

bool Exporter::export2bmp(const Arena& arena) const {
    Image image{arena.get_size_x(), arena.get_size_y()};

    for (std::int32_t y = 0; y < image.height(); ++y) {
        for (std::int32_t x = 0; x < image.width(); ++x) {
            const std::vector<double> rgb_val =
                calc_rgb(arena.get_element(x, y).elevation, arena.get_element(x, y).is_river);
            const RGB color(rgb_val.at(0), rgb_val.at(1), rgb_val.at(2));
            image.set(x, y, color);
        }
    }

    return image.saveBMP("elevation.bmp");
}

}  // namespace wg
