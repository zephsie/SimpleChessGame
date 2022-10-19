#ifndef CHESS_KNIGHTPIECE_H
#define CHESS_KNIGHTPIECE_H


#include "GamePiece.h"

class KnightPiece : public GamePiece {
private:
    bool validation(int xStart, int yStart, int xEnd, int yEnd, Spot **board) override;

public:
    KnightPiece(Color color) : GamePiece(color) {}
};

#endif
