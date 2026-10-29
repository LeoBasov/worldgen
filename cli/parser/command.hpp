#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "../../src/worldgen.hpp"

namespace wg {

class Command {
   public:
    Command(std::shared_ptr<WorldGen> arena);
    ~Command() = default;

    virtual int parse(std::vector<std::string> args, std::ostream& out, std::ostream& err) = 0;
    virtual std::string help() = 0;

   protected:
    std::shared_ptr<WorldGen> worldgen;
};

}  // namespace wg
