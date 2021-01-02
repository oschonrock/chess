// File: gui.cpp
// This is quick, ugly, pragmatic, temporary.

#include "gui.hpp"
#include <cctype>
#include <iostream>
#include <map>

namespace chess {

// clang-format off
static const std::map<piece, char> sprites =
    {{piece::pawn,            'A'},
     {piece::pawn_en_passant, 'P'},
     {piece::rook,            'H'},
     {piece::rook_castle,     'H'},
     {piece::knight,          'F'},
     {piece::bishop,          'I'},
     {piece::king,            'K'},
     {piece::king_castle,     'K'},
     {piece::none,            '+'},
     {piece::out_of_board,    '#'},
     {piece::queen,           'Q'}};
// clang-format on

void print_board(const board& b, move last_move) {
  std::cout << "\n   1 2 3 4 5 6 7 8";
  for (ssize_t i = 20; i < static_cast<ssize_t>(board_size); ++i) {
    if (i % 10 == 9) continue;
    if (i / 10 == 10) break;
    if (i % 10 == 0) {
      std::cout << "\n " << i / 10 << " ";
      continue;
    }
    char s = sprites.at(b.get(i).pce);
    if (b.get(i).pce_color == color::black) s = tolower(static_cast<unsigned char>(s));
    std::cout << s;
    if (last_move.from_ == i || last_move.to_ == i)
      std::cout << '<';
    else
      std::cout << ' ';
  }
  std::cout << "\n" << std::endl;
}

move read_move(const move_set& valid_moves, color turn) {
  if (std::cin.fail()) {
    std::cin.clear();
    std::string dummy;
    std::cin >> dummy;
  }

  int  in;
  move ret;
  std::cout << "Your move, " << (turn == color::white ? "white" : "black") << ": ";
  std::cin >> in;

  // Command to undo 1 or 2 moves (2 to revert AI+own)
  if (in == 0 || in == 1 || in == 2) {
    ret.from_ = in;
    return ret;
  }

  ret.to_ = in % 10 + in / 10 % 10 * 10;
  in /= 100;
  ret.from_ = in % 10 + in / 10 % 10 * 10;

  for (const auto m: valid_moves)
    if (m.from_ == ret.from_ && m.to_ == ret.to_) return ret;
  std::cout << "Invalid move\n";
  return read_move(valid_moves, turn);
}

} // namespace chess
