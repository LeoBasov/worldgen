#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "command.hpp"

namespace wg {

class Parser {
   public:
    Parser();
    ~Parser() = default;

    int parse_args(std::vector<std::string> args, std::ostream& out, std::ostream& err);

   private:
    std::map<std::string, std::shared_ptr<Command>> map;

    void write_header(std::ostream& out);
    void write_footer(std::ostream& out);
    void write_help(std::ostream& out);
};

}  // namespace wg