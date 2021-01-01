#pragma once

#include "move.hpp"

namespace chess {

int ai_move(board& b, board_history& bh, color turn, int depth, move& _bm, int alpha = -400,
            int beta = 400);
} // namespace chess
