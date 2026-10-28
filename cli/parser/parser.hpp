#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <string_view>
#include <vector>

#include "command.hpp"

namespace wg {

class Parser {
   public:
    Parser();
    ~Parser() = default;

    int parse_args(std::vector<std::string_view> args, std::ostream& out, std::ostream& err);

   private:
    std::map<std::string, std::shared_ptr<Command>> map;

    void write_header(std::ostream& out);
    void write_footer(std::ostream& out);
};

}  // namespace wg