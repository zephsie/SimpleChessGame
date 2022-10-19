#ifndef CHESS_SPOT_H
#define CHESS_SPOT_H

#include "Color.h"

class GamePiece;

class Spot {

private:
    GamePiece *piece;
    Color color;

public:
    Spot() = default;

    Spot(GamePiece *piece, Color color);

    ~Spot() = default;

    GamePiece *getPiece();

    void setPiece(GamePiece *piece);

    Color getColor();

    void setColor(Color color);
};

#endif
