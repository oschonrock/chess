/*
    File: board.h

    Content on and manipulation of a chess board implemented as 10x12 squares,
    with default out_of_border values.

    "magic" numbers will be used, they aren't magic if you know this structure.
    Source way too messy with a lot of variable names and variable calculations instead of plain
   numbers.

    Author: Ken Rouwas
    Date 2017-09-14
*/

#pragma once

#include <array>

namespace chess {

const size_t board_size = 10 * 12;

enum class color { white, black, none };

color flip_turn(color turn);

enum class piece {
  king,        // a king without castle potential
  king_castle, // a king with castle potential
  queen,
  pawn,            // a pawn without en passant potential
  pawn_en_passant, // a pawn with en passant potential
  rook,
  rook_castle,
  knight,
  bishop,
  none,
  out_of_board // Illegal position
};

struct square {
  color pce_color;
  piece pce;
  square(piece, color);
  square();
};

class board {
private:
  std::array<square, board_size> squares;

public:
  void                 init();
  void                 set(size_t where, square);
  [[nodiscard]] square get(size_t where) const;
};

} // namespace chess
