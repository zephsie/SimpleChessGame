#ifndef CHESS_PAWNPIECE_H
#define CHESS_PAWNPIECE_H

#include "GamePiece.h"

class PawnPiece : public GamePiece {

public:
    PawnPiece(Color color) : GamePiece(color) {}

    bool validation(int xStart, int yStart, int xEnd, int yEnd, Spot** board) override;
};


#endif
