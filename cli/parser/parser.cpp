#include "parser.hpp"

namespace wg {

int Parser::parse_args(std::vector<std::string> args, Config& config, std::ostream& out, std::ostream& err) {
    for (int i = 0; i < args.size(); i++) {
        std::string str = args.at(i);

        if (str == "--resolution") {
            std::vector<std::string> params;

            while (i < (args.size() - 1) && args.at(i + 1).substr(0, 2) != std::string("--")) {
                str = args.at(++i);
                params.push_back(args.at(i));
            }

            if (params.size() == 2) {
                out << "setting size_x to: " << params.at(0) << std::endl;
                config.map.size_x = std::stoi(params.at(0));

                out << "setting size_y to: " << params.at(1) << std::endl;
                config.map.size_y = std::stoi(params.at(1));
            } else {
                throw std::runtime_error("wrong number of arguments");
            }
        }
    }

    return 0;
}

}  // namespace wg