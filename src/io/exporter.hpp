#pragma once

#include "../data/arena.hpp"
#include "bmp_export.hpp"

namespace wg {

class Exporter {
   public:
    Exporter(/* args */);
    ~Exporter() = default;

    bool export2bmp(const Arena& arena) const;

   private:
    /* data */
};

}  // namespace wg
