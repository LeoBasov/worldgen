#pragma once

#include "command.hpp"

namespace wg {

class Create : public Command {
   public:
    Create();
    ~Create() = default;

    virtual int parse(std::vector<std::string_view> args) override;
    virtual void help(std::ostream& out) override;
};

}  // namespace wg
