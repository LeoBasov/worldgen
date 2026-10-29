#include <iostream>
#include <string>
#include <vector>

// #include "parser/parser.hpp"
#include "../src/worldgen.hpp"

/*int main(int argc, char** argv) {
    try {
        wg::Parser parser = wg::Parser();
        std::vector<std::string> args;
        args.reserve(static_cast<std::size_t>(argc > 1 ? argc - 1 : 0));

        for (int i = 1; i < argc; ++i) {
            args.emplace_back(argv[i]);
        }

        return parser.parse_args(args, std::cout, std::cerr);

    } catch (std::exception e) {
        std::cout << "error: " << e.what() << std::endl;
        return 3;
    }
}*/

int main(int argc, char** argv) {
    wg::Config config;
    wg::WorldGen worldgen;

    worldgen.create_map();
    worldgen.export2bmp();
}