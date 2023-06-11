/*
  Stockfish, a UCI chess playing engine derived from Glaurung 2.1
  Copyright (C) 2004-2023 The Stockfish developers (see AUTHORS file)

  Stockfish is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Stockfish is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <algorithm>
#include <bitset>

#include "bitboard.h"

namespace Stockfish {

namespace Bitboards {

/// Bitboards::pretty() returns an ASCII representation of a bitboard suitable
/// to be printed to standard output. Useful for debugging.

std::string pretty(const Bitboard b) {

  std::string s = "+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+\n";

  for (Rank r = RANK_16; r >= RANK_1; --r) {
    for (File f = FILE_A; f <= FILE_P; ++f)
       s += getBit(b, f, r) ? "| X " : "|   ";
    s += "| " + std::to_string(1 + r) + "\n+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+\n";
  }
  s += "  a   b   c   d   e   f   g   h   i   j   k   l   m   n   o   p\n";

  return s;
}

}

} // namespace Stockfish
