#pragma once

#include "../arena/field.hpp"
#include "../data/config.hpp"
#include "bmp_export.hpp"

namespace wg {

class Exporter {
   public:
    Exporter(/* args */);
    ~Exporter() = default;

    bool export_elevation(const Field<double>& elevation, const Field<bool>& rivers, const Field<bool>& lakes,
                          const Field<bool>& trapped, const std::string file_name) const;
    bool export_rho(const Field<double>& rho, const std::string file_name) const;

   private:
    std::vector<double> calc_rgb(double elevation, bool is_river, bool is_lake, bool is_trapped) const;
};

}  // namespace wg
