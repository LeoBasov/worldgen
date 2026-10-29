#pragma once

namespace wg {

struct Config {
    struct Map {
        int size_x = 256;
        int size_y = 256;
        double sea_level = 0.3;
    };

    struct Noise {
        double frequency = 5.0;
    };

    struct Hydrography {
        int n_rivers = 20;
        double min_elevation = 0.7;
        double max_elevation = 0.9;
    };

    Map map;
    Noise noise;
    Hydrography hydrography;
};

}  // namespace wg
