#pragma once

#include "../data/config.hpp"
#include "../data/old_arena.hpp"
#include "bmp_export.hpp"

namespace wg {

class Exporter {
   public:
    Exporter(/* args */);
    ~Exporter() = default;

    bool export2bmp(const OldArena& arena, const Config::Export exports) const;

   private:
    std::vector<double> calc_rgb(double elevation, bool is_river, bool is_lake, bool is_trapped) const;
};

}  // namespace wg
