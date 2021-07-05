#include "Tetris.h"

using namespace std;

Block::Block(uint16_t color) : _color(color) {
}

void Block::set(uint16_t color) {
    _color = color;
}

bool Block::empty() const {
    return _color == TFT_BLACK;
}

uint16_t Block::color() const {
    return _color;
}

Block &Board::at(IntVec at) {
    return _board[at[1]][at[0]];
}

Block Board::getAt(IntVec at) const {
    if (at[0] < 0 || width <= at[0] ||
        at[1] < 0 || height <= at[1])
        return {TFT_WHITE};
    return _board[at[1]][at[0]];
}

int Board::deleteLines() {
    auto line_full = [](const Line &line) {
        return all_of(line.begin(), line.end(),
                      [](Block block) {return !block.empty();}
        );
    };
    auto new_rend = remove_if(_board.rbegin(), _board.rend(), line_full);
    fill(new_rend, _board.rend(), Line());

    return _board.rend() - new_rend;
}

bool Board::collision(const Piece &piece) const {
    for (int i=0; i<4; ++i)
        if (!getAt(piece[i]).empty())
            return true;

    return false;
}

void Board::addPiece(const Piece &piece) {
    for (int i=0; i<4; ++i){
        at(piece[i]) = Block(piece.color());
    }
}

bool Board::isFull() const {
    auto line_full = [](const Line &line) { //gibt nicht zurück ob Zeile ganz voll, sondern
        return any_of(line.begin(), line.end(), //ob ein Block der Zeile
                      [](Block block) {return !block.empty();} //nicht leer ist
        );
    };

    return any_of(_board.begin(), _board.begin()+2, line_full);
}
