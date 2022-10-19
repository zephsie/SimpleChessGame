#include "BishopPiece.h"

bool BishopPiece::validation(int xStart, int yStart, int xEnd, int yEnd, Spot** board) {
    if (abs(xStart - xEnd) != abs(yStart - yEnd)) {
        return false;
    }

    if (xStart < xEnd&& yStart < yEnd) {
        for (int i = xStart + 1, j = yStart + 1; i < xEnd && j < yEnd; i++, j++) {
            if (board[i][j].getPiece() != nullptr) {
                return false;
            }
        }
    }
    else if (xStart < xEnd && yStart > yEnd) {
        for (int i = xStart + 1, j = yStart - 1; i < xEnd && j > yEnd; i++, j--) {
            if (board[i][j].getPiece() != nullptr) {
                return false;
            }
        }
    }
    else if (xStart > xEnd && yStart < yEnd) {
        for (int i = xStart - 1, j = yStart + 1; i > xEnd && j < yEnd; i--, j++) {
            if (board[i][j].getPiece() != nullptr) {
                return false;
            }
        }
    }
    else if (xStart > xEnd && yStart > yEnd) {
        for (int i = xStart - 1, j = yStart - 1; i > xEnd && j > yEnd; i--, j--) {
            if (board[i][j].getPiece() != nullptr) {
                return false;
            }
        }
    }

    return true;
}