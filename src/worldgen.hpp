#pragma once

#include "./data/arena.hpp"
#include "./io/exporter.hpp"

namespace wg {

class WorldGen {
   public:
    WorldGen();
    ~WorldGen() = default;

   private:
    Arena areana;
    Exporter exporter;
};

}  // namespace wg