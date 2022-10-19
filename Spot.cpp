#include "Spot.h"

GamePiece *Spot::getPiece() {
    return piece;
}

void Spot::setPiece(GamePiece *piece) {
    this->piece = piece;
}

Color Spot::getColor() {
    return color;
}

void Spot::setColor(Color color) {
    this->color = color;
}

Spot::Spot(GamePiece *piece, Color color) {
    this->piece = piece;
    this->color = color;
}
