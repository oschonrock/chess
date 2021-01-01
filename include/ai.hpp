#pragma once

#include "move.hpp"

namespace chess {

int ai_move(board& b, board_history& bh, color turn, int depth, move& best_move, int alpha = -400,
            int beta = 400);
} // namespace chess
