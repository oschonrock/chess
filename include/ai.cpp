#include "ai.hpp"
#include <algorithm>
#include <iostream>
#include <random>

namespace chess {

std::random_device rnd_seed; // NOLINT
std::mt19937       rnd_engine(rnd_seed()); // NOLINT

void moveorder(move_set& ms) { std::shuffle(ms.begin(), ms.end(), rnd_engine); }

static int evaluate_leaf(const board& b) {
  int sum = 0;
  for (size_t i = 21; i < 99; ++i) {
    if (b.get(i).pce_color == color::none) continue;
    int c = b.get(i).pce_color == color::white ? 1 : -1;
    int v = 0;
    switch (b.get(i).pce) {
    case piece::pawn:
    case piece::pawn_en_passant:
      v = 1;
      break;
    case piece::rook:
    case piece::rook_castle:
      v = 5;
      break;
    case piece::knight:
    case piece::bishop:
      v = 3;
      break;
    case piece::queen:
      v = 9;
      break;
    default:
      break;
    }
    sum += c * v;
  }
  return sum;
}

int ai_move(board& b, board_history& bh, color turn, int depth, move& _bm, int alpha, int beta) {
  /*
  MoveSet valid = valid_moves(b, turn);
  _bm = valid[mt()%valid.size()];
  return 0;
  */

  move best_move;
  int  best_score = turn == color::white ? -300 : 300;
  if (depth == 0) return evaluate_leaf(b);

  move_set vmoves = valid_moves(b, turn);
  moveorder(vmoves);
  // int progress = 0; // Temporary hack to show progress
  for (move m: vmoves) {
    // if(depth == 8) // Temporary hack to show progress
    //  std::cout<<"\r"<<++progress<<"/"<<vmoves.size()<<std::flush;

    if (b.get(m.to).pce == piece::king || b.get(m.to).pce == piece::king_castle) {
      best_score = turn == color::white ? 200 + depth : -200 - depth;
      best_move  = m;
      break;
    }

    do_move(m, b, bh);
    int new_score = ai_move(b, bh, flip_turn(turn), depth - 1, _bm, alpha, beta);
    undo_move(b, bh);

    if ((turn == color::white && new_score > best_score) ||
        (turn == color::black && new_score < best_score)) {
      best_move  = m;
      best_score = new_score;

      if (turn == color::black) {
        beta = new_score;
        if (beta <= alpha) break;
      }

      if (turn == color::white) {
        alpha = new_score;
        if (alpha >= beta) break;
      }
    }
  }
  _bm = best_move;
  return best_score;
}
} // namespace chess
