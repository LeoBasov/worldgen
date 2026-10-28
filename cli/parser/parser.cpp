#include "parser.hpp"

#include "create.hpp"

namespace wg {

Parser::Parser() { map.insert({"create", std::make_shared<Create>()}); }

int Parser::parse_args(std::vector<std::string> args, std::ostream& out, std::ostream& err) {
    write_header(out);

    if (args.size() == 0) {
        write_help(out);
    }

    for (int i = 0; i < args.size(); i++) {
        std::string str = args.at(i);

        if (str.substr(0, 2) == std::string("--")) {
            if (auto search = map.find(str.substr(2)); search != map.end()) {
                std::vector<std::string> params;

                out << "executing " << search->first;

                while (i < (args.size() - 1) && args.at(i + 1).substr(0, 2) != std::string("--")) {
                    str = args.at(++i);
                    params.push_back(args.at(i));
                    out << " " << args.at(i);
                }

                out << std::endl;
                search->second->parse(params);
            }
        }
    }

    write_footer(out);

    return 0;
}

void Parser::write_header(std::ostream& out) {
    out << "cli app for world generation" << std::endl;
    out << "============================" << std::endl;
}

void Parser::write_footer(std::ostream& out) { out << "============================" << std::endl; }

void Parser::write_help(std::ostream& out) {
    for (const auto& [key, value] : map) {
        out << "--" << key << "    " << value->help() << std::endl;
    }
}

}  // namespace wg