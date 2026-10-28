#pragma once

#include <string_view>
#include <vector>

namespace wg {

class Command {
   public:
    Command();
    ~Command() = default;

    virtual int parse(std::vector<std::string_view> args) = 0;
};

}  // namespace wg
