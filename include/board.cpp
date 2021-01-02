// File: board.cpp

#include "board.hpp"
#include "move.hpp"

namespace chess {

void board::init() {
  // Place pawns
  for (size_t i = 0; i < 8; ++i) {
    set(31 /*col 1, 2nd row*/ + i, square(piece::pawn, color::black));
    set(81 /*col 1, 7th row*/ + i, square(piece::pawn, color::white));
  }

  // Place the rest but with s/kings/queens
  int w = 0;
  for (auto p: {piece::rook_castle, piece::knight, piece::bishop, piece::queen}) {
    set(21 + w, square(p, color::black));
    set(28 /*col 8, 1st row*/ - w, square(p, color::black));
    set(91 /*col 1, 8th row*/ + w, square(p, color::white));
    set(98 /*col 8, 8th row*/ - w, square(p, color::white));
    ++w;
  }

  // Place kings
  set(25, square(piece::king_castle, color::black));
  set(95, square(piece::king_castle, color::white));

  // Empty squares inbetween the pieces
  for (int x = 0; x < 8; ++x)
    for (int y = 0; y < 4; ++y) set(41 + x + y * 10, square(piece::none, color::none));
}

} // namespace chess
