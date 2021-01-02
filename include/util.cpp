// File: board.cpp

#include "board.hpp"
#include "move.hpp"

namespace chess {

// for testing purposes
unsigned long performance_test(board& b, board_history& bh, int depth, color turn) {
  turn = flip_turn(turn);
  if (depth == 0) return 1;
  unsigned long leafs = 0;
  for (move m: valid_moves(b, turn)) {
    if (b.get(m.to).pce == piece::king || b.get(m.to).pce == piece::king_castle) {
      ++leafs;
      continue;
    }
    do_move(m, b, bh);
    leafs += performance_test(b, bh, depth - 1, turn);
    undo_move(b, bh);
  }
  return leafs;
}

} // namespace chess
