#include "PawnPiece.h"
#include <stdlib.h>

bool PawnPiece::validation(int xStart, int yStart, int xEnd, int yEnd, Spot **board) {
    if (this->getColor() == Color::WHITE && xEnd == xStart - 1 && yStart == yEnd) {
        if (board[xEnd][yEnd].getPiece() == nullptr) {
            return true;
        }
    } else if (this->getColor() == Color::BLACK && xEnd == xStart + 1 && yStart == yEnd) {
        if (board[xEnd][yEnd].getPiece() == nullptr) {
            return true;
        }
    }

    if (this->getColor() == Color::WHITE && xEnd == xStart - 1 && abs(yStart - yEnd) == 1) {
        if (board[xEnd][yEnd].getPiece() != nullptr) {
            return true;
        }
    } else if (this->getColor() == Color::BLACK && xEnd == xStart + 1 && abs(yStart - yEnd) == 1) {
        if (board[xEnd][yEnd].getPiece() != nullptr) {
            return true;
        }
    }

    if (this->getColor() == Color::WHITE && xStart == 6 && xEnd == xStart - 2 && yStart == yEnd) {
        if (board[xEnd][yEnd].getPiece() == nullptr && board[xEnd + 1][yEnd].getPiece() == nullptr) {
            return true;
        }
    } else if (this->getColor() == Color::BLACK && xStart == 1 && xEnd == xStart + 2 && yStart == yEnd) {
        if (board[xEnd][yEnd].getPiece() == nullptr && board[xEnd - 1][yEnd].getPiece() == nullptr) {
            return true;
        }
    }

    return false;
}
