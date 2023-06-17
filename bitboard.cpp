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
#include <fstream>
#include <algorithm>
#include <bitset>
#include <string> //

#include "bitboard.h"

namespace Stockfish {

uint8_t PopCnt16[1 << 16];
uint8_t SquareDistance[SQUARE_NB][SQUARE_NB];

Bitboard LineBB[SQUARE_NB][SQUARE_NB];
Bitboard BetweenBB[SQUARE_NB][SQUARE_NB];
Bitboard PseudoAttacks[PIECE_TYPE_NB][SQUARE_NB];
Bitboard PawnAttacks[COLOR_NB][SQUARE_NB];

Magic RookMagics[SQUARE_NB];
Magic BishopMagics[SQUARE_NB];

namespace {

// ******************************************
// I need to change the numbers, definitely!!
Bitboard RookTable[0x10000000]; // 0x19000 // To store rook attacks
Bitboard BishopTable[0x10000000]; // 1480 // To store bishop attacks

void init_magics(PieceType pt, Bitboard table[], Magic magics[]);

}

/// safe_destination() returns the bitboard of target square for the given step
/// from the given square. If the step is off the board, returns empty bitboard.

inline Bitboard safe_destination(Square s, int step) {
    Square to = Square(s + step);
    return is_ok(to) && distance(s, to) <= 2 ? square_bb(to) : NoSquares;
}

/// Bitboards::pretty() returns an ASCII representation of a bitboard suitable
/// to be printed to standard output. Useful for debugging.

std::string Bitboards::pretty(const Bitboard b) {

  std::string s = "+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+\n";

  for (Rank r = RANK_16; r >= RANK_1; --r) {
    for (File f = FILE_A; f <= FILE_P; ++f)
       s += getBit(b, f, r) ? "| X " : "|   ";
    s += "| " + std::to_string(1 + r) + "\n+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+\n";
  }
  s += "  a   b   c   d   e   f   g   h   i   j   k   l   m   n   o   p\n";

  return s;
}

/// Bitboards::init() initializes various bitboard tables. It is called at
/// startup and relies on global objects to be already zero-initialized.

void Bitboards::init() {

  for (unsigned i = 0; i < (1 << 16); ++i)
      PopCnt16[i] = uint8_t(std::bitset<16>(i).count());

  for (Square s1 = SQ_A1; s1 <= SQ_P16; ++s1)
      for (Square s2 = SQ_A1; s2 <= SQ_P16; ++s2)
          SquareDistance[s1][s2] = std::max(distance<File>(s1, s2), distance<Rank>(s1, s2));

  init_magics(ROOK, RookTable, RookMagics);
  init_magics(BISHOP, BishopTable, BishopMagics);

  for (Square s1 = SQ_A1; s1 <= SQ_P16; ++s1)
  {
      PawnAttacks[WHITE][s1] = pawn_attacks_bb<WHITE>(square_bb(s1));
      PawnAttacks[BLACK][s1] = pawn_attacks_bb<BLACK>(square_bb(s1));

      for (int step : {-17, -16, -15, -1, 1, 15, 16, 17} )
         PseudoAttacks[KING][s1] |= safe_destination(s1, step);

      for (int step : {-33, -31, -18, -14, 14, 18, 31, 33} )
         PseudoAttacks[KNIGHT][s1] |= safe_destination(s1, step);

      PseudoAttacks[QUEEN][s1]  = PseudoAttacks[BISHOP][s1] = attacks_bb<BISHOP>(s1, NoSquares);
      PseudoAttacks[QUEEN][s1] |= PseudoAttacks[  ROOK][s1] = attacks_bb<  ROOK>(s1, NoSquares);

      for (PieceType pt : { BISHOP, ROOK })
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

namespace {

  Bitboard sliding_attack(PieceType pt, Square sq, Bitboard occupied) {

    Bitboard attacks = NoSquares;
    Direction   RookDirections[4] = {NORTH, SOUTH, EAST, WEST};
    Direction BishopDirections[4] = {NORTH_EAST, SOUTH_EAST, SOUTH_WEST, NORTH_WEST};

    for (Direction d : (pt == ROOK ? RookDirections : BishopDirections))
    {
        Square s = sq;
        while (nonemptyBB(safe_destination(s, d)) && !(nonemptyBB(occupied & square_bb(s))))
            attacks |= square_bb(s += d);
    }

    return attacks;
  }


  // init_magics() computes all rook and bishop attacks at startup. Magic
  // bitboards are used to look up attacks of sliding pieces. As a reference see
  // www.chessprogramming.org/Magic_Bitboards. In particular, here we use the so
  // called "fancy" approach.

  void init_magics(PieceType pt, Bitboard table[], Magic magics[]) {

    int seeds[][RANK_NB] = {{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16},
                            {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}};

    // Optimal PRNG seeds to pick the correct magics in the shortest time
    //int seeds[][RANK_NB] = { { 8977, 44560, 54343, 38998,  5731, 95205, 104912, 17020 },
    //                         {  728, 10316, 55013, 32803, 12281, 15100,  16645,   255 } };

    Bitboard* occupancy = new Bitboard[1 << (FILE_NB + RANK_NB - 4)];
    Bitboard* reference = new Bitboard[1 << (FILE_NB + RANK_NB - 4)];
    int* epoch = new int[1 << (FILE_NB + RANK_NB - 4)]();
    Bitboard edges, b;
    int cnt = 0, size = 0;

    if (pt == BISHOP)
        std::cout << "Starting init_magics function for bishop" << std::endl;
    if (pt == ROOK)
        std::cout << "Starting init_magics function for rook" << std::endl;

    // for calculating the sizes of RookTable and BishopTable
    uint64_t rookTableSize = 0ULL, bishopTableSize = 0ULL;

    for (Square s = SQ_A1; s <= SQ_P16; ++s)
    {
        if (pt == BISHOP)
            std::cout << "Bishop, square " << int(s)+1 << "/64" << std::endl;
        if (pt == ROOK)
            std::cout << "Rook, square " << int(s)+1 << "/64" << std::endl;

        // Board edges are not considered in the relevant occupancies
        edges = ((Rank1BB | Rank16BB) & ~rank_bb(s)) | ((FileABB | FilePBB) & ~file_bb(s));

        // Given a square 's', the mask is the bitboard of sliding attacks from
        // 's' computed on an empty board. The index must be big enough to contain
        // all the attacks for each possible subset of the mask and so is 2 power
        // the number of 1s of the mask. Hence we deduce the size of the shift to
        // apply to the 64 or 32 bits word to get the index.
        Magic& m = magics[s];
        m.mask  = sliding_attack(pt, s, NoSquares) & ~edges;
        m.shift = 256 - popcount(m.mask);

        // Set the offset for the attacks table of the square. We have individual
        // table sizes for each square with "Fancy Magic Bitboards".
        m.attacks = s == SQ_A1 ? table : magics[s - 1].attacks + size;

        //
        if (pt == ROOK) {
            rookTableSize += s == SQ_A1 ? 1 : size;
            if (rookTableSize > 0x10000000) {
                std::cout << "Error: RookTable size is bigger than 0x10000000" << std::endl;
                exit(-1);
            }
        }
        if (pt == BISHOP) {
            bishopTableSize += s == SQ_A1 ? 1 : size;
            if (bishopTableSize > 0x10000000) {
                std::cout << "Error: BishopTable size is bigger than 0x10000000" << std::endl;
                exit(-1);
            }
        }

        std::cout << "  executing Carry-Rippler trick" << std::endl;
        // Use Carry-Rippler trick to enumerate all subsets of masks[s] and
        // store the corresponding sliding attack bitboard in reference[].
        b = NoSquares;
        size = 0;
        do {
            occupancy[size] = b;
            reference[size] = sliding_attack(pt, s, b);
            size++;
            b = (b - m.mask) & m.mask;
        } while (nonemptyBB(b));

        std::cout << "  initializing PRNG" << std::endl;
        // assumed 64bit computer and replaced is64Bit with true (LR)
        PRNG rng(seeds[true][rank_of(s)]);

        std::cout << "  searching for a magic for the square" << std::endl;
        // Find a magic for square 's' picking up an (almost) random number
        // until we find the one that passes the verification test.
        for (int i = 0; i < size; )
        {
            if (pt == BISHOP)
                std::cout << "Bishop, square " << (int(s)+1) << "/64 " << i << "/" << size << std::endl;
            if (pt == ROOK)
                std::cout << "Rook, square " << (int(s)+1) << "/64 " << i << "/" << size << std::endl;

            std::cout << "    generating m.magic" << std::endl;
            for (m.magic = NoSquares; popcount((m.magic * m.mask) >> (SQUARE_NB - FILE_NB)) < FILE_NB - 2; )
                m.magic = rng.sparse_rand(); // rng.sparse_rand<Bitboard>();

            // A good magic must map every possible occupancy to an index that
            // looks up the correct sliding attack in the attacks[s] database.
            // Note that we build up the database for square 's' as a side
            // effect of verifying the magic. Keep track of the attempt count
            // and save it in epoch[], little speed-up trick to avoid resetting
            // m.attacks[] after every failed attempt.

            std::cout << "    something to do with m.attacks" << std::endl;
            for (++cnt, i = 0; i < size; ++i)
            {
                if (pt == BISHOP)
                    std::cout << "    Bishop, square " << (int(s)+1) << "/64 " << i << "/" << size << std::endl;
                if (pt == ROOK)
                    std::cout << "    Rook, square " << (int(s)+1) << "/64 " << i << "/" << size << std::endl;

                if (i >= 1 << (FILE_NB + RANK_NB - 4)) {
                    std::cout << "      index i is too big, it must be less than " << (1 << (FILE_NB + RANK_NB - 4)) << std::endl;
                    exit(-1);
                }

                unsigned idx = m.index(occupancy[i]);

                if (idx >= 1 << (FILE_NB + RANK_NB - 4)) {
                    std::cout << "      index idx is too big, it must be less than " << (1 << (FILE_NB + RANK_NB - 4)) << std::endl;
                    exit(-1);
                }

                if (epoch[idx] < cnt)
                {
                    epoch[idx] = cnt;
                    m.attacks[idx] = reference[i];
                }
                else if (m.attacks[idx] != reference[i])
                    break;
            }
        }
    }

    delete[] occupancy;
    delete[] reference;
    delete[] epoch;

    //
    if (pt == ROOK) {
        std::cout << "RookTable size is " << rookTableSize << std::endl;
        std::ofstream rookTableFile;
        rookTableFile.open("rookTable.txt", std::ios::out);
        for (uint64_t i = 0; i < rookTableSize; i++) {
            rookTableFile << RookTable[i].b[0] << " " << RookTable[i].b[1] << " " << RookTable[i].b[2] << " " << RookTable[i].b[3] << std::endl;
        }
        rookTableFile.close();
        std::ofstream rookMagicsFile;
        rookMagicsFile.open("rookMagics.txt", std::ios::out);
        for (int i = 0; i < SQUARE_NB; i++) {
            rookMagicsFile << RookMagics[i].mask.b[0] << " " << RookMagics[i].mask.b[1] << " " << RookMagics[i].mask.b[2] << " " << RookMagics[i].mask.b[3] << std::endl;
            rookMagicsFile << RookMagics[i].magic.b[0] << " " << RookMagics[i].magic.b[1] << " " << RookMagics[i].magic.b[2] << " " << RookMagics[i].magic.b[3] << std::endl;
            rookMagicsFile << (RookMagics[i].attacks - RookTable)/sizeof(Bitboard) << std::endl;
            rookMagicsFile << RookMagics[i].shift << std::endl;
        }
        rookMagicsFile.close();
    }
    if (pt == BISHOP) {
        std::cout << "BishopTable size is " << bishopTableSize << std::endl;
        std::ofstream bishopTableFile;
        bishopTableFile.open("bishopTable.txt", std::ios::out);
        for (uint64_t i = 0; i < bishopTableSize; i++) {
            bishopTableFile << BishopTable[i].b[0] << " " << BishopTable[i].b[1] << " " << BishopTable[i].b[2] << " " << BishopTable[i].b[3] << std::endl;
        }
        bishopTableFile.close();
        std::ofstream bishopMagicsFile;
        bishopMagicsFile.open("bishopMagics.txt", std::ios::out);
        for (int i = 0; i < SQUARE_NB; i++) {
            bishopMagicsFile << BishopMagics[i].mask.b[0] << " " << BishopMagics[i].mask.b[1] << " " << BishopMagics[i].mask.b[2] << " " << BishopMagics[i].mask.b[3] << std::endl;
            bishopMagicsFile << BishopMagics[i].magic.b[0] << " " << BishopMagics[i].magic.b[1] << " " << BishopMagics[i].magic.b[2] << " " << BishopMagics[i].magic.b[3] << std::endl;
            bishopMagicsFile << (BishopMagics[i].attacks - BishopTable)/sizeof(Bitboard) << std::endl;
            bishopMagicsFile << BishopMagics[i].shift << std::endl;
        }
        bishopMagicsFile.close();
    }

  }
}

} // namespace Stockfish
