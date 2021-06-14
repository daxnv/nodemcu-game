#ifndef _TETRIS_H
#define _TETRIS_H

#include <cstdint>
#include <array>
#include "Piece.h"
#include <TFT_eSPI.h>

class Block {
public:
    Block(uint16_t color = TFT_BLACK);
    void set(uint16_t color);
    bool empty() const;
    uint16_t color();
private:
    uint16_t _color;
};

class Board {
public:
    static constexpr size_t width = 12;
    static constexpr size_t height = 40;
    bool collision(Piece piece);
    int deleteLines();
    void addPiece(Piece piece);
    Block &at(IntVec at);
    Block getAt(IntVec at);
    bool isFull();
private:
    using Line = std::array<Block, width>;
    // _board[y][x]
    std::array<Line, height> _board;
};
 
#endif //_TETRIS_H
