#include "worldgen.hpp"

namespace wg {

WorldGen::WorldGen() {}

void WorldGen::create_map(unsigned int x, unsigned int y) {
    arena.resize(x, y);
    arena.add_noise();
    arena.create_rivers(0.7, 0.9, 20);
}

Arena WorldGen::get_arena() const { return arena; }

bool WorldGen::export2bmp() const { return exporter.export2bmp(arena); }

}  // namespace wg