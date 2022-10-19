#include "RookPiece.h"

bool RookPiece::validation(int xStart, int yStart, int xEnd, int yEnd, Spot **board) {
    if (xStart == xEnd) {
        if (yStart < yEnd) {
            for (int i = yStart + 1; i < yEnd; i++) {
                if (board[xStart][i].getPiece() != nullptr) {
                    return false;
                }
            }

			return true;
        } else {
            for (int i = yStart - 1; i > yEnd; i--) {
                if (board[xStart][i].getPiece() != nullptr) {
                    return false;
                }
            }

			return true;
        }
    } else if (yStart == yEnd) {
        if (xStart < xEnd) {
            for (int i = xStart + 1; i < xEnd; i++) {
                if (board[i][yStart].getPiece() != nullptr) {
                    return false;
                }
            }

			return true;
        } else {
            for (int i = xStart - 1; i > xEnd; i--) {
                if (board[i][yStart].getPiece() != nullptr) {
                    return false;
                }
            }

			return true;
        }
    }

    return false;
}
