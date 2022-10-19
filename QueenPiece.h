#ifndef CHESS_QUEENPIECE_H
#define CHESS_QUEENPIECE_H

#include <utility>

#include "GamePiece.h"

class QueenPiece : public GamePiece {
private:
    bool validation(int xStart, int yStart, int xEnd, int yEnd, Spot **board) override;

public:
    QueenPiece(Color color) : GamePiece(color) {}
};


#endif
