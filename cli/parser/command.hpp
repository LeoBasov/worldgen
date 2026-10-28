#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "../../src/data/arena.hpp"

namespace wg {

class Command {
   public:
    Command(std::shared_ptr<Arena> arena);
    ~Command() = default;

    virtual int parse(std::vector<std::string> args, std::ostream& out, std::ostream& err) = 0;
    virtual std::string help() = 0;

   protected:
    std::shared_ptr<Arena> arena_;
};

}  // namespace wg
