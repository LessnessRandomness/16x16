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

#include <iostream>
#include "types.h"
#include "bitboard.h"
using namespace std;
using namespace Stockfish;
using namespace Bitboards;

int main() {
    cout << "Hello world!" << endl;
    Bitboard t = (NoSquares | SQ_P16);
    unsigned n;
    while (true) {
        cin >> n;
        cout << pretty(t.auxForRightShift(n & 0x3F)) << endl;
        cout << pretty(t >> n) << endl;
    }

    //Bitboards::init();
    return 0;
}
