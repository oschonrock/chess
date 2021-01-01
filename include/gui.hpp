/*
    File: gui.h

    Interactive graphical interface of a chess board.

    This is temporarily a quick and dirty solution.

    Author: Ken Rouwas
    Date 2017-09-15
*/

#pragma once

#include "board.hpp"
#include "move.hpp"
#include <vector>

namespace chess {

void print_board(const board&, move last_move);

/* Returns when one of the provided valid moves is read */
move read_move(const move_set& valid_moves, color turn);

} // namespace chess
