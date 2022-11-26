#ifndef CHESS_BISHOPPIECE_H
#define CHESS_BISHOPPIECE_H

#include "GamePiece.h"

class BishopPiece : public GamePiece {

public:
    BishopPiece(Color color) : GamePiece(color) {}

    bool validation(int xStart, int yStart, int xEnd, int yEnd, Spot** board) override;
};

#endif