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

#include <iostream> // for debugging
#include <algorithm>
#include <bitset>

#include "bitboard.h"

namespace Stockfish
{

uint8_t PopCnt16[1 << 16];
uint8_t SquareDistance[SQUARE_NB][SQUARE_NB];

Bitboard LineBB[SQUARE_NB][SQUARE_NB];
Bitboard BetweenBB[SQUARE_NB][SQUARE_NB];
Bitboard PseudoAttacks[PIECE_TYPE_NB][SQUARE_NB];
Bitboard PawnAttacks[COLOR_NB][SQUARE_NB];

/// safe_destination() returns the bitboard of target square for the given step
/// from the given square. If the step is off the board, returns empty bitboard.

inline Bitboard safe_destination(Square s, int step)
{
    Square to = Square(s + step);
    return is_ok(to) && (distance(s, to) <= 2) ? square_bb(to) : NoSquares;
}

/// Bitboards::pretty() returns an ASCII representation of a bitboard suitable
/// to be printed to standard output. Useful for debugging.

std::string Bitboards::pretty(const Bitboard b)
{

    std::string s = "+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+\n";

    for (Rank r = RANK_16; r >= RANK_1; --r)
    {
        for (File f = FILE_A; f <= FILE_P; ++f)
            s += getBit(b, f, r) ? "| X " : "|   ";
        s += "| " + std::to_string(1 + r) + "\n+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+\n";
    }
    s += "  a   b   c   d   e   f   g   h   i   j   k   l   m   n   o   p\n";

    return s;
}

/// Bitboards::init() initializes various bitboard tables. It is called at
/// startup and relies on global objects to be already zero-initialized.

namespace Bitboards {

Bitboard RookAttacks(Square s, Bitboard occupied) {
    Bitboard attacks = NoSquares;
    Direction RookDirections[4] = {NORTH, SOUTH, EAST, WEST};

    for (Direction d : RookDirections)
    {
        Square sq = s;
        while (nonemptyBB(safe_destination(sq, d)) && !(nonemptyBB(occupied & square_bb(sq))))
            attacks |= square_bb(sq += d);
    }
    return attacks;
}

Bitboard BishopAttacks(Square s, Bitboard occupied) {
    Bitboard attacks = NoSquares;
    Direction RookDirections[4] = {NORTH_EAST, SOUTH_EAST, SOUTH_WEST, NORTH_WEST};

    for (Direction d : RookDirections)
    {
        Square sq = s;
        while (nonemptyBB(safe_destination(sq, d)) && !(nonemptyBB(occupied & square_bb(sq))))
            attacks |= square_bb(sq += d);
    }
    return attacks;
}

void init()
{

    for (unsigned i = 0; i < (1 << 16); ++i)
        PopCnt16[i] = uint8_t(std::bitset<16>(i).count());

    for (Square s1 = SQ_A1; s1 <= SQ_P16; ++s1)
        for (Square s2 = SQ_A1; s2 <= SQ_P16; ++s2)
            SquareDistance[s1][s2] = std::max(distance<File>(s1, s2), distance<Rank>(s1, s2));

    for (Square s1 = SQ_A1; s1 <= SQ_P16; ++s1)
    {
        PawnAttacks[WHITE][s1] = pawn_attacks_bb<WHITE>(square_bb(s1));
        PawnAttacks[BLACK][s1] = pawn_attacks_bb<BLACK>(square_bb(s1));

        for (int step :
                {
                    -17, -16, -15, -1, 1, 15, 16, 17
                    } )
                        PseudoAttacks[KING][s1] |= safe_destination(s1, step);

        for (int step :
                {
                    -33, -31, -18, -14, 14, 18, 31, 33
                    } )
                        PseudoAttacks[KNIGHT][s1] |= safe_destination(s1, step);

        PseudoAttacks[QUEEN][s1]  = PseudoAttacks[BISHOP][s1] = attacks_bb<BISHOP>(s1, NoSquares);
        PseudoAttacks[QUEEN][s1] |= PseudoAttacks[  ROOK][s1] = attacks_bb<  ROOK>(s1, NoSquares);

        for (PieceType pt :
                {
                    BISHOP, ROOK
                })
            for (Square s2 = SQ_A1; s2 <= SQ_P16; ++s2)
            {
                if (nonemptyBB(PseudoAttacks[pt][s1] & s2))
                {
                    LineBB[s1][s2]    = (attacks_bb(pt, s1, NoSquares) & attacks_bb(pt, s2, NoSquares)) | s1 | s2;
                    BetweenBB[s1][s2] = (attacks_bb(pt, s1, square_bb(s2)) & attacks_bb(pt, s2, square_bb(s1)));
                }
                BetweenBB[s1][s2] |= s2;
            }
    }
}


} // namespace Bitboards

} // namespace Stockfish
