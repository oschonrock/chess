// File: board.cpp

#include "board.hpp"

namespace chess {

// for testing purposes
unsigned long perft(board& b, board_history& h, int depth, color turn) {
  turn = flip_turn(turn);
  if (depth == 0) return 1;
  unsigned long leafs = 0;
  for (move m: valid_moves(b, turn)) {
    if (b.get(m.to).pce == piece::king || b.get(m.to).pce == piece::king_castle) {
      ++leafs;
      continue;
    }
    do_move(m, b, h);
    leafs += perft(b, h, depth - 1, turn);
    undo_move(b, h);
  }
  return leafs;
}

} // namespace chess
