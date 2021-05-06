#include "Tetris.h"
#include <tuple>
#include <TFT_eSPI.h>
using namespace std;

Block::Block(uint16_t color) : _color(color) {
}

void Block::set(uint16_t color) {
    _color = color;
}

bool Block::empty() {
    return _color == TFT_BLACK;
}

//storage[id][daten/farbe][rotation][x/y]
array<tuple<array<array<uint8_t, 2>, 4>, uint16_t>, 7> storage = {{
    { {{ {{0x55,0x1b}}, {{0x1b,0x55}}, {{0xaa,0x1b}}, {{0x1b,0xaa}} }}, TFT_BLUE   }, //I
    { {{ {{0x51,0x1a}}, {{0x06,0x15}}, {{0x65,0x06}}, {{0x1a,0x56}} }}, TFT_RED    }, //J
    { {{ {{0x56,0x1a}}, {{0x1c,0x56}}, {{0x15,0x06}}, {{0x86,0x15}} }}, TFT_YELLOW }, //L
    { {{ {{0x66,0x66}}, {{0x66,0x66}}, {{0x66,0x66}}, {{0x66,0x66}} }}, TFT_CYAN   }, //O
    { {{ {{0x61,0x5a}}, {{0x05,0x16}}, {{0x61,0x05}}, {{0x5a,0x16}} }}, TFT_PURPLE }, //S
    { {{ {{0x16,0x56}}, {{0x45,0x16}}, {{0x46,0x15}}, {{0x59,0x16}} }}, TFT_GREEN  }, //T
    { {{ {{0x16,0x5a}}, {{0x44,0x16}}, {{0x16,0x05}}, {{0x99,0x16}} }}, TFT_ORANGE }  //Z
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

void Piece::rotate() {
    _rot = (_rot + 1) % 4;
}

void Piece::move(IntVec by) {
    _off += by;
}

IntVec Piece::operator[](size_t i) {
    return IntVec{
        get<0>(storage[_id])[_rot][0] >> 2*i & 0x3,
        get<0>(storage[_id])[_rot][1] >> 2*i & 0x3
    } += _off;
}
