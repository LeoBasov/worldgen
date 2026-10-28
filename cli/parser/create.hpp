#pragma once

#include <string>

#include "command.hpp"

namespace wg {

class Create : public Command {
   public:
    Create(std::shared_ptr<Arena> arena);
    ~Create() = default;

    virtual int parse(std::vector<std::string> args, std::ostream& out, std::ostream& err) override;
    virtual std::string help() override;
};

}  // namespace wg
