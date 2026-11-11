#pragma once

#include "./data/config.hpp"
#include "./data/old_arena.hpp"
#include "./io/exporter.hpp"

namespace wg {

class WorldGen {
   public:
    WorldGen();
    ~WorldGen() = default;

    void set_config(Config _config);
    void create_map();
    OldArena get_arena() const;
    bool export2bmp() const;

   private:
    Config config;
    OldArena arena;
    Exporter exporter;
};

}  // namespace wg