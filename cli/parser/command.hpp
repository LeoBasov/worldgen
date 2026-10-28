#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../../src/data/arena.hpp"

namespace wg {

class Command {
   public:
    Command(std::shared_ptr<Arena> arena);
    ~Command() = default;

    virtual int parse(std::vector<std::string> args) = 0;
    virtual std::string help() = 0;

   private:
    std::shared_ptr<Arena> arena_;
};

}  // namespace wg
