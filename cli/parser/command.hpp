#pragma once

#include <string>
#include <vector>

namespace wg {

class Command {
   public:
    Command();
    ~Command() = default;

    virtual int parse(std::vector<std::string> args) = 0;
    virtual std::string help() = 0;
};

}  // namespace wg
