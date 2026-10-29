#include <iostream>
#include <string>
#include <vector>

#include "../src/worldgen.hpp"
#include "parser/parser.hpp"

int main(int argc, char** argv) {
    try {
        wg::Config config;
        wg::WorldGen worldgen;
        wg::Parser parser;
        std::vector<std::string> args;

        args.reserve(static_cast<std::size_t>(argc > 1 ? argc - 1 : 0));

        for (int i = 1; i < argc; ++i) {
            args.emplace_back(argv[i]);
        }

        parser.parse_args(args, config, std::cout, std::cerr);

        worldgen.set_config(config);
        worldgen.create_map();
        worldgen.export2bmp();
    } catch (std::runtime_error e) {
        std::cout << "error: " << e.what() << std::endl;
        return 3;
    } catch (std::exception e) {
        std::cout << "error: " << e.what() << std::endl;
        return 3;
    }
}