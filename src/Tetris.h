#ifndef _TETRIS_H
#define _TETRIS_H

#include <TFT_eSPI.h>
#include "Piece.h"

class Block {
public:
    Block(uint16_t color = TFT_BLACK);
    void set(uint16_t color);
    bool empty() const;
    uint16_t color() const;
private:
    uint16_t _color;
};

class Board {
public:
    static constexpr size_t width = 12;
    static constexpr size_t height = 40;
    bool collision(const Piece &piece) const;
    int deleteLines();
    void addPiece(const Piece &piece);
    Block &at(IntVec at);
    Block getAt(IntVec at) const;
    bool isFull() const;
private:
    using Line = std::array<Block, width>;
    // _board[y][x]
    std::array<Line, height> _board;
};
 
#endif //_TETRIS_H
