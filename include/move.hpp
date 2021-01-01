/*
    File: move.h

    Move utils and move history utils.
    Assumes that a history list and the board it's used with are consistent.

    Boardhistory:
    A move event consists of a series of board changes and a "move done",
    namely BoardChange.where == (boardsize = move_done), which is pushed last for each move event.

    Author: Ken Rouwas
    Date 2017-09-14
*/

#pragma once

#include <list>
#include <vector>

#include "board.hpp"

namespace chess {

const size_t move_done = board_size;

struct move {
  ssize_t from, to;
  move(ssize_t, ssize_t);
  move();
};

struct board_change {
  size_t where;
  square old_square;
};

using board_history = std::list<board_change>;
using move_set      = std::vector<move>;

void undo_move(board&, board_history&);

/* Castling is identified by king move to its castling destination if permitted.
   Pawn promotion move deduced.
   All other moves are unconditional moves.
*/
void do_move(move, board&, board_history&, piece pawn_promotion = piece::queen);

move_set valid_moves(board&, color turn); // This is the move generator

} // namespace chess

