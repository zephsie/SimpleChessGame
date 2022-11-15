#ifndef CHESS_PAWNPIECE_H
#define CHESS_PAWNPIECE_H

#include "GamePiece.h"

class PawnPiece : public GamePiece {
private:
    bool validation(int xStart, int yStart, int xEnd, int yEnd, Spot **board) override;

public:
    PawnPiece(Color color) : GamePiece(color) {}
};


#endif
