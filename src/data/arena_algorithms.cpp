#include "arena_algorithms.hpp"

namespace wg {

std::vector<int> get_neighbour_ids(const int id, const int size_x, const int size_y) {
    const int x = id % size_y;
    const int y = id / size_y;

    const bool right_condition = x < size_x - 1;
    const bool up_condition = y > 0;
    const bool left_condition = x > 0;
    const bool down_condition = y < size_y - 1;

    const int id_right = right_condition ? size_x * y + x + 1 : id;
    const int id_right_up = right_condition && up_condition ? size_x * (y - 1) + x + 1 : id;
    const int id_up = left_condition ? size_x * (y - 1) + x : id;
    const int id_up_left = up_condition && left_condition ? size_x * (y - 1) + x - 1 : id;
    const int id_left = left_condition ? size_x * y + x - 1 : id;
    const int id_left_dowd = left_condition && down_condition ? size_x * (y + 1) + x - 1 : id;
    const int id_down = down_condition ? size_x * (y + 1) + x : id;
    const int id_down_right = down_condition && right_condition ? size_x * (y + 1) + x + 1 : id;

    return {id_right, id_right_up, id_up, id_up_left, id_left, id_left_dowd, id_down, id_down_right};
}

}  // namespace wg