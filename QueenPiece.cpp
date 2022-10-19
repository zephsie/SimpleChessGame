#include "QueenPiece.h"

bool QueenPiece::validation(int xStart, int yStart, int xEnd, int yEnd, Spot **board) {
    if (board[xEnd][yEnd].getPiece() != nullptr && board[xEnd][yEnd].getPiece()->getColor() == this->getColor()) {
        return false;
    }

    if (abs(xStart - xEnd) != abs(yStart - yEnd) && xStart != xEnd && yStart != yEnd) {
        return false;
    }

    if (abs(xStart - xEnd) == abs(yStart - yEnd)) {
        if (xStart < xEnd && yStart < yEnd) {
            for (int i = xStart + 1, j = yStart + 1; i < xEnd && j < yEnd; i++, j++) {
                if (board[i][j].getPiece() != nullptr) {
                    return false;
                }
            }
        } else if (xStart < xEnd && yStart > yEnd) {
            for (int i = xStart + 1, j = yStart - 1; i < xEnd && j > yEnd; i++, j--) {
                if (board[i][j].getPiece() != nullptr) {
                    return false;
                }
            }
        } else if (xStart > xEnd && yStart < yEnd) {
            for (int i = xStart - 1, j = yStart + 1; i > xEnd && j < yEnd; i--, j++) {
                if (board[i][j].getPiece() != nullptr) {
                    return false;
                }
            }
        } else if (xStart > xEnd && yStart > yEnd) {
            for (int i = xStart - 1, j = yStart - 1; i > xEnd && j > yEnd; i--, j--) {
                if (board[i][j].getPiece() != nullptr) {
                    return false;
                }
            }
        }
    }

    if (xStart == xEnd) {
        if (yStart < yEnd) {
            for (int i = yStart + 1; i < yEnd; i++) {
                if (board[xStart][i].getPiece() != nullptr) {
                    return false;
                }
            }
        } else {
            for (int i = yStart - 1; i > yEnd; i--) {
                if (board[xStart][i].getPiece() != nullptr) {
                    return false;
                }
            }
        }
    } else if (yStart == yEnd) {
        if (xStart < xEnd) {
            for (int i = xStart + 1; i < xEnd; i++) {
                if (board[i][yStart].getPiece() != nullptr) {
                    return false;
                }
            }
        } else {
            for (int i = xStart - 1; i > xEnd; i--) {
                if (board[i][yStart].getPiece() != nullptr) {
                    return false;
                }
            }
        }
    }

    return true;
}

