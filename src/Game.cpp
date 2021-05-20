#include "Game.h"

#ifdef abs
#undef abs
#endif

#include <random>
#include <PolledTimeout.h>

using namespace std;

default_random_engine eng;
uniform_int_distribution<int8_t> from1to7(0,6);

Piece random_piece() {
    return {from1to7(eng), {0, 0}, 0};
}

Game::Game() :
    _piece(moveToStart(random_piece())),
    _preview(random_piece()),
    _board(),
    _points(0),
    _counter(0)
{
}

void Game::cycleDown() {
    Piece moved_piece = _piece.moved({0,1});
    if (_board.collision(moved_piece)) {
        levelUp();
    }
    else {
        _piece = moved_piece;
    }
}

void Game::levelUp() {
    _board.addPiece(_piece);

    int n = _board.deleteLines();
    _points += n * n;
    ++_counter;

    _piece = moveToStart(_preview);
    _preview = random_piece();
}

void Game::cycleUser() {
    Piece changed_piece = _piece;

    if (_controller.rotate())
        changed_piece.rotate();

    IntVec shift{0,0};
    shift[0] = _controller.shift();
    changed_piece.move(shift);

    if (!_board.collision(changed_piece))
        _piece = changed_piece;
}

Piece &Game::moveToStart(Piece piece) {
    return piece.move({_start_pos, 0});
}

using Clock = esp8266::polledTimeout::periodicFastMs;
Clock user_cycle(31); // 1/32 = 0.03125

void Game::loop() {
    Clock down_cycle(1000);
    while (!_board.isFull()) {
        while (!down_cycle) {
            if (user_cycle) {
                _controller.measure();
                cycleUser();
            }
        }
        down_cycle.reset(1000 - _counter);
        cycleDown();
    }
}
