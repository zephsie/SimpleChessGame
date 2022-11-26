#ifndef CHESS_ROOKPIECE_H
#define CHESS_ROOKPIECE_H

#include "GamePiece.h"

class RookPiece : public GamePiece {

public:
    RookPiece(Color color) : GamePiece(color) {}

    bool validation(int xStart, int yStart, int xEnd, int yEnd, Spot** board) override;
};

#endif
