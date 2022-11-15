#include "KnightPiece.h"
#include <stdlib.h>

bool KnightPiece::validation(int xStart, int yStart, int xEnd, int yEnd, Spot **board) {
    if (abs(xStart - xEnd) == 2 && abs(yStart - yEnd) == 1) {
        return true;
    }

    if (abs(xStart - xEnd) == 1 && abs(yStart - yEnd) == 2) {
        return true;
    }

    return false;
}
