#pragma once

#include "move.hpp"

namespace chess {

constexpr int ai_max_depth = 7;

int ai_move(board& b, color turn, int depth, move& best_move, int alpha = -400,
            int beta = 400);

} // namespace chess
