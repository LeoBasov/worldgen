#include "exporter.hpp"

namespace wg {

Exporter::Exporter(/* args */) {}

std::vector<double> Exporter::calc_rgb(double elevation, bool is_river, bool is_lake, bool is_trapped) const {
    std::vector<double> rgb = {0.0, 0.0, 0.0};
    const double water_level = 0.3;
    const double grassland_level = 0.4;
    const double aridland_level = 0.7;

    if (is_trapped) {
        return {1, 0, 0};
    }

    if (is_river) {
        return {0, 0, 1};
    } else if (is_lake) {
        return {0, 1, 1};
    }

    // 0. Water level
    if(elevation < water_level){
        double r = 0.10;
        double g = 0.30;
        double b = 0.70;

        rgb = {r, g, b};
    }
    // 1. Blue to Green
    else if (elevation < grassland_level) {
        double t = (elevation - water_level) / (grassland_level - water_level);
        double r = 0.10 * (1 - t) + 0.20 * t;
        double g = 0.30 * (1 - t) + 0.60 * t;
        double b = 0.70 * (1 - t) + 0.20 * t;

        rgb = {r, g, b};
    }
    // 2. Green to Brown
    else if (elevation < aridland_level) {
        double t = (elevation - grassland_level) / (aridland_level - grassland_level);
        double r = 0.20 * (1 - t) + 0.55 * t;
        double g = 0.60 * (1 - t) + 0.40 * t;
        double b = 0.20 * (1 - t) + 0.25 * t;

        rgb = {r, g, b};
    }
    // 3. Brown to White
    else {
        double t = (elevation - aridland_level) / (1.0 - aridland_level);
        double r = 0.55 * (1 - t) + 1.00 * t;
        double g = 0.40 * (1 - t) + 1.00 * t;
        double b = 0.25 * (1 - t) + 1.00 * t;

        rgb = {r, g, b};
    }

    return rgb;
}

bool Exporter::export2bmp(const Arena& arena, const Config::Export exports) const {
    Image image{arena.get_size_x(), arena.get_size_y()};

    for (std::int32_t y = 0; y < image.height(); ++y) {
        for (std::int32_t x = 0; x < image.width(); ++x) {
            const std::vector<double> rgb_val =
                calc_rgb(arena.get_element(x, y).elevation, arena.get_element(x, y).is_river,
                         arena.get_element(x, y).is_lake, arena.get_element(x, y).is_trapped);
            const RGB color(rgb_val.at(0), rgb_val.at(1), rgb_val.at(2));
            image.set(x, y, color);
        }
    }

    return image.saveBMP(exports.file_name);
}

}  // namespace wg
