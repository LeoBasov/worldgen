#include "exporter.hpp"

namespace wg {

Exporter::Exporter(/* args */) {}

std::vector<double> Exporter::calc_rgb(double height, bool is_river, bool is_lake, bool is_trapped) const {
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
    if (height < water_level) {
        double r = 0.10;
        double g = 0.30;
        double b = 0.70;

        rgb = {r, g, b};
    }
    // 1. Blue to Green
    else if (height < grassland_level) {
        double t = (height - water_level) / (grassland_level - water_level);
        double r = 0.10 * (1 - t) + 0.20 * t;
        double g = 0.30 * (1 - t) + 0.60 * t;
        double b = 0.70 * (1 - t) + 0.20 * t;

        rgb = {r, g, b};
    }
    // 2. Green to Brown
    else if (height < aridland_level) {
        double t = (height - grassland_level) / (aridland_level - grassland_level);
        double r = 0.20 * (1 - t) + 0.55 * t;
        double g = 0.60 * (1 - t) + 0.40 * t;
        double b = 0.20 * (1 - t) + 0.25 * t;

        rgb = {r, g, b};
    }
    // 3. Brown to White
    else {
        double t = (height - aridland_level) / (1.0 - aridland_level);
        double r = 0.55 * (1 - t) + 1.00 * t;
        double g = 0.40 * (1 - t) + 1.00 * t;
        double b = 0.25 * (1 - t) + 1.00 * t;

        rgb = {r, g, b};
    }

    return rgb;
}

bool Exporter::export_elevation(const Field<double>& elevation, const Field<bool>& rivers, const Field<bool>& lakes,
                                const Field<bool>& trapped, const std::string file_name) const {
    Image image{elevation.size_x, elevation.size_y};

    for (std::int32_t y = 0; y < image.height(); ++y) {
        for (std::int32_t x = 0; x < image.width(); ++x) {
            const std::vector<double> rgb_val = calc_rgb(elevation(x, y), rivers(x, y), lakes(x, y), trapped(x, y));
            const RGB color(rgb_val.at(0), rgb_val.at(1), rgb_val.at(2));
            image.set(x, y, color);
        }
    }

    return image.saveBMP(file_name);
}

bool Exporter::export_rho(const Field<double>& rho, const std::string file_name) const {
    Image image{rho.size_x, rho.size_y};

    for (std::int32_t y = 0; y < image.height(); ++y) {
        for (std::int32_t x = 0; x < image.width(); ++x) {
            const RGB color(rho(x, y));
            image.set(x, y, color);
        }
    }

    return image.saveBMP(file_name);
}

}  // namespace wg
