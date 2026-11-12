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
    // std::cout << "attractiveness factor and gradient calculation" << std::endl;
    // arena.calc_attractiveness_factor(config.hydrography);
}

Arena WorldGen::get_arena() const { return arena; }

bool WorldGen::export2bmp() const { return exporter.export2bmp(arena, config.exports); }

}  // namespace wg