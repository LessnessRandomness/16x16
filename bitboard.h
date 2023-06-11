#ifndef BITBOARD_H_INCLUDED
#define BITBOARD_H_INCLUDED

#include <string>
#include "types.h"

namespace Stockfish {

namespace Bitboards {

//void init();
std::string pretty(const Bitboard bb);

inline bool getBit(Bitboard bb, int f, int r) {
    return ((bb.b[r >> 2] >> ((r & 3) << 4)) & (1 << f));
}

constexpr Bitboard NoSquares = {.b = {0ULL, 0ULL, 0ULL, 0ULL}};
constexpr Bitboard AllSquares = {.b = {~NoSquares.b[0], ~NoSquares.b[1], ~NoSquares.b[2], ~NoSquares.b[3]}};
constexpr Bitboard DarkSquares = {.b = {0xAAAA5555AAAA5555ULL, 0xAAAA5555AAAA5555ULL, 0xAAAA5555AAAA5555ULL, 0xAAAA5555AAAA5555ULL}};
constexpr Bitboard LightSquares = {.b = {~DarkSquares.b[0], ~DarkSquares.b[1], ~DarkSquares.b[2], ~DarkSquares.b[3]}};

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

}

} // namespace Stockfish

#endif // #ifndef BITBOARD_H_INCLUDED
