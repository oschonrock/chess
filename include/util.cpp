// File: board.cpp

#include "ai.hpp"
#include "board.hpp"
#include "move.hpp"

namespace chess {

// for testing purposes

unsigned long performance_test(board& b, board_history& bh, int depth, color turn) {
  turn = flip_turn(turn);
  if (depth == 0) return 1;
  unsigned long leafs = 0;
  for (move m: valid_moves(b, turn)) {
    if (b.get(m.to_).pce == piece::king || b.get(m.to_).pce == piece::king_castle) {
      ++leafs;
      continue;
    }
    do_move(m, b, bh);
    leafs += performance_test(b, bh, depth - 1, turn);
    undo_move(b, bh);
  }
  return leafs;
}

int run_perf_test(board& b, board_history& bh) {

  unsigned long t = time(nullptr);
  std::cerr << "DEBUG: Perft(5) = (expecting 4897256): "
            << performance_test(b, bh, 5, color::black);
  t = time(nullptr) - t;
  std::cout << "\nTime " << t << "\n";
  return 0;
}

int run_ai_test(board& b, board_history& bh, color turn) {
  move          mv;
  unsigned long t = time(nullptr);
  ai_move(b, bh, turn, 7, mv);
  t = time(nullptr) - t;
  std::cout << "\nAI Time: " << t << "\n";
  return 0;
}

} // namespace chess
