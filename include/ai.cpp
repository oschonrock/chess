#include "ai.hpp"
#include "board.hpp"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <random>

namespace chess {

// initialise piece value lookup array at compile time for performance
// about a 16% gain over the switch statement at runtime
static constexpr std::array<int, static_cast<int>(piece::out_of_board) -
                                     static_cast<int>(piece::king) + 1>
    piece_values = [] {
      auto a = decltype(piece_values){};
      for (int i = static_cast<int>(piece::king); i <= static_cast<int>(piece::out_of_board); ++i) {
        int v = 0;
        switch (static_cast<piece>(i)) {
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
        a[i] = v;
      }
      return a;
    }();

static std::random_device rnd_seed;               // NOLINT
static std::minstd_rand   rnd_engine(rnd_seed()); // NOLINT faster than mt

static int evaluate_leaf(const board& b) {
  int sum = 0;
  for (size_t i = 21; i < 99; ++i) {
    if (b.get(i).piece_color_ == color::none) continue;
    int c = b.get(i).piece_color_ == color::white ? 1 : -1;
    int v = piece_values[static_cast<int>(b.get(i).piece_)];
    sum += c * v;
  }
  return sum;
}

int ai_move(board& b, color turn, int depth, move& best_move, int alpha, int beta) {
  move bm;
  int  best_score = turn == color::white ? -300 : 300;
  if (depth == 0) return evaluate_leaf(b);

  move_set vmoves = valid_moves(b, turn);
  std::shuffle(vmoves.begin(), vmoves.end(), rnd_engine);

  int progress = 0;
  for (const move& m: vmoves) {
    if (depth == ai_max_depth)
      std::cout << "\r" << ++progress << "/" << vmoves.size() << std::flush;

    if (b.get(m.to_).piece_ == piece::king || b.get(m.to_).piece_ == piece::king_castle) {
      best_score = turn == color::white ? 200 + depth : -200 - depth;
      bm         = m;
      break;
    }

    do_move(m, b);
    int new_score = ai_move(b, flip_turn(turn), depth - 1, best_move, alpha, beta);
    b.undo_move();

    if ((turn == color::white && new_score > best_score) ||
        (turn == color::black && new_score < best_score)) {
      bm         = m;
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
  best_move = bm;
  return best_score;
}
} // namespace chess
