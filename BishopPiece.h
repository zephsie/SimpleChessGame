#ifndef CHESS_BISHOPPIECE_H
#define CHESS_BISHOPPIECE_H

#include "GamePiece.h"

class BishopPiece : public GamePiece {
private:
    bool validation(int xStart, int yStart, int xEnd, int yEnd, Spot **board) override;

public:
    BishopPiece(Color color) : GamePiece(color) {}
};

#endif