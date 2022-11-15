#include "KingPiece.h"
#include <stdlib.h>
#include <stdlib.h>

bool KingPiece::validation(int xStart, int yStart, int xEnd, int yEnd, Spot **board) {
    if (abs(xStart - xEnd) > 1 || abs(yStart - yEnd) > 1) {
        return false;
    }

    return true;
}


