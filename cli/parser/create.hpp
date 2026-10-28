#pragma once

#include <string>

#include "command.hpp"

namespace wg {

class Create : public Command {
   public:
    Create();
    ~Create() = default;

    virtual int parse(std::vector<std::string> args) override;
    virtual std::string help() override;
};

}  // namespace wg
