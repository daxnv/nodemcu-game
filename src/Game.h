#ifndef _GAME_H
#define _GAME_H

#include "Tetris.h"

class Game {
public:
    Game();
    void cycleDown();
    void cycleUser();
    void levelUp();
    void loop();

private:
    class Input {
    public:
        Input();
        void measure();
        int shift();
        bool rotation();
        bool down();

    private:
        bool last_was_rot;
    };

    static Piece &moveToStart(Piece &piece);
    static Piece moveToStart(Piece &&piece);

    Input _controller;
    Piece _piece, _preview;
    Board _board;
    int _points, _level;
    static constexpr int _start_pos = Board::width / 2;

    //Display part
    void drawBlock(IntVec at, uint32_t color);
    void drawPiece(Piece piece);
    void clearPiece(Piece piece);
    void drawBoard();

    class Display {
    public:
        static constexpr size_t width = 240;
        static constexpr size_t height = 340;
    };

    static constexpr int block_size = 5;
    TFT_eSPI tft;
};

extern Game game;

#endif  //_GAME_H
