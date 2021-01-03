/**
 * File: board.h
 *
 * Content on and manipulation of a chess board implemented as 10x12 squares,
 * with default out_of_border values.
 *
 * "magic" numbers will be used, they aren't magic if you know this structure.
 * Source way too messy with a lot of variable names and variable calculations instead of plain
 * numbers.
 *
 * Author: Ken Rouwas
 * Date 2017-09-14
 *
 * Refactored and expanded upon by Oliver Schönrock - 2021
 */
#pragma once

#include <array>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <sys/types.h>
#include <vector>

namespace chess {

constexpr size_t board_size = 10 * 12;

enum class color { white, black, none };

inline color flip_turn(color turn) { return (turn == color::white) ? color::black : color::white; }

enum class piece : int {
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
  square(piece p, color c) : piece_color_(c), piece_(p) {}

  color piece_color_ = color::none;
  piece piece_       = piece::out_of_board;
};

struct board_change {
  board_change() : where_(move_done) {} // default is terminator, at end of move sequence

  board_change(ssize_t where, square old_square) : where_(where), old_square_(old_square) {}

  [[nodiscard]] bool is_terminator() const { return where_ == move_done; }

  size_t where_;
  square old_square_;

  constexpr static size_t move_done = board_size;
};

class board {
public:
  void init();

  void set(const size_t where, square s) {
    if (where >= board_size) throw std::out_of_range("board::set : out of range error");
    squares_[where] = s;
  }

  [[nodiscard]] square get(const size_t where) const {
    if (where >= board_size) throw std::out_of_range("board::get : out of range error");
    return squares_[where];
  }

  void undo_move() {
    if (bh_.empty()) return;
    if (bh_.back().is_terminator()) bh_.pop_back();

    while (!bh_.empty() && !bh_.back().is_terminator()) {
      set(bh_.back().where_, bh_.back().old_square_);
      bh_.pop_back();
    }
  }

  void do_change(size_t where, square new_square) {
    bh_.emplace_back(where, get(where));
    set(where, new_square);
  }

  void terminate_change() {
    bh_.emplace_back(); // default constructor gives the terminator
  }

private:
  std::array<square, board_size> squares_;
  std::vector<board_change>      bh_;
};

} // namespace chess
