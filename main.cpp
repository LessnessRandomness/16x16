#include <iostream>
#include "types.h"
#include "bitboard.h"
using namespace std;
using namespace Stockfish;
using namespace Bitboards;

int main() {
    cout << "Hello world!" << endl;
    Bitboard b1 = {.b {0x123456789ABCDEFULL, 0x123456789ABCDEFULL, 0x123456789ABCDEFULL, 0x123456789ABCDEFULL}};
    Bitboard b2 = {.b {0xFEDCBA987654321ULL, 0xFEDCBA987654321ULL, 0xFEDCBA987654321ULL, 0xFEDCBA987654321ULL}};
    Bitboard b = b1 * b2;
    cout << b1.b[0] << " " << b1.b[1] << " " << b1.b[2] << " " << b1.b[3] << endl;
    cout << b2.b[0] << " " << b2.b[1] << " " << b2.b[2] << " " << b2.b[3] << endl;
    cout << b.b[0] << " " << b.b[1] << " " << b.b[2] << " " << b.b[3] << endl;
    // cout << pretty(b1 * b1) << endl;
    return 0;
}
