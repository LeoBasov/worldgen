#include "create.hpp"

namespace wg {

Create::Create(std::shared_ptr<Arena> arena) : Command(arena) {}

int Create::parse(std::vector<std::string> args, std::ostream& out, std::ostream& err) {
    const int x = std::stoi(args.at(0));
    const int y = std::stoi(args.at(1));

    arena_->resize(x, y);

    for (int i = 0; i < y; i++) {
        for (int k = 0; k < x; k++) {
            out << arena_->field.at(i).at(k).elevation << " ";
        }
        out << std::endl;
    }

    return 0;
}

std::string Create::help() { return "create"; }

}  // namespace wg