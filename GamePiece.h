#ifndef CHESS_GAMEPIECE_H
#define CHESS_GAMEPIECE_H

#include <string>
#include "Color.h"
#include "Spot.h"

class GamePiece {

private:
    Color color;

public:
    GamePiece(Color color);

    virtual ~GamePiece() = default;

    Color getColor();

    void setColor(Color color);

    virtual bool validation(int xStart, int yStart, int xEnd, int yEnd, Spot** board) = 0;
};

#endif