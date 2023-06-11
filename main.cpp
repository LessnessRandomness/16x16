#include <iostream>
#include "types.h"
#include "bitboard.h"
using namespace std;

int main() {
    cout << "Hello world!" << endl;
    cout << Stockfish::Bitboards::pretty(Stockfish::Bitboards::DarkSquares) << endl;
    cout << Stockfish::Bitboards::pretty(Stockfish::Bitboards::FileFBB) << endl;
    cout << Stockfish::Bitboards::pretty(Stockfish::Bitboards::Rank10BB) << endl;
    return 0;
}
