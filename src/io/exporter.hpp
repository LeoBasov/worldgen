#pragma once

#include "../data/arena.hpp"
#include "png_export.hpp"

namespace wg {

class Exporter {
   public:
    Exporter(/* args */);
    ~Exporter() = default;

    bool export2png(const Arena& arena) const;

   private:
    /* data */
};

}  // namespace wg
