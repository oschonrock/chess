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
#include <vector>
#include <cstdlib>
#include <exception>
#include <iostream>

namespace chess {

constexpr size_t board_size = 10 * 12;

enum class color { white, black, none };

inline color flip_turn(color turn) { return (turn == color::white) ? color::black : color::white; }

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
  square() = default;
  square(piece p, color c) : pce_color(c), pce(p) {}

  color pce_color = color::none;
  piece pce       = piece::out_of_board;
};

struct board_change {
  board_change() : where(move_done) {} // terminator, at end of move sequence
  [[nodiscard]] bool is_terminator() const { return where == move_done; }
  size_t             where;
  square             old_square;

  constexpr static size_t move_done = board_size;
};

using board_history = std::vector<board_change>;

class board {
private:
  std::array<square, board_size> squares;

public:
  void init();

  void set(const size_t where, square s) {
    if (where >= board_size) throw std::out_of_range("board::set : out of range error");
    squares[where] = s;
  }

  [[nodiscard]] square get(const size_t where) const {
    if (where >= board_size) throw std::out_of_range("board::get : out of range error");
    return squares[where];
  }

  board_history bh_;
};

} // namespace chess
