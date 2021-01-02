#include "move.hpp"
#include <iostream>

// Read move.hpp for specifications

namespace chess {

void do_move(move m, board& b, piece pawn_promotion) {
  // Pawn promotion
  if (b.get(m.from_).piece_ == piece::pawn && m.to_ / 10 == 2)
    b.do_change(m.from_, square(pawn_promotion, color::white));

  if (b.get(m.from_).piece_ == piece::pawn && m.to_ / 10 == 9)
    b.do_change(m.from_, square(pawn_promotion, color::black));

  // Move rook if castling
  if (b.get(m.from_).piece_ == piece::king_castle &&
      (m.from_ - m.to_ == 2 || m.from_ - m.to_ == -2)) {
    if (m.to_ == 23) {
      b.do_change(21, square(piece::none, color::none));
      b.do_change(24, square(piece::rook, color::black));
    }

    if (m.to_ == 27) {
      b.do_change(28, square(piece::none, color::none));
      b.do_change(26, square(piece::rook, color::black));
    }

    if (m.to_ == 93) {
      b.do_change(91, square(piece::none, color::none));
      b.do_change(94, square(piece::rook, color::white));
    }

    if (m.to_ == 97) {
      b.do_change(98, square(piece::none, color::none));
      b.do_change(96, square(piece::rook, color::white));
    }
  }

  piece pawn_replaced = b.get(m.to_).piece_;
  // Regular piece move
  b.do_change(m.to_, b.get(m.from_));
  b.do_change(m.from_, square(piece::none, color::none));

  // Pawn replaced empty square
  if (b.get(m.to_).piece_ == piece::pawn && pawn_replaced == piece::none) {
    // En passant move
    if (b.get(m.from_ - 1).piece_ == piece::pawn_en_passant &&
        (m.from_ - m.to_ == 11 || m.from_ - m.to_ == -9))
      b.do_change(m.from_ - 1, square(piece::none, color::none));
    else if (b.get(m.from_ + 1).piece_ == piece::pawn_en_passant &&
             (m.from_ - m.to_ == 9 || m.from_ - m.to_ == -11))
      b.do_change(m.from_ + 1, square(piece::none, color::none));
  }

  // clear flag of pawns with en passant potential
  for (size_t i = 1; i < 9; ++i) {
    if (b.get(50 + i).piece_ == piece::pawn_en_passant)
      b.do_change(50 + i, square(piece::pawn, color::black));
    if (b.get(60 + i).piece_ == piece::pawn_en_passant)
      b.do_change(60 + i, square(piece::pawn, color::white));
  }

  // Give two-square moved pawns en passant flag
  if (b.get(m.to_).piece_ == piece::pawn) {
    if (m.from_ / 10 == 3 && m.to_ / 10 == 5)
      b.do_change(m.to_, square(piece::pawn_en_passant, color::black));

    if (m.from_ / 10 == 8 && m.to_ / 10 == 6)
      b.do_change(m.to_, square(piece::pawn_en_passant, color::white));
  }

  // Lose castling potential
  if (b.get(m.to_).piece_ == piece::king_castle)
    b.do_change(m.to_, square(piece::king, b.get(m.to_).piece_color_));
  if (b.get(m.to_).piece_ == piece::rook_castle)
    b.do_change(m.to_, square(piece::rook, b.get(m.to_).piece_color_));

  b.terminate_change();
}

move_set valid_moves(board& b, color turn) {
  move_set ret;
  color   enemy_color = flip_turn(turn);
  int     pawn_dir    = (turn == color::white) ? -1 : 1;

  for (size_t from = 21 /*skip padding*/; from < 99 /*padding after this*/; ++from) {
    if (b.get(from).piece_color_ == turn) {
      switch (b.get(from).piece_) {
      case piece::king_castle:
        if (from == 95 && b.get(96).piece_ == piece::none && b.get(97).piece_ == piece::none &&
            b.get(98).piece_ == piece::rook_castle)
          ret.push_back(move(from, 97));
        if (from == 25 && b.get(26).piece_ == piece::none && b.get(27).piece_ == piece::none &&
            b.get(28).piece_ == piece::rook_castle)
          ret.push_back(move(from, 27));
        if (from == 95 && b.get(94).piece_ == piece::none && b.get(93).piece_ == piece::none &&
            b.get(92).piece_ == piece::none && b.get(91).piece_ == piece::rook_castle)
          ret.push_back(move(from, 93));
        if (from == 25 && b.get(24).piece_ == piece::none && b.get(23).piece_ == piece::none &&
            b.get(22).piece_ == piece::none && b.get(21).piece_ == piece::rook_castle)
          ret.push_back(move(from, 23));
      // fallthrough
      case piece::king:
        for (auto to:
             {from - 11, from - 10, from - 9, from - 1, from + 1, from + 9, from + 10, from + 11}) {
          if (b.get(to).piece_color_ == turn || b.get(to).piece_ == piece::out_of_board) continue;
          ret.push_back(move(from, to));
        }
        break;
      case piece::pawn:
        if (b.get(from + pawn_dir * 11).piece_color_ == enemy_color)
          ret.push_back(move(from, from + pawn_dir * 11));
        if (b.get(from + pawn_dir * 9).piece_color_ == enemy_color)
          ret.push_back(move(from, from + pawn_dir * 9));
        if (b.get(from + pawn_dir * 10).piece_ == piece::none)
          ret.push_back(move(from, from + pawn_dir * 10));
        if (b.get(from + pawn_dir * 10).piece_ == piece::none &&
            b.get(from + pawn_dir * 20).piece_ == piece::none) {
          size_t row = from / 10;
          if ((row == 3 && pawn_dir == 1) || (row == 8 && pawn_dir == -1))
            ret.push_back(move(from, from + pawn_dir * 20));
        }
        if (b.get(from - 1).piece_ == piece::pawn_en_passant && pawn_dir == -1)
          ret.push_back(move(from, from - 11));
        if (b.get(from + 1).piece_ == piece::pawn_en_passant && pawn_dir == -1)
          ret.push_back(move(from, from - 9));
        if (b.get(from - 1).piece_ == piece::pawn_en_passant && pawn_dir == 1)
          ret.push_back(move(from, from + 9));
        if (b.get(from + 1).piece_ == piece::pawn_en_passant && pawn_dir == 1)
          ret.push_back(move(from, from + 11));
        break;
      case piece::knight:
        for (auto to: {from + 8, from + 12, from + 19, from + 21, from - 8, from - 12, from - 21,
                       from - 19}) {
          if (b.get(to).piece_color_ == turn || b.get(to).piece_ == piece::out_of_board) continue;
          ret.push_back(move(from, to));
        }
        break;
      case piece::queen:
      // fallthrough
      case piece::rook:
      case piece::rook_castle:
        for (int dd: {1, -1, 10, -10})
          for (int d = dd; b.get(from + d).piece_color_ != b.get(from).piece_color_ &&
                           b.get(from + d).piece_ != piece::out_of_board;
               d += dd) {
            ret.push_back(move(from, from + d));
            if (b.get(from + d).piece_ != piece::none) break;
          }
        if (b.get(from).piece_ != piece::queen) break;
      case piece::bishop:
        for (int dd: {9, 11, -9, -11})
          for (int d = dd; b.get(from + d).piece_color_ != b.get(from).piece_color_ &&
                           b.get(from + d).piece_ != piece::out_of_board;
               d += dd) {
            ret.push_back(move(from, from + d));
            if (b.get(from + d).piece_ != piece::none) break;
          }
        break;
      default: // warnings unwanted
        break;
      }
    }
  }
  return ret;
}
} // namespace chess
