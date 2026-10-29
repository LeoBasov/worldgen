#include "create.hpp"

namespace wg {

Create::Create(std::shared_ptr<WorldGen> worldgen) : Command(worldgen) {}

int Create::parse(std::vector<std::string> args, std::ostream& out, std::ostream& err) {
    const int x = std::stoi(args.at(0));
    const int y = std::stoi(args.at(1));

    worldgen->create_map(x, y);
    worldgen->export2png();

    const Arena arena = worldgen->get_arena();

    for (int i = 0; i < y; i++) {
        for (int k = 0; k < x; k++) {
            out << arena.field.at(i).at(k).elevation << " ";
        }
        out << std::endl;
    }

    return 0;
}

std::string Create::help() { return "create"; }

}  // namespace wg