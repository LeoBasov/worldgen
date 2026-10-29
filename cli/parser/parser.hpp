#pragma once

#include <exception>
#include <iostream>
#include <vector>

#include "../../src/data/config.hpp"

namespace wg {

class Parser {
   public:
    Parser() = default;
    ~Parser() = default;

    int parse_args(std::vector<std::string> args, Config& config, std::ostream& out, std::ostream& err);
};

}  // namespace wg