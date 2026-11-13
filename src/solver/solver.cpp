#include "solver.hpp"

namespace wg {

void Solver::solve(const Arena& arena, const double sea_level) {
    rho.resize(arena.size_x, arena.size_y);

    for (int i = 0; i < arena.size; i++) {
        if (arena.rivers(i) || arena.lakes(i) || arena.elevation.value(i) < sea_level) {
            rho.value(i) = 0.0;
        } else {
            rho.value(i) = arena.elevation.value(i);
        }
    }
}

}  // namespace wg