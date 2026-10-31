#pragma once

#include <string>

namespace wg {

struct Config {
    struct Map {
        int size_x = 256;
        int size_y = 256;
    };

    struct Noise {
        double frequency = 5.0;
        int seed = 123456789;
    };

    struct Hydrography {
        int n_rivers = 20;
        double min_elevation = 0.7;
        double max_elevation = 0.9;
        double sea_level = 0.3;
    };

    struct Export {
        std::string file_name = "elevation.bmp";
    };

    Map map;
    Noise noise;
    Hydrography hydrography;
    Export exports;
};

}  // namespace wg
