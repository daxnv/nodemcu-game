#ifndef _GAME_H
#define _GAME_H

#include "MPU9250.h"
#include "TFT_eSPI.h"
#include "Tetris.h"
#include "Wire.h"

class Game {
    public:
    Game();
    void cycleDown();
    void cycleUser();
    void levelUp();
    void loop();
    void go();

protected:
    class Input {
    public:
        Input();
        void measure();
        int shift();
        bool rotation();

    private:
        MPU9250 Sensor;
        bool last_was_rot;
    };

    static Piece &moveToStart(Piece piece);

    Input _controller;
    Piece _piece, _preview;
    Board _board;
    int _points, _counter;
    static constexpr int _start_pos = Board::width / 2;
};

class Output : public Game {
public:
    Output();
    void draw();
    void drawBlock(IntVec at, uint32_t color);
    void drawPiece(Piece piece);

private:
    static constexpr size_t width = 240;
    static constexpr size_t height = 340;
    static constexpr int block_size = 2;
    TFT_eSPI tft;
};

#endif  //_GAME_H
