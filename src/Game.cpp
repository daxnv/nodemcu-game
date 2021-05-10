#include "Game.h"

#ifdef abs
#undef abs
#endif

#include <random>

using namespace std;

default_random_engine eng;
uniform_int_distribution<int8_t> from1to7(0,6);

Piece random_piece() {
    return {from1to7(eng), {0, 0}, 0};
}

Game::Game() :
    _piece(random_piece().move({_start_pos, 0})),
    _preview(random_piece()),
    _board(),
    _points(0)
{
}

void Game::cycle() {
    Piece moved_piece = _piece.moved({0,1});
    if (_board.collision(moved_piece)) {
        _board.addPiece(_piece);

        int n = _board.deleteLines();
        _points += n * n;

        _piece = moveToStart(_preview);
        _preview = random_piece();
    }
    else {
        _piece = moved_piece;
    }
}

Piece &Game::moveToStart(Piece &piece) {
    return piece.move({_start_pos, 0});
}
