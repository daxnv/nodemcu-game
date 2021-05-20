#ifndef _GAME_H
#define _GAME_H

#include "Tetris.h"
#include "Input.h"

class Game {
public:
    Game();
    void levelUp();
    void cycleDown();
    void cycleUser();
private:
    static Piece &moveToStart(Piece piece);

    Input _controller;
    Piece _piece, _preview;
    Board _board;
    int _points, _counter;
    static constexpr int _start_pos = Board::width / 2;
};

#endif //_GAME_H
