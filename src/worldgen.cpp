#include "worldgen.hpp"

namespace wg {

WorldGen::WorldGen() {}

void WorldGen::set_config(Config _config) { config = _config; }

void WorldGen::create_map() {
    std::cout << "--------------------------------------------------------------" << std::endl;
    std::cout << "started map creation" << std::endl;
    arena.resize(config.map.size_x, config.map.size_y);
    std::cout << "noise generation and gradient calculation" << std::endl;
    arena.seed = config.noise.seed;
    arena.add_noise(config.noise.frequency);
    std::cout << "river and lake creation" << std::endl;
    arena.create_rivers(config.hydrography.n_rivers, config.hydrography.min_height, config.hydrography.max_height,
                        config.hydrography.sea_level);
    std::cout << "solver call" << std::endl;
    solver.solve(arena, config.hydrography.sea_level);
}

Arena WorldGen::get_arena() const { return arena; }

bool WorldGen::export2bmp() const {
    bool ret_value = true;
    for (const auto& type : config.exports.types) {
        switch (type) {
            case Config::Export::ELEVATION:
                ret_value = ret_value && exporter.export_elevation(arena.elevation.value, arena.rivers, arena.lakes,
                                                                   arena.trapped, "elevation.bmp");
                break;
            case Config::Export::RHO:
                ret_value = ret_value && exporter.export_rho(solver.rho.value, "rho.bmp");
                break;
            default:
                ret_value = ret_value && false;
                break;
        }
    }

    return ret_value;
}

}  // namespace wg