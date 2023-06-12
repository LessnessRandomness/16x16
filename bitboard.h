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

#ifndef BITBOARD_H_INCLUDED
#define BITBOARD_H_INCLUDED

#include <iostream> // to remove later
#include <string>
#include "types.h"

namespace Stockfish {

namespace Bitboards {

void init();
std::string pretty(const Bitboard bb);

inline bool getBit(Bitboard bb, int f, int r) {
    return ((bb.b[r >> 2] >> ((r & 3) << 4)) & (1 << f));
}

inline bool nonemptyBB(Bitboard bb) {
    return (bb.b[0] | bb.b[1] | bb.b[2] | bb.b[3]);
}

constexpr Bitboard NoSquares = {.b = {0ULL, 0ULL, 0ULL, 0ULL}};
constexpr Bitboard AllSquares = ~NoSquares;
constexpr Bitboard DarkSquares = {.b = {0xAAAA5555AAAA5555ULL, 0xAAAA5555AAAA5555ULL, 0xAAAA5555AAAA5555ULL, 0xAAAA5555AAAA5555ULL}};
constexpr Bitboard LightSquares = ~DarkSquares;

constexpr Bitboard FileABB = {.b = {0x0001000100010001ULL, 0x0001000100010001ULL, 0x0001000100010001ULL, 0x0001000100010001ULL}};
constexpr Bitboard FileBBB = {.b = {FileABB.b[0]<<1, FileABB.b[1]<<1, FileABB.b[2]<<1, FileABB.b[3]<<1}};
constexpr Bitboard FileCBB = {.b = {FileABB.b[0]<<2, FileABB.b[1]<<2, FileABB.b[2]<<2, FileABB.b[3]<<2}};
constexpr Bitboard FileDBB = {.b = {FileABB.b[0]<<3, FileABB.b[1]<<3, FileABB.b[2]<<3, FileABB.b[3]<<3}};
constexpr Bitboard FileEBB = {.b = {FileABB.b[0]<<4, FileABB.b[1]<<4, FileABB.b[2]<<4, FileABB.b[3]<<4}};
constexpr Bitboard FileFBB = {.b = {FileABB.b[0]<<5, FileABB.b[1]<<5, FileABB.b[2]<<5, FileABB.b[3]<<5}};
constexpr Bitboard FileGBB = {.b = {FileABB.b[0]<<6, FileABB.b[1]<<6, FileABB.b[2]<<6, FileABB.b[3]<<6}};
constexpr Bitboard FileHBB = {.b = {FileABB.b[0]<<7, FileABB.b[1]<<7, FileABB.b[2]<<7, FileABB.b[3]<<7}};
constexpr Bitboard FileIBB = {.b = {FileABB.b[0]<<8, FileABB.b[1]<<8, FileABB.b[2]<<8, FileABB.b[3]<<8}};
constexpr Bitboard FileJBB = {.b = {FileABB.b[0]<<9, FileABB.b[1]<<9, FileABB.b[2]<<9, FileABB.b[3]<<9}};
constexpr Bitboard FileKBB = {.b = {FileABB.b[0]<<10, FileABB.b[1]<<10, FileABB.b[2]<<10, FileABB.b[3]<<10}};
constexpr Bitboard FileLBB = {.b = {FileABB.b[0]<<11, FileABB.b[1]<<11, FileABB.b[2]<<11, FileABB.b[3]<<11}};
constexpr Bitboard FileMBB = {.b = {FileABB.b[0]<<12, FileABB.b[1]<<12, FileABB.b[2]<<12, FileABB.b[3]<<12}};
constexpr Bitboard FileNBB = {.b = {FileABB.b[0]<<13, FileABB.b[1]<<13, FileABB.b[2]<<13, FileABB.b[3]<<13}};
constexpr Bitboard FileOBB = {.b = {FileABB.b[0]<<14, FileABB.b[1]<<14, FileABB.b[2]<<14, FileABB.b[3]<<14}};
constexpr Bitboard FilePBB = {.b = {FileABB.b[0]<<15, FileABB.b[1]<<15, FileABB.b[2]<<15, FileABB.b[3]<<15}};

constexpr Bitboard Rank1BB  = {.b = {0xFFFFULL, 0, 0, 0}};
constexpr Bitboard Rank2BB  = {.b = {0xFFFFULL << (16*1), 0, 0, 0}};
constexpr Bitboard Rank3BB  = {.b = {0xFFFFULL << (16*2), 0, 0, 0}};
constexpr Bitboard Rank4BB  = {.b = {0xFFFFULL << (16*3), 0, 0, 0}};
constexpr Bitboard Rank5BB  = {.b = {0, 0xFFFFULL, 0, 0}};
constexpr Bitboard Rank6BB  = {.b = {0, 0xFFFFULL << (16*1), 0, 0}};
constexpr Bitboard Rank7BB  = {.b = {0, 0xFFFFULL << (16*2), 0, 0}};
constexpr Bitboard Rank8BB  = {.b = {0, 0xFFFFULL << (16*3), 0, 0}};
constexpr Bitboard Rank9BB  = {.b = {0, 0, 0xFFFFFULL, 0}};
constexpr Bitboard Rank10BB = {.b = {0, 0, 0xFFFFULL << (16*1), 0}};
constexpr Bitboard Rank11BB = {.b = {0, 0, 0xFFFFULL << (16*2), 0}};
constexpr Bitboard Rank12BB = {.b = {0, 0, 0xFFFFULL << (16*3), 0}};
constexpr Bitboard Rank13BB = {.b = {0, 0, 0, 0xFFFFULL}};
constexpr Bitboard Rank14BB = {.b = {0, 0, 0, 0xFFFFULL << (16*1)}};
constexpr Bitboard Rank15BB = {.b = {0, 0, 0, 0xFFFFULL << (16*2)}};
constexpr Bitboard Rank16BB = {.b = {0, 0, 0, 0xFFFFULL << (16*3)}};

extern uint8_t PopCnt16[1 << 16];
extern uint8_t SquareDistance[SQUARE_NB][SQUARE_NB];

extern Bitboard BetweenBB[SQUARE_NB][SQUARE_NB];
extern Bitboard LineBB[SQUARE_NB][SQUARE_NB];
extern Bitboard PseudoAttacks[PIECE_TYPE_NB][SQUARE_NB];
extern Bitboard PawnAttacks[COLOR_NB][SQUARE_NB];

/// Magic holds all magic bitboards relevant data for a single square
struct Magic {
  Bitboard  mask;
  Bitboard  magic;
  Bitboard* attacks;
  unsigned  shift;

  // Compute the attack's index using the 'magic bitboards' approach
  unsigned index(Bitboard occupied) const {
    // I presume 64bit computer (if that's what Is64Bit meant)
    return unsigned((((occupied & mask) * magic) >> shift).b[0]);
  }
};

extern Magic RookMagics[SQUARE_NB];
extern Magic BishopMagics[SQUARE_NB];

inline Bitboard square_bb(Square s) {
  assert(is_ok(s));
  unsigned int r = s >> 4, f = s & 0xF;
  Bitboard bb = {.b {0, 0, 0, 0}};
  bb.b[r >> 2] = 1ULL << (((r & 3) << 4) | f);
  return bb;
}

/// Overloads of bitwise operators between a Bitboard and a Square for testing
/// whether a given bit is set in a bitboard, and for setting and clearing bits.

inline Bitboard  operator&( Bitboard  b, Square s) { return b &  square_bb(s); }
inline Bitboard  operator|( Bitboard  b, Square s) { return b |  square_bb(s); }
inline Bitboard  operator^( Bitboard  b, Square s) { return b ^  square_bb(s); }
inline Bitboard& operator|=(Bitboard& b, Square s) { return b |= square_bb(s); }
inline Bitboard& operator^=(Bitboard& b, Square s) { return b ^= square_bb(s); }

inline Bitboard  operator&(Square s, Bitboard b) { return b & s; }
inline Bitboard  operator|(Square s, Bitboard b) { return b | s; }
inline Bitboard  operator^(Square s, Bitboard b) { return b ^ s; }

inline Bitboard  operator|(Square s1, Square s2) { return square_bb(s1) | s2; }

constexpr inline more_than_one_b64(uint64_t x) {
    return x & (x - 1);
}

constexpr bool more_than_one(Bitboard b) {
    if (more_than_one_b64(b.b[0])) return true;
    if (more_than_one_b64(b.b[1])) return true;
    if (more_than_one_b64(b.b[2])) return true;
    if (more_than_one_b64(b.b[3])) return true;
    unsigned int t = ((b.b[0] ? 1 : 0) << 3) | ((b.b[1] ? 1 : 0) << 2) | ((b.b[2] ? 1 : 0) << 1) | (b.b[3] ? 1 : 0);
    return (t & (t - 1));
}

constexpr bool opposite_colors(Square s1, Square s2) {
  return (s1 + rank_of(s1) + s2 + rank_of(s2)) & 1;
}

/// rank_bb() and file_bb() return a bitboard representing all the squares on
/// the given file or rank.

constexpr Bitboard rank_bb(Rank r) {
    Bitboard b = NoSquares;
    b.b[r >> 2] = 0xFFFFULL << ((r & 3) << 4);
    return b;
}

constexpr Bitboard rank_bb(Square s) {
  return rank_bb(rank_of(s));
}

constexpr Bitboard file_bb(File f) {
  return {.b = {FileABB.b[0]<<f, FileABB.b[1]<<f, FileABB.b[2]<<f, FileABB.b[3]<<f}};
}

constexpr Bitboard file_bb(Square s) {
  return file_bb(file_of(s));
}

/// shift() moves a bitboard one or two steps as specified by the direction D

template<Direction D>
constexpr Bitboard shift(Bitboard b) {
  return  D == NORTH      ?  b             <<16 : D == SOUTH      ?  b             >>16
        : D == NORTH+NORTH?  b             <<32 : D == SOUTH+SOUTH?  b             >>32
        : D == EAST       ? (b & ~FilePBB) << 1 : D == WEST       ? (b & ~FileABB) >> 1
        : D == NORTH_EAST ? (b & ~FilePBB) <<17 : D == NORTH_WEST ? (b & ~FileABB) <<15
        : D == SOUTH_EAST ? (b & ~FilePBB) >>15 : D == SOUTH_WEST ? (b & ~FileABB) >>17
        : NoSquares;
}

/// pawn_attacks_bb() returns the squares attacked by pawns of the given color
/// from the squares in the given bitboard.

template<Color C>
constexpr Bitboard pawn_attacks_bb(Bitboard b) {
  return C == WHITE ? shift<NORTH_WEST>(b) | shift<NORTH_EAST>(b)
                    : shift<SOUTH_WEST>(b) | shift<SOUTH_EAST>(b);
}

inline Bitboard pawn_attacks_bb(Color c, Square s) {
  assert(is_ok(s));
  return PawnAttacks[c][s];
}

/// pawn_double_attacks_bb() returns the squares doubly attacked by pawns of the
/// given color from the squares in the given bitboard.

template<Color C>
constexpr Bitboard pawn_double_attacks_bb(Bitboard b) {
  return C == WHITE ? shift<NORTH_WEST>(b) & shift<NORTH_EAST>(b)
                    : shift<SOUTH_WEST>(b) & shift<SOUTH_EAST>(b);
}

/// adjacent_files_bb() returns a bitboard representing all the squares on the
/// adjacent files of a given square.

constexpr Bitboard adjacent_files_bb(Square s) {
  return shift<EAST>(file_bb(s)) | shift<WEST>(file_bb(s));
}

/// line_bb() returns a bitboard representing an entire line (from board edge
/// to board edge) that intersects the two given squares. If the given squares
/// are not on a same file/rank/diagonal, the function returns 0. For instance,
/// line_bb(SQ_C4, SQ_F7) will return a bitboard with the A2-G8 diagonal.

inline Bitboard line_bb(Square s1, Square s2) {
  assert(is_ok(s1) && is_ok(s2));
  return LineBB[s1][s2];
}

/// between_bb(s1, s2) returns a bitboard representing the squares in the semi-open
/// segment between the squares s1 and s2 (excluding s1 but including s2). If the
/// given squares are not on a same file/rank/diagonal, it returns s2. For instance,
/// between_bb(SQ_C4, SQ_F7) will return a bitboard with squares D5, E6 and F7, but
/// between_bb(SQ_E6, SQ_F8) will return a bitboard with the square F8. This trick
/// allows to generate non-king evasion moves faster: the defending piece must either
/// interpose itself to cover the check or capture the checking piece.

inline Bitboard between_bb(Square s1, Square s2) {
  assert(is_ok(s1) && is_ok(s2));
  return BetweenBB[s1][s2];
}

/// forward_ranks_bb() returns a bitboard representing the squares on the ranks in
/// front of the given one, from the point of view of the given color. For instance,
/// forward_ranks_bb(BLACK, SQ_D3) will return the 16 squares on ranks 1 and 2.

constexpr Bitboard forward_ranks_bb(Color c, Square s) {
  return c == WHITE ? ~Rank1BB << 16 * relative_rank(WHITE, s)
                    : ~Rank8BB >> 16 * relative_rank(BLACK, s);
}

/// forward_file_bb() returns a bitboard representing all the squares along the
/// line in front of the given one, from the point of view of the given color.

constexpr Bitboard forward_file_bb(Color c, Square s) {
  return forward_ranks_bb(c, s) & file_bb(s);
}


/// pawn_attack_span() returns a bitboard representing all the squares that can
/// be attacked by a pawn of the given color when it moves along its file, starting
/// from the given square.

constexpr Bitboard pawn_attack_span(Color c, Square s) {
  return forward_ranks_bb(c, s) & adjacent_files_bb(s);
}

/// passed_pawn_span() returns a bitboard which can be used to test if a pawn of
/// the given color and on the given square is a passed pawn.

constexpr Bitboard passed_pawn_span(Color c, Square s) {
  return pawn_attack_span(c, s) | forward_file_bb(c, s);
}

/// aligned() returns true if the squares s1, s2 and s3 are aligned either on a
/// straight or on a diagonal line.

inline bool aligned(Square s1, Square s2, Square s3) {
    Bitboard t = line_bb(s1, s2) & s3;
    return (t.b[0] | t.b[1] | t.b[2] | t.b[3]);
}

/// distance() functions return the distance between x and y, defined as the
/// number of steps for a king in x to reach y.

template<typename T1 = Square> inline int distance(Square x, Square y);
template<> inline int distance<File>(Square x, Square y) { return std::abs(file_of(x) - file_of(y)); }
template<> inline int distance<Rank>(Square x, Square y) { return std::abs(rank_of(x) - rank_of(y)); }
template<> inline int distance<Square>(Square x, Square y) { return SquareDistance[x][y]; }

inline int edge_distance(File f) { return std::min(f, File(FILE_P - f)); }
inline int edge_distance(Rank r) { return std::min(r, Rank(RANK_16 - r)); }

/// attacks_bb(Square) returns the pseudo attacks of the give piece type
/// assuming an empty board.

template<PieceType Pt>
inline Bitboard attacks_bb(Square s) {
  assert((Pt != PAWN) && (is_ok(s)));
  return PseudoAttacks[Pt][s];
}


/// attacks_bb(Square, Bitboard) returns the attacks by the given piece
/// assuming the board is occupied according to the passed Bitboard.
/// Sliding piece attacks do not continue past an occupied square.

template<PieceType Pt>
inline Bitboard attacks_bb(Square s, Bitboard occupied) {
  assert((Pt != PAWN) && (is_ok(s)));
  switch (Pt)
  {
  case BISHOP: return BishopMagics[s].attacks[BishopMagics[s].index(occupied)];
  case ROOK  : return   RookMagics[s].attacks[  RookMagics[s].index(occupied)];
  case QUEEN : return attacks_bb<BISHOP>(s, occupied) | attacks_bb<ROOK>(s, occupied);
  default    : return PseudoAttacks[Pt][s];
  }
}

inline Bitboard attacks_bb(PieceType pt, Square s, Bitboard occupied) {
  assert((pt != PAWN) && (is_ok(s)));
  switch (pt)
  {
  case BISHOP: return attacks_bb<BISHOP>(s, occupied);
  case ROOK  : return attacks_bb<  ROOK>(s, occupied);
  case QUEEN : return attacks_bb<BISHOP>(s, occupied) | attacks_bb<ROOK>(s, occupied);
  default    : return PseudoAttacks[pt][s];
  }
}

/// popcount() counts the number of non-zero bits in a bitboard
// Assumed gcc or compatible compiler
inline int popcount(Bitboard b) {
  return __builtin_popcountll(b.b[0]) + __builtin_popcountll(b.b[1]) + __builtin_popcountll(b.b[2]) + __builtin_popcountll(b.b[3]);
}

/// lsb() and msb() return the least/most significant bit in a non-zero bitboard
// Assumed gcc or compatible compiler
inline Square lsb(Bitboard b) {
  assert(nonemptyBB(b));
  if (b.b[0]) {
    unsigned t = __builtin_ctzll(b.b[0]);
    return make_square((File)(t & 0xF), (Rank)(t >> 4));
  }
  if (b.b[1]) {
    unsigned t = __builtin_ctzll(b.b[1]);
    return make_square((File)(t & 0xF), (Rank)((t >> 4) + 4));
  }
  if (b.b[2]) {
    unsigned t = __builtin_ctzll(b.b[2]);
    return make_square((File)(t & 0xF), (Rank)((t >> 4) + 8));
  }
  unsigned t = __builtin_ctzll(b.b[3]);
  return make_square((File)(t & 0xF), (Rank)((t >> 4) + 12));
}

inline Square msb(Bitboard b) {
  assert(nonemptyBB(b));
  if (b.b[3]) {
    unsigned t = 63 ^ __builtin_clzll(b.b[3]);
    return make_square((File)(t & 0xF), (Rank)((t >> 4) + 12));
  }
  if (b.b[2]) {
    unsigned t = 63 ^ __builtin_clzll(b.b[2]);
    return make_square((File)(t & 0xF), (Rank)((t >> 4) + 8));
  }
  if (b.b[1]) {
    unsigned t = 63 ^ __builtin_clzll(b.b[1]);
    return make_square((File)(t & 0xF), (Rank)((t >> 4) + 4));
  }
  unsigned t = 63 ^ __builtin_clzll(b.b[0]);
  return make_square((File)(t & 0xF), (Rank)(t >> 4));
}

/// least_significant_square_bb() returns the bitboard of the least significant
/// square of a non-zero bitboard. It is equivalent to square_bb(lsb(bb)).

inline Bitboard least_significant_square_bb(Bitboard b) {
  assert(nonemptyBB(b));
  return square_bb(lsb(b));
}

/// pop_lsb() finds and clears the least significant bit in a non-zero bitboard

inline Square pop_lsb(Bitboard& b) {
  assert(nonemptyBB(b));
  const Square s = lsb(b);
  b &= ~square_bb(s);
  return s;
}

/// frontmost_sq() returns the most advanced square for the given color,
/// requires a non-zero bitboard.
inline Square frontmost_sq(Color c, Bitboard b) {
  assert(nonemptyBB(b));
  return c == WHITE ? msb(b) : lsb(b);
}

} // namespace Bitboards

} // namespace Stockfish

#endif // #ifndef BITBOARD_H_INCLUDED
