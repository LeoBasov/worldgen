#pragma once

#include "./arena/arena.hpp"
#include "./data/config.hpp"
#include "./io/exporter.hpp"
#include "./solver/solver.hpp"

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
    Solver solver;
    Exporter exporter;
};

}  // namespace wg