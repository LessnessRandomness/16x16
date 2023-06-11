/*
  Stockfish, a UCI chess playing engine derived from Glaurung 2.1
  Copyright (C) 2004-2022 The Stockfish developers (see AUTHORS file)
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

uint8_t PopCnt16[1 << 16]; // ??
uint8_t SquareDistance[SQUARE_NB][SQUARE_NB];

Bitboard LineBB[SQUARE_NB][SQUARE_NB];
Bitboard BetweenBB[SQUARE_NB][SQUARE_NB];
Bitboard PseudoAttacks[PIECE_TYPE_NB][SQUARE_NB];
Bitboard PawnAttacks[COLOR_NB][SQUARE_NB];

Bitboards::Magic RookMagics[SQUARE_NB];
Bitboards::Magic BishopMagics[SQUARE_NB];

namespace {

  Bitboard RookTable[0x19000];  // To store rook attacks
  Bitboard BishopTable[0x1480]; // To store bishop attacks

  // void init_magics(PieceType pt, Bitboard table[], Magic magics[]);

}

/// safe_destination() returns the bitboard of target square for the given step
/// from the given square. If the step is off the board, returns empty bitboard.

/* inline Bitboard safe_destination(Square s, int step) {
    Square to = Square(s + step);
    return is_ok(to) && distance(s, to) <= 2 ? square_bb(to) : Bitboard(0);
} */

/// Bitboards::pretty() returns an ASCII representation of a bitboard suitable
/// to be printed to standard output. Useful for debugging.

std::string Bitboards::pretty(Bitboard b) {

  std::string s = "+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+\n";

  for (Rank r = RANK_16; r >= RANK_1; --r) {
    for (File f = FILE_A; f <= FILE_P; ++f)
       s += getBit(b, f, r) ? "| X " : "|   ";
    s += "| " + std::to_string(1 + r) + "\n+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+\n";
  }
  s += "  a   b   c   d   e   f   g   h   i   j   k   l   m   n   o   p\n";

  return s;
}

} // namespace Stockfish
