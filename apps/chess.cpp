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
  board_history h;
  b.init();

  color turn     = color::white;
  color ai_color = color::black;

  bool ai_has_king    = true;
  bool human_has_king = true;

  constexpr bool run_perf_test = true;
  if (run_perf_test) {
    unsigned long t = time(nullptr);
    std::cerr << "DEBUG: Perft(5) = (expecting 4897256): " << performance_test(b, h, 5, color::black);
    t = time(nullptr) - t;
    std::cout << "\nTime " << t << "\n";
    return 0;
  }

  constexpr bool run_ai_test = false;
  if (run_ai_test) {
    move          mv;
    unsigned long t = time(nullptr);
    ai_move(b, h, turn, 7, mv);
    t = time(nullptr) - t;
    std::cout << "\nAI Time: " << t << "\n";
    return 0;
  }

  move mv{0, 0};
  while (ai_has_king && human_has_king) {
    print_board(b, mv);
    if (turn == ai_color)
      ai_move(b, h, turn, 7, mv);
    else
      mv = read_move(valid_moves(b, turn), turn);

    if (mv.from == 0) {
      undo_move(b, h);
      undo_move(b, h);
      continue;
    }
    if (mv.from == 1) {
      ai_color = flip_turn(ai_color);
      continue;
    }
    if (mv.from == 2) {
      human_has_king = false;
      break;
    }

    do_move(mv, b, h);
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
