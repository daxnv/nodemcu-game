#ifndef _GAME_H
#define _GAME_H

#include "Tetris.h"

class Game {
public:
    Game();
    void cycle();
private:
    static Piece &moveToStart(Piece &piece);
    Piece _piece, _preview;
    Board _board;
    int _points;
    static constexpr int _start_pos = Board::width / 2;
};

#endif //_GAME_H
