#include "worldgen.hpp"

namespace wg {

WorldGen::WorldGen() {}

void WorldGen::create_map(unsigned int x, unsigned int y) {
    arena.resize(x, y);
    arena.add_noise();
}

Arena WorldGen::get_arena() const { return arena; }

}  // namespace wg