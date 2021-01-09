/**
 * File: move.h
 *
 * Move utils and move history utils.
 * Assumes that a history list and the board it's used with are consistent.
 *
 * Boardhistory:
 * A move event consists of a series of board changes and a "move done",
 * namely BoardChange.where == (boardsize = move_done), which is pushed last for each move event.
 *
 * Author: Ken Rouwas
 * Date 2017-09-14
 */
#pragma once

#include <list>
#include <vector>

#include "board.hpp"

namespace chess {

struct move {
  move() = default;
  move(ssize_t from, ssize_t to) : from_(from), to_(to) {}

  ssize_t from_ = 0;
  ssize_t to_   = 0;

  friend std::ostream& operator<<(std::ostream& os, const move& mv)  {
    os << where_to_str(mv.from_) << where_to_str(mv.to_);
    return os;
  }

};

using move_set = std::vector<move>;

/* Castling is identified by king move to its castling destination if permitted.
   Pawn promotion move deduced.
   All other moves are unconditional moves.
*/
void do_move(move, board&, piece pawn_promotion = piece::queen);

move_set valid_moves(board&, color turn); // This is the move generator

} // namespace chess
