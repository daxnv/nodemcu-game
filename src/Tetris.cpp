#include "Tetris.h"
#include <algorithm>
#include <TFT_eSPI.h>

using namespace std;

Block::Block(uint16_t color) : _color(color) {
}

void Block::set(uint16_t color) {
    _color = color;
}

bool Block::empty() const {
    return _color == TFT_BLACK;
}

Block &Board::at(IntVec at) {
    return _board[at[1]][at[0]];
}

Block Board::getAt(IntVec at) {
    if (at[0] < 0 || width <= at[0] ||
        at[1] < 0 || height <= at[1])
        return {TFT_WHITE};
    return _board[at[1]][at[0]];
}

int Board::deleteLines() {
    auto line_full = [](Line line) {
        return all_of(line.begin(), line.end(),
                      [](const Block &block) {return !block.empty();}
        );
    };
    auto new_end = remove_if(_board.begin(), _board.end(), line_full);
    fill(new_end, _board.end(), Line());

    return _board.end() - new_end;
}

bool Board::collision(Piece piece) {
    for (int i=0; i<4; ++i)
        if (!getAt(piece[i]).empty())
            return true;

    return false;
}

void Board::addPiece(Piece piece) {
    for (int i=0; i<4; ++i){
        at(piece[i]) = Block(piece.color());
    }
}

bool Board::isFull() {
    auto line_full = [](Line line) { //gibt nicht zurück ob Zeile ganz voll, sondern
        return any_of(line.begin(), line.end(), //ob ein Block der Zeile
                      [](const Block &block) {return !block.empty();} //nicht leer ist
        );
    };
    return any_of(_board.rend(), _board.rend()+2, line_full);
}
