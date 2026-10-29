#pragma once

#include "./data/arena.hpp"
#include "./data/config.hpp"
#include "./io/exporter.hpp"

namespace wg {

class WorldGen {
   public:
    WorldGen();
    ~WorldGen() = default;

    void set_config(Config _config);
    void create_map();
    Arena get_arena() const;
    bool export2bmp() const;

   private:
    Config config;
    Arena arena;
    Exporter exporter;
};

}  // namespace wg