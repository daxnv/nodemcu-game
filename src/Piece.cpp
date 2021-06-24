#include "Piece.h"
#include <array>
#include <tuple>
#include <TFT_eSPI.h>

using namespace std;

//storage[id][daten/farbe][rotation][x/y]
array<tuple<array<array<uint8_t, 2>, 4>, uint16_t>, 7> storage = {{
        { {{ {{0x1b,0x55}}, {{0xaa,0x1b}}, {{0x1b,0xaa}}, {{0x55,0x1b}} }}, TFT_CYAN   }, //I
        { {{ {{0x06,0x15}}, {{0x65,0x06}}, {{0x1a,0x56}}, {{0x51,0x1a}} }}, TFT_BLUE   }, //J
        { {{ {{0x86,0x15}}, {{0x56,0x1a}}, {{0x18,0x56}}, {{0x15,0x06}} }}, 0xFC00     }, //L
        { {{ {{0x66,0x05}}, {{0x66,0x05}}, {{0x66,0x05}}, {{0x66,0x05}} }}, TFT_YELLOW }, //O
        { {{ {{0x61,0x05}}, {{0x5a,0x16}}, {{0x61,0x5a}}, {{0x05,0x16}} }}, TFT_GREEN  }, //S
        { {{ {{0x46,0x15}}, {{0x59,0x16}}, {{0x19,0x56}}, {{0x45,0x16}} }}, 0xF80F     }, //T
        { {{ {{0x16,0x05}}, {{0x99,0x16}}, {{0x16,0x5a}}, {{0x44,0x16}} }}, TFT_RED    }  //Z
}};

/*
Beispiel S:
    00  01  10  11
00 [ ] [ ] [ ] [ ]
01 [ ] [1] [2] [ ]
10 [3] [4] [ ] [ ]
11 [ ] [ ] [ ] [ ]

x-Koordinate der Nummern
  1| 2| 3| 4
 01 10|00 01 = 0x61

y-Koordinate der Nummern
  1| 2| 3| 4
 01 01|10 10 = 0x5a
*/

Piece::Piece(int8_t id, IntVec start_pos, int8_t rotation) : _id(id % 7), _off(start_pos), _rot(rotation % 4) {
}

Piece &Piece::rotate() {
    _rot = (_rot + 1) % 4;
    return *this;
}

Piece &Piece::move(IntVec by) {
    _off += by;
    return *this;
}

Piece Piece::moved(IntVec by) const {
    Piece temp = *this;
    return temp.move(by);
}

IntVec Piece::operator[](size_t i) const {
    return IntVec{
            get<0>(storage[_id])[_rot][0] >> 2*i & 0x3,
            get<0>(storage[_id])[_rot][1] >> 2*i & 0x3
    } += _off;
}

uint16_t Piece::color() const {
    return get<1>(storage[_id]);
}
