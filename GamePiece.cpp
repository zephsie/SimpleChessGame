#include "GamePiece.h"

GamePiece::GamePiece(Color color) {
    this->color = color;
}

Color GamePiece::getColor() {
    return this->color;
}

void GamePiece::setColor(Color color) {
    this->color = color;
}