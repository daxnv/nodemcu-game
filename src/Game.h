#ifndef _GAME_H
#define _GAME_H

#include "Tetris.h"

class Game {
public:
    Game();
    void cycleDown();
    void cycleUser();
    void levelUp();
    void play();

private:
    class Display {
    public:
        static constexpr size_t width = 240;
        static constexpr size_t height = 340;
    };
    static constexpr int start_pos = Board::width / 2 - 2;
    static constexpr int block_size = 8;
    static constexpr size_t left_bound = (Display::width - Board::width * block_size) / 2, upper_bound = 16 * 2 + 1;
    class Preview {
    public:
        static constexpr size_t left_bound = Display::width - 5 * block_size, upper_bound = block_size;
    };
    static constexpr int initial_speed = 666, speed_c = 5;

    class Input {
    public:
        Input();
        void measure();
        int shift();
        bool rotation();
        bool down();

    private:
        static constexpr float y_threshold = 7;
        static constexpr float x_threshold = 2;
        static constexpr float rot_threshold = -2.5;

        bool last_was_rot;
    };

    static Piece &moveToStart(Piece &piece);
    static Piece moveToStart(Piece &&piece);

    Input _controller;
    Piece _piece, _preview;
    Board _board;
    int _points, _level;

    //Display part
    void drawBlock(IntVec at, uint32_t color);
    void drawPiece(Piece piece);
    void clearPiece(Piece piece);
    void drawPreview(Piece piece);
    void drawBoard();
    void drawScore();
    void drawInit();
    void drawOver();

    TFT_eSPI tft;
};

extern Game game;

#endif  //_GAME_H
