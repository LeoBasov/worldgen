#pragma once

#include "../data/arena.hpp"
#include "../data/config.hpp"
#include "bmp_export.hpp"

namespace wg {

class Exporter {
   public:
    Exporter(/* args */);
    ~Exporter() = default;

    bool export2bmp(const Arena& arena, const Config::Export exports) const;

   private:
    std::vector<double> calc_rgb(double elevation, bool is_river, bool is_lake, bool is_trapped) const;
};

}  // namespace wg
