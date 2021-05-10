//
// Created by dan on 10.05.21.
//

#ifndef _PIECE_H
#define _PIECE_H

#include <cstdint>
#include "Vector.h"

using IntVec = Vector<int,2>;

class Piece {
public:
    Piece(int8_t id, IntVec start_pos, int8_t rotation);
    Piece &rotate();
    Piece &move(IntVec by);
    Piece moved(IntVec by);
    IntVec operator[](size_t i);
    uint16_t color() const;
private:
    int8_t _id;
    IntVec _off;
    int8_t _rot;
};

#endif //_PIECE_H
