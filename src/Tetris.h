#ifndef _TETRIS_H
#define _TETRIS_H

#include <stdint.h>
#include <array>
#include "Vector.h"
#include <TFT_eSPI.h>

using IntVec = Vector<int,2>;

class Block {
public:
    Block(uint16_t color = TFT_BLACK);
    void set(uint16_t color);
    bool empty();
private:
    uint16_t _color;
};

class Piece {
public:
    Piece(int8_t id, IntVec IntVec, int8_t rotation);
    void rotate();
    void move(IntVec by);
    IntVec operator[](size_t i);
    uint16_t color();
private:
    int8_t _id;
    IntVec _off;
    int8_t _rot;
};

class Board {
public:
    static constexpr size_t width = 12;
    static constexpr size_t height = 40;
    bool collision(Piece piece);
    int delete_lines();
    void addPiece(Piece piece);
private:
    std::array<std::array<Block, height>, width> _board;
};
 
#endif //_TETRIS_H