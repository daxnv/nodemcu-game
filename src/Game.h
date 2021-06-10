#ifndef _GAME_H
#define _GAME_H

#include "MPU9250.h"
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
  class Output {
   public:
    Output(int width, int heigh);
    void draw();

   private:
    int width;
    int heigh;
    TFT_eSPI tft;
  };
  Output display;

 private:
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

#endif  //_GAME_H
