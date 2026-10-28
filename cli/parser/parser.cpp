#include "parser.hpp"

#include "create.hpp"

namespace wg {

Parser::Parser() { map.insert({"create", std::make_shared<Create>()}); }

int Parser::parse_args(std::vector<std::string_view> args, std::ostream& out, std::ostream& err) {
    write_header(out);

    for (auto arg : args) {
        if (arg.substr(0, 2) == std::string("--")) {
            out << arg.substr(2) << " ";
        }
    }

    out << std::endl;

    write_footer(out);

    return 0;
}

void Parser::write_header(std::ostream& out) {
    out << "cli app for world generation" << std::endl;
    out << "============================" << std::endl;
}

void Parser::write_footer(std::ostream& out) { out << "============================" << std::endl; }

}  // namespace wg