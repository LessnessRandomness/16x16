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

#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <algorithm>

namespace Stockfish
{

struct Bitboard {
    uint64_t b[4];

    constexpr Bitboard operator >> (unsigned int bits) const {
        Bitboard bb = {.b {b[0], b[1], b[2], b[3]}};
        unsigned int nn = 64 - bits;
        uint64_t mask = ~0 >> nn;
        uint64_t r = b[1] & mask;
        bb.b[0] = (b[0] >> bits) | (r << nn);
        r = b[2] & mask;
        bb.b[1] = (b[1] >> bits) | (r << nn);
        r = b[3] & mask;
        bb.b[2] = (b[2] >> bits) | (r << nn);
        bb.b[3] >>= bits;
        return bb;
    };

    constexpr Bitboard operator << (unsigned int bits) const {
        Bitboard bb = {.b {b[0], b[1], b[2], b[3]}};
        unsigned int nn = 64 - bits;
        uint64_t mask = ~0 << nn;
        uint64_t r = b[1] & mask;
        bb.b[0] = (b[0] << bits) | (r >> nn);
        r = b[2] & mask;
        bb.b[1] = (b[1] << bits) | (r >> nn);
        r = b[3] & mask;
        bb.b[2] = (b[2] << bits) | (r >> nn);
        bb.b[3] <<= bits;
        return bb;
    };

    inline Bitboard& operator |=(const Bitboard x) {
        b[0] |= x.b[0];
        b[1] |= x.b[1];
        b[2] |= x.b[2];
        b[3] |= x.b[3];
        return *this;
    }
    inline Bitboard& operator &=(const Bitboard x) {
        b[0] &= x.b[0];
        b[1] &= x.b[1];
        b[2] &= x.b[2];
        b[3] &= x.b[3];
        return *this;
    }
    inline Bitboard& operator ^=(const Bitboard x) {
        b[0] ^= x.b[0];
        b[1] ^= x.b[1];
        b[2] ^= x.b[2];
        b[3] ^= x.b[3];
        return *this;
    }

    constexpr Bitboard operator ~ () const {
        return {.b {~b[0], ~b[1], ~b[2], ~b[3]}};
    }
};

constexpr inline Bitboard operator |(const Bitboard x, const Bitboard y) {
    return {.b {x.b[0] | y.b[0], x.b[1] | y.b[1], x.b[2] | y.b[2], x.b[3] | y.b[3]}};
}
constexpr inline Bitboard operator &(const Bitboard x, const Bitboard y) {
    return {.b {x.b[0] & y.b[0], x.b[1] & y.b[1], x.b[2] & y.b[2], x.b[3] & y.b[3]}};
}
constexpr inline Bitboard operator ^(const Bitboard x, const Bitboard y) {
    return {.b {x.b[0] ^ y.b[0], x.b[1] ^ y.b[1], x.b[2] ^ y.b[2], x.b[3] ^ y.b[3]}};
}

inline Bitboard operator *(const Bitboard x, const Bitboard y) {
    uint64_t w[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint64_t xx[8] = {(uint32_t)x.b[0], x.b[0]>>32, (uint32_t)x.b[1], x.b[1]>>32, (uint32_t)x.b[2], x.b[2]>>32, (uint32_t)x.b[3], x.b[3]>>32};
    uint64_t yy[8] = {(uint32_t)y.b[0], y.b[0]>>32, (uint32_t)y.b[1], y.b[1]>>32, (uint32_t)y.b[2], y.b[2]>>32, (uint32_t)y.b[3], y.b[3]>>32};
    for (int i = 0; i <= 7; i++) {
        uint64_t k = 0ULL;
        for (int j = 0; j <= 7-i; j++) {
            uint64_t t = xx[i] * yy[j] + w[i+j] + k;
            w[i+j] = (uint32_t)t;
            k = t >> 32;
        }
    }
    return {.b {w[0] | (w[1] << 32), w[2] | (w[3] << 32), w[4] | (w[5] << 32), w[6] | (w[7] << 32)}};
}

constexpr Bitboard operator - (const Bitboard x, const Bitboard y) {
    Bitboard result = {.b = {0, 0, 0, 0}};
    /// ???
    return result;
}


/// A move needs 19 (three bytes?) bits to be stored
///
/// bit  0- 7: destination square (from 0 to 255)
/// bit  8-15: origin square (from 0 to 255)
/// bit 16-17: promotion piece type - 2 (from KNIGHT-2 to QUEEN-2)
/// bit 18-19: special move flag: promotion (1), en passant (2)
/// NOTE: en passant bit is set only when a pawn can be captured
///
/// Special cases are MOVE_NONE and MOVE_NULL. We can sneak these in because in
/// any normal move destination square is always different from origin square
/// while MOVE_NONE and MOVE_NULL have the same origin and destination square.

enum Move : int {
  MOVE_NONE,
  MOVE_NULL = 257
};

enum MoveType {
  NORMAL,
  PROMOTION = 1 << 18,
  EN_PASSANT = 2 << 18,
};

enum Color {
  WHITE, BLACK, COLOR_NB = 2
};

enum PieceType {
  NO_PIECE_TYPE, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING,
  ALL_PIECES = 0,
  PIECE_TYPE_NB = 8
};

enum Piece {
  NO_PIECE,
  W_PAWN = PAWN,     W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
  B_PAWN = PAWN + 8, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING,
  PIECE_NB = 16
};

enum Square : int {
  SQ_A1,  SQ_B1,  SQ_C1,  SQ_D1,  SQ_E1,  SQ_F1,  SQ_G1,  SQ_H1,  SQ_I1,  SQ_J1,  SQ_K1,  SQ_L1,  SQ_M1,  SQ_N1,  SQ_O1,  SQ_P1,
  SQ_A2,  SQ_B2,  SQ_C2,  SQ_D2,  SQ_E2,  SQ_F2,  SQ_G2,  SQ_H2,  SQ_I2,  SQ_J2,  SQ_K2,  SQ_L2,  SQ_M2,  SQ_N2,  SQ_O2,  SQ_P2,
  SQ_A3,  SQ_B3,  SQ_C3,  SQ_D3,  SQ_E3,  SQ_F3,  SQ_G3,  SQ_H3,  SQ_I3,  SQ_J3,  SQ_K3,  SQ_L3,  SQ_M3,  SQ_N3,  SQ_O3,  SQ_P3,
  SQ_A4,  SQ_B4,  SQ_C4,  SQ_D4,  SQ_E4,  SQ_F4,  SQ_G4,  SQ_H4,  SQ_I4,  SQ_J4,  SQ_K4,  SQ_L4,  SQ_M4,  SQ_N4,  SQ_O4,  SQ_P4,
  SQ_A5,  SQ_B5,  SQ_C5,  SQ_D5,  SQ_E5,  SQ_F5,  SQ_G5,  SQ_H5,  SQ_I5,  SQ_J5,  SQ_K5,  SQ_L5,  SQ_M5,  SQ_N5,  SQ_O5,  SQ_P5,
  SQ_A6,  SQ_B6,  SQ_C6,  SQ_D6,  SQ_E6,  SQ_F6,  SQ_G6,  SQ_H6,  SQ_I6,  SQ_J6,  SQ_K6,  SQ_L6,  SQ_M6,  SQ_N6,  SQ_O6,  SQ_P6,
  SQ_A7,  SQ_B7,  SQ_C7,  SQ_D7,  SQ_E7,  SQ_F7,  SQ_G7,  SQ_H7,  SQ_I7,  SQ_J7,  SQ_K7,  SQ_L7,  SQ_M7,  SQ_N7,  SQ_O7,  SQ_P7,
  SQ_A8,  SQ_B8,  SQ_C8,  SQ_D8,  SQ_E8,  SQ_F8,  SQ_G8,  SQ_H8,  SQ_I8,  SQ_J8,  SQ_K8,  SQ_L8,  SQ_M8,  SQ_N8,  SQ_O8,  SQ_P8,
  SQ_A9,  SQ_B9,  SQ_C9,  SQ_D9,  SQ_E9,  SQ_F9,  SQ_G9,  SQ_H9,  SQ_I9,  SQ_J9,  SQ_K9,  SQ_L9,  SQ_M9,  SQ_N9,  SQ_O9,  SQ_P9,
  SQ_A10, SQ_B10, SQ_C10, SQ_D10, SQ_E10, SQ_F10, SQ_G10, SQ_H10, SQ_I10, SQ_J10, SQ_K10, SQ_L10, SQ_M10, SQ_N10, SQ_O10, SQ_P10,
  SQ_A11, SQ_B11, SQ_C11, SQ_D11, SQ_E11, SQ_F11, SQ_G11, SQ_H11, SQ_I11, SQ_J11, SQ_K11, SQ_L11, SQ_M11, SQ_N11, SQ_O11, SQ_P11,
  SQ_A12, SQ_B12, SQ_C12, SQ_D12, SQ_E12, SQ_F12, SQ_G12, SQ_H12, SQ_I12, SQ_J12, SQ_K12, SQ_L12, SQ_M12, SQ_N12, SQ_O12, SQ_P12,
  SQ_A13, SQ_B13, SQ_C13, SQ_D13, SQ_E13, SQ_F13, SQ_G13, SQ_H13, SQ_I13, SQ_J13, SQ_K13, SQ_L13, SQ_M13, SQ_N13, SQ_O13, SQ_P13,
  SQ_A14, SQ_B14, SQ_C14, SQ_D14, SQ_E14, SQ_F14, SQ_G14, SQ_H14, SQ_I14, SQ_J14, SQ_K14, SQ_L14, SQ_M14, SQ_N14, SQ_O14, SQ_P14,
  SQ_A15, SQ_B15, SQ_C15, SQ_D15, SQ_E15, SQ_F15, SQ_G15, SQ_H15, SQ_I15, SQ_J15, SQ_K15, SQ_L15, SQ_M15, SQ_N15, SQ_O15, SQ_P15,
  SQ_A16, SQ_B16, SQ_C16, SQ_D16, SQ_E16, SQ_F16, SQ_G16, SQ_H16, SQ_I16, SQ_J16, SQ_K16, SQ_L16, SQ_M16, SQ_N16, SQ_O16, SQ_P16,
  SQ_NONE,

  SQUARE_ZERO = 0,
  SQUARE_NB   = 256
};

enum Direction : int {
  NORTH =  16,
  EAST  =  1,
  SOUTH = -NORTH,
  WEST  = -EAST,

  NORTH_EAST = NORTH + EAST,
  SOUTH_EAST = SOUTH + EAST,
  SOUTH_WEST = SOUTH + WEST,
  NORTH_WEST = NORTH + WEST
};

enum File : int {
  FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_I, FILE_J, FILE_K, FILE_L, FILE_M, FILE_N, FILE_O, FILE_P, FILE_NB
};

enum Rank : int {
  RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_9, RANK_10, RANK_11, RANK_12, RANK_13, RANK_14, RANK_15, RANK_16, RANK_NB
};

#define ENABLE_BASE_OPERATORS_ON(T)                                \
constexpr T operator+(T d1, int d2) { return T(int(d1) + d2); }    \
constexpr T operator-(T d1, int d2) { return T(int(d1) - d2); }    \
constexpr T operator-(T d) { return T(-int(d)); }                  \
inline T& operator+=(T& d1, int d2) { return d1 = d1 + d2; }       \
inline T& operator-=(T& d1, int d2) { return d1 = d1 - d2; }

#define ENABLE_INCR_OPERATORS_ON(T)                                \
inline T& operator++(T& d) { return d = T(int(d) + 1); }           \
inline T& operator--(T& d) { return d = T(int(d) - 1); }

#define ENABLE_FULL_OPERATORS_ON(T)                                \
ENABLE_BASE_OPERATORS_ON(T)                                        \
constexpr T operator*(int i, T d) { return T(i * int(d)); }        \
constexpr T operator*(T d, int i) { return T(int(d) * i); }        \
constexpr T operator/(T d, int i) { return T(int(d) / i); }        \
constexpr int operator/(T d1, T d2) { return int(d1) / int(d2); }  \
inline T& operator*=(T& d, int i) { return d = T(int(d) * i); }    \
inline T& operator/=(T& d, int i) { return d = T(int(d) / i); }

ENABLE_FULL_OPERATORS_ON(Direction)

ENABLE_INCR_OPERATORS_ON(Piece)
ENABLE_INCR_OPERATORS_ON(PieceType)
ENABLE_INCR_OPERATORS_ON(Square)
ENABLE_INCR_OPERATORS_ON(File)
ENABLE_INCR_OPERATORS_ON(Rank)

#undef ENABLE_FULL_OPERATORS_ON
#undef ENABLE_INCR_OPERATORS_ON
#undef ENABLE_BASE_OPERATORS_ON

constexpr Square operator+(Square s, Direction d) { return Square(int(s) + int(d)); }
constexpr Square operator-(Square s, Direction d) { return Square(int(s) - int(d)); }
inline Square& operator+=(Square& s, Direction d) { return s = s + d; }
inline Square& operator-=(Square& s, Direction d) { return s = s - d; }

constexpr Color operator~(Color c) {
  return Color(c ^ BLACK); // Toggle color
}

constexpr Square flip_rank(Square s) { // Swap A1 <-> A8
  return Square(s ^ SQ_A16);
}

constexpr Square flip_file(Square s) { // Swap A1 <-> H1
  return Square(s ^ SQ_P1);
}

constexpr Piece operator~(Piece pc) {
  return Piece(pc ^ 8); // Swap color of piece B_KNIGHT <-> W_KNIGHT
}

constexpr Square make_square(File f, Rank r) {
  return Square((r << 4) + f);
}

constexpr Piece make_piece(Color c, PieceType pt) {
  return Piece((c << 3) + pt);
}

constexpr PieceType type_of(Piece pc) {
  return PieceType(pc & 7);
}

inline Color color_of(Piece pc) {
  assert(pc != NO_PIECE);
  return Color(pc >> 3);
}

constexpr bool is_ok(Move m) {
  return m != MOVE_NONE && m != MOVE_NULL;
}

constexpr bool is_ok(Square s) {
  return s >= SQ_A1 && s <= SQ_P16;
}

constexpr File file_of(Square s) {
  return File(s & 15);
}

constexpr Rank rank_of(Square s) {
  return Rank(s >> 4);
}

constexpr Square relative_square(Color c, Square s) {
  return Square(s ^ (c * 240));
}

constexpr Rank relative_rank(Color c, Rank r) {
  return Rank(r ^ (c * 15));
}

constexpr Rank relative_rank(Color c, Square s) {
  return relative_rank(c, rank_of(s));
}

constexpr Direction pawn_push(Color c) {
  return c == WHITE ? NORTH : SOUTH;
}

constexpr Square from_sq(Move m) {
  assert(is_ok(m));
  return Square((m >> 8) & 0xFF);
}

constexpr Square to_sq(Move m) {
  assert(is_ok(m));
  return Square(m & 0xFF);
}

constexpr int from_to(Move m) {
  return m & 0xFFFF;
}

constexpr MoveType type_of(Move m) {
  return MoveType(m & (3 << 18));
}

constexpr PieceType promotion_type(Move m) {
  return PieceType(((m >> 16) & 3) + KNIGHT);
}

constexpr Move make_move(Square from, Square to) {
  return Move((from << 8) + to);
}

template<MoveType T>
constexpr Move make(Square from, Square to, PieceType pt = KNIGHT) {
  return Move(T + ((pt - KNIGHT) << 16) + (from << 8) + to);
}

}

#endif
