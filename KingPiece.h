#ifndef CHESS_KINGPIECE_H
#define CHESS_KINGPIECE_H

#include "GamePiece.h"

class KingPiece : public GamePiece {

public:
    KingPiece(Color color) : GamePiece(color) {}

    bool validation(int xStart, int yStart, int xEnd, int yEnd, Spot** board) override;
};

#endif
