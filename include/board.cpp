// File: board.cpp

#include "board.hpp"

namespace chess {
square::square(piece p, color c) {
  pce       = p;
  pce_color = c;
}

square::square() {
  pce       = piece::out_of_board;
  pce_color = color::none;
}

void board::set(const size_t where, square s) {
  if (where >= board_size) return;
  squares[where] = s;
}

square board::get(const size_t where) const {
  if (where >= board_size) return square(piece::out_of_board, color::none);
  return squares[where];
}

void init_classic_board(board& b) {
  // Place pawns
  for (size_t i = 0; i < 8; ++i) {
    b.set(31 /*col 1, 2nd row*/ + i, square(piece::pawn, color::black));
    b.set(81 /*col 1, 7th row*/ + i, square(piece::pawn, color::white));
  }

  // Place the rest but with s/kings/queens
  int w = 0;
  for (auto p: {piece::rook_castle, piece::knight, piece::bishop, piece::queen}) {
    b.set(21 + w, square(p, color::black));
    b.set(28 /*col 8, 1st row*/ - w, square(p, color::black));
    b.set(91 /*col 1, 8th row*/ + w, square(p, color::white));
    b.set(98 /*col 8, 8th row*/ - w, square(p, color::white));
    ++w;
  }

  // Place kings
  b.set(25, square(piece::king_castle, color::black));
  b.set(95, square(piece::king_castle, color::white));

  // Empty squares inbetween the pieces
  for (int x = 0; x < 8; ++x)
    for (int y = 0; y < 4; ++y) b.set(41 + x + y * 10, square(piece::none, color::none));
}

} // namespace chess
