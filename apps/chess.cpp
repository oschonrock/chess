/*
    // File: main.cpp

    TODO: don't allow moves that leaves king in check, and count check-mate as victory. Hence, perft
   will be off by a few.
    TODO: prompt pawn promotion, instead of the default queen.
    TODO: handle cases of no valid moves
*/

#include "ai.hpp"
#include "board.hpp"
#include "gui.hpp"
#include "move.hpp"
#include "util.hpp"
#include <ctime>
#include <iostream>

using namespace chess; // NOLINT

int main() {
  std::cout << "\nChesscpu 1.0\n\n";
  std::cout << "Commands:\nyxyx: fromto move.\n0: regret move (last AI move will be reverted as "
               "well).\n1: change color (AI will make this move)\n2: exit.\n\n";
  board         b;
  b.init();

  color turn     = color::white;
  color ai_color = color::black;

  bool ai_has_king    = true;
  bool human_has_king = true;

  // return run_perf_test(b);
  // return run_ai_test(b, turn);

  move mv;
  while (ai_has_king && human_has_king) {
    print_board(b, mv);
    if (turn == ai_color)
      ai_move(b, turn, 7, mv);
    else
      mv = read_move(valid_moves(b, turn), turn);

    if (mv.from_ == 0) {
      b.undo_move();
      b.undo_move();
      continue;
    }
    if (mv.from_ == 1) {
      ai_color = flip_turn(ai_color);
      continue;
    }
    if (mv.from_ == 2) {
      human_has_king = false;
      break;
    }

    do_move(mv, b);
    turn = flip_turn(turn);

    ai_has_king = human_has_king = false;
    for (size_t i = 21; i < 99; ++i) { // board.h about these magic numbers
      if (b.get(i).pce == piece::king || b.get(i).pce == piece::king_castle) {
        if (b.get(i).pce_color == ai_color) {
          ai_has_king = true;
        } else {
          human_has_king = true;
        }
      }
    }
  }
  print_board(b, mv);

  std::cout << "\n\n";
  if (!human_has_king) std::cout << "You lose.";
  if (!ai_has_king) std::cout << "You win.";
  std::cout << "\n\nBye!\n\n";
}
