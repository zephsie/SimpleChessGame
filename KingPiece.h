#ifndef CHESS_KINGPIECE_H
#define CHESS_KINGPIECE_H

#include "GamePiece.h"

class KingPiece : public GamePiece {
private:
    bool validation(int xStart, int yStart, int xEnd, int yEnd, Spot **board) override;

public:
    KingPiece(Color color) : GamePiece(color) {}
};

#endif
