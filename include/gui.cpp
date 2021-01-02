// File: gui.cpp
// This is quick, ugly, pragmatic, temporary.

#include "gui.hpp"
#include <cctype>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string_view>

namespace chess {

// clang-format off
static const std::map<piece, unsigned char> sprites = // NOLINT
    {{piece::pawn,            'P'},
     {piece::pawn_en_passant, 'P'},
     {piece::rook,            'R'},
     {piece::rook_castle,     'R'},
     {piece::knight,          'F'},
     {piece::bishop,          'B'},
     {piece::king,            'K'},
     {piece::king_castle,     'K'},
     {piece::none,            ' '},
     {piece::out_of_board,    '#'},
     {piece::queen,           'Q'}};
// clang-format on

constexpr std::string_view term_reset    = "\x1b[0m";
constexpr std::string_view term_fg_blue  = "\x1b[38;5;4m";
constexpr std::string_view term_fg_gold  = "\x1b[38;5;3m";
constexpr std::string_view term_bg_white = "\x1b[48;5;15m";
constexpr std::string_view term_bg_black = "\x1b[48;5;0m";

static void set_fg_color(color pce_color) {
  std::cout << ((pce_color == color::black) ? term_fg_blue : term_fg_gold);
}

void print_board(const board& b, move last_move) {
  const std::string_view col_labels = "    a  b  c  d  e  f  g  h";
  std::cout << '\n' << col_labels;

  for (ssize_t i = 20; i < static_cast<ssize_t>(board_size); ++i) {
    if (i % 10 == 9) continue;
    if (i / 10 == 10) break;
    if (i % 10 == 0) {
      std::cout << term_reset;
      if (i != 20) std::cout << " " << 10 - (i - 1) / 10;
      std::cout << "\n " << 10 - i / 10 << " ";
      continue;
    }
    unsigned char s = sprites.at(b.get(i).pce);
    set_fg_color(b.get(i).pce_color);
    if (b.get(i).pce_color == color::black) s = std::tolower(s);
    std::cout << (((i + ((i / 10) % 2)) % 2) != 0 ? term_bg_white : term_bg_black);
    std::cout << " " << s;
    if (last_move.from_ == i || last_move.to_ == i) {
      set_fg_color(b.get(last_move.to_).pce_color);
      std::cout << '<';
    } else {
      std::cout << " ";
    }
  }
  std::cout << term_reset << " 1" // last row label
            << "\n"
            << col_labels << '\n'
            << std::endl; // with flush
}

static bool is_digit(char c) { return c >= '0' && c <= '9'; }
static bool is_letter(char c) { return c >= 'a' && c <= 'h'; }

move read_move(const move_set& valid_moves, color turn) {
  if (std::cin.fail()) {
    std::cin.clear();
    std::string dummy;
    std::cin >> dummy;
  }

  std::string in;
  move        ret;
  std::cout << "Your move, " << (turn == color::white ? "white" : "black") << ": ";
  std::cin >> in;

  // Command to undo 1 or 2 moves (2 to revert AI+own)
  try {
    int in_int = std::stoi(in);
    if (in_int == 0 || in_int == 1 || in_int == 2) {
      ret.from_ = in_int;
      return ret;
    }
  } catch (std::logic_error& e) {
    // ignore
  }

  if (in.length() != 4 || !is_letter(in[0]) || !is_digit(in[1]) || !is_letter(in[2]) ||
      !is_digit(in[3])) {
    std::cout << "Invalid move\n";
    return read_move(valid_moves, turn);
  }
  ret.from_ = in[0] - 'a' + 1 + (10 - (in[1] - '0')) * 10;
  ret.to_   = in[2] - 'a' + 1 + (10 - (in[3] - '0')) * 10;

  for (const auto m: valid_moves)
    if (m.from_ == ret.from_ && m.to_ == ret.to_) return ret;
  std::cout << "Invalid move\n";
  return read_move(valid_moves, turn);
}

} // namespace chess
