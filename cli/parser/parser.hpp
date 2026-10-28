#pragma once

#include <iostream>
#include <string_view>
#include <vector>

namespace wg {

int parse_args(std::vector<std::string_view> args, std::ostream& out, std::ostream& err);

}  // namespace wg