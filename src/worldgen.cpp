#include "worldgen.hpp"

namespace wg {

WorldGen::WorldGen() {}

void WorldGen::set_config(Config _config) { config = _config; }

void WorldGen::create_map() {
    arena.resize(config.map);
    arena.add_noise(config.noise);
    arena.create_rivers(config.hydrography, config.noise);
}

Arena WorldGen::get_arena() const { return arena; }

bool WorldGen::export2bmp() const { return exporter.export2bmp(arena, config.exports); }

}  // namespace wg