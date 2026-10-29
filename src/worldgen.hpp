#pragma once

#include "./data/arena.hpp"
#include "./io/exporter.hpp"

namespace wg {

class WorldGen {
   public:
    WorldGen();
    ~WorldGen() = default;

    void create_map(unsigned int x, unsigned int y);
    Arena get_arena() const;
    bool export2bmp() const;

   private:
    Arena arena;
    Exporter exporter;
};

}  // namespace wg